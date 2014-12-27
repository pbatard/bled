/*
 * Bled (Busybox Library for Easy Decompression) - test function
 *
 * Copyright � 2014 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#if defined(_MSC_VER)
#pragma warning(disable: 4996)		// Ignore deprecated
#endif

#include <stdio.h>
#include "../src/bled.h"

printf_t print_function = NULL;

#define BASE_PATH "D:\\bled\\test\\data\\"
#define BASE_FILE "xz"

#ifndef ARRAYSIZE
#define ARRAYSIZE(A)                (sizeof(A)/sizeof((A)[0]))
#endif

struct {
	const char* ext;
	int type;
} test_files[] = {
//	{ "zip", BLED_COMPRESSION_ZIP},	// Not working
//	{ "7z", BLED_COMPRESSION_7ZIP},	// No implemented
	{ "Z", BLED_COMPRESSION_LZW},
	{ "gz", BLED_COMPRESSION_GZIP},
	{ "lzma", BLED_COMPRESSION_LZMA},
	{ "bz2", BLED_COMPRESSION_BZIP2},
	{ "xz", BLED_COMPRESSION_XZ},
};

int main(int argc, char** argv)
{
	int i;
	int64_t r;
	char src[256], dst[256];

	for (i = 0; i<ARRAYSIZE(test_files); i++) {
		sprintf(src, "%s%s.%s", BASE_PATH, BASE_FILE, test_files[i].ext);
		sprintf(dst, "%s!%s.txt", BASE_PATH, test_files[i].ext);
		r = bled_uncompress(src, dst, test_files[i].type);
		printf("%s: %I64d\n", test_files[i].ext, r);
	}

	printf("Press <ENTER> to exit.");
	while(getchar() != '\n');

	return 0;
}