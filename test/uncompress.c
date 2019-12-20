/*
 * Bled (Base Library for Easy Decompression) - simple uncompress app
 *
 * Copyright © 2015 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

/* Memory leaks detection - define _CRTDBG_MAP_ALLOC as preprocessor macro */
#ifdef _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4996)	// Ignore deprecated
#pragma warning(disable: 28159)	// Code analysis warning for GetTickCount vs GetTickCount64
#endif

#include <stdio.h>
#include "../src/bled.h"

typedef struct {
	const char* ext;
	bled_compression_type type;
} comp_assoc;

static comp_assoc file_assoc[] = {
	{ ".zip", BLED_COMPRESSION_ZIP },
	{ ".Z", BLED_COMPRESSION_LZW },
	{ ".gz", BLED_COMPRESSION_GZIP },
	{ ".lzma", BLED_COMPRESSION_LZMA },
	{ ".bz2", BLED_COMPRESSION_BZIP2 },
	{ ".xz", BLED_COMPRESSION_XZ },
};

static DWORD LastRefresh;
static uint64_t src_size;

void progress_func(const uint64_t read_bytes)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	float progress;

	if ((GetTickCount() < (LastRefresh + 100)) && (read_bytes != src_size))
		return;
	LastRefresh = GetTickCount();

	progress = (100.0f*read_bytes) / (1.0f*src_size);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	csbi.dwCursorPosition.X = 0;
	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
	printf("Uncompressing: %3.2f%%", progress);
}

int main(int argc, char** argv)
{
	char *p;
	int r = 1;
	size_t i;
	int64_t wb = 0;
	LARGE_INTEGER li;
	HANDLE hSrc = INVALID_HANDLE_VALUE, hDst = INVALID_HANDLE_VALUE;

	if (argc != 3) {
		fprintf(stderr, "Uncompress an archive to a file\n");
		fprintf(stderr, "Usage: %s <archive> <file>\n", argv[0]);
		goto out;
	}

	for (p = (char*)&argv[1][strlen(argv[1])-1]; (*p != '.') && (p != argv[1]); p--);
	if (p == argv[1]) {
		fprintf(stderr, "Input file must have an extension\n");
		goto out;
	}

	for (i = 0; i<ARRAYSIZE(file_assoc); i++) {
		if (strcmp(p, file_assoc[i].ext) == 0) {

			li.QuadPart = 0;
			hSrc = CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
			if (hSrc == INVALID_HANDLE_VALUE) {
				fprintf(stderr, "Could not open '%s': error 0x%0X\n", argv[1], (uint32_t)GetLastError());
				goto out;
			}
			if (!GetFileSizeEx(hSrc, &li)) {
				fprintf(stderr, "Unable to get the size of '%s': error 0x%0X\n", argv[1], (uint32_t)GetLastError());
				goto out;
			}
			src_size = li.QuadPart;

			hDst = CreateFileA(argv[2], GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
				CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
			if (hDst == INVALID_HANDLE_VALUE) {
				fprintf(stderr, "Could not create '%s': error 0x%0X\n", argv[2], (uint32_t)GetLastError());
				goto out;
			}

			LastRefresh = 0;
			bled_init((printf_t)printf, progress_func, NULL);
			wb = bled_uncompress_with_handles(hSrc, hDst, file_assoc[i].type);
			bled_exit();
			printf("\n");
			if (wb > 0) {
				printf("Uncompressed size: %I64d bytes\n", wb);
				r = 0;
			} else {
				fprintf(stderr, "Decompression error: %d\n", (int)wb);
			}
			break;
		}
	}

	if (i >= ARRAYSIZE(file_assoc))
		fprintf(stderr, "Unsupported compression type '%s'\n", p);

out:
	if (hSrc != INVALID_HANDLE_VALUE)
		CloseHandle(hSrc);
	if (hDst != INVALID_HANDLE_VALUE)
		CloseHandle(hDst);
#if defined(_MSC_VER) && defined(_DEBUG)
	printf("\nPress <ENTER> to exit.\n");
	while(getchar() != '\n');
#endif

#ifdef _CRTDBG_MAP_ALLOC
	_CrtDumpMemoryLeaks();
#endif
	return r;
}
