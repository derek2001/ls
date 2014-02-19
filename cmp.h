/* cmp.h, v1.0 2013/9/21 20:55:00 Hui Zheng */

/*
 * Copyright (c) 1989, 1993  All rights reserved.
 *
 */

int namecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int revnamecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int ctimecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int revctimecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int mtimecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int revmtimecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int atimecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int revatimecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int sizecmp(const FTSENT** fsent1, const FTSENT** fsent2);
int revsizecmp(const FTSENT** fsent1, const FTSENT** fsent2);
