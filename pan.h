#pragma once
#define endclaim	endstate-1

short nstates0=6;	/* _init */
#define endstate0	5
short src_ln0 [] = {
	  0,  26,  27,  28,  30,  30,   0, };
uchar reached0 [] = {
	  0,   0,   0,   0,   0,   0,   0, };

short nstates1=3;	/* Y2 */
#define endstate1	2
short src_ln1 [] = {
	  0,  22,  23,   0, };
uchar reached1 [] = {
	  0,   0,   0,   0, };

short nstates2=4;	/* Y1 */
#define endstate2	3
short src_ln2 [] = {
	  0,  16,  17,  18,   0, };
uchar reached2 [] = {
	  0,   0,   0,   0,   0, };

short nstates3=3;	/* X2 */
#define endstate3	2
short src_ln3 [] = {
	  0,  11,  12,   0, };
uchar reached3 [] = {
	  0,   0,   0,   0, };

short nstates4=4;	/* X1 */
#define endstate4	3
short src_ln4 [] = {
	  0,   5,   6,   7,   0, };
uchar reached4 [] = {
	  0,   0,   0,   0,   0, };
#define SYNC	0
#define ASYNC	0

typedef struct P0 { /* _init */
	unsigned _t : 4; /* proctype */
	unsigned _p : 4; /* state    */
} P0;
typedef struct P1 { /* Y2 */
	unsigned _t : 4; /* proctype */
	unsigned _p : 4; /* state    */
} P1;
typedef struct P2 { /* Y1 */
	unsigned _t : 4; /* proctype */
	unsigned _p : 4; /* state    */
} P2;
typedef struct P3 { /* X2 */
	unsigned _t : 4; /* proctype */
	unsigned _p : 4; /* state    */
} P3;
typedef struct P4 { /* X1 */
	unsigned _t : 4; /* proctype */
	unsigned _p : 4; /* state    */
} P4;
typedef struct P5 { /* _progress */
	unsigned _t : 4; /* proctype */
	unsigned _p : 4; /* state    */
} P5;
#define Version	"[0;32mSpin Version 1.6.5 - October 1994 + Partial Order Package Version 3.1.3[0m"
#define qptr(x)	(((uchar *)&now)+q_offset[x])
#define pptr(x)	(((uchar *)&now)+proc_offset[x])
#define pptr_oldkey(p,x)	(((uchar *)&p)+proc_offset[x])
#define Pptr(x)	((proc_offset[x])?pptr(x):noptr)
#define q_sz(x)	(((Q0 *)qptr(x))->Qlen)

#define MAXQ   	255
#define WS		sizeof(long)   /* word size in bytes */
#ifndef VECTORSZ
#define VECTORSZ	1024           /* sv   size in bytes */
#endif
#define MAXPROC        40
//extern char *memcpy(), *memset();
extern void exit();

typedef struct Stack  {	 /* for queues and processes */
	short o_delta;
	short o_offset;
	short o_skip;
	short o_delqs;
       short *o_sleep;
       struct ConfTag **o_ct;
	char *body;
	struct Stack *nxt;
	struct Stack *lst;
} Stack;

typedef struct Svtack { /* for complete state vector */
	short o_delta;	 /* current size of frame */
	short m_delta;	 /* maximum size of frame */
#if SYNC
	short o_boq;
#endif
	char *body;
	struct Svtack *nxt;
	struct Svtack *lst;
} Svtack;

Trans ***trans;	/* 1 ptr per state per proctype */

short proc_offset[MAXPROC], proc_skip[MAXPROC];
short q_offset[MAXQ], q_skip[MAXQ];
short vsize;		/* vector size in bytes */
short proc_num;
short boq = -1;	/* blocked_on_queue status */
typedef struct State {
	uchar _nr_pr;
	uchar _nr_qs;
	uchar _p_t; /* non-progress detection magic bit */
	uchar _a_t; /* acceptance cycle dectection */
	int x;
	int y;
	uchar sv[VECTORSZ];
} State;
#define start0	1
#define start1	1
#define start2	1
#define start3	1
#define start4	1
#define CONNECT	0 /* accept labels */
uchar *accpstate[6];
uchar *progstate[6];
uchar *reached[6];
uchar *stopstate[6];
typedef struct Q0 {	/* generic q */
	uchar Qlen, _t;
} Q0;
short nb_chan=0;
