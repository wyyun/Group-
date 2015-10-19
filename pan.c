#include	<stdio.h>
#include	<signal.h>
#include   <stdlib.h>
#include   <string.h>
#include        <sys/time.h>
#define uchar unsigned char
#ifndef DEADLOCK
#define NEWDEP
#endif
#define MEMCNT
#define SLEEP
typedef struct Trans {
  short              atom;	
  short              st;	
  short              ist;	
  char               *tp;	
  int                forw;	
  int                back;	
  struct Trans       *nxt;
  struct Trans       *seq;
  uchar              rtype;
  int				 gid;
  char				 *proc_name;
  int                pid;
  int                tid;
} Trans;

typedef struct Rtrans_stat {
  uchar type;
  short pr;
  Trans *tr;
} Rtrans_stat;

typedef struct Rtrans_rdv {
  uchar type;
  short pr;
  Trans *tr;
  short rcv_pr;
  Trans *rcv_tr;
} Rtrans_rdv;

typedef union {
  Rtrans_rdv rdv;
  Rtrans_stat n;
} TRANS;



typedef struct ACC {
  short var;
  int   ind;
  uchar op;
  uchar misc;
} ACC;









typedef struct TRANS_DEF {
  TRANS    *tr;
  unsigned enabled:1;
  unsigned in_stack:1;
  unsigned in_sleep:1;
} TRANS_DEF;




#define SHORT 16
#define SHORT_PWR 4   
#define SHORT_AND_CST 0x000F  
#define BYTE_PWR 4   
#define BYTE_AND_CST 0x0007  

#define set(b,i) (b)[(i)>>SHORT_PWR] |= (1<<((i)&SHORT_AND_CST))
#define is_set(b,i) ((b)[(i)>>SHORT_PWR] & 1<<((i)&SHORT_AND_CST))
#define unset(b,i) (b)[(i)>>SHORT_PWR] &= ~(1<<((i)&SHORT_AND_CST))
#define bset(b,i) (b)[(i)>>BYTE_PWR] |= (1<<((i)&BYTE_AND_CST))
#define bis_set(b,i) ((b)[(i)>>BYTE_PWR] & 1<<((i)&BYTE_AND_CST))
#define bunset(b,i) (b)[(i)>>BYTE_PWR] &= ~(1<<((i)&BYTE_AND_CST))

#define min(a,b) (((a)>(b)) ? (b) : (a))
#define max(a,b) (((a)<(b)) ? (b) : (a))
#define WRITE 1
#define READ  2
#define SEND  3
#define RECV  4
#define TRCV  5
#define EMP   6
#define FUL   7
#define TIMO  8
#define LN    9

#define NORMAL_type    1
#define ATOMIC_type    2
#define RDV_type       3



int MAX_TRAIL=10000;
int MAX_TRANS_DEF_TRAIL=1000;

#define MAX_ct_g 100






typedef struct Trail {
  short pr;	
  short st;	
  short nb_trans;
  uchar  o_ot, tau;
  short o_tt;
  Trans *o_t;
  int oval;
  unsigned prog :1;
} Trail;

typedef struct ConfTag {
  short var;       
  int ind;         
  unsigned type:4; 
  unsigned misc:5; 
  int *old; 
} ConfTag; 





struct ConfTag ***ct; 
struct ConfTag *ct_g;




#define D    1
#define FVE  2
#define EM   3
#define FMO  4
#define ON   5
#define FU   6
#define OLD  7
#define NE   8
#define NF   9
#ifdef NEWDEP
uchar con[10][10]={
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 },
  {0, D, D,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, D, 0,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  NF,  FVE, EM,  EM,  NF,  0,   NF}, 
  {0, 0, 0,  FVE, NE,  NE,  NE,  FU,   0,   NE}, 
  {0, 0, 0,  EM,  NE,  0,   0,   0,    0,   0 }, 
  {0, 0, 0,  EM,  NE,  0,   0,   0,    0,   0 }, 
  {0, 0, 0,  NF, FU,  0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   0,   0,   0,   0,    D,   0 },  
  {0, 0, 0,  NF,  NE,  0,   0,   0,    0,   0 }}; 
#else
uchar con[10][10]={
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 },
  {0, D, D,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, D, 0,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  NF,  FVE, EM,  EM,  FMO,  0,   NF}, 
  {0, 0, 0,  FVE, NE,  NE,  ON,  FU,   0,   NE}, 
  {0, 0, 0,  EM,  NE,  0,   0,   0,    0,   0 }, 
  {0, 0, 0,  EM,  ON,  0,   0,   0,    0,   0 }, 
  {0, 0, 0,  FMO, FU,  0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   0,   0,   0,   0,    D,   0 },  
  {0, 0, 0,  NF,  NE,  0,   0,   0,    0,   0 }}; 
#endif

uchar con_rdv[10][10]={
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 },
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  D,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   D,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 }, 
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 },  
  {0, 0, 0,  0,   0,   0,   0,   0,    0,   0 }}; 

short ***Proc_vu, ***Proc_vm, ***Proc_cs, ***Proc_cr, ***Proc_ct, ***Proc_ce;
short ***Proc_cf, ***Proc_to, ***Proc_cu;

char M_LOSS=0;             

Rtrans_stat ***rtrans;
int *first_tr; 
int *last_tr;
int nb_tr; 
int *nst; 
short nb_pr; 
short nb_var; 
uchar *type_tr; 
Trail *tmp_trpt=0; 
uchar timeout_enabled=0; 


uchar ot;
short tt;



#ifdef OUT
extern FILE *fo;
#endif
extern short MAX_TRANS_DEF;
extern TRANS_DEF **trans_def;
extern short *P_size, *P_1;
extern short nb; 
extern short II; 
extern int depthfound;
extern int depth;

#ifdef SLEEP
extern uchar is_sleeping();
#endif

#ifdef PROV
extern uchar is_in_stack();
#endif

extern void  efree();
extern char *emalloc();

#include	"pan.h"
#define max(a,b) (((a)<(b)) ? (b) : (a))
Trail *trail, *trpt, *trans_def_trail, *trpt_par[2],*trpt_cp[2];
uchar *this;

//char *malloc(), *memset();
int  svmax=0, smax=0;
int Maxbody=0;
int state_tables=0;
uchar *noptr;	/* used by macro Pptr(x) */
State	now;		/* the full state vector */
State	copy_now;		/* the full state vector */
Stack	*stack; 	/* for queues, processes */
Svtack	*svtack;	/* for old state vectors */
#if SYNC
#define IfNotBlocked	if (boq != -1) return 0;
#define UnBlock     	boq = -1
#else
#define IfNotBlocked	/* cannot block */
#define UnBlock     	/* don't bother */
#endif

#ifndef NOBOUNDCHECK
#define Index(x, y)	Boundcheck(x, y, II, tt, t)
#else
#define Index(x, y)	x
#endif
char *procname[] = {
	"_init",
	"Y2",
	"Y1",
	"X2",
	"X1",
	"_progress",
};

addproc(n)
{
       uchar *tmp;
	int j, h = now._nr_pr;
	if (h >= MAXPROC)
		Uerror("too many processes");
	switch (n) {
	case 0: j = sizeof(P0); break;
	case 1: j = sizeof(P1); break;
	case 2: j = sizeof(P2); break;
	case 3: j = sizeof(P3); break;
	case 4: j = sizeof(P4); break;
	case 5: j = sizeof(P5); break;
	default: Uerror("bad proc - addproc");
	}
	if (vsize%WS && (j > WS-(vsize%WS)))
	{	proc_skip[h] = WS-(vsize%WS);
		vsize += proc_skip[h];
	} else
		proc_skip[h] = 0;
	proc_offset[h] = vsize;
	now._nr_pr += 1;
       proc_num= max(now._nr_pr,proc_num);
	vsize += j;
	if (vsize >= VECTORSZ)
		Uerror("VECTORSZ is too small, edit pan.h");
	memset((char *)pptr(h), 0, j);
	switch (n) {
	case 0:	/* _init */
		((P0 *)pptr(h))->_t = 0;
		((P0 *)pptr(h))->_p = 1;
 reached0[1]=1;
		break;
	case 1:	/* Y2 */
		((P1 *)pptr(h))->_t = 1;
		((P1 *)pptr(h))->_p = 1;
 reached1[1]=1;
		break;
	case 2:	/* Y1 */
		((P2 *)pptr(h))->_t = 2;
		((P2 *)pptr(h))->_p = 1;
 reached2[1]=1;
		break;
	case 3:	/* X2 */
		((P3 *)pptr(h))->_t = 3;
		((P3 *)pptr(h))->_p = 1;
 reached3[1]=1;
		break;
	case 4:	/* X1 */
		((P4 *)pptr(h))->_t = 4;
		((P4 *)pptr(h))->_p = 1;
 reached4[1]=1;
		break;
	case 5: /* progress checker */
		((P5 *)pptr(h))->_t = 5;
		((P5 *)pptr(h))->_p = 1;
		now._p_t = 0;
		break;
	}
       tmp=this;
       this = pptr(h);
       set_proc_var(h,n,1);
       init_new_proc(h,n);
       this = tmp;
	return (h>0)?h:0;
}

