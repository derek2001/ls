/* print.c, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */

#include "head.h"
#include "cal.h"
#include "ls.h"
#include "print.h"

/*print block or return the block by string*/
void printblock(int len, blkcnt_t blocks, char** bstr)
{
	float j;
	char* bs;
	double i;
	char str[64];

	if(hflag)
	{	//printf("plen: %d\n", len);
		j = blocks*1.00/2;
		if(j>1000000)
			(j/1000000)>=10?
				sprintf(str, "%*.lfG ", len, j/1000000)
				:sprintf(str, "%*.1lfG ", len, j/1000000);
		else if(j>1000)
			(j/1000)>=10?sprintf(str, "%*.lfM ", len, j/1000)
					:sprintf(str, "%*.1lfM ", len, j/1000);
		else if(j == 0)
			sprintf(str, "%*.lf ", len==1?len:len+1, j*0.01);
		else
			j>=10?sprintf(str, "%*.lfk ", len, j)
				:sprintf(str, "%*.1lfk ", len, j);
		}
	else
	{	
		bs = getenv("BLOCKSIZE");
                if(bs == NULL)
                        i = 512;
                else
                        i = atof(bs);
                if(i == 0)
                        i = 512;
		if(kflag)
			i = 1024;
                if(i != 512)
                {
                        j = 512/(i*1.0);
                        j = (blocks*1.0)*j+0.5;
                }
                else
                        j = blocks*1.0;
                sprintf(str, "%*.lf ", len, j);

	}
	if(bstr == NULL)
		printf("%s", str);
	else 
		strcpy(*bstr, str);
}

/*output the total bytes*/
void printsize(int len, off_t size)
{
	double j;
	char str[64];

	if(hflag)
	{	//printf("plen: %d\n", len);
		j = size*1.00/1024;
		if(j>1000000)
			(j/1000000)>=10?
				sprintf(str, "%*.lfG ", len, j/1000000)
				:sprintf(str, "%*.1lfG ", len, j/1000000+0.05);
		else if(j>1000)
			(j/1000)>=10?sprintf(str, "%*.lfM ", len, j/1000)
					:sprintf(str, "%*.1lfM ", len, j/1000+0.05);
		else if(j == 0)
			sprintf(str, "%*.lf ", len+1, j*0.01);
		else
			j>=10?sprintf(str, "%*.lfk ", len, j)
				:sprintf(str, "%*.1lfk ", len, j + 0.05);
	}
	printf("%s", str);
}

/*output the total blocks*/
void printtotal(size_t tblocks)
{	
	char* bs;
        double i, j;
        bs = getenv("BLOCKSIZE");
        if(bs==NULL)
                i = 512;
        else
                i = atof(bs);
        if(i == 0)
                i = 512;
	if(kflag)
		i = 1024;
        j = 512/(i*1.0);
        printf("total %.f\n", (tblocks)*j+0.5);
}

/*output the special character*/
void printchar(mode_t md, char* ch)
{	
	char fch[1];
	fch[0] = '0';
	if(S_ISDIR(md))
		fch[0] = '/';
	if(S_ISLNK(md))
		fch[0] = '@';
#ifdef S_IFWHT
	if((md & S_IFMT) == S_IFWHT)
		fch[0] = '%';
#endif
	if(S_ISSOCK(md))
		fch[0] = '=';
	if(S_ISFIFO(md))
		fch[0] = '|';
	if(S_ISREG(md) && md & 0111)
		fch[0] = '*';
	if(ch == NULL && fch[0] != '0')
		(void)putchar(fch[0]);
	else if(ch != NULL)
		ch[0] = fch[0];
}

