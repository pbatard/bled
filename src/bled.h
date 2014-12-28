/*
 * Bled (Busybox Library for Easy Decompression)
 *
 * Copyright © 2014 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include <stdint.h>

#pragma once

#ifndef ARRAYSIZE
#define ARRAYSIZE(A)                (sizeof(A)/sizeof((A)[0]))
#endif

typedef void (*printf_t) (const char* format, ...);

enum bled_compression_type {
	BLED_COMPRESSION_ZIP = 0,	// .zip
	BLED_COMPRESSION_LZW,		// .Z
	BLED_COMPRESSION_GZIP,		// .gz
	BLED_COMPRESSION_LZMA,		// .lzma
	BLED_COMPRESSION_BZIP2,		// .bz2
	BLED_COMPRESSION_XZ,		// .xz
//	BLED_COMPRESSION_7ZIP		// .7z
	BLED_COMPRESSION_MAX
};

/* Uncompress file 'src', compressed using 'type', to file 'dst' */
int64_t bled_uncompress(const char* src, const char* dst, int type);

/* Initialize the library */
int bled_init(printf_t print_function);

void bled_exit(void);
