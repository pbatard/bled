/*
 * Bled (Busybox Library for Easy Decompression)
 *
 * Copyright © 2014 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include "libbb.h"
#include "bb_archive.h"
#include "bled.h"

typedef int smallint;
smallint bb_got_signal;

static int64_t bled_unpack(const char* src, const char* dst, long long int (*unpacker)(transformer_state_t *xstate))
{
	int64_t status = 0;
	transformer_state_t xstate;

	init_transformer_state(&xstate);
	xstate.check_signature = 1;

	xstate.src_fd = _open(src, _O_RDONLY | _O_BINARY, 0);
	if (xstate.src_fd < 0) {
		bb_printf("Could not open '%s' (errno: %d)", src, errno);
		return -1;
	}

	xstate.dst_fd = _open(dst, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
	if (xstate.dst_fd < 0) {
		bb_printf("Could not open '%s' (errno: %d)", dst, errno);
		return -1;
	}

	return unpacker(&xstate);
}

/* Uncompress file 'src', compressed using 'type', to file 'dst' */
int64_t bled_uncompress(const char* src, const char* dst, int type)
{
	switch (type) {
	case BLED_COMPRESSION_ZIP:
		return bled_unpack(src, dst, inflate_unzip);
	case BLED_COMPRESSION_LZW:
		return bled_unpack(src, dst, unpack_Z_stream);
	case BLED_COMPRESSION_GZIP:
		return bled_unpack(src, dst, unpack_gz_stream);
	case BLED_COMPRESSION_LZMA:
		return bled_unpack(src, dst, unpack_lzma_stream);
	case BLED_COMPRESSION_BZIP2:
		return bled_unpack(src, dst, unpack_bz2_stream);
	case BLED_COMPRESSION_XZ:
		return bled_unpack(src, dst, unpack_xz_stream);
	default:
		bb_printf("unsupported compression");
		return -1;
	}
}
