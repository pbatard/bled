/*
 * Bled (Base Library for Easy Decompression) - large image comparison test
 * The following decompresses a large zipped Raspbian image, and compares it
 * with its uncompressed version to detect potential decompression errors.
 *
 * Copyright © 2016-2023 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "../src/bled.h"

#define BUFSIZE 65536

#define SRC_NAME "D:\\Imgs\\2015-02-16-raspbian-wheezy.zip"
#define DST_NAME "C:\\Downloads\\test.img"
#define REF_NAME "C:\\Downloads\\2015-02-16-raspbian-wheezy.img"

int main(int argc, char** argv)
{
	int ret = 1;
	static char *src = SRC_NAME, *dst = DST_NAME, *ref = REF_NAME;
	int64_t rb, ref_size;
	DWORD rSize[2];
	uint8_t buffer[2][BUFSIZE];
	HANDLE hSrc = NULL, hDst = NULL, hRef = NULL;
	LARGE_INTEGER li;

	// Needed to prevent stdout buffering with MinGW
	setbuf(stdout, NULL);

	hSrc = CreateFileA(src, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hSrc == INVALID_HANDLE_VALUE) {
		printf("Could not open source file '%s' - Error %ld\n", src, GetLastError());
		goto out;
	}
	hDst = CreateFileA(dst, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hDst == INVALID_HANDLE_VALUE) {
		printf("Could not create destination file '%s' - Error %ld\n", dst, GetLastError());
		goto out;
	}
	hRef = CreateFileA(ref, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hRef == INVALID_HANDLE_VALUE) {
		printf("Could not open reference file '%s' - Error %ld\n", ref, GetLastError());
		goto out;
	}
	li.QuadPart = 0;
	if (!SetFilePointerEx(hRef, li, &li, FILE_END)) {
		printf("Could not get reference file size - Error %ld\n", GetLastError());
		goto out;
	}
	ref_size = li.QuadPart;
	li.QuadPart = 0;
	if (!SetFilePointerEx(hRef, li, NULL, FILE_BEGIN)) {
		printf("Could not reset cmp file - Error %ld\n", GetLastError());
		goto out;
	}

	bled_init(0, NULL, NULL, NULL, NULL, NULL, NULL);
	printf("Decompressing...\n");
	bled_uncompress_with_handles(hSrc, hDst, BLED_COMPRESSION_ZIP);
	bled_exit();
	CloseHandle(hSrc);
	hSrc = NULL;
	li.QuadPart = 0;
	if (!SetFilePointerEx(hDst, li, NULL, FILE_BEGIN)) {
		printf("Could not reset dst file - Error %ld\n", GetLastError());
		goto out;
	}

	printf("Comparing...\n");
	for (rb = 0; rb < ref_size; rb += rSize[0]) {
#ifdef _DEBUG
		if ((rb != 0LL) && (rb % (1024 * 1024 * 1024LL) == 0))
			printf("Processed %" PRIi64 " bytes\n", rb);
#endif
		if (!ReadFile(hDst, buffer[0], BUFSIZE, &rSize[0], NULL)) {
			printf("Could not read dst file - Error %ld\n", GetLastError());
			goto out;
		}
		if (!ReadFile(hRef, buffer[1], BUFSIZE, &rSize[1], NULL)) {
			printf("Could not read comparison file - Error %ld\n", GetLastError());
			goto out;
		}
		if ((rSize[0] == 0) || (rSize[1] == 0)) {
			printf("Error - Zero size buffer\n");
			goto out;
		}
		// Ideally, we should consider partial reads as an ever present possibility
		// but in practice the only partial read we get is with the last block, so...
		if (rSize[0] != rSize[1]) {
			printf("Error - Size mismatch (%ld vs %ld)\n", rSize[0], rSize[1]);
			goto out;
		}
		if (memcmp(buffer[0], buffer[1], rSize[0]) != 0) {
			printf("Error - data mismatch at position %" PRIi64 "\n", rb);
			goto out;
		}
	}
	ret = 0;
	printf("Match\n");

out:
	if (hSrc != NULL)
		CloseHandle(hSrc);
	if (hDst != NULL)
		CloseHandle(hDst);
	if (hRef != NULL)
		CloseHandle(hRef);

	return ret;
}