/*output the long format*/
int printlongform(FTSENT* fsent)
{
	struct passwd  *pwd;
	struct group   *grp;	
	char  mode[20];
	int len;
	char buf[PATH_MAX+1];
	char name[PATH_MAX+1];
	char str[20];
	
	int i;
	if(fsent->fts_info == FTS_D && fts_file==1)
		return 0;
	struct StrLength strlng;
	statistics(fsent, &strlng);
	
	if(!dflag)
	{
		if(hflag)
		{
			printf("total ");
			printblock(1, strlng.total_blocks, NULL);
			printf("\n");
		}
		else
			printtotal(strlng.total_blocks);
	}

	while(fsent != NULL)
	{
		if(Aflag)
		{
			if( ((strcmp(fsent->fts_name, ".") == 0) 
				|| (strcmp(fsent->fts_name, "..") == 0)) 
				&& (!dflag))
			{
				fsent = fsent->fts_link;
				continue;
			}
		}		
		else if((!aflag) 
			&& ((fsent->fts_name[0]) == '.') 
			&& (!Aflag)  
			&& !dflag)
		{	
			fsent = fsent->fts_link;
			continue;
		} 
		/*if(fsent->fts_info == FTS_D && fts_file==1)
		{	
			fsent = fsent->fts_link;
			continue;
		}*/
		if(iflag)
			printf("%*zu ", strlng.ino_length, 
				fsent->fts_statp->st_ino);

		if(sflag)
		{
			printblock(strlng.block_length, 
				fsent->fts_statp->st_blocks, NULL);
		}
		
		strmode(fsent->fts_statp->st_mode, mode);
		printf("%s", mode);
		printf("%*u", strlng.link_length, 
			(unsigned int)(fsent->fts_statp->st_nlink));
		
		if(nflag)
		{
			printf(" %*d", strlng.uid_length, 
				fsent->fts_statp->st_uid);
			printf(" %*d ", strlng.gid_length,
				fsent->fts_statp->st_gid);
		}
		else			
		{
			if ((pwd = getpwuid(fsent->fts_statp->st_uid)) != NULL)
                        {
                                printf(" %s", pwd->pw_name);
                                for(i = strlen(pwd->pw_name); i < strlng.uid_length; i++)
                                        putchar(' ');
                        }
                        else
                        {
                                printf(" %d", fsent->fts_statp->st_uid);
                                sprintf(str, "%d", fsent->fts_statp->st_uid);
                                for(i = strlen(str); i < strlng.uid_length; i++)
                                        putchar(' ');
                        }
                        if ((grp = getgrgid(fsent->fts_statp->st_gid)) != NULL)
                        {
                                printf(" %s ", grp->gr_name);
                                for(i = strlen(grp->gr_name); i < strlng.gid_length; i++)
                                        putchar(' ');
                        }
                        else
                        {
                                printf(" %d ", fsent->fts_statp->st_gid);
                                sprintf(str, "%d", fsent->fts_statp->st_gid);
                                for(i = strlen(str); i < strlng.gid_length; i++)
                                        putchar(' ');
                        }

		}
		if(S_ISCHR(fsent->fts_statp->st_mode) 
			|| S_ISBLK(fsent->fts_statp->st_mode))
		{
			printf("%*d, ", strlng.major_length, 
				major(fsent->fts_statp->st_rdev));
			printf("%*d ", strlng.minor_length, 
				minor(fsent->fts_statp->st_rdev));
		}
		else if(hflag)
		{
			if(strlng.block_length < strlng.size_length)
				len = strlng.size_length;
			else
				len = strlng.block_length;
			
			if(fsent->fts_statp->st_size >= 1000)
				printsize(strlng.block_length, 
					fsent->fts_statp->st_size);
			else
				printf("%*zu ", len+1, fsent->fts_statp->st_size);
		}
		else	
			printf("%*zu ", strlng.size_length, 
				fsent->fts_statp->st_size);
		time_t t = fsent->fts_statp->st_mtime;
		if(cflag)
			t = fsent->fts_statp->st_ctime;
		else if(uflag)
			t = fsent->fts_statp->st_atime;
		struct tm lt;
		localtime_r(&t, &lt);
		char timbuf[80];
		strftime(timbuf, sizeof(timbuf), "%b %d %H:%M", &lt);
	
		printf("%s ", timbuf);
		
		if(qflag)
			printnptname(fsent->fts_name, fsent->fts_namelen);
		else
			printf("%s", fsent->fts_name);

		if(S_ISLNK(fsent->fts_statp->st_mode))
		{	
			if (fsent->fts_level == FTS_ROOTLEVEL)
				sprintf(name, "%s", fsent->fts_name);
			else
				sprintf(name,"%s/%s", 
					fsent->fts_parent->fts_accpath, 
					fsent->fts_name);
			if (readlink(name, buf, PATH_MAX) == -1) 
			{
				fprintf(stderr, "\nls: %s: %s\n", name, 
					strerror(errno));
				return 1;
			}
			else 
				printf(" -> %s", buf);
		}
		else if(Fflag)
			printchar(fsent->fts_statp->st_mode, NULL);
		printf("\n");
		fsent = fsent->fts_link;
	}
	return 0;
}

/*print '?' instead of nonprintable character*/
void printnptname(char* name, short namelen)
{
	int i;
	for(i =0; i<namelen; i++)
	{
		if(isprint(name[i]) != 0)
			printf("%c", name[i]);
		else 
			printf("?");
	}
}