print_sv()
{
uchar *tmp;
uchar _t;
short _p;
int i;
for(i=0; i<=now._nr_pr;i++)
{
tmp = this;
this = pptr(i);
_t = (uchar) ((P0*)this)->_t;
_p = (short) ((P0*)this)->_p;
printf("sv[%d]=(%d,%d)\n",i,_t,_p);
}
}
run()
{	short i;

       memset((char *)&now, 0, sizeof(State));
	vsize = sizeof(State) - VECTORSZ;
	settable();
    trail = (Trail *) emalloc(MAX_TRAIL*sizeof(Trail));
    trans_def_trail = (Trail *) emalloc(MAX_TRANS_DEF_TRAIL*sizeof(Trail));
    //wyy_beg
    trpt_par[0] = (Trail *) emalloc(MAX_TRAIL*sizeof(Trail));
    trpt_par[1] = (Trail *) emalloc(MAX_TRAIL*sizeof(Trail));
    //wyy_end


    nb_pr=6;
    nb_tr=15;
    type_tr=(uchar *)emalloc((nb_tr+1)*sizeof(uchar));
    settype();
    ct=(struct ConfTag ***)emalloc(MAXPROC*sizeof(struct ConfTag **));
    nst=(int *)emalloc(6*sizeof(int));
    first_tr=(int *)emalloc(6*sizeof(int));
    last_tr=(int *)emalloc(6*sizeof(int));
    rtrans=(Rtrans_stat ***)emalloc(MAXPROC*sizeof(Rtrans_stat **));
    i=(MAXPROC-1)/SHORT+1;
    init_proc_lst();
    first_tr[0]=1;
    last_tr[0]=5;
    first_tr[1]=6;
    last_tr[1]=7;
    first_tr[2]=8;
    last_tr[2]=10;
    first_tr[3]=11;
    last_tr[3]=12;
    first_tr[4]=13;
    last_tr[4]=15;
    first_tr[5]=0;
    last_tr[5]=0;
    Maxbody = max(Maxbody, sizeof(P0));
    Maxbody = max(Maxbody, sizeof(P1));
    Maxbody = max(Maxbody, sizeof(P2));
    Maxbody = max(Maxbody, sizeof(P3));
    Maxbody = max(Maxbody, sizeof(P4));
    reached[0] = reached0;
    reached[1] = reached1;
    reached[2] = reached2;
    reached[3] = reached3;
    reached[4] = reached4;
    nst[0] = nstates0;
    nst[1] = nstates1;
    nst[2] = nstates2;
    nst[3] = nstates3;
    nst[4] = nstates4;
    accpstate[0] = (uchar *) emalloc(nstates0);
    accpstate[1] = (uchar *) emalloc(nstates1);
    accpstate[2] = (uchar *) emalloc(nstates2);
    accpstate[3] = (uchar *) emalloc(nstates3);
    accpstate[4] = (uchar *) emalloc(nstates4);
    progstate[0] = (uchar *) emalloc(nstates0);
    progstate[1] = (uchar *) emalloc(nstates1);
    progstate[2] = (uchar *) emalloc(nstates2);
    progstate[3] = (uchar *) emalloc(nstates3);
    progstate[4] = (uchar *) emalloc(nstates4);
    stopstate[0] = (uchar *) emalloc(nstates0);
    stopstate[1] = (uchar *) emalloc(nstates1);
    stopstate[2] = (uchar *) emalloc(nstates2);
    stopstate[3] = (uchar *) emalloc(nstates3);
    stopstate[4] = (uchar *) emalloc(nstates4);
    stopstate[0][endstate0] = 1;
    stopstate[1][endstate1] = 1;
    stopstate[2][endstate2] = 1;
    stopstate[3][endstate3] = 1;
    stopstate[4][endstate4] = 1;
    retrans(0, nstates0, start0, src_ln0, reached0);
    retrans(1, nstates1, start1, src_ln1, reached1);
    retrans(2, nstates2, start2, src_ln2, reached2);
    retrans(3, nstates3, start3, src_ln3, reached3);
    retrans(4, nstates4, start4, src_ln4, reached4);
    RealTrans(0, nstates0, start0);
    RealTrans(1, nstates1, start1);
    RealTrans(2, nstates2, start2);
    RealTrans(3, nstates3, start3);
    RealTrans(4, nstates4, start4);
    if (state_tables)
    {	printf("\nTransition Type: ");
        printf("A=atomic;\n");
        printf("Source-State Labels: ");
		printf("p=progress; e=end; a=accept;\n");
		exit(0);
	}
		now.x = 0;
		now.y = 1;
	UnBlock;	/* disable rendez-vous */
	stack	= ( Stack *) emalloc(sizeof(Stack));
	svtack	= (Svtack *) emalloc(sizeof(Svtack));
	/* a place to point for Pptr of non-running procs: */
	noptr	= (uchar *) emalloc(Maxbody * sizeof(char));
	addproc(0);	/* init */
	trpt = &trail[0];
       init_structures();
       new_state_PO();
}

assert(a, s, ii, tt, t)
	char *s;
	Trans *t;
{	if (!a)
	{	printf("assertion violated %s", s);
		uerror("aborted");
               return 0;
	}
       return 1;
}
#ifndef NOBOUNDCHECK
Boundcheck(x, y, a1, a2, a3)
	Trans *a3;
{	assert((x >= 0 && x < y), "- invalid array index", a1, a2, a3);
	return x;
}
#endif
r_ck(which, N, M, src)
	uchar *which;
	short *src;
{	int i, m=0;

	printf("unreached in proctype %s:\n", procname[M]);
	for (i = 1; i < N; i++)
	  if (which[i] == 0)
	  	xrefsrc(src[i], M, i);
	  else
	    m++;
	printf("	(%d of %d states)\n", N-1-m, N-1);
}

xrefsrc(lno, M, i)
{
	printf("\tline %d, state %d", lno, i);
	if (trans[M][i] && trans[M][i]->tp)
	{	if (strcmp(trans[M][i]->tp, "") != 0)
			printf(", \"%s\"", trans[M][i]->tp);
		else if (stopstate[M][i])
			printf(", -endstate-");
	} else
		printf(", ?");
	printf("\n");
}

putrail()
{	int fd, i, j, q;
	char snap[64];

       if(tmp_trpt)
	  trpt=tmp_trpt;
	if ((fd = creat("pan.trail", 0666)) <= 0)
	{	printf("cannot create pan.trail\n");
		return;
	}
	for (i = 1, j = 0; i<=(int)(trpt-trail); i++)
	{	q = trail[i].pr;
		if (i == depthfound)
			write(fd, "-1:-1:-1:-1\n", 12);
		if (trail[i].o_t->ist)
		{  sprintf(snap, "%d:%d:%d:%d\n", j++,
				q, trail[i].o_t->ist, i);
		   write(fd, snap, strlen(snap));
		}
		sprintf(snap, "%d:%d:%d:%d\n", j++, 
			q, trail[i].o_t->st, i);
		write(fd, snap, strlen(snap));
	}
	printf("pan: wrote pan.trail\n");
	close(fd);
}

sv_save()	/* push state vector onto save stack */
{	if (!svtack->nxt)
	{  svtack->nxt = (Svtack *) emalloc(sizeof(Svtack));
	   svtack->nxt->body = emalloc(vsize*sizeof(char));
	   svtack->nxt->lst = svtack;
	   svtack->nxt->m_delta = vsize;
	   svmax++;
	} else if (vsize > svtack->nxt->m_delta)
	{  svtack->nxt->body = emalloc(vsize*sizeof(char));
	   svtack->nxt->lst = svtack;
	   svtack->nxt->m_delta = vsize;
	   svmax++;
	}
	svtack = svtack->nxt;
#if SYNC
	svtack->o_boq = boq;
#endif
	svtack->o_delta = vsize;
	memcpy((char *)(svtack->body), (char *)&now, vsize);
}

