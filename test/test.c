/*
 * Bled (Busybox Library for Easy Decompression) - test function
 *
 * Copyright © 2014 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include <stdio.h>
#include "../src/bled.h"

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
		sprintf_s(src, sizeof(src), "%s%s.%s", BASE_PATH, BASE_FILE, test_files[i].ext);
		sprintf_s(dst, sizeof(dst), "%s!%s.txt", BASE_PATH, test_files[i].ext);
		r = bled_uncompress(src, dst, test_files[i].type);
		printf("%s: %I64d\n", test_files[i].ext, r);
	}

	return 0;
}
