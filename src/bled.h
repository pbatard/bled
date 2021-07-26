/*
 * Bled (Base Library for Easy Decompression)
 *
 * Copyright © 2014-2015 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include <windows.h>
#include <stdint.h>

#pragma once

#ifndef ARRAYSIZE
#define ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))
#endif

typedef void (*printf_t) (const char* format, ...);
typedef void (*progress_t) (const uint64_t read_bytes);
typedef int (*read_t)(int fd, void* buf, unsigned int count);
typedef int (*write_t)(int fd, const void* buf, unsigned int count);
typedef void (*switch_t)(const char* filename, const uint64_t size);

typedef enum {
	BLED_COMPRESSION_NONE = 0,
	BLED_COMPRESSION_ZIP,		// .zip
	BLED_COMPRESSION_LZW,		// .Z
	BLED_COMPRESSION_GZIP,		// .gz
	BLED_COMPRESSION_LZMA,		// .lzma
	BLED_COMPRESSION_BZIP2,		// .bz2
	BLED_COMPRESSION_XZ,		// .xz
	BLED_COMPRESSION_7ZIP,		// .7z
	BLED_COMPRESSION_VTSI,		// .vtsi
	BLED_COMPRESSION_MAX
} bled_compression_type;

/* Uncompress file 'src', compressed using 'type', to file 'dst' */
int64_t bled_uncompress(const char* src, const char* dst, int type);

/* Uncompress using Windows handles */
int64_t bled_uncompress_with_handles(HANDLE hSrc, HANDLE hDst, int type);

/* Uncompress file 'src', compressed using 'type', to buffer 'buf' of size 'size' */
int64_t bled_uncompress_to_buffer(const char* src, char* buf, size_t size, int type);

/* Uncompress all files from archive 'src', compressed using 'type', to destination dir 'dir' */
int64_t bled_uncompress_to_dir(const char* src, const char* dir, int type);

/* Uncompress buffer 'src' of length 'src_len' to buffer 'dst' of size 'dst_len' */
int64_t bled_uncompress_from_buffer_to_buffer(const char* src, const size_t src_len, char* dst, size_t dst_len, int type);

/* Initialize the library.
 * When the parameters are not NULL you can:
 * - specify the printf-like function you want to use to output message
 *   void print_function(const char* format, ...);
 * - specify the read/write functions you want to use;
 * - specify the function you want to use to display progress, based on number of source archive bytes read
 *   void progress_function(const uint64_t read_bytes);
 * - specify the function you want to use when switching files in an archive
 *   void switch_function(const char* filename, const uint64_t filesize);
 * - point to an unsigned long variable, to be used to cancel operations when set to non zero
 */
int bled_init(printf_t print_function, read_t read_function, write_t write_function,
    progress_t progress_function, switch_t switch_function, unsigned long* cancel_request);

/* This call frees any resource used by the library */
void bled_exit(void);