sv_restor()	/* pop state vector from save stack */
{	memcpy((char *)&now, svtack->body, svtack->o_delta);
#if SYNC
	boq = svtack->o_boq;
#endif
	if (vsize != svtack->o_delta)
		Uerror("sv_restor");
	if (!svtack->lst)
		Uerror("error: v_restor");
	svtack  = svtack->lst;
}

p_restor(h)
{	int i; char *z = (char *) &now;
	proc_offset[h] = stack->o_offset;
	proc_skip[h]   = stack->o_skip;
       ct[h]=stack->o_ct;
	vsize += stack->o_skip;
	memcpy(z+vsize, stack->body, stack->o_delta);
	vsize += stack->o_delta;
	i = stack->o_delqs;
	now._nr_pr += 1;
	if (!stack->lst)	/* debugging */
		Uerror("error: p_restor");
	stack = stack->lst;
	this = pptr(h);
       set_proc_var(h,((P0 *)this)->_t,1);
	while (i-- > 0)
		q_restor();
}

q_restor()
{	char *z = (char *) &now;
	q_offset[now._nr_qs] = stack->o_offset;
	q_skip[now._nr_qs]   = stack->o_skip;
	vsize += stack->o_skip;
	memcpy(z+vsize, stack->body, stack->o_delta);
	vsize += stack->o_delta;
	now._nr_qs += 1;
	if (!stack->lst)	/* debugging */
		Uerror("error: q_restor");
	stack = stack->lst;
}




conddelproc(sav,h)
{      if (h+1 != now._nr_pr) return 0;
       return 1;
}

delproc(sav, h)
{	int d, i=0;
       void *tmp=this;

       this=pptr(h);
       set_proc_var(h,((P0 *)pptr(h))->_t,0);
       this=tmp;
	while (now._nr_qs
	&&     q_offset[now._nr_qs-1] > proc_offset[h])
	{	delq(sav);
		i++;
	}
	d = vsize - proc_offset[h];
	if (sav)
	{	if (!stack->nxt)
		{	stack->nxt = (Stack *)
				emalloc(sizeof(Stack));
			stack->nxt->body = 
				emalloc(Maxbody*sizeof(char));
			stack->nxt->lst = stack;
			smax++;
		}
		stack = stack->nxt;
		stack->o_offset = proc_offset[h];
		stack->o_skip   = proc_skip[h];
		stack->o_delta  = d;
		stack->o_delqs  = i;
               stack->o_ct=ct[h];
		memcpy(stack->body, (char *)pptr(h), d);
	}
	vsize = proc_offset[h];
	now._nr_pr = now._nr_pr - 1;
	memset((char *)pptr(h), 0, d);
	vsize -= proc_skip[h];
}

	void tp_now()
	{
		int i = 0;
		for(;i<now._nr_pr;i++)
		{
			P0 *p = (P0*)pptr(i);
			printf("sv[%d]: _t=%d, _p=%d\n",i,p->_t,p->_p);
		}
	}
	void tp_oldkey(uchar *oldkey)
	{
		int i = 0;
		for(;i<now._nr_pr;i++)
		{
			P0 *p = (P0*)pptr_oldkey(oldkey,i);
			printf("sv[%d]: _t=%d, _p=%d\n",i,p->_t,p->_p);
		}
	}
	void set_p(int h,int _p)
	{
		P0 *p = (P0*)pptr(h);
		p->_p = _p;
	}
delq(sav)
{	int h = now._nr_qs - 1;
	int d = vsize - q_offset[now._nr_qs - 1];
	if (sav)
	{	if (!stack->nxt)
		{	stack->nxt = (Stack *)
				emalloc(sizeof(Stack));
			stack->nxt->body = 
				emalloc(Maxbody*sizeof(char));
			stack->nxt->lst = stack;
			smax++;
		}
		stack = stack->nxt;
		stack->o_offset = q_offset[h];
		stack->o_skip   = q_skip[h];
		stack->o_delta  = d;
		memcpy(stack->body, (char *)qptr(h), d);
	}
	vsize = q_offset[h];
	now._nr_qs = now._nr_qs - 1;
	memset((char *)qptr(h), 0, d);
	vsize -= q_skip[h];
}

endstate()
{	int i; P0 *ptr;
	for (i = 0; i < now._nr_pr; i++)
	{
		ptr = (P0 *) pptr(i);
		if (!stopstate[ptr->_t][ptr->_p])
			return 0;
	}
	return 1;
}

void update_p(int pid)
{
	P0 *ptr;
	int i;
	for(i=0;i<now._nr_pr;i++)
	{
		ptr = (P0*)pptr(i);
		int _t = ptr->_t;
		if(pid==_t)
		{
			switch(_t)
			{
				case 1:
					ptr->_p = endstate1;
					break;
				case 2:
					ptr->_p = endstate2;
					break;
				case 3:
					ptr->_p = endstate3;
					break;
				case 4:
					ptr->_p = endstate4;
					break;
			}
		}
	}
}
void reset_p(int pid)
{
	P0 *ptr;
	int i;
	for(i=0;i<now._nr_pr;i++)
	{
		ptr = (P0*)pptr(i);
		int _t = ptr->_t;
		if(pid==_t)
		{
			switch(_t)
			{
				case 1:
					ptr->_p = 1;
					break;
				case 2:
					ptr->_p = 1;
					break;
				case 3:
					ptr->_p = 1;
					break;
				case 4:
					ptr->_p = 1;
					break;
			}
		}
	}
}
void reset_var(int gid)
{
	if(gid>0)
	{
		switch(gid)
		{
			case 1:
				now.y = 1;
				break;
		}
	}
}
void print_now()
{
	printf("now{ x=%d,y=%d}\n",now.x,now.y);
}
	int match_now(int x,int y)
	{
		if((now.x==x)&&(now.y==y))
			return 1;
		else
			return 0;
	}
#include "pan.t"
#include "pan.v"
#include "pan.cond"
#include "pan.vi"
#include "pan.vt"
#include "pan.p"
#include "pan.m"
#include "pan.b"
#include "pan.x"
#include "pan.tp"
#define EMPTY 1
#define ONE 2
#define FULL 4
#define FULLMONE 8
#define RDV 16


#include "pan.rdv"







uchar
same_trans(t1,t2)
     TRANS *t1,*t2;
{
  if((*t1).n.type != (*t2).n.type)
    return 0;
  switch((*t1).n.type)
    {
    case NORMAL_type:
    case ATOMIC_type:
      return (t1==t2);
    case RDV_type:
      return ((*t1).rdv.tr==(*t2).rdv.tr && (*t1).rdv.pr==(*t2).rdv.pr
	      && (*t1).rdv.rcv_tr==(*t2).rdv.rcv_tr && (*t1).rdv.rcv_pr==(*t2).rdv.rcv_pr);
    default:
      Uerror("bad type of transition");
    }
}

insert_p(plst,Pid,What) 
     short *plst,Pid,What;
{
  if(Pid>=MAXPROC)
    Uerror("too many processes, try a greater value of MAXPROC (see pan.h)\n");
  if(What)
    set(plst,Pid);
  else
    unset(plst,Pid);
}

