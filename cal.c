/* cal.c, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */

#include "head.h"
#include "print.h"
#include "ls.h"
#include "cal.h"

/*calculate the number of cols for multi column output*/

void calculatecolums(FTSENT* fsent, struct table* dimention)
{
	struct winsize size;
	int cols, win_length, i=0, j, count;
	int p, rows, width=0, max_len=0, mark = 0;
	struct StrLength strlng;
	int len = 0;
	statistics(fsent, &strlng);
	count = strlng.num_list;
	char* name[count];
	char ch[1];
	char* rt;

	for(j=0; j<count; j++)
	{
		if((name[j] = (char*)malloc(sizeof(char)*(NAME_MAX+1))) == NULL)
		{
			fprintf(stderr, "\nmalloc %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	
	/*get the env setting, if return NULL, get
	* the current terminal column, if output 
	* to a file, set cols = 80
	*/
	rt = getenv("COLUMN");
        if(rt==NULL && pterminal)
        {
                if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) <0 )
                {
                        fprintf(stderr, "\nls: %s\n", strerror(errno));
                        exit(EXIT_FAILURE);
                }
                win_length = size.ws_col;
        }
        else if(pterminal)
                win_length = atoi(rt);

        if(pfile)
                win_length = 80;

	if(strlng.name_length >= win_length)
		width = strlng.name_length;
	if(iflag)
		len += strlng.ino_length + 1;	

	if(sflag)
		len += strlng.block_length + 1;

	/*get the smallest columns we can get
	*then increase the column until the width bigger than 
	*the terminal width
	*/
	cols = win_length / (strlng.name_length + 1 + len);
	if(cols < 1)
		cols = 1;
	rows = count / cols;
	if((count%cols) != 0)
		rows++;
	while((cols*rows - count) > rows)
	{
		cols--;
		if(cols == 0)
			cols = 1;
		rows = count / cols;
		if((count%cols) != 0)
			rows++;
	}
	j=0;
	
	/* store the name at the array*/
	while(fsent != NULL && j<count)
	{
		if(Aflag)
		{
			if( (strcmp(fsent->fts_name, ".") == 0) 
				|| (strcmp(fsent->fts_name, "..") == 0))
			{
				fsent = fsent->fts_link;
				continue;
			}
		}
		else if(((fsent->fts_name[0]) == '.') 
			&& (!aflag) && (!Aflag))
		{	
			fsent = fsent->fts_link;
			continue;
		}
		
		strncpy(name[j++], fsent->fts_name, 
			(strlen(fsent->fts_name)+1));
		if(Fflag)
		{
			printchar(fsent->fts_statp->st_mode, ch);
			sprintf(name[j-1], "%s%c", name[j-1], ch[0]);
		}
		fsent = fsent->fts_link;	
	}
	
	/*increase the column to get the width*/
	while(((win_length - 1) > width) && (rows > 1))
	{
		mark = 1;
		p = cols;
		width = 0;
		max_len = 0;
		if(Cflag)
		{
			for(i=0; i<cols; i++)
			{
				for(j=0; j<rows; j++)
				{
					if((j+i*rows) >= count)
						continue;
					if((strlen(name[j+i*rows]) + len) > max_len)
						max_len = strlen(name[j+i*rows])+len;	
				}
				width += (max_len+2);
				max_len = 0;
			}
		}
		else if(xflag)
		{
			for(i=0; i<cols; i++)
			{
				for(j=0; j<rows; j++)
				{
					if((i+j*cols) >= count)
						continue;
					if((strlen(name[i+j*cols]) + len) > max_len)
						max_len = strlen(name[i+j*cols])+len;	
				}
				width += (max_len+2);
				max_len = 0;
			}
		}

		cols++;
		rows = count / cols;
		if((count%cols) != 0)
			rows++;
		
		while((cols*rows - count) > rows)
		{
			cols++;
			rows = count / cols;
			if((count%cols) != 0)
				rows++;
		}
	}

	if((mark == 1) && 1 /*((win_length - 1) < width)*/)
	{
		cols = p - 1;
		if(cols < 1)
			cols = 1;
		rows = count / cols;
		if((count%cols) != 0)
			rows++;
		while((cols*rows - count) > rows)
		{	
			cols--;
			rows = count / cols;
			if((count%cols) != 0)
				rows++;
		}
	}

	dimention->cols = cols;
	dimention->rows = rows;
	for(j=0; j<count; j++)
		free(name[j]);
}

/* calculate the string len for the size*/
short calculatelen(double j)
{
	short len=1;

	while(j>1000)
		j=j/1000;
	if(j<100 && j>=10)
		len = 2;
	else if(j>0)
		len = 3;

	return len;
}

/*calculate the length for each category of output*/
int statistics(FTSENT* fsent, struct StrLength* stl)
{
	ino_t max_ino=0;
	nlink_t max_link=0;
	uid_t max_uid=0;
	gid_t max_gid=0;
	off_t max_size=0, total_size=0, total_bytes=0;
	int max_name = 0;
	blkcnt_t max_block = 0;
	int count = 0;
	char str[100];
	char* bs;
	int i, len=1, slen = 1;
	int minorlen = 0, majorlen = 0;
	double j;
	struct passwd  *pwd;
	struct group   *grp;

	while(fsent != NULL)
	{
		if(Aflag)
		{
			if( (strcmp(fsent->fts_name, ".") == 0) 
				|| (strcmp(fsent->fts_name, "..") == 0))
			{
				fsent = fsent->fts_link;
				continue;
			}
		}
		else if(((fsent->fts_name[0]) == '.')
				 && (!aflag) && (!Aflag))
		{	
			fsent = fsent->fts_link;
			continue;
		}
		if(fsent->fts_info == FTS_D && fts_file==1)
		{	
			fsent = fsent->fts_link;
			continue;
		}
		if(fsent->fts_statp->st_blocks > max_block && !hflag)
                        max_block = fsent->fts_statp->st_blocks;
                else if(hflag)
                        max_block = fsent->fts_statp->st_blocks;

		if(len!=3 && hflag)
		{
			j = max_block*1.0/2;
			len = calculatelen(j);
		}

		if(fsent->fts_statp->st_ino > max_ino)
			max_ino = fsent->fts_statp->st_ino;
		if(fsent->fts_statp->st_nlink > max_link)
			max_link = fsent->fts_statp->st_nlink;
		if((nflag))
		{		
			if(fsent->fts_statp->st_uid > max_uid)
				max_uid = fsent->fts_statp->st_uid;
			if(fsent->fts_statp->st_gid > max_gid)
				max_gid = fsent->fts_statp->st_gid;
		}
		else
		{
			if ((pwd = getpwuid(fsent->fts_statp->st_uid)) != NULL)
				if((strlen(pwd->pw_name)) > max_uid)
					max_uid = strlen(pwd->pw_name);
			if ((grp = getgrgid(fsent->fts_statp->st_gid)) != NULL)
				if((strlen(grp->gr_name)) > max_gid)
					max_gid = strlen(grp->gr_name);
		}
		
		if(S_ISCHR(fsent->fts_statp->st_mode)
			 || S_ISBLK(fsent->fts_statp->st_mode))
		{
			if(major(fsent->fts_statp->st_rdev) > majorlen)
				majorlen = major(fsent->fts_statp->st_rdev);
			if(minor(fsent->fts_statp->st_rdev) > minorlen)
				minorlen = minor(fsent->fts_statp->st_rdev);
		}

		if(hflag)
		{
			max_size = fsent->fts_statp->st_size;
			if(slen!=3)
			{
				j = max_size*1.0;
				if(j>1000)
					slen = calculatelen(j/1000);
				else if(j>100)
					slen = 3;
				else if(j>10)
					slen = 2;
			}
		}
		else
		{
			if(fsent->fts_statp->st_size > max_size)
				max_size = fsent->fts_statp->st_size;
		}	
		total_size += fsent->fts_statp->st_blocks;
		total_bytes += fsent->fts_statp->st_size;
		
		if(fsent->fts_namelen > max_name)
			max_name = fsent->fts_namelen;
		count++;
		fsent = fsent->fts_link;
	}

	max_block = max_block ;
	bs = getenv("BLOCKSIZE");
	
	if(hflag)
		stl->block_length = len;
	else
	{
		if(bs==NULL)
			i = 512;
		else
			i = atoi(bs);
		if(i == 0)
			i = 512;
		if(i != 512)
                {
                        j = 512/(i*1.0);
                        j = (max_block*1.0)*j+0.5;
                }
                else
                        j = max_block*1.0;
		sprintf(str, "%.lf", j);
		stl->block_length = strlen(str);	
	}	

	sprintf(str, "%zu", max_ino);
	stl->ino_length = strlen(str);

	sprintf(str, "%zu", max_link);
	stl->link_length = strlen(str);

	if(nflag)
	{	
		sprintf(str, "%d", max_uid);
		stl->uid_length = strlen(str);

		sprintf(str, "%d", max_gid);
		stl->gid_length = strlen(str);
	}
	else
	{
		stl->uid_length = max_uid;
		stl->gid_length = max_gid;
	}
	sprintf(str, "%d", minorlen);
	stl->minor_length = strlen(str);
	
	sprintf(str, "%d", majorlen);
	stl->major_length = strlen(str);
	
	if(hflag)
		stl->size_length = slen;
	else
	{
		sprintf(str, "%zu", max_size);
		stl->size_length = strlen(str);
	}

	if((stl->size_length < 
		(stl->minor_length + stl->major_length)) 
		&& (minorlen>0 || majorlen>0))
		stl->size_length = stl->minor_length 
			+ stl->major_length + 2;
	stl->name_length = max_name;
	stl->num_list = count;
	stl->total_blocks = total_size;
	stl->total_bytes = total_bytes;

	return 1;
}
