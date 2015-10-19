#ifndef _TRACE_H
#define _TRACE_H
#include "group_2.h"
typedef struct Trace{
	const char **t; //trace
	int **tid; //transition id in trace
	const char *s; //status: select_transitions return 0|hstore returns 1|2
	int traceid;
	int *pid;
	char **proc_name; //proc name of each transition in current trace
	int depth;
	struct Trace *next;
}Trace;
Trace *Traces;
Trace **Tail;
int trace_count_total;
int trace_count_equivalent;
int trace_count_sleep;
int trace_count_end;
int trace_count_compressed;
int endstate_count[num_group];
int visitedstate_count[num_group];
int total_visitedstate_count;
int expectedstate_count;
int expectedtrace_count;

void init_trace()
{
	Traces = NULL;
	Tail = NULL;
	trace_count_total = 0;
	trace_count_equivalent = 0;
	trace_count_sleep = 0;
	trace_count_end = 0;
	trace_count_compressed = 0;
	int i = 0;
	for(;i<num_group;i++)
	{
		endstate_count[i] = 0;
		visitedstate_count[i] = 0;
	}
	total_visitedstate_count = 0;
	expectedstate_count = 0;
	expectedtrace_count = 0;
}

/*set_trace is called when an trace reaches end*/
insert_trace(const char *status, int depth, const DFS_stack *dfs_stpt)
{
	Trace *trace = (Trace*)malloc(sizeof(Trace));
	trace->traceid = trace_count_total;
	trace->s = status;
	trace->depth = depth;
	trace->t = (const char **)malloc(sizeof(char*)*depth);	
	trace->tid = (int*)malloc(sizeof(int*)*depth);
	trace->pid = (int*)malloc(sizeof(int)*depth);
	trace->proc_name = (char**)malloc(sizeof(char*)*depth);
	trace->next = NULL;
	const DFS_stack *beg = dfs_stpt - depth;
	const DFS_stack *end = dfs_stpt;
	const DFS_stack *tmp = beg;
	int index = 0;
	while (tmp != end)
	{
		TRANS *t = (*tmp).tr;
		Trans *tr = (*t).n.tr;
		char *id = (*tr).tp;
		int tid = (*tr).tid;
		trace->t[index] = id;
		trace->tid[index] = tid;
		trace->pid[index] = (*tr).pid;
		trace->proc_name[index] = (*tr).proc_name;
		index++;
		tmp++;
	}
	if(NULL==Traces)
	{
		Traces = trace;
		Tail = &(trace->next);
		return;
	}else
	{
		*Tail = trace;
		Tail = &(trace->next);
	}
}

print_cur_trace(int depth)
{
	const DFS_stack *beg = dfs_stpt - depth;
	const DFS_stack *end = dfs_stpt;
	const DFS_stack *tmp = beg;
	int index = 0;
	while (tmp != end)
	{
		TRANS *t = (*tmp).tr;
		PTRANS(t);
		tmp++;
	}	
}
/*ptraces is called during debugging to verify indicator*/
print_traces(FILE *fo)
{
	if(NULL!=fo)
	{
		fprintf(fo,"*************************\n");
		fprintf(fo,"\All the traces remained:\n");
	}
	Trace *trace = Traces;
	while(NULL!=trace)
	{
		int traceid = trace->traceid;
		const char *status = trace->s;
		const char **t = trace->t;
		int *pid = trace->pid;
		int *tid = trace->tid;
		int depth = trace->depth;
		char **proc_name = trace->proc_name;
		if(NULL!=fo)
		{
			fprintf(fo, "(trace %d|status=%s|depth=%d):\n",\
					traceid,status,depth);
			int d;
			for(d=0;d<depth;d++)
			{
				fprintf(fo, "%s[%s|%s|%d|%d]",(d != 0) ? ("->"):(" "),proc_name[d],t[d],pid[d],tid[d]);
			}
		}
		fprintf(fo, "\n");
		trace = trace->next;
	}
	if(NULL!=fo)
	{
		fprintf(fo,"\n");
	}
}

#endif
