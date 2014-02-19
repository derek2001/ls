/* print.h, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */

int printlongform(FTSENT* fsent);
int printsinglecolumn(FTSENT* fsent);
int printmulticolumn(FTSENT* fsent);
void printtotal(size_t tblocks);
void printblock(int len, blkcnt_t blocks, char** bstr);
void printsize(int len, off_t size);
void printchar(mode_t md, char* ch);
void printnptname(char* name, short namelen);