exec(t,pr,flag,fo,flag_par)//flag_par=(-1,gid)
    Trans *t;
    short pr,flag;
    FILE *fo;
    int flag_par;
{
    II=pr;
    this = pptr(II); 
    ot = (uchar) ((P0 *)this)->_t; 
    tt = (short) ((P0 *)this)->_p;
    if(flag_par == -1)
    {
        trpt++;
        trpt->o_t  =  t; 
        trpt->o_ot = ot;
        trpt->o_tt = tt;
        trpt--;
    }
    else
    {
    /*    trpt_par[flag_par]++;
        trpt_par[flag_par] = (Trail)emalloc(sizeof(Trail));
        trpt_par[flag_par]->o_t  =  t; 
        trpt_par[flag_par]->o_ot = ot;
        trpt_par[flag_par]->o_tt = tt;
        trpt_par[flag_par]--;
    */
        trpt++;
        trpt->o_t  =  t; 
        trpt->o_ot = ot;
        trpt->o_tt = tt;
        trpt--;

        trpt_par[flag_par]++;
     //   (trpt_par[flag_par]) = (Trail *)emalloc(sizeof(Trail));
        trpt_par[flag_par]->o_t  =  t; 
        trpt_par[flag_par]->o_ot = ot;
        trpt_par[flag_par]->o_tt = tt;
        trpt_par[flag_par]--;
    }
    if(t->forw==13) 
    { 
        printf("exec:t->forw=%d\n",t->forw);
    } 
    move(t,1,flag_par);

    /*if(fo!=NULL)
      {
      fprintf(fo,"Exec %d:proc %d (%s) exec trans %d (%s)\n",depth,II,procname[ot],t->forw,t->tp);
      fflush(fo);
#ifdef STDO
printf("\033[0;31mExec %d:proc %d (%s) exec trans %d (%s)\033[0m\n",depth,II,procname[ot],t->forw,t->tp);
#endif
}*/
    if(flag_par == -1)
    {
        trpt++;
    //    printf("****exec:trpt=%s****\n",trpt->o_t->tp);
    }
    else
    {
        trpt_par[flag_par]++;
        trpt++;
    //    printf("****exec:trpt=%d,trpt_par=%d****\n",trpt,trpt_par[flag_par]);
    //    printf("****exec:trpt=%s,trpt_par=%s****\n",trpt->o_t->tp,trpt_par[flag_par]->o_t->tp);
    }
    

    if(flag)
    {
        if((int)
            (trpt-trail)>=MAX_TRAIL)
        if(!double_trail())
            Uerror("MAX_TRAIL too small\n");
    }
    else
    if((int)
        (trpt-trans_def_trail)>=MAX_TRANS_DEF_TRAIL)
    if(!double_trans_def_trail())
        Uerror("MAX_TRANS_DEF_TRAIL too small\n");

    if(flag_par == -1)
        trpt->pr = II; 
    else
    {
        trpt_par[flag_par]->pr = II;
        trpt->pr = II; 
    }
    if (t->st)
    {
        ((P0 *)this)->_p = t->st;
        if(flag)
            reached[ot][t->st] = 1; 
    }
    return 1;
}

undo_all_trans(int flag_par)
{
    Trans *t;
    if(flag_par == -1)
    {
        while(trpt!=trans_def_trail)
        {
           // printf("undo_all_trans:trpt:%s,t->back=%d\n",trpt->o_t->tp,trpt->o_t->back);
            //printf("tr_def_in(): trans_def_trail:%s,t->back=%d\n",(trans_def_trail)->o_t->tp,(trans_def_trail)->o_t->back);
            t  = trpt->o_t; 
            ot = trpt->o_ot; II = trpt->pr;
            tt = trpt->o_tt; this = pptr(II);
            ((P0 *)this)->_p=tt;
            back(t);
            trpt--;
        }
    }
    else
    {
        //while(trpt_par[flag_par] != trans_def_trail)
        while((trpt_par[flag_par] != trans_def_trail)&&trpt_par[flag_par]->o_t)
        {
         //   printf("undo_all_trans:trpt_par[%d]:%s,t->back=%d\n",flag_par,trpt_par[flag_par]->o_t->tp,trpt_par[flag_par]->o_t->back);
           // printf("tr_def_in(): trans_def_trail:%s,t->back=%d\n",(trans_def_trail)->o_t->tp,(trans_def_trail)->o_t->back);
            t  = trpt_par[flag_par]->o_t; 
            ot = trpt_par[flag_par]->o_ot;
            II = trpt_par[flag_par]->pr;
            tt = trpt_par[flag_par]->o_tt; 
            this = pptr(II);
            ((P0 *)this)->_p=tt;
            back(t);
            trpt_par[flag_par]--;
        }

    }
}

undo_one_trans(int flag_par)
{
    Trans *t;
    
    if(flag_par == -1)
    {
        t  = trpt->o_t; 
        ot = trpt->o_ot; 
        II = trpt->pr;
        tt = trpt->o_tt;
    }
    else
    {
        t  = trpt_par[flag_par]->o_t; 
        ot = trpt_par[flag_par]->o_ot; 
        II = trpt_par[flag_par]->pr;
        tt = trpt_par[flag_par]->o_tt;

    }
    this = pptr(II);
    ((P0 *)this)->_p=tt;
    back(t);
    if(flag_par == -1)
        trpt--;
    else
        trpt_par[flag_par]--;
}

init_new_proc(h,type)
     short type;
     short h;
{
  short i,j;
  Trans *t;

  ct[h]=(struct ConfTag **)emalloc((last_tr[type]-first_tr[type]+1)
				  *(sizeof (struct ConfTag *)));
  setvar_size(h,type); 
  setvar(type,h); 

  rtrans[h]=(Rtrans_stat **)emalloc(nst[type]*sizeof(Rtrans_stat *));
  for(i=0;i<nst[type];i++)
    {
      for(j=0,t=trans[type][i];t;t=t->nxt,j++)
	{
	  if(t->atom&2)
	    t->rtype=ATOMIC_type;
	  else
	    t->rtype=NORMAL_type;
	}
      rtrans[h][i]=(Rtrans_stat *)emalloc(j*sizeof(Rtrans_stat));
      for(j=0,t=trans[type][i];t;t=t->nxt,j++)
	{
	  rtrans[h][i][j].type=t->rtype;
	  rtrans[h][i][j].pr=h;
	  rtrans[h][i][j].tr=t;
	}
    }
}

update_ch(x) 
{
  char r=0;
  short l=q_len(x);

  if (l==0) r|= EMPTY;
  if (l==1) r|= ONE;
  if (l==q_size(x)) r|= FULL;
  if (l==q_size(x)-1) r|= FULLMONE;
#if SYNC
  if (q_zero(x)) r|=RDV;
#endif
  return (r);
}


uchar *visited;
short nxt_st;

RealTrans(n,m,is) 
{
  visited=(uchar *)emalloc(m*sizeof(uchar));

  dfs_rt(n,is);
  efree(visited);
}

Trans **
link(n,s,lk)
     Trans **lk;
{
  Trans *t;

  if(visited[s])
    return(lk);
  
  visited[s]=1;
  for(t=trans[n][s];t;t=t->nxt)
    {
      t->rtype=ATOMIC_type;
      *lk=t;
      if(t->atom&2)
	lk=link(n,t->st,&(t->seq));
      else
	{
	  dfs_rt(n,t->st);
	  return(&(t->seq));
	}
    }
  return(lk);
}

dfs_rt(n,s)
{
  Trans *t;

  if(visited[s])
    return;
  visited[s]=1;
  
  for(t=trans[n][s];t;t=t->nxt)
    {
      if(t->atom&2)
	{
	  t->rtype=ATOMIC_type;
	  link(n,t->st,&(t->seq));
	}
      else
	{
	  t->rtype=NORMAL_type;
	  dfs_rt(n,t->st);
	}
    }
}

print_trans(f,t)
     FILE *f;
     TRANS *t;
{
  switch((*t).n.type)
    {
    case NORMAL_type:
      fprintf(f,"Proc %d (%s), trans %d (%s)\n",(*t).n.pr,
	     procname[(uchar)((P0 *)pptr((*t).n.pr))->_t],
	     (*t).n.tr->forw,(*t).n.tr->tp);
      break;
    case ATOMIC_type:
      fprintf(f,"Proc %d (%s), trans ATOM %d (%s)\n",(*t).n.pr,
	     procname[(uchar)((P0 *)pptr((*t).n.pr))->_t],
	     (*t).n.tr->forw,(*t).n.tr->tp);
      break;
    case RDV_type:
      fprintf(f,"RDV Proc %d (%s), trans %d (%s)\n",(*t).rdv.pr,
	     procname[(uchar)((P0 *)pptr((*t).rdv.pr))->_t],
	     (*t).rdv.tr->forw,(*t).rdv.tr->tp);
      fprintf(f,"\tand Proc %d (%s), trans %d (%s)\n",(*t).rdv.rcv_pr,
	     procname[(uchar)((P0 *)pptr((*t).rdv.rcv_pr))->_t],
	     (*t).rdv.rcv_tr->forw,(*t).rdv.rcv_tr->tp);
      break;
    }
}

