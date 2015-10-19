/* Copyright (c) 1993 by University of Liege. 

   All Rights Reserved. This software is distributed free of charge for
   educational and research purposes only.  No guarantee is expressed or
   implied by the distribution of this code.

   Software written by Patrice Godefroid, Didier Pirottin and Pierre
   Wolper, Computer Science Department, University of Liege, with the
   collaboration of Gerard J. Holzmann, AT&T Bell Laboratories. 

   This work was partially supported by the European Community ESPRIT BRA
   projects SPEC (3096) and REACT (6021), and by the Belgian Incentive
   Program ``Information Technology'' -- Computer Science of the future,
   initiated by Belgian State -- Prime Minister's Service -- Science
   Policy Office. The scientific responsibility is assumed by its
   authors.

   Send comments and bug-reports to: po-package@montefiore.ulg.ac.be
 */   
#ifndef _PO_H
#define _PO_H
#define uchar unsigned char

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
	int	               gid;
	char               *proc_name;
	int                pid;
	int				   tid;
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


#ifndef DEADLOCK
#define PROV
#endif
#ifdef BITSTATE
#define DXBITS
#endif
#define MEMCNT
#define SHOW
#define OVER
#define SLEEP

typedef struct DFS_stack {
	TRANS *tr;
	short branch;
	struct H_el *hashp;
	TRANS **m;
	TRANS **l;
} DFS_stack;

typedef struct Sleep { 
	TRANS *tr;
	unsigned short mark:1;
} Sleep;

struct H_el {
	struct H_el *nxt;
	unsigned tagged:6;
#ifndef HASHC
	unsigned sl_unmatch:1;
	uchar nb_sleep;
#endif
	unsigned state;
} **H_tab;

#define  MAXSTSZ 2000 
int MAX_Stack_tr_size = 5000; 
short MAX_TRANS_DEF=500;       

#if (defined SHOW)
#define NB_STEP 500000        
int step=0;                   
#endif

#ifdef SLEEP
int MAX_SLEEP=100;             
int MAX_L_LIST=1000;          
int MAX_M_LIST=10000;         
#endif







extern extern_wrapup();
extern extern_usage();
extern short extern_option();
extern putrail();



extern char *addr_current_state(); 
extern short nb_proc(); 
extern short endstate(); 
extern short trans_def_in(); 







extern ACC get_acc(); 
extern ACC get_acc_dis(); 
extern uchar same_trans(); 
extern short same_proc(); 
extern execute(); 
extern backtr(); 

/*zjr_beg*/
extern int update_barrier();
/*zjr_end*/





extern uchar valid(); 
extern uchar depend(); 
extern addpr(); 





extern short vsize;       








char caching_flag=0; 
int n_= 0; 
int MAX_STATES;     
int ncleared;       
uchar stack_flag=0; 





#ifdef SLEEP

TRANS **cur_sleep; 
short nb_trans_in_sleep=0; 
TRANS  **mptr, **mptr_max, **m_list_table;
TRANS  **lptr, **lptr_max, **l_list_table;
uchar sleep_mismatch=0;
#endif




short *Pst,Ppt,*P,*new_P,*P_1,*P_2,*enabled,*proviso,*P_size,
	  *P_visited, *P_set,**Tab;




int truncs=0; 
int nstates=0,nstates_max=0; 
TRANS **stpt, **stpt_org;
TRANS **stpt_par[2];
int stpt_ind=0;
DFS_stack *dfs_stpt,*dfs_stack;
//wyy_beg
DFS_stack *dfs_stpt_par[2];
DFS_stack *dfs_stpt_cp;
//wyy_end
TRANS_DEF **trans_def; 
int depth,maxdepth;
short MAX_proc=40;  
int mreached;       
uchar static_stack=0; 
int errors;         
int depthfound = -1; 
int memcnt=0;  
uchar done=0;
int hmax=0; 

FILE *fo;           

short nb;
short II; 




#ifdef BITSTATE

int nb_bits=0;
int MAX_BITS;

#endif







#ifdef BITSTATE
int ssize=22;    
#else
int ssize=18;
#endif

char dest[8];	  
unsigned long *hashmatrix; 
#if (defined HASHC) || (defined BITSTATE)
int bsize=32, tw=4; 
#endif

int mask, bitst_addr_mask, hcmp=0, upto=1;
#ifdef BITSTATE
uchar *SS; 
#endif

#define SHORT 16
#define SHORT_PWR 4   
#define SHORT_AND_CST 0x000F  
#define BYTE_PWR 3   
#define BYTE_AND_CST 0x0007  

#define set(b,i) (b)[(i)>>SHORT_PWR] |= (1<<((i)&SHORT_AND_CST))
#define is_set(b,i) ((b)[(i)>>SHORT_PWR] & 1<<((i)&SHORT_AND_CST))
#define unset(b,i) (b)[(i)>>SHORT_PWR] &= ~(1<<((i)&SHORT_AND_CST))
#define bset(b,i) (b)[(i)>>BYTE_PWR] |= (1<<((i)&BYTE_AND_CST))
#define bis_set(b,i) ((b)[(i)>>BYTE_PWR] & 1<<((i)&BYTE_AND_CST))
#define bunset(b,i) (b)[(i)>>BYTE_PWR] &= ~(1<<((i)&BYTE_AND_CST))

#define min(a,b) (((a)>(b)) ? (b) : (a))
#define max(a,b) (((a)<(b)) ? (b) : (a))
/*ZJR_BEG*/
int num_traces = 0;
int interval = 16;
/*ZJR_END*/
//wyy_beg
#define NUM_THREADS 2
//wyy_end
#endif
