/*
 * Bled (Busybox Library for Easy Decompression) - test function
 *
 * Copyright © 2014-2015 Pete Batard <pete@akeo.ie>
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
#endif

#include <stdio.h>
#include "../src/bled.h"

printf_t print_function = NULL;

// It's a unit test, not a sample, so I'll hardcode whatever the hell I want
#define BASE_PATH "D:\\bled\\test\\data\\"
#define BASE_FILE "xz"

struct {
	const char* ext;
	int type;
} test_files[] = {
	{ "zip", BLED_COMPRESSION_ZIP},
	{ "Z", BLED_COMPRESSION_LZW},
	{ "gz", BLED_COMPRESSION_GZIP},
	{ "lzma", BLED_COMPRESSION_LZMA},
	{ "bz2", BLED_COMPRESSION_BZIP2},
	{ "xz", BLED_COMPRESSION_XZ},
//	{ "7z", BLED_COMPRESSION_7ZIP},	// Not implemented
};

// With a 9044 size, the last word in the buffer should resolve to "ready" on success
#define BUFSIZE 9044
int main(int argc, char** argv)
{
	int i;
	int64_t r;
	char src[256], dst[256], *buffer = NULL;

	bled_init(NULL, NULL, NULL);

	printf("DECOMPRESS TO BUFFER:\n");
	buffer = malloc(BUFSIZE);
	for (i = 0; i<ARRAYSIZE(test_files); i++) {
		sprintf(src, "%s%s.%s", BASE_PATH, BASE_FILE, test_files[i].ext);
		r = bled_uncompress_to_buffer(src, buffer, BUFSIZE, test_files[i].type);
		buffer[BUFSIZE-1] = 0;
		printf("  %s:\t%I64d - \"%s\"\n", test_files[i].ext, r, &buffer[BUFSIZE-6]);
		if (r < 0)
			goto out;
	}

	printf("\nDECOMPRESS TO FILE:\n");
	for (i = 0; i<ARRAYSIZE(test_files); i++) {
		sprintf(src, "%s%s.%s", BASE_PATH, BASE_FILE, test_files[i].ext);
		sprintf(dst, "%s!%s.txt", BASE_PATH, test_files[i].ext);
		r = bled_uncompress(src, dst, test_files[i].type);
		printf("  %s:\t%I64d\n", test_files[i].ext, r);
		if (r < 0)
			goto out;
	}

out:
	free(buffer);
	bled_exit();

#if defined(_MSC_VER) && defined(_DEBUG)
	printf("\nPress <ENTER> to exit.\n");
	while(getchar() != '\n');
#endif

#ifdef _CRTDBG_MAP_ALLOC
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