double_trail()  
{
  Trail *tmppt,*tmppt0,*tmppt1;
  int ndpth;

  if (MAX_TRAIL<100000)
    ndpth = MAX_TRAIL*2;
  else
    ndpth = MAX_TRAIL+50000;

#ifdef DYNSTRUCTCONT
  printf("doubling trail, newsiz=%d\tmemory=%d\n",ndpth,ndpth*sizeof(Trail));
#endif  
  
  tmppt = (Trail *)emalloc((ndpth+2)*sizeof(Trail));

  if (tmppt)
    {
      memcpy(tmppt,trail,(MAX_TRAIL+10)*sizeof(Trail));
      efree(trail,(MAX_TRAIL+2)*sizeof(Trail));
      trpt = tmppt+(trpt-trail);
      trail = tmppt;
      tmppt0 = (Trail *)emalloc((ndpth+2)*sizeof(Trail));
      if(tmppt0)
      {
          memcpy(tmppt0,trail,(MAX_TRAIL + 10)*sizeof(Trail));
          efree(trail,(MAX_TRAIL + 2)*sizeof(Trail));
          trpt_par[0] = tmppt0+(trpt_par[0] - trail);
          trail = tmppt0;
      }
      
      tmppt1 = (Trail *)emalloc((ndpth+2)*sizeof(Trail));
      if(tmppt1)
      {
          memcpy(tmppt1,trail,(MAX_TRAIL + 10)*sizeof(Trail));
          efree(trail,(MAX_TRAIL + 2)*sizeof(Trail));
          trpt_par[1] = tmppt1+(trpt_par[1] - trail);
          trail = tmppt1;
      }
      
      MAX_TRAIL = ndpth;
      return 1;
    }
  else
    return 0;
}

double_trans_def_trail()  
{
  Trail *tmppt;
  int ndpth;
  
  if (MAX_TRANS_DEF_TRAIL<100000)
    ndpth = MAX_TRANS_DEF_TRAIL*2;
  else
    ndpth = MAX_TRANS_DEF_TRAIL+50000;
  
#ifdef DYNSTRUCTCONT
  printf("doubling trans_def_trail, newsiz=%d\tmemory=%d\n",ndpth,ndpth*sizeof(Trail));
#endif  
  tmppt = (Trail *)emalloc((ndpth+2)*sizeof(Trail));

  if (tmppt)
    {
      memcpy(tmppt,trans_def_trail,(MAX_TRANS_DEF_TRAIL+2)*sizeof(Trail));
      efree(trans_def_trail,(MAX_TRANS_DEF_TRAIL+2)*sizeof(Trail));
      trpt = tmppt+(trpt - trans_def_trail);
      trans_def_trail = tmppt; 
      MAX_TRANS_DEF_TRAIL = ndpth;
      return 1;
    }
  else
    return 0;
}


int RDV_MEM;

#if SYNC
short RDVsize=14;
int RDVmask;

typedef struct RDV_el {
  Rtrans_rdv rdv;
  struct RDV_el *nxt;
} RDV_el;

RDV_el **RDV_table;

int
RDV_hash(cp, om)
     uchar *cp;
{
  register long z = 0x88888EEFL;
  register long *q;
  register int h;
  
  register m = -1;
  h = (om+3)>>2;
  q = (long *) cp;
  do {
    m += m;
    if (m < 0)
      m ^= z;
    m ^= *q++;
  } while (--h > 0);
  return((m ^ (m>>(8*sizeof(unsigned)-RDVsize)))&RDVmask);
}

RDV_table_init()
{
  RDVmask = ((1<<RDVsize)-1);
  RDV_table = (struct RDV_el **)
    emalloc((1<<RDVsize)*sizeof(struct RDV_el *));
}

TRANS *
new_RDV(t)
     TRANS *t;
{
  int key=RDV_hash((uchar *)t,sizeof(TRANS));
  RDV_el *tmp;
  
  tmp=RDV_table[key];
  if(!tmp)
    tmp=RDV_table[key]=(RDV_el *)emalloc(sizeof(RDV_el));
  else
    {
      for(;;tmp=tmp->nxt)
	{
	  if(same_trans(t,(TRANS *)&(tmp->rdv)))
	    return ((TRANS *)&tmp->rdv);
	  if(!tmp->nxt)
	    break;
	}
      tmp->nxt=(RDV_el *)emalloc(sizeof(RDV_el));
      tmp=tmp->nxt;
    }
  tmp->rdv=*(Rtrans_rdv *)t;
  RDV_MEM+=sizeof(RDV_el);
  return ((TRANS *)&tmp->rdv);
}
#endif

TRANS *
add_new_trans(t)
     TRANS *t;
{
  Rtrans_rdv *res;
  switch((*t).n.type)
    {
    case RDV_type:
#if SYNC
      return(new_RDV(t));
#else
      Uerror("bad type of transition");
#endif
    case NORMAL_type:
    case ATOMIC_type:
    default:
      Uerror("bad type of transition");
      break;
    }
}






init_structures()
{
  ct_g=(ConfTag *)emalloc(MAX_ct_g*sizeof(ConfTag));
#if SYNC
  RDV_table_init();
#endif
}





char *
addr_current_state()
{
    printf("now:_nr_pr=%d,_nr_qs=%d,_p_t=%d,_a_t=%d,x=%d,y=%d,sv=%d\n",now._nr_pr,now._nr_qs,now._p_t,now._a_t,now.x,now.y,now.sv[0]);
  return ((char *)&now);
}

execute(tr,fo,flag_par)//flag_par = (-1:trpt,gid:trpt_par[gid])
     TRANS *tr;
     FILE  *fo;
     int flag_par;
{
    Trans *t;
    uchar ot;
    short tt;
    short nb_trans=0;

    switch((*tr).n.type)
    {
        case NORMAL_type:
            exec((*tr).n.tr,(*tr).n.pr,1,fo,flag_par);
            nb_trans++;
            break;
        case ATOMIC_type:
            II=(*tr).n.pr;
            this = pptr(II); 
            ot = (uchar) ((P0 *)this)->_t; 
            exec((*tr).n.tr,II,1,fo,flag_par);
            nb_trans++;
            do{
                tt = (short) ((P0 *)this)->_p; 
                for(t=trans[ot][tt];t;t=t->nxt)
                    if(precond(t))
                    {
                        exec(t,II,1,fo,flag_par);
                        nb_trans++;
                        break;
                    }
                if(!t)
                    Uerror("atomic sequence block");
            }while(t->atom&2);
            break;
        case RDV_type:
            exec((*tr).rdv.tr,(*tr).rdv.pr,1,fo,flag_par);
            nb_trans++;
            exec((*tr).rdv.rcv_tr,(*tr).rdv.rcv_pr,1,fo,flag_par);
            nb_trans++;
            break;
        default:
            Uerror("bad transition type");
            break;
    }
    //wyy_beg
    //      trpt->nb_trans=nb_trans;
    if(flag_par == -1)
        trpt->nb_trans=nb_trans;
    else 
    {
        trpt_par[flag_par]->nb_trans = nb_trans;
        trpt->nb_trans=nb_trans;
    }
    //wyy_end
}

backtr(fo,flag_par)//flag_par(-1:trpt,gid:trpt_par[gid])
{
    Trans *t;
    short i;
    short nb_trans;
    if(flag_par == -1)
        nb_trans = trpt->nb_trans;
    else
        nb_trans = trpt_par[flag_par]->nb_trans;

//    printf("******backtr:nb_trans=%d******\n");
    for(i=0;i<nb_trans;i++)
    {
        if(flag_par == -1)
        {
            t  = trpt->o_t; 
            ot = trpt->o_ot; 
            II = trpt->pr;
            tt = trpt->o_tt; 
        }
        else
        {
            t  = trpt_par[flag_par]->o_t; 
            ot = trpt_par[flag_par]->o_ot; 
            II = trpt_par[flag_par]->pr;
            tt = trpt_par[flag_par]->o_tt; 
        }
        this = pptr(II);
        ((P0 *)this)->_p=tt;
        if(t->forw==13) 
        { 
            printf("backtr:t->forw=%d\n",t->forw);
        }
        //back(t,flag_par);
        back(t,flag_par);
        //wyy_beg
        printf("backtr:t->back:%d,backtr:trpt=%s\n",t->back,t->tp);
        if(flag_par == -1)
            trpt--;
        else
            trpt_par[flag_par]--;

        /*if(fo!=NULL)
          {
          fprintf(fo,"Back %d:proc %d (%s) back trans %d (%s)\n",depth,II,
          procname[ot],t->forw,t->tp);
          fflush(fo);
          }*/
    }
}



short
nb_proc()
{
  return(now._nr_pr);
}

