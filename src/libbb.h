/*
 * Library header for busybox
 *
 * Rewritten for Bled (Busybox Library for Easy Decompression)
 * Copyright © 2014 Pete Batard <pete@akeo.ie>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#ifndef LIBBB_H
#define LIBBB_H 1

#ifndef _WIN32
#error Only Windows platforms are supported
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4715)		// not all control paths return a value
#pragma warning(disable: 4996)		// Ignore deprecated
#endif

#include "platform.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>

#include <winsock2.h>
#include <shlwapi.h>
#include <shlobj.h>

#define IF_DESKTOP(x) x
#define IF_NOT_DESKTOP(x)
#define IF_NOT_FEATURE_LZMA_FAST(x) x

#define uoff_t unsigned off_t
#define OFF_FMT "I64"

#ifndef _MODE_T_
#define _MODE_T_
typedef unsigned short mode_t;
#endif

#ifndef _PID_T_
#define _PID_T_
typedef int pid_t;
#endif

#ifndef _GID_T_
#define _GID_T_
typedef unsigned int gid_t;
#endif

#ifndef _UID_T_
#define _UID_T_
typedef unsigned int uid_t;
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#ifndef get_le64
#define get_le64(ptr) (*(const uint64_t *)(ptr))
#endif

#ifndef get_le32
#define get_le32(ptr) (*(const uint32_t *)(ptr))
#endif

#ifndef get_le16
#define get_le16(ptr) (*(const uint16_t *)(ptr))
#endif

extern smallint bb_got_signal;
extern uint32_t *global_crc32_table;

uint32_t* crc32_filltable(uint32_t *crc_table, int endian);
uint32_t crc32_le(uint32_t crc, unsigned char const *p, size_t len, uint32_t *crc32table_le);
uint32_t crc32_be(uint32_t crc, unsigned char const *p, size_t len, uint32_t *crc32table_be);
#define crc32_block_endian0 crc32_le
#define crc32_block_endian1 crc32_be

#define bb_msg_read_error "read error"
#define bb_msg_write_error "write error"

#if defined(_MSC_VER)
#if _FILE_OFFSET_BITS == 64
#define stat _stat32i64
#define lseek _lseeki64
#else
#define stat _stat32
#define lseek _lseek
#endif
#define read _read
#define write _write
#define open _open
#define close _close
#endif

#define full_read read
#define full_write write
#define safe_read full_read
#define lstat stat

extern const char *bb_mode_string(mode_t mode);

#define xmalloc malloc
#define xzalloc(x) calloc(x, 1)
#define xrealloc realloc
#define malloc_or_warn malloc
#define xopen3 open

#define xfunc_die()

#define mkdir(x, y) _mkdir(x)

extern void (*print_function)(const char *format, ...);
#define bb_printf(...) do {(print_function != NULL)?print_function(__VA_ARGS__):printf(__VA_ARGS__);} while(0)

#define bb_error_msg bb_printf
#define bb_error_msg_and_die(...) do {bb_printf(__VA_ARGS__); return;} while(0)
#define bb_error_msg_and_err(...) do {bb_printf(__VA_ARGS__); goto err;} while(0)
#define bb_perror_msg bb_error_msg
#define bb_perror_msg_and_die bb_error_msg_and_die
#define bb_putchar putchar

static inline int link(const char *oldpath, const char *newpath) {errno = ENOSYS; return -1;}
static inline int symlink(const char *oldpath, const char *newpath) {errno = ENOSYS; return -1;}
static inline int chown(const char *path, uid_t owner, gid_t group) {errno = ENOSYS; return -1;}
static inline int mknod(const char *pathname, mode_t mode, dev_t dev) {errno = ENOSYS; return -1;}
static inline int utimes(const char *filename, const struct timeval times[2]) {errno = ENOSYS; return -1;}
static inline pid_t wait(int* status) { *status = 4; return -1; }
#define wait_any_nohang wait

extern void bb_copyfd_exact_size(int fd1, int fd2, off_t size);

#if defined(_MSC_VER)
static inline struct tm *localtime_r(const time_t *timep, struct tm *result) {
	result = localtime(timep);
	return result;
}
#endif

static inline int fnmatch(const char *pattern, const char *string, int flags) {
	return PathMatchSpecA(string, pattern)?0:1;
}

typedef struct _llist_t {
	struct _llist_t *link;
	char *data;
} llist_t;

#define bb_make_directory(path, mode, flags) SHCreateDirectoryExA(NULL, path, NULL)

#if defined(_MSC_VER)
#define S_IFMT _S_IFMT
#define S_IFDIR _S_IFDIR
#define S_IFCHR _S_IFCHR
#define S_IFIFO _S_IFIFO
#define S_IFREG _S_IFREG
#define S_IREAD _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC _S_IEXEC

#define S_IFLNK  (S_IEXEC + 100)
#define S_IFSOCK (S_IFLNK + 1)
#define S_IFBLK  (S_IFSOCK + 1)

#define S_ISLNK(x) FALSE
#define S_ISDIR(x) ((x) & S_IFDIR)
#define S_ISREG(x) ((x) & S_IFREG)

#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_RDWR _O_RDWR
#define O_APPEND _O_APPEND

#define O_CREAT _O_CREAT
#define O_TRUNC _O_TRUNC
#define O_EXCL _O_EXCL
#endif

#endif
