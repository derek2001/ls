/* ls.h, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */

/* flags */
int Aflag;           /*-A flag, list all entries except for . and ..*/
int aflag;           /*-a flag, Include directory begin with a dot  */
int Cflag;	     /*-C flag, Force multi-column output           */
int cflag;           /*-c flag, use time file was last changed      */
int dflag;           /*-d flag, directories are list as plain file  */
int Fflag;           /*-F flag, Display a slash after a directory...*/
int fflag;           /*-f flag, output is not sort                  */
int hflag;           /*-h flag, the size displayed in bytes         */
int iflag;           /*-i flag, display the inode number            */
int kflag;           /*-k flag, display the size in kilobytes       */      
int lflag;           /*-l flag, display in long format              */
int nflag;           /*-n flag, owner and id displayed numerically  */
int qflag;           /*-q flag, display"?" for non-printable char   */
int Rflag;           /*-R flag, display subfolder directories       */
int rflag;           /*-r flag, sort reverse                        */
int Sflag;           /*-S flag, sort by size                        */
int sflag;           /*-s flag, display number of blocks used       */
int tflag;           /*-t flag, sort by time modified               */
int uflag;           /*-u flag, sort by time of last modification   */
int wflag;           /*-w flag, force raw print of non-printable ch */
int Iflag;           /*-1 flag, one per line                        */
int xflag;	     /*-x flag, multi-col with entries sorted across*/

int pterminal;       /* output to a terminal                        */
int pfile;           /* output to others other than terminal        */
int fts_file;

int ls(int argc, char* argv[]);
void traversrecursive(FTS* fts, FTSENT* fsent, FTSENT* child);
int distribute(FTS* fts, char* dir, int opts);
int rootcmp(const FTSENT** fsent1, const FTSENT** fsent2);

