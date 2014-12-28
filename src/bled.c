/*
 * Bled (Busybox Library for Easy Decompression)
 *
 * Copyright © 2014 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#ifdef _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "libbb.h"
#include "bb_archive.h"
#include "bled.h"

smallint bb_got_signal;
printf_t bled_printf = NULL;
static bool in_use = 0;

typedef long long int(*unpacker_t)(transformer_state_t *xstate);

unpacker_t unpacker[BLED_COMPRESSION_MAX] = {
	inflate_unzip,
	unpack_Z_stream,
	unpack_gz_stream,
	unpack_lzma_stream,
	unpack_bz2_stream,
	unpack_xz_stream
};

/* Uncompress file 'src', compressed using 'type', to file 'dst' */
int64_t bled_uncompress(const char* src, const char* dst, int type)
{
	transformer_state_t xstate;
	int64_t ret;

	init_transformer_state(&xstate);
	xstate.src_fd = -1;
	xstate.dst_fd = -1;
	xstate.check_signature = 1;

	xstate.src_fd = _openU(src, _O_RDONLY | _O_BINARY, 0);
	if (xstate.src_fd < 0) {
		bb_printf("Could not open '%s' (errno: %d)", src, errno);
		goto err;
	}

	xstate.dst_fd = _openU(dst, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
	if (xstate.dst_fd < 0) {
		bb_printf("Could not open '%s' (errno: %d)", dst, errno);
		goto err;
	}

	if ((type < 0) || (type >= BLED_COMPRESSION_MAX)) {
		bb_printf("unsupported compression format");
		goto err;
	}

	ret = unpacker[type](&xstate);
	_close(xstate.src_fd);
	_close(xstate.dst_fd);
	return ret;

err:
	if (xstate.src_fd > 0)
		_close(xstate.src_fd);
	if (xstate.dst_fd > 0)
		_close(xstate.dst_fd);
	return -1;
}

int bled_init(printf_t print_function)
{
	if (in_use)
		return -1;
	in_use = true;
	bled_printf = print_function;
	return 0;
}

void bled_exit(void)
{
	bled_printf = NULL;
	if (global_crc32_table)
		free(global_crc32_table);
	in_use = false;
}