short
tr_def_in(int flag_par)
{
    uchar flag;
    short i=0;
    short j,tmp_II;
    Trans *t,*tmp_t;
    TRANS *tr_;
    short tmp_boq;
    Rtrans_rdv rdv;
    short res=0;
    uchar lot;
    short ltt,lII;

    boq=-1;
    if(flag_par == -1)
    {
        tmp_trpt = trpt;
        trpt = trans_def_trail;
    }
    else
    {
        tmp_trpt = trpt_par[flag_par];
        trpt_par[flag_par] = trans_def_trail;
    }
   // printf("tr_def_in(): trans_def_trail:%s,t->back=%d\n",(trans_def_trail)->o_t->tp,(trans_def_trail)->o_t->back);
    for(lII=now._nr_pr-1;lII>=0;lII--) 
    {
        P_size[lII]=0;
        this=pptr(lII);
        ltt=(short) ((P0 *)this)->_p;
        lot=(uchar) ((P0 *)this)->_t;
        t=trans[lot][ltt];
        //wyy_beg
        printf("trans[%d][%d]=%s\n",lot,ltt,t->tp);
        //wyy_end
        for(j=i=0;t;t=t->nxt,j++) 
        {
            switch(t->rtype)
            {
                case ATOMIC_type:
                    tmp_t=t;
                    flag=0;
                    tr_=(TRANS *) &rtrans[lII][ltt][j];
                    trans_def[lII][i].tr=tr_;
                    if(precond(t))
                        do
                        {
                            exec(t,lII,0,0,flag_par);
                            tt=(short) ((P0 *)this)->_p;
                            flag=0;
                            fflush(stdout);
                            for(t=trans[lot][tt];t;t=t->nxt)
                            {
                                if(precond(t))
                                {
                                    flag=1;
                                    break;
                                }
                            }
                            if(!flag)
                                Uerror("atomic sequence block\n");
                        } while(flag && (t->atom&2));
                    if(flag) exec(t,lII,0,0,flag_par);
                    t=tmp_t;
                    if(flag)
                    {
                        trans_def[lII][i].enabled=1;
                        res++;
                        trans_def[lII][i].in_stack=is_in_stack();
#ifdef SLEEP
                        if(is_sleeping(trans_def[lII][i].tr))
                            trans_def[lII][i].in_sleep=1;
                        else
                        {
                            trans_def[lII][i].in_sleep=0;
                            P_size[lII]++;
                        }
#else
                        P_size[lII]++;
#endif
                    }
                    else
                    {
                        trans_def[lII][i].enabled=0;
                        trans_def[lII][i].in_stack=0;
                        trans_def[lII][i].in_sleep=0;
                    }
                    undo_all_trans(flag_par);
                    i++;
                    break;

                case NORMAL_type:
                    if(is_rdv(t))
                    {
                        rdv.type=RDV_type;
                        rdv.pr=lII;
                        rdv.tr=t;
                        if(precond(t))
                        {
                            exec(t,lII,0,0,flag_par);
                            if(boq==-1) Uerror("boq\n");
                            tmp_II=lII;tmp_t=t; tmp_boq=boq;

                            for(lII=nb_proc()-1;lII>=0;lII--)
                            {
                                if(lII==tmp_II)
                                    continue;
                                this=pptr(lII);
                                tt=(short) ((P0 *)this)->_p;
                                lot=(uchar) ((P0 *)this)->_t;
                                for(t=trans[lot][tt];t;t=t->nxt)

                                    if(precond(t))
                                    {

                                        exec(t,lII,0,0,flag_par);
                                        rdv.rcv_tr=t;
                                        rdv.rcv_pr=lII;
                                        trans_def[tmp_II][i].enabled=1;
                                        res++;
                                        trans_def[tmp_II][i].in_stack=is_in_stack();
#ifdef SLEEP
                                        if(is_sleeping((TRANS *)&rdv))
                                            trans_def[tmp_II][i].in_sleep=1;
                                        else
                                        {
                                            trans_def[tmp_II][i].in_sleep=0;
                                            P_size[tmp_II]++;
                                        }
#else
                                        P_size[tmp_II]++;
#endif

                                        trans_def[tmp_II][i++].tr=
                                            add_new_trans((TRANS *)&rdv);

                                        rdv.type=RDV_type;
                                        rdv.pr=tmp_II;
                                        rdv.tr=tmp_t;
                                        undo_one_trans(flag_par);
                                        boq=tmp_boq;
                                    }
                            }
                            boq=-1;
                            lII=tmp_II;t=tmp_t;
                            rdv.rcv_tr=0;
                            rdv.rcv_pr=0;
                            trans_def[lII][i].tr=
                                add_new_trans((TRANS *)&rdv);
                            trans_def[lII][i].enabled=0;
                            trans_def[lII][i].in_stack=0;
                            trans_def[lII][i].in_sleep=0;
                        }
                        else
                        {
                            rdv.rcv_tr=0;
                            rdv.rcv_pr=0;
                            trans_def[lII][i].tr=
                                add_new_trans((TRANS *)&rdv);
                            trans_def[lII][i].enabled=0;
                            trans_def[lII][i].in_stack=0;
                            trans_def[lII][i].in_sleep=0;
                        }
                    }
                    else
                    {
                        trans_def[lII][i].tr=(TRANS *) &rtrans[lII][ltt][j];
                        if(precond(t))
                        {
                            trans_def[lII][i].enabled=1;
                            res++;
                            exec(t,lII,0,0,flag_par);
                          //printf("trans_def[%d][%d]=%s\n",lII,i,trans_def[lII][i].tr->n.tr->tp);
                            trans_def[lII][i].in_stack=is_in_stack();
#ifdef SLEEP
                            if(is_sleeping(trans_def[lII][i].tr))
                                trans_def[lII][i].in_sleep=1;
                            else
                            {
                                trans_def[lII][i].in_sleep=0;
                                P_size[lII]++;
                            }
#else
                            P_size[lII]++;
#endif
                        }
                        else
                        {
                            trans_def[lII][i].enabled=0;
                            trans_def[lII][i].in_stack=0;
                            trans_def[lII][i].in_sleep=0;
                        }
                    }
                    undo_all_trans(flag_par);
                    i++;
                    break;

                default:
                    Uerror("bad type of transition");
            }//end switch
        }//end for
        if(i>=MAX_TRANS_DEF)
            Uerror("MAX_TRANS_DEF too small, try a greater value\n");
        trans_def[lII][i].tr=0;
    }
    if(flag_par == -1)
        trpt=tmp_trpt;
    else
        trpt_par[flag_par] = tmp_trpt;
    tmp_trpt=0;
    return res;
}

short
trans_def_in(int flag_par)
{
  short nb_tr_enabled;

  timeout_enabled=0;
  if(!(nb_tr_enabled=tr_def_in(flag_par))) 
    {
      timeout_enabled=1;
      nb_tr_enabled=tr_def_in(flag_par);
    }
  return nb_tr_enabled;  
}

addpr(acc)
     ACC acc;
{
  short i,mis;
  
  mis=acc.misc;
  switch(acc.op)
    {
    case READ:
      for(i=0;i<nb;i++)
	P_1[i] |= Proc_vm[acc.var][acc.ind][i];
      break;
    case WRITE:
      for(i=0;i<nb;i++)
	{
	  P_1[i] |= Proc_vu[acc.var][acc.ind][i];
	  P_1[i] |= Proc_vm[acc.var][acc.ind][i];
	}
      break;
    case SEND:
      for(i=0;i<nb;i++)
	{
#if SYNC
	  if(mis&RDV)
	    {
	      P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	      P_1[i] |= Proc_cr[acc.var][acc.ind][i];
	    }
	  else
	    {
#endif
	      if(mis&FULL)
		P_1[i] |= Proc_cr[acc.var][acc.ind][i];
	      else
		{
		  P_1[i] |= Proc_cu[acc.var][acc.ind][i];
		  P_1[i] |= Proc_cs[acc.var][acc.ind][i];
		  P_1[i] |= Proc_ct[acc.var][acc.ind][i];
		  P_1[i] |= Proc_ce[acc.var][acc.ind][i];
#ifdef NEWDEP
		  P_1[i] |= Proc_cf[acc.var][acc.ind][i];
#endif
		}	    
#ifndef NEWDEP
	      if(mis&FULLMONE)
		P_1[i] |= Proc_cf[acc.var][acc.ind][i];
#endif
#if SYNC
	    }
#endif
	}
      break;
    case RECV:
      for(i=0;i<nb;i++)
	{
#if SYNC
	  if(mis&RDV)
	    {
	      P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	      P_1[i] |= Proc_cr[acc.var][acc.ind][i];
	    }
	  else
	    {
#endif	    
	      if(M_LOSS)
		P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	      if(mis&EMPTY)
		P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	      else
		{
		  P_1[i] |= Proc_cr[acc.var][acc.ind][i];
		  P_1[i] |= Proc_ct[acc.var][acc.ind][i];
		  P_1[i] |= Proc_cf[acc.var][acc.ind][i];
		  P_1[i] |= Proc_cu[acc.var][acc.ind][i];
#ifdef NEWDEP
		  P_1[i] |= Proc_ce[acc.var][acc.ind][i];
#endif
		}
#ifndef NEWDEP
	      if(mis&ONE)
		P_1[i] |= Proc_ce[acc.var][acc.ind][i];
#endif
#if SYNC
	    }
#endif
	}
      break;
      
    case TRCV:
#if SYNC
      if(!(mis&RDV))
#endif
	for(i=0;i<nb;i++)
	  {
	    if(mis&EMPTY)
	      P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	    else
	      P_1[i] |= Proc_cr[acc.var][acc.ind][i];
	  }
      break;
      
    case EMP:
#if SYNC
      if(!(mis&RDV))
#endif
	for(i=0;i<nb;i++)
	  {
	    if(mis&EMPTY)
	      P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	    else
	      P_1[i] |= Proc_cr[acc.var][acc.ind][i];
	  }
      break;
      
    case FUL:
#if SYNC
      if(!(mis&RDV))
#endif
	for(i=0;i<nb;i++)
	  {
	    if(mis&FULL)
	      P_1[i] |= Proc_cr[acc.var][acc.ind][i];
	    else
	      P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	  }
      break;
    case LN:
#if SYNC
      if(!(mis&RDV))
#endif
	for(i=0;i<nb;i++)
	  {
	    if(!(mis&EMPTY))
	      P_1[i] |= Proc_cr[acc.var][acc.ind][i];
	    if(!(mis&FULL))
	      P_1[i] |= Proc_cs[acc.var][acc.ind][i];
	  }
      break;
    case TIMO:
      if(timeout_enabled==1)
	for(i=0;i<nb;i++)
	  P_1[i] |= Proc_to[acc.var][acc.ind][i];
      break;
    }
}

