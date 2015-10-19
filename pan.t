settable()
{	Trans *T, *settr();

	trans = (Trans ***) emalloc(6*sizeof(Trans **));

	/* proctype 0: _init */

	trans[0] = (Trans **) emalloc(6*sizeof(Trans *));

	trans[0][1]	= settr(9,2,1,1,"(run X1(x))");	/* line 26 */
	trans[0][2]	= settr(9,3,2,2,"(run X2(x))");	/* line 27 */
	trans[0][3]	= settr(9,4,3,3,"(run Y1(y))");	/* line 28 */
	trans[0][4]	= settr(9,5,4,4,"(run Y2(y))");	/* line 30 */
	trans[0][5]	= settr(9,0,5,5,"");	/* line 30 */

	/* proctype 1: Y2 */

	trans[1] = (Trans **) emalloc(3*sizeof(Trans *));

	trans[1][1]	= settr(9,2,6,6,"y = (y+3)");	/* line 22 */
	trans[1][2]	= settr(9,0,7,7,"");	/* line 23 */

	/* proctype 2: Y1 */

	trans[2] = (Trans **) emalloc(4*sizeof(Trans *));

	trans[2][1]	= settr(9,2,8,8,"y = (y+1)");	/* line 16 */
	trans[2][2]	= settr(9,3,9,9,"y = (y*2)");	/* line 17 */
	trans[2][3]	= settr(9,0,10,10,"");	/* line 18 */

	/* proctype 3: X2 */

	trans[3] = (Trans **) emalloc(3*sizeof(Trans *));

	trans[3][1]	= settr(9,2,11,11,"x = (x+3)");	/* line 11 */
	trans[3][2]	= settr(9,0,12,12,"");	/* line 12 */

	/* proctype 4: X1 */

	trans[4] = (Trans **) emalloc(4*sizeof(Trans *));

	trans[4][1]	= settr(9,2,13,13,"x = (x+1)");	/* line 5 */
	trans[4][2]	= settr(9,3,14,14,"x = (x+2)");	/* line 6 */
	trans[4][3]	= settr(9,0,15,15,"");	/* line 7 */

}

Trans *
settr(a, b, c, d, t)
	char *t;
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	tmp->atom = a&6;
	tmp->st = b;
	tmp->tp = t;
	tmp->forw = c;
	tmp->back = d;
	return tmp;
}

Trans *
cpytr(a)
	Trans *a;
{	Trans *tmp = (Trans *) emalloc(sizeof(Trans));

	tmp->atom = a->atom;
	tmp->st = a->st;
	tmp->ist = a->ist;
	tmp->forw = a->forw;
	tmp->back = a->back;
	tmp->tp = a->tp;
	return tmp;
}

int cnt;
retrans(n, m, is, srcln, reach) /* proc n, m states, is=initial state */
	short srcln[];
	uchar reach[];
{	Trans *T0, *T1, *T2, *T3;
	int i, j=0;
	if (state_tables >= 2)
	{	printf("RAW proctype %s\n", 
			procname[n]);
		for (i = 1; i < m; i++)
			reach[i] = 1;
		tagtable(n, m, is, srcln, reach);
		return;
	}
	do {	j++;
		for (i = 1, cnt = 0; i < m; i++)
		{	T1 = trans[n][i]->nxt;
			T2 = trans[n][i];
/* prescan: */		for (T0 = T1; T0; T0 = T0->nxt)
/* choice inside choice */	if (trans[n][T0->st]->nxt)
					break;
			if (T0)
			for (T0 = T1; T0; T0 = T0->nxt)
			{	T3 = trans[n][T0->st];
				if (!T3->nxt)
				{	T2->nxt = cpytr(T0);
					T2 = T2->nxt;
					imed(T2, T0->st, n);
					continue;
				}
				do {	T3 = T3->nxt;
					T2->nxt = cpytr(T3);
					T2 = T2->nxt;
					imed(T2, T0->st, n);
				} while (T3->nxt);
				cnt++;
			}
		}
	} while (cnt);
	for (i = 1; i < m; i++)
	if (trans[n][i]->nxt)	/* optimize the list a bit */
	{	T1 = trans[n][i]->nxt;
		T0 = trans[n][i] = cpytr(trans[n][T1->st]);
		imed(T0, T1->st, n);
		for (T1 = T1->nxt; T1; T1 = T1->nxt)
		{	T0->nxt = cpytr(trans[n][T1->st]);
			T0 = T0->nxt;
			imed(T0, T1->st, n);
	}	}
	if (state_tables)
	{	printf("proctype %s\n", procname[n]);
		for (i = 1; i < m; i++)
			reach[i] = 1;
		tagtable(n, m, is, srcln, reach);
	}
}
imed(T, v, n)	/* set intermediate state */
	Trans *T;
{	static uchar warned=0;
	if (T->ist && !warned)
	{	warned=1;
		printf("warning: %s has ", procname[n]);
		printf("ambiguous flow ctl structures, ");
		printf("revise model\n");
	}
	progstate[n][T->st] |= progstate[n][v];
	accpstate[n][T->st] |= accpstate[n][v];
	stopstate[n][T->st] |= stopstate[n][v];
	T->ist = v;
}
tagtable(n, m, is, srcln, reach)
	short srcln[];
	uchar reach[];
{
	Trans *z;
	if (is >= m || !trans[n][is]
	||  is <= 0 || reach[is] == 0)
		return;
	reach[is] = 0;
	if (state_tables)
	for (z = trans[n][is]; z; z = z->nxt)
		crack(n, is, z, srcln);
	for (z = trans[n][is]; z; z = z->nxt)
		tagtable(n, m, z->st, srcln, reach);
}
crack(n, j, z, srcln)
	Trans *z;
	short srcln[];
{	int i;
	printf("	state %2d --[%2d]-->", j, z->forw);
	printf(" state %2d [%s%s%s%s] line %d => ",
		z->st, z->atom & 6 ?"A":"-",
		accpstate[n][j]?"a" :"-",
		stopstate[n][j]?"e" : "-",
		progstate[n][j]?"p" : "-",
		srcln[j]);
	for (i = 0; z->tp[i]; i++)
		if (z->tp[i] == '\n')
			printf("\\n");
		else
			putchar(z->tp[i]);
	printf("\n");
	fflush(stdout);
}
/*ZJR_BEG*/
set_trans_gid(int pid,int tid,int gid)
{
	Trans *tr = trans[pid][tid];
	while(tr)
	{
		tr->gid = gid;
		tr = tr->nxt;
	}
}
set_trans_proc_name(int pid, int tid, char *name)
{
	Trans *tr = trans[pid][tid];
	while(tr)
	{
		tr->proc_name = name;
		tr = tr->nxt;
	}
}
set_pid(int pid,int tid)
{
	Trans *tr = trans[pid][tid];
	while(tr)
	{
		tr->pid = pid;
		tr = tr->nxt;
	}
}
set_tid(int pid,int tid)
{
	Trans *tr = trans[pid][tid];
	while(tr)
	{
		tr->tid = tid;
		tr = tr->nxt;
	}
}
/*ZJR_END*/
