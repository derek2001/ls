/* head.h, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <bsd/string.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fts.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

struct StrLength
{
	int block_length;
	int ino_length;
	int link_length;
	int uid_length;
	int gid_length;
	int size_length;
	int name_length;
	int num_list;
	int minor_length;
	int major_length;
	size_t total_blocks;
	off_t total_bytes;
};

struct table
{
	short cols;
	short rows;
};