update_conflig_tags(t)
     TRANS *t;
{
  short pr;
  Trans *tr;
  uchar ot;
  
  switch((*t).n.type)
    {
    case NORMAL_type:
      
      pr=(*t).n.pr;
      this=pptr(pr);
      ot=(uchar) ((P0 *)this)->_t;
      setvar_ind((*t).n.tr->forw,pr,ot);
      break;
      
    case ATOMIC_type:
      
      pr=(*t).n.pr;
      this=pptr(pr);
      ot=(uchar) ((P0 *)this)->_t;
      for(tr=(*t).n.tr;tr;tr=tr->seq)
	setvar_ind(tr->forw,pr,ot);	
      break;

    case RDV_type:

      pr=(*t).rdv.pr;
      this=pptr(pr);
      ot=(uchar) ((P0 *)this)->_t;
      setvar_ind((*t).rdv.tr->forw,pr,ot);
	
      if((*t).rdv.rcv_tr)
	{
	  pr=(*t).rdv.rcv_pr;
	  this=pptr(pr);
	  ot=(uchar) ((P0 *)this)->_t;
	  setvar_ind((*t).rdv.rcv_tr->forw,pr,ot);
	}
      break;

    default:
      Uerror("Bad transition type");
      break;
    }
}



ACC
get_acc(t,first,n)
     TRANS *t;
{
  ACC res;
  static short i_[2];
  static Trans *tr_[2];
  static uchar rdv_[2];
  short i,pr;
  Trans *tr;
  uchar rdv;
  int f;
  
  switch((*t).n.type)
    {
    case NORMAL_type:
      
      pr=(*t).n.pr;
      f=(*t).n.tr->forw - first_tr[(uchar) ((P0 *)pptr(pr))->_t];
      if(first)
	i = i_[n] = 0;
      else
	i = ++i_[n];
      
      res.var=(ct[pr][f][i]).var; 
      res.ind=(ct[pr][f][i]).ind; 
      res.op=(ct[pr][f][i]).type;
      res.misc=(ct[pr][f][i]).misc; 
      break;
      
    case ATOMIC_type:
      
      pr=(*t).n.pr;
      if(first)
	{
	  tr=tr_[n]=(*t).n.tr;
	  i=i_[n]=0;
	}
      else
	{
	  tr=tr_[n];
	  i=++i_[n];
	}
      while(tr &&
	    !(ct[pr][tr->forw -first_tr[(uchar) ((P0 *)pptr(pr))->_t]][i]).var)
	{
	  tr=tr_[n]=tr->seq;
	  i=i_[n]=0;
	}
	
      if(tr && (ct[pr][f=tr->forw -
		       first_tr[(uchar) ((P0 *)pptr(pr))->_t]][i]).var)
	{
	  res.var=(ct[pr][f][i]).var; 
	  res.ind=(ct[pr][f][i]).ind; 
	  res.op=(ct[pr][f][i]).type;
	  res.misc=(ct[pr][f][i]).misc; 
	}
      else
	res.var=0;
      break;

    case RDV_type:

      if(first)
	rdv=rdv_[n]=i=i_[n]=0;
      else
	i=++i_[n];
      if(!rdv)
	{
	  pr=(*t).rdv.pr;
	  f=(*t).rdv.tr->forw - first_tr[(uchar) ((P0 *)pptr(pr))->_t];
	  if((ct[pr][f][i]).var)
	    {
	      res.var=(ct[pr][f][i]).var; 
	      res.ind=(ct[pr][f][i]).ind; 
	      res.op=(ct[pr][f][i]).type;
	      res.misc=(ct[pr][f][i]).misc; 
	    }
	  else
	    {
	      rdv=1;
	      i=i_[n]=0;
	    }
	}
      if(rdv)
	if((*t).rdv.rcv_tr)
	  {
	    pr=(*t).rdv.rcv_pr;
	    f=(*t).rdv.rcv_tr->forw - first_tr[(uchar) ((P0 *)pptr(pr))->_t];
	    res.var=(ct[pr][f][i]).var; 
	    res.ind=(ct[pr][f][i]).ind; 
	    res.op=(ct[pr][f][i]).type;
	    res.misc=(ct[pr][f][i]).misc; 
	  }
	else
	  res.var=0;
      break;
    }
  return res;
}

ACC
get_acc_dis(t,first)
     TRANS *t;
{
  ACC res;
  static short i;
  static Trans *tr;
  static uchar rdv;
  short pr;
  int f;
  static ConfTag *cond_ct;
  
  switch((*t).n.type)
    {
    case NORMAL_type:
    case ATOMIC_type: 
      if(first)
	{
	  i=0;
	  pr=(*t).n.pr;
	  this=pptr(pr);
	  f=(*t).n.tr->forw;
	  if(type_tr[f]==1)
	    {
	      CT_cond(f);
	      cond_ct=ct_g;
	    }	  
	  else
	    cond_ct=ct[pr][f - first_tr[(uchar) ((P0 *)pptr(pr))->_t]];
	}
      else
	i++;
      res.var = (cond_ct[i]).var; 
      res.ind=(cond_ct[i]).ind; 
      res.op=(cond_ct[i]).type;
      res.misc=(cond_ct[i]).misc; 
      break;

    case RDV_type:
      
      if(first)
	rdv=i=0;
      else
	i++;
      if(!rdv)
	{
	  pr=(*t).rdv.pr;
	  f=(*t).rdv.tr->forw - first_tr[(uchar) ((P0 *)pptr(pr))->_t];
	  if((ct[pr][f][i]).var)
	    {
	      res.var=(ct[pr][f][i]).var; 
	      res.ind=(ct[pr][f][i]).ind; 
	      res.op=(ct[pr][f][i]).type;
	      res.misc=(ct[pr][f][i]).misc; 
	    }
	  else
	    {
	      rdv=1;
	      i=0;
	    }
	}
      if(rdv)
	if((*t).rdv.rcv_tr)
	  {
	    pr=(*t).rdv.rcv_pr;
	    f=(*t).rdv.rcv_tr->forw - first_tr[(uchar) ((P0 *)pptr(pr))->_t];
	    res.var=(ct[pr][f][i]).var; 
	    res.ind=(ct[pr][f][i]).ind; 
	    res.op=(ct[pr][f][i]).type;
	    res.misc=(ct[pr][f][i]).misc; 
	  }
	else
	  res.var=0;
      break;
    }
  return res;
}

uchar
valid(acc)
     ACC acc;
{
  return(acc.var);
}
same_proc(t1,t2)
     TRANS *t1,*t2;
{
  switch((*t1).n.type)
    {
    case NORMAL_type:
    case ATOMIC_type:
      return same_proc_((*t1).n.pr,t2);
      break;
    case RDV_type:
      return(same_proc_((*t1).rdv.pr,t2)||same_proc_((*t1).rdv.rcv_pr,t2));
      break;
    default:
      Uerror("invalid transition type");
      break;
    }
}

