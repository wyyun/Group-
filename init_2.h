#ifndef _INIT_H
#define _INIT_H
#ifdef GROUP
#include "group_2.h"
#endif
#include "trace_2.h"
#define num_thread 5 
int *trans_num;
const char **proc_name;
void set_trans_num();
void init_pid();
void set_proc_name();
void init_zjr()
{
	set_trans_num();
	init_pid();
	set_proc_name();
	init_trace();
#ifdef GROUP
	init_group();
	init_trans_group();
	init_nmitl();
#endif
    //wyy_beg
    init_wyy();
    //wyy_end
}

void init_pid()
{
	int pid,tid;
	for(pid=0;pid<num_thread;pid++)
	{
		for(tid=1;tid<=trans_num[pid];tid++)
		{
			set_pid(pid,tid);
			set_tid(pid,tid);
		}
	}
}
void set_proc_name()
{
	proc_name = (const char**)malloc(sizeof(char*)*num_thread);
	proc_name[0] = "init";
	proc_name[1] = "Y2";	
	proc_name[2] = "Y1";	
	proc_name[3] = "X2";	
	proc_name[4] = "X1";	
	int pid,tid;
	for(pid=0;pid<num_thread;pid++)
	{
		for(tid=1;tid<=trans_num[pid];tid++)
		{
			set_trans_proc_name(pid,tid,proc_name[pid]);
		}
	}
}
set_trans_num()
{
	trans_num = (int*)malloc(sizeof(int)*num_thread);
	trans_num[0] = 4;
	trans_num[1] = 1; 
	trans_num[2] = 2; 
	trans_num[3] = 1; 
	trans_num[4] = 2; 
}
#endif