/*output in the single column format*/
int printsinglecolumn(FTSENT* fsent)
{
	struct StrLength strlng;
	statistics(fsent, &strlng);
	if(fsent->fts_info == FTS_D && fts_file==1)
		return 0;
	if(sflag)
	{
		if(hflag)
		{
			printf("total ");
			printblock(1, strlng.total_blocks, NULL);
			printf("\n");
		}
		else
			printtotal(strlng.total_blocks);
	}

	while(fsent != NULL)
	{
		if(Aflag)
		{
			if( ((strcmp(fsent->fts_name, ".") == 0) 
				|| (strcmp(fsent->fts_name, "..") == 0) )
				&& (!dflag))
			{
				fsent = fsent->fts_link;
				continue;
			}
		}
		else if(((fsent->fts_name[0]) == '.') && (!aflag) 
				&& (!Aflag) && !dflag)
		{	
			fsent = fsent->fts_link;
			continue;
		}
		/*if(fsent->fts_info == FTS_D && fts_file==1)
		{	
			fsent = fsent->fts_link;
			continue;
		}*/
		if(iflag)
			printf("%*zu ",strlng.ino_length ,
				fsent->fts_statp->st_ino);

		if(sflag)
		{
			printblock(strlng.block_length, 
				fsent->fts_statp->st_blocks, NULL);
		}
		
		if(qflag)
			printnptname(fsent->fts_name, 
				fsent->fts_namelen);
		else
			printf("%s", fsent->fts_name);
	
		if(Fflag)
			printchar(fsent->fts_statp->st_mode, NULL);
		printf("\n");

		fsent = fsent->fts_link;
	}

	return 0;
}


/*output the files in multi columns format*/
int printmulticolumn(FTSENT* fsent)
{
	int cols, i=0, j, count;
	int p, rows, max_len=0, index = 0;
	struct StrLength strlng;
	struct table dimention;
	char ch[1];
	if(fsent->fts_info == FTS_D && fts_file==1)
		return 0;
	calculatecolums(fsent, &dimention);
	statistics(fsent, &strlng);
	cols = dimention.cols;
	rows = dimention.rows;
	count = strlng.num_list;

	char* name[count];
	char* block[count];
	char* funnychar[count];	
	
	if(cols == 1)
	{
		printsinglecolumn(fsent);
		return 0;
	}
	
	if(sflag)
	{
		if(hflag)
		{
			printf("total ");
			printblock(1, strlng.total_blocks, NULL);
			printf("\n");
		}
		else
			printtotal(strlng.total_blocks);
	}
	
	ino_t ino[count];

	for(j=0; j<count; j++)
	{
		if((name[j] = (char*)malloc(sizeof(char)*(NAME_MAX+1))) == NULL)
		{
			fprintf(stderr, "\n%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if((block[j] = (char*)malloc(64)) == NULL)
		{
			fprintf(stderr, "\n%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if((funnychar[j] = (char*)malloc(2)) ==NULL)
		{
			fprintf(stderr, "\n%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	short maxlencol[cols];
	for(j=0; j<cols; j++)
		maxlencol[j] = 0;

	ch[0] = '0';
	j = 0;
	p = 0;
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
			
		strncpy(name[j], fsent->fts_name, 
			(strlen(fsent->fts_name)+1));
		if(Fflag)
		{
			printchar(fsent->fts_statp->st_mode, ch);
			if(ch[0] != '0')
				sprintf(name[j], "%s%c", name[j], ch[0]);
		}
		printblock(strlng.block_length, 
			fsent->fts_statp->st_blocks, &(block[j]));
		ino[j++] = fsent->fts_statp->st_ino;
		if(Cflag)
		{
			if(fsent->fts_namelen > max_len)
				max_len = fsent->fts_namelen;
			if(((j%rows) == 0) && (rows != 1))
			{
				maxlencol[p++] = max_len;
				max_len = 0;
			}
		}
		else if(xflag)
		{
			if(fsent->fts_namelen > maxlencol[p++])
				maxlencol[p-1] = fsent->fts_namelen;
			if(p%cols == 0)
				p = 0;
		}
			
		fsent = fsent->fts_link;	
	}
	if(Cflag)
		maxlencol[p] = max_len;
	for(i=0; i<rows; i++)
	{
		for(j=0; j<cols; j++)
		{
			if((i+rows*j) >= count && Cflag)
				continue;
			
			if(Cflag)
			{
				if(iflag)
					printf("%*zu ", strlng.ino_length,
						 ino[i+rows*j]);
				if(sflag)
					printf("%s", block[i+rows*j]);
				if(qflag)
					printnptname(name[i+rows*j], 
						strlen(name[i+rows*j]));
				else
					printf("%s", name[i+rows*j]);
				p = strlen(name[i+rows*j]);
			}
			else if(xflag && (index < count))
			{
				if(iflag)
					printf("%*zu ", strlng.ino_length,
						 ino[index]);
				if(sflag)
					printf("%s", block[index]);
				if(qflag)
					printnptname(name[index], 
						strlen(name[index]));
				else
					printf("%s", name[index]);
				p = strlen(name[index++]);
			}

			if(j == cols-1)
				while((p++ < (maxlencol[j]+1)) 
					&& (rows > 1))
					printf(" ");
			else
				while((p++ < (maxlencol[j]+2)) 
					&& (rows > 1))
					printf(" ");
			if(rows == 1)
				printf("  ");
		}
		printf("\n");
	}

	for(j=0; j<count; j++)
	{
		free(name[j]);
		free(block[j]);
		free(funnychar[j]);
	}
	return 0;
}


