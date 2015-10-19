init_proc_lst()
{
	int I;
	enum{ nil,_var_timeout, x, y, First_queue};

Proc_vu=(short ***)emalloc(5*sizeof(short **));
Proc_cu=(short ***)emalloc(5*sizeof(short **));
Proc_to=(short ***)emalloc(5*sizeof(short **));
Proc_vm=(short ***)emalloc(5*sizeof(short **));
Proc_cs=(short ***)emalloc(5*sizeof(short **));
Proc_cr=(short ***)emalloc(5*sizeof(short **));
Proc_ce=(short ***)emalloc(5*sizeof(short **));
Proc_cf=(short ***)emalloc(5*sizeof(short **));
Proc_ct=(short ***)emalloc(5*sizeof(short **));
Proc_vu[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_cu[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_to[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_vm[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_cs[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_cr[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_ce[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_cf[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_ct[_var_timeout]=(short **)emalloc(sizeof(short *));
Proc_vu[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cu[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_to[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_vm[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cs[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cr[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_ce[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cf[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_ct[_var_timeout][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_vu[x]=(short **)emalloc(1*sizeof(short *));
Proc_cu[x]=(short **)emalloc(1*sizeof(short *));
Proc_vm[x]=(short **)emalloc(1*sizeof(short *));
Proc_cs[x]=(short **)emalloc(1*sizeof(short *));
Proc_cr[x]=(short **)emalloc(1*sizeof(short *));
Proc_ce[x]=(short **)emalloc(1*sizeof(short *));
Proc_cf[x]=(short **)emalloc(1*sizeof(short *));
Proc_ct[x]=(short **)emalloc(1*sizeof(short *));
Proc_vu[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cu[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_vm[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cs[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cr[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_ce[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cf[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_ct[x][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_vu[y]=(short **)emalloc(1*sizeof(short *));
Proc_cu[y]=(short **)emalloc(1*sizeof(short *));
Proc_vm[y]=(short **)emalloc(1*sizeof(short *));
Proc_cs[y]=(short **)emalloc(1*sizeof(short *));
Proc_cr[y]=(short **)emalloc(1*sizeof(short *));
Proc_ce[y]=(short **)emalloc(1*sizeof(short *));
Proc_cf[y]=(short **)emalloc(1*sizeof(short *));
Proc_ct[y]=(short **)emalloc(1*sizeof(short *));
Proc_vu[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cu[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_vm[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cs[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cr[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_ce[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_cf[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
Proc_ct[y][0]=(short *)emalloc(MAXPROC*sizeof(short));
for(I=First_queue;I<5;I++)
	{
	Proc_vu[I]=(short **)emalloc(sizeof(short *));
	Proc_cu[I]=(short **)emalloc(sizeof(short *));
	Proc_vm[I]=(short **)emalloc(sizeof(short *));
	Proc_cr[I]=(short **)emalloc(sizeof(short *));
	Proc_ce[I]=(short **)emalloc(sizeof(short *));
	Proc_cf[I]=(short **)emalloc(sizeof(short *));
	Proc_ct[I]=(short **)emalloc(sizeof(short *));
	Proc_cs[I]=(short **)emalloc(sizeof(short *));
	Proc_vu[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	Proc_cu[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	Proc_vm[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	Proc_cs[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	Proc_cr[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	Proc_ce[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	Proc_cf[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	Proc_ct[I][0]=(short *)emalloc(MAXPROC*sizeof(short));
	}
}

set_proc_var(Pid,P_type,What)
	short Pid,P_type,What;
{
	short I;
	enum{ nil,_var_timeout, x, y, First_queue};

switch (P_type) {

		 /* PROC _init */
	case 0:
insert_p(Proc_vu[y][0],Pid,What);
insert_p(Proc_vu[x][0],Pid,What);
break;

		 /* PROC Y2 */
	case 1:
insert_p(Proc_vm[y][0],Pid,What);
insert_p(Proc_vu[y][0],Pid,What);
break;

		 /* PROC Y1 */
	case 2:
insert_p(Proc_vm[y][0],Pid,What);
insert_p(Proc_vu[y][0],Pid,What);
break;

		 /* PROC X2 */
	case 3:
insert_p(Proc_vm[x][0],Pid,What);
insert_p(Proc_vu[x][0],Pid,What);
break;

		 /* PROC X1 */
	case 4:
insert_p(Proc_vm[x][0],Pid,What);
insert_p(Proc_vu[x][0],Pid,What);
break;
	}
}

