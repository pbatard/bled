/*
 * Bled (Busybox Library for Easy Decompression)
 *
 * Copyright © 2014 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include <stdint.h>

#pragma once

#define BLED_COMPRESSION_ZIP     1	// .zip
#define BLED_COMPRESSION_7ZIP    2	// .7z
#define BLED_COMPRESSION_LZW     3	// .Z
#define BLED_COMPRESSION_GZIP    4	// .gz
#define BLED_COMPRESSION_LZMA    5	// .lzma
#define BLED_COMPRESSION_BZIP2   6	// .bz2
#define BLED_COMPRESSION_XZ      7	// .xz

/* Uncompress file 'src', compressed using 'type', to file 'dst' */
int64_t bled_uncompress(const char* src, const char* dst, int type);
