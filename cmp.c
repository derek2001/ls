/* cmp.c, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */

#include "head.h"
#include "cmp.h"

/*name compare function*/
int namecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	return (strcasecmp((*fsent1)->fts_name,(*fsent2)->fts_name));
}

/*reverse name compare function*/
int revnamecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	return (strcasecmp((*fsent2)->fts_name,(*fsent1)->fts_name));
}
/*ctime compare function*/
int ctimecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent1)->fts_statp->st_ctime > (*fsent2)->fts_statp->st_ctime)
		return -1;
	else if((*fsent1)->fts_statp->st_ctime < (*fsent2)->fts_statp->st_ctime)
		return 1;
	else 
		return namecmp(fsent1, fsent2);
}

/*reverse ctime compare function*/
int revctimecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent2)->fts_statp->st_ctime > (*fsent1)->fts_statp->st_ctime)
		return -1;
	else if((*fsent2)->fts_statp->st_ctime < (*fsent1)->fts_statp->st_ctime)
		return 1;
	else 
		return revnamecmp(fsent1, fsent2);
}
/*mtime compare function*/
int mtimecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent1)->fts_statp->st_mtime > (*fsent2)->fts_statp->st_mtime)
		return -1;
	else if((*fsent1)->fts_statp->st_mtime < (*fsent2)->fts_statp->st_mtime)
		return 1;
	else 
		return namecmp(fsent1, fsent2);
}
/*reverse mtime compare function*/
int revmtimecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent2)->fts_statp->st_mtime > (*fsent1)->fts_statp->st_mtime)
		return -1;
	else if((*fsent2)->fts_statp->st_mtime < (*fsent1)->fts_statp->st_mtime)
		return 1;
	else 
		return revnamecmp(fsent1, fsent2);
}
/*atime compare function*/
int atimecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent1)->fts_statp->st_atime > (*fsent2)->fts_statp->st_atime)
		return -1;
	else if((*fsent1)->fts_statp->st_atime < (*fsent2)->fts_statp->st_atime)
		return 1;
	else 
		return namecmp(fsent1, fsent2);
}
/*reverse atime compare function*/
int revatimecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent2)->fts_statp->st_atime > (*fsent1)->fts_statp->st_atime)
		return -1;
	else if((*fsent2)->fts_statp->st_atime < (*fsent1)->fts_statp->st_atime)
		return 1;
	else 
		return revnamecmp(fsent1, fsent2);
}
/*size compare function*/
int sizecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent1)->fts_statp->st_size > (*fsent2)->fts_statp->st_size)
		return -1;
	else if((*fsent1)->fts_statp->st_size < (*fsent2)->fts_statp->st_size)
		return 1;
	else 
		return namecmp(fsent1, fsent2);
}
/*reverse size compare function*/
int revsizecmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent2)->fts_statp->st_size > (*fsent1)->fts_statp->st_size)
		return -1;
	else if((*fsent2)->fts_statp->st_size < (*fsent1)->fts_statp->st_size)
		return 1;
	else 
		return revnamecmp(fsent1, fsent2);
}
