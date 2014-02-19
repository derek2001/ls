/* ls.c, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */

#include "head.h"
#include "print.h"
#include "cmp.h"
#include "ls.h"

int (*compare)(const FTSENT**, const FTSENT**);

int
main(int argc, char *argv[]) {
        ls(argc, argv);

	exit(EXIT_SUCCESS);
}

int ls(int argc, char* argv[])
{
	int ch;
	static char dot[]=".";
	char *dir[]={dot, NULL};
	struct stat st;
	FTS* fts;
	uid_t uid;
	//compare p;

	Cflag = 1;	/*default multi column ouput*/
	qflag = 1;	/*-q flag is default*/	

	/*determine if output to a terminal*/
	fstat(STDOUT_FILENO, &st);
	if(S_ISCHR(st.st_mode))
        	pterminal = 1;	
	else if(S_ISREG(st.st_mode))
	{
	        pfile = 1;
		Iflag = wflag = 1;
		qflag = 0;
	}
	
	while ((ch = getopt(argc, argv, "1ACFRSacdfhiklnqrstuwx")) != -1) {
		switch (ch) {
		case '1':
			Iflag = 1;
			lflag =  nflag = 0;
			Cflag =  xflag = 0;
			break;
		case 'A':
			Aflag = 1;
			break;
		case 'a':
			aflag = 1;
			break;
		case 'C':
			Cflag = 1;
			xflag = 0;
			Iflag = nflag = lflag = 0;
			break;
		case 'c':
			cflag = 1;
			uflag = 0;
			break;
		case 'd':
			dflag = 1;
			break;
		case 'F':
			Fflag = 1;
			break;
		case 'f':
			fflag = 1;
			break;
		case 'h':
			hflag = 1;
			kflag = 0;
			break;
		case 'i':
			iflag = 1;
			break;
		case 'k':
			kflag = 1;
			hflag = 0;
			break;
		case 'l':
			lflag = 1;
			Iflag = nflag = 0;
			Cflag = xflag = 0;
			break;
		case 'n':
			nflag = 1;
			lflag = Iflag = 0;
			Cflag = xflag = 0;
			break;
		case 'q':
			qflag = 1;
			wflag = 0;
			break;
		case 'R':
			Rflag = 1;
			break;
		case 'r':
			rflag = 1;
			break;
		case 'S':
			Sflag = 1;
			break;
		case 's':
			sflag = 1;
			break;
		case 't':
			tflag = 1;
			break;
		case 'u':
			uflag = 1;
			cflag = 0;
			break;
		case 'w':
			wflag = 1;
			qflag = 0;
			break;
		case 'x':
			xflag = 1;
			Cflag = 0;
			Iflag = nflag = lflag = 0;
			break;
		default:
		case '?':
			fprintf(stderr, "Try 'ls --help' for more information.\n");
			exit(EXIT_FAILURE);
			break;
		}
	}
	argc -= optind;
	argv += optind;

	/*always set -A flag for super user*/
	uid = geteuid();
	if(uid == 0)
		Aflag = 1;	
	if(aflag)
		Aflag = 0;

	if(rflag)
	{
		if(cflag)
			compare = revctimecmp;
		else if(Sflag)
			compare = revsizecmp;
		else if(tflag)
			compare = revmtimecmp;
		else if(uflag)
			compare = atimecmp;
		else
			compare = revnamecmp;
	}
	else
	{
		if(cflag)
			compare = ctimecmp;
		else if(Sflag)
			compare = sizecmp;
		else if(tflag)
			compare = mtimecmp;
		else if(uflag)
			compare = atimecmp;
		else
			compare = namecmp;
	}
	
	if(lflag && rflag && !Sflag && !tflag)
		compare = revnamecmp;
	else if(!rflag && lflag && !Sflag && !tflag)
		compare = namecmp;

	if(argc)
	{
		fts = fts_open(argv, 
			FTS_PHYSICAL | FTS_NOCHDIR | FTS_SEEDOT, 
			fflag?NULL:rootcmp);
	}
	else
	{
		fts = fts_open(dir, 
			FTS_PHYSICAL | FTS_NOCHDIR | FTS_SEEDOT, 
			fflag?NULL:rootcmp);
	}
	
	distribute(fts, *argv, argc);

	exit(EXIT_SUCCESS);
}

/*compare for the rootlevel of fts
*display the file before the directory
*/

int rootcmp(const FTSENT** fsent1, const FTSENT** fsent2)
{
	if((*fsent1)->fts_level == 0 && 
		(*fsent1)->fts_info != (*fsent2)->fts_info 
		&& !dflag)
 	{
		if((*fsent1)->fts_info == FTS_D)
			return 1;
		else if((*fsent2)->fts_info == FTS_D)
			return -1;
	}

	return (compare(fsent1, fsent2));
}

/*output the regula file*/
int traversefile(FTSENT* fsent)
{	
	if (fsent->fts_info == FTS_ERR 
		|| fsent->fts_info == FTS_NS)
	{
		fprintf(stderr, "ls can't access '%s': %s\n",
			fsent->fts_name, 
			strerror(fsent->fts_errno));
		return 0;
	}

	fts_file = 1;
	if(lflag || nflag)
		printlongform(fsent);
	else if(Iflag)
		printsinglecolumn(fsent);
	else
		printmulticolumn(fsent);
	
	return 1;
}

/*distribute the output task by calling the different funtions.*/
int distribute(FTS* fts, char* dir, int opts)
{
	FTSENT* fsent = NULL;
        FTSENT* child = NULL;
	int flag = 0;

	if (fts == NULL ) {
		fprintf(stderr, "ls can't open '%s': %s\n",dir , strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	if(dflag)
	{
		//aflag = 1;
		if(lflag || nflag)
			printlongform(fts_children(fts, 0));
		else
			printsinglecolumn(fts_children(fts, 0));
		return 1;
	}	

	/*output regula file*/
	traversefile(fts_children(fts, 0));

	/*output the directory*/
	while( (fsent = fts_read(fts)) != NULL)
        {	
		if(fsent->fts_info == FTS_D)			
		{	
       			child = fts_children(fts,0);
			fts_file = 0;

			if(fsent->fts_level != FTS_ROOTLEVEL 
				&& fsent->fts_name[0] == '.' 
				&& !aflag)
			{
				fts_set(fts, fsent, FTS_SKIP);
				continue;
			}

       			if (errno != 0)
       			{
       				fprintf(stderr, 
					"ls: can't open directory '%s': %s\n",
					 fsent->fts_parent->fts_accpath,
					 strerror(errno));
				if(Rflag)
					continue;
				else 
					exit(EXIT_SUCCESS);
       			}
			
			if ((opts > 1 || Rflag) && flag == 0 ) {
				printf("%s:\n", fsent->fts_path);
				flag = 1;
			}
			else if((opts > 1 || Rflag) && flag == 1)
				printf("\n%s:\n", fsent->fts_path);

			if(lflag || nflag)
				printlongform(child);
			else if(Iflag)
				printsinglecolumn(child);
			else
				printmulticolumn(child);

			if (!Rflag)
			{
				fts_set(fts, fsent, FTS_SKIP);
			}
		}
	}
	return 1;
}