same_proc_(pr,t2)
     TRANS *t2;
     short pr;
{
  switch((*t2).n.type)
    {
    case NORMAL_type:
    case ATOMIC_type:
      return (pr==(*t2).n.pr);
      break;
    case RDV_type:
      return(pr==(*t2).rdv.pr || pr==(*t2).rdv.rcv_pr);
      break;
    default:
      Uerror("invalid transition type");
      break;
    }
}
      
#define D    1
#define FVE  2
#define EM   3
#define FMO  4
#define ON   5
#define FU   6

#define NE   8
#define NF   9

uchar
depend(acc1,acc2) 
     ACC acc1,acc2;
{
  if(acc1.var != acc2.var || acc1.ind != acc2.ind)
    return 0;
#if SYNC
  if(acc1.misc&RDV)
    {
      switch(con_rdv[acc1.op][acc2.op])
	{
	case D:
	  return 1;
	  break;
	default:
	  break;
	}
    }
  else
    {
#endif
      switch(con[acc1.op][acc2.op])
	{
	case D:
	  return 1;
	case FVE:
	  if(acc1.misc&EMPTY || acc2.misc&EMPTY)
	    return 1;
	  if(acc1.misc&FULL || acc2.misc&FULL)
	    return 1;
	  break;
	case EM:
	  if(acc1.misc&EMPTY || acc2.misc&EMPTY)
	    return 1;
	  break;
	case NE:
	  if(!(acc1.misc&EMPTY || acc2.misc&EMPTY))
	    return 1;
	  break;
	case FMO:
	  if(acc1.misc&FULLMONE || acc2.misc&FULLMONE)
	    
	    return 1;
	  break;
	case ON:
	  if(acc1.misc&ONE || acc2.misc&ONE)
	    return 1;
	  break;
	case FU:
	  if(acc1.misc&FULL || acc2.misc&FULL)
		    return 1;
	  break;
	case NF:
	  if(!(acc1.misc&FULL || acc2.misc&FULL))
	    return 1;
	  break;
	  
	default:
	  break;
	}
#if SYNC
    }
#endif
  return 0;      
}




#include "pan.av"

extern_option(arg)
     char arg;
{
  switch (arg)
    {
    default: return 0;
    case 'V': printf("Generated by %s\n", Version);
      exit(0); break;
    case 'd': state_tables++; break;
    }
  return 1;
}

extern_usage()
{
  fprintf(stderr, "-d  print state tables and stop\n");
  fprintf(stderr, "-d -d  print un-optimized state tables\n");
  fprintf(stderr, "-V  print SPIN version number and exit\n");
}  

extern_wrapup(done)
     uchar done;
{
  //printf("stackframes: %d/%d)\n\n", smax, svmax);
#ifdef MEMCNT
  //printf("RDV memory used: %d\n", RDV_MEM);
#endif
  if (done) do_reach();
}

do_reach()
{
	r_ck(reached0, nstates0, 0, src_ln0);
	r_ck(reached1, nstates1, 1, src_ln1);
	r_ck(reached2, nstates2, 2, src_ln2);
	r_ck(reached3, nstates3, 3, src_ln3);
	r_ck(reached4, nstates4, 4, src_ln4);
}

addqueue(n)
{	int j=0, i = now._nr_qs;
	if (i >= MAXQ)
		Uerror("too many queues");
	switch (n) {
	default: Uerror("bad queue - addqueue");
	}
	if (vsize%WS && (j > WS-(vsize%WS)))
	{	q_skip[i] = WS-(vsize%WS);
		vsize += q_skip[i];
	} else
		q_skip[i] = 0;
	q_offset[i] = vsize;
	now._nr_qs += 1;
	vsize += j;
	if (vsize >= VECTORSZ)
		Uerror("VECTORSZ is too small, edit pan.h");
	memset((char *)qptr(i), 0, j);
	((Q0 *)qptr(i))->_t = n;
	return i+1;
}

qsend(into)
{	int j; uchar *z;

	if (!into--)
	uerror("reference to uninitialized chan name (sending)");
	if (into >= now._nr_qs || into < 0)
		Uerror("qsend bad queue#");
	z = qptr(into);
	switch (((Q0 *)qptr(into))->_t) {
	case 0: printf("queue was deleted\n");
	default: Uerror("bad queue - qsend");
	}
}

q_zero(from)
{	if (!from--)
	uerror("reference to uninitialized chan name (receiving)");
	switch(((Q0 *)qptr(from))->_t) {
	case 0: printf("queue was deleted\n");
	}
	Uerror("bad queue q-zero");
}
q_len(x)
{	if (!x--) uerror("reference to uninitialized chan name");
	return ((Q0 *)qptr(x))->Qlen;
}

q_full(from)
{	if (!from--)
	uerror("reference to uninitialized chan name (sending)");
	switch(((Q0 *)qptr(from))->_t) {
	case -1: printf("queue was deleted\n");
	}
	Uerror("bad queue - q_full");
}

q_size(from)
{	if (!from--)
	uerror("reference to uninitialized chan name (sending)");
	switch(((Q0 *)qptr(from))->_t) {
	case 0: printf("queue was deleted\n");
	}
	Uerror("bad queue - q_size");
}

qrecv(from, slot, fld, done)
{	uchar *z;
	int j, k, r=0;
	if (!from--)
	uerror("reference to uninitialized chan name (receiving)");
	if (from >= now._nr_qs || from < 0)
		Uerror("qrecv bad queue#");
	z = qptr(from);
	switch (((Q0 *)qptr(from))->_t) {
	case 0: printf("queue was deleted\n");
	default: Uerror("bad queue - qrecv");
	}
	return r;
}

printq(queue)
{	uchar *z;
	int j, k, r=0;
	if (!queue--)
	uerror("reference to uninitialized chan name (printq)");
	if (queue >= now._nr_qs || queue < 0)
		Uerror("printq bad queue#");
	z = qptr(queue);
	switch (((Q0 *)qptr(queue))->_t) {
	case 0: printf("queue was deleted\n");
	default: Uerror("bad queue - printq");
	}
}

unsend(into)
{	int m=0, j; uchar *z;
	if (!into--) uerror("reference to uninitialized chan name");
	z = qptr(into);
	j = ((Q0 *)z)->Qlen;
	((Q0 *)z)->Qlen = --j;
	switch (((Q0 *)qptr(into))->_t) {
	default: Uerror("bad queue - unsend");
	}
	return m;
}

unrecv(from, slot, fld, fldvar, strt)
{	int j; uchar *z;
	if (!from--) uerror("reference to uninitialized chan name");
	z = qptr(from);
	j = ((Q0 *)z)->Qlen;
	if (strt) ((Q0 *)z)->Qlen = j+1;
	switch (((Q0 *)qptr(from))->_t) {
	default: Uerror("bad queue - qrecv");
	}
}

deleted_trail(int flag_par)
{
    int k = 0;
    while(trpt_par[flag_par]->o_t)
    {
        k++;
        trpt_cp[flag_par]++;
        trpt_cp[flag_par] = trpt_par[flag_par];
        printf("trpt_cp:%s, trpt_cp=%d,trpt_par=%d\n",trpt_cp[flag_par]->o_t->tp,trpt_cp[flag_par],trpt_par[flag_par]);
        trpt_par[flag_par]--;
        trpt--;
    }
    printf("deleted_trail:copy loop  = %d\n",k);
}

copy_trail(int groupnum)
{
    int k = 0;
    int i = 0;
    for(; i < groupnum; i++)
    {
        while(trpt_cp[i]->o_t)
        {
            k++;
            trpt++;
            // *trpt = *trpt_cp;
            trpt->pr = trpt_cp[i]->pr;
            trpt->st = trpt_cp[i]->st;
            trpt->nb_trans = trpt_cp[i]->nb_trans;
            trpt->o_ot = trpt_cp[i]->o_ot;
            trpt->tau = trpt_cp[i]->tau;
            trpt->o_tt = trpt_cp[i]->o_tt;
            trpt->o_t = trpt_cp[i]->o_t;
            trpt->oval = trpt_cp[i]->oval;
            printf("copy_trail:%s=%d\n",trpt_cp[i]->o_t->tp,trpt_cp[i]->oval);
            trpt->prog = trpt_cp[i]->prog;
            printf("trpt:%s trpt=%d,trpt_cp=%d\n",trpt->o_t->tp,trpt,trpt_cp[i]);
            trpt_cp[i]++;
        }
    }
    printf("copy_trail:number of loop = %d\n",k);
}
