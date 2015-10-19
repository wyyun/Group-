#pragma once
#define num_group 2 
#define num_thread 5 


/*static info extracted from pan.t*/
int *trans_num;
const char **proc_name;
typedef struct Group{
	const char *id;
	int *pid;
	int *pblocked;
	int garrive;
	int numarrive;
	int numthread;
	int gid;
	Trans *indicator;
}Group;
Group **group; //no interleaving between groups is allowed at version1.0

/*dynamic info mainted by runtime*/
enum stage{before_group=0,in_group,after_group}; //three stage of state space exploration
unsigned char first_tran_group;//related to the time when set an indicator
short *nmitl; //no more interleavings flag bit for each group

typedef struct GROUP_INFO{
	int blocked[2];
}GROUP_INFO;

typedef struct DFS_GROUP{
	GROUP_INFO group_info[num_group];
	int active_gid;
	int stage;
	int compressed;
    int flag;//wyy_beg 标记回溯时是否要拷贝下层dfs_group的blocked信息至当前的dfs_group中
}DFS_GROUP;
DFS_GROUP *dfs_group;
DFS_GROUP **dfs_group_entry;

/*statistics*/
int state_redundant;
init_nmitl()
{
	int size = num_group/SHORT;
	nmitl = (short*)malloc(sizeof(short)*size);
	memset(nmitl,0,sizeof(short)*size);
}

/*TODO: provide interface init_trans_group() which can apply to the circumstance that transitions in one process doesn't belong to one group 2014-11-24*/
init_trans_group()
{
	int found = 0;
	int pid, tid, gid;
	int index;
	for(pid=0;pid<num_thread;pid++)
	{
		found = 0;
		for(gid=0;gid<num_group;gid++)
		{
			Group *g = group[gid];
			for(index=0;index<(g->numthread);index++)
			{
				if(g->pid[index]==pid)
					found = 1;
			}
			if(found)
				break;
		}
		if(gid==num_group) /*condition is true when this process doesn't belong to a group*/
			gid = -1;
		for(tid=1;tid <= trans_num[pid];tid++)
		{
            //printf("init_group   gid:%d  ",gid);
			set_trans_gid(pid,tid,gid);
		}
	}
}

Group *set_group(const char *id,int num,int *pid,int gid)
{
	Group* group = (Group*) malloc(sizeof(Group));
	group->id = id;
	group->pid = (int*)malloc(sizeof(int)*num);
	memcpy(group->pid,pid,num*sizeof(int));
	group->pblocked= (int*)malloc(sizeof(int)*num);
	memset(group->pblocked,0,sizeof(int)*num);
	group->garrive = 0;
	group->numarrive = 0;
	group->numthread = num;
	group->gid = gid;
	group->indicator = 0;
	return group;
}

init_group()
{
	group = (Group**)malloc(sizeof(Group*)*num_group);
	int pid_0[2] = {3,4};
	int pid_1[2] = {1,2};
	group[0] = (Group*)set_group("0",2,pid_0,0);
	group[1] = (Group*)set_group("1",2,pid_1,1);
	first_tran_group = 0x00; 
	init_dfs_group();
	state_redundant = 0;
}
//wyy_beg
init_wyy()
{
    int i = 0;
    stpt_ind_para = (int *)malloc(num_group * sizeof(int));
    for(;i<num_group;i++)
        stpt_ind_para[i] = 0;
}
//wyy_end

void print_dfs_group()
{
	if(dfs_group<*dfs_group_entry)
	{
		printf("dfs_group<*dfs_group_entry\n");
		return;
	}
	int i = 0;
	//printf("{group_info={ {dfs_group=%x} {stage=%d} {active_gid=%d} ", dfs_group, (dfs_group->stage),(dfs_group->active_gid));
	printf("{group_info={ {compressed=%d} {stage=%d} {active_gid=%d} ",(dfs_group)->compressed, (dfs_group->stage),(dfs_group->active_gid));
	GROUP_INFO *group_info = (GROUP_INFO*)(dfs_group->group_info);
	for(;i<num_group;i++)
	{
		int *blocked = group_info[i].blocked;
		printf("{blocked={%d,%d}} ",blocked[0],blocked[1]);
	}	
	printf("}\n");
}

init_dfs_group()
{
	dfs_group = (DFS_GROUP*)malloc(sizeof(DFS_GROUP)*maxdepth);
	memcnt +=  sizeof(DFS_GROUP)*maxdepth;
	dfs_group_entry = &dfs_group;
	memset(dfs_group,0,sizeof(DFS_GROUP));
	dfs_group->active_gid = -1;
	dfs_group->stage = 0;
	dfs_group->compressed = 0;
    dfs_group->flag = 0;
	//printf("in init_dfs_group: ");
	//print_dfs_group();
}

int get_gid(const Trans* tr)
{
	return tr->gid;
}

/*set_nmitl is called when backtracking happens*/
set_nmitl(FILE *fo,const DFS_stack *ds, int depth)
{
	TRANS* TR = ds->tr;
	Trans* tr = ((*TR).n).tr;
	int gid;
	int modified = 0;
	for(gid=0; gid<num_group; gid++)
	{
		if(is_set(nmitl,gid))
			continue;
		Group *g = group[gid];
		Trans* ttr = g->indicator;
		if(tr==ttr)
		{
			//printf("group%d completed\n",gid);
			set(nmitl,gid); //set(b,i) is a macro defined in po.h
			modified = 1;
		}
	}
	//print_nmitl(modified,fo); //consume a lot time

}

void compress_group(int gid)
{
	//update _p to endstate
	int numthread = group[gid]->numthread;
	int i = 0;
	int *pid = group[gid]->pid;
	for(;i<numthread;i++)
	{
		update_p(pid[i]);
	}
	//update group_info
	GROUP_INFO *group_info = dfs_group->group_info;
	int *blocked = group_info->blocked;
	for(i=0;i<numthread;i++)
	{
		blocked[i] = 1;
	}
}

void update_compressed(int forw,int pid, int gid)
{
	switch(forw)
	{
		case 6:
		case 8:
		case 11:
        case 13:
			//printf("update_compressed: forw=%d,gid=%d\n",forw,gid);
			if(is_set(nmitl,gid))
			{
				//printf("compress: (forw=%d,gid=%d)\n",forw,gid);
				//compress_group(gid);
				dfs_group->compressed = 1;
			}else
			{
				//printf("uncompress: (forw=%d,gid=%d)\n",forw,gid);
			}
			break;
		default:
			break;
	}
}

void update_indicator(int gid, const DFS_stack *dfs_stpt)
{
	if(gid<0)
		return;
	if(first_tran_group&(1<<gid))
		return;
	first_tran_group|=1<<gid;
	Trans *indicator=(*(*(dfs_stpt-1)).tr).n.tr;
	group[gid]->indicator=indicator;
    printf("****group[%d].indicator:%s****\n",gid,group[gid]->indicator->tp);

}

void update_group_info(int forw)
{
	memcpy(dfs_group,dfs_group-1,sizeof(DFS_GROUP));
	int gid,pid;
	int stage = dfs_group->stage;
	if(stage==before_group)
	{
		int reach = 0;
		switch(forw)
		{
			case 1:
				gid = 0;
				pid = 4;
				reach = 1;
				break;
			case 2:
				gid = 0;
				pid = 3;
				reach = 1;
				break;
			case 3:
				gid = 1;
				pid = 2;
				reach = 1;
				break;
			case 4:
				gid = 1;
				pid = 1;
				reach = 1;
				break;
		}
        printf("forw=%d,reach=%d\n",forw,reach);
		if(reach)
		{
			GROUP_INFO *group_info = (GROUP_INFO*)(dfs_group->group_info);
			int ind = get_index(gid,pid);
			group_info[gid].blocked[ind] = 1;
            //wyy_beg
            printf("reach:group[%d].blocked[%d] = 1\n",gid,ind);
            //wyy_end
		}
	}else if(stage==in_group)
	{
		int leave = 0;
		switch(forw)
		{
			case 7:
				gid = 1;
				pid = 1;
				leave = 1;
				break;
			case 10:
				gid = 1;
				pid = 2;
				leave = 1;
				break;
			case 12:
				gid = 0;
				pid = 3;
				leave = 1;
				break;
			case 15:
				gid = 0;
				pid = 4;
				leave = 1;
				break;
		}
        printf("forw=%d,leave=%d\n",forw,leave);
		if(leave)
		{
			GROUP_INFO *group_info = (GROUP_INFO*)(dfs_group->group_info);
			int ind = get_index(gid,pid);
			group_info[gid].blocked[ind] = 1;
            //wyy_beg
            printf("leave:group[%d].blocked[%d] = 1\n",gid,ind);
            //wyy_end
		}
	} 
}

void refresh(TRANS *t, const DFS_stack *dfs_stpt)
{
    printf("refresh:%s,forw=%d,",t->n.tr->tp,t->n.tr->forw);
	Trans *tr = (*t).n.tr;
	int forw = tr->forw;	
	int gid = tr->gid;
	int pid = tr->pid;
	update_compressed(forw,pid,gid);
	update_indicator(gid,dfs_stpt);
	dfs_group++;
    //wyy_beg
    dfs_group->flag = 0;
    //wyy_end
	update_group_info(forw);
	//print_dfs_group();
}

/*test_enum()
{
	stage = 0;
	if(stage==before_group)
		printf("before_group success\n");
	stage++;
	if(stage==in_group)
		printf("in_group success\n");
	stage++;
	if(stage==after_group)
		printf("after_group success\n");
}
*/


void active_group(int gid)
{
	GROUP_INFO *group_info = (GROUP_INFO*)((dfs_group)->group_info);
	int numthread = group[gid]->numthread;
	int i = 0;
	for(;i<numthread;i++)
	{
		group_info[gid].blocked[i] = 0;
        //wyy_beg
        printf("group[%d].blocked[%d] = 0\t",gid,i);
	}
    printf("\n");
    //wyy_end
}

void reset_group(int gid)
{
	//update _p to startstate
	int numthread = group[gid]->numthread;
	int i = 0;
	int *pid = group[gid]->pid;
	for(;i<numthread;i++)
	{
		reset_p(pid[i]);
	}
	
}

void transfer()
{
	int *pstage =(int*)(&(dfs_group->stage));
	int *pgid = (int*)(&(dfs_group->active_gid));
    //wyy_beg
    printf("dfs_group->stage:%d(0:before,1:in,2:after)\n",*pstage);
    //wyy_end
	if(*pstage==before_group)
	{
		*pstage = in_group;
		//printf("before_group=>in_group\n");
		*pgid = 0;
		active_group(*pgid);
		//printf("new dfs_group:");
		//print_dfs_group();
	}else if(*pstage==in_group)
	{
		*pgid = *pgid + 1;
		//printf("gid:%d->%d\n",(*pgid)-1,*pgid);
		if((*pgid<num_group)&&(*pgid>-1))
		{
			//printf("in_group: active group%d\n",*pgid);
			active_group(*pgid);
			reset_var(*pgid);
			reset_group(*pgid);

			//printf("new dfs_group:");
			//print_dfs_group();
		}else
		{
			*pstage = after_group;
			//printf("in_group=>after_group\n");
			int gid = 0;
			for(;gid<num_group;gid++)
			{
				active_group(gid);
			}
			//printf("new dfs_group:");
			//print_dfs_group();
		}
	}
}

//wyy_beg
void transfer_par(TRANS *tr)
{
	int *pstage =(int*)(&(dfs_group->stage));
	int *pgid = (int*)(&(dfs_group->active_gid));
//	int gid = 0;
    int gid = tr->n.tr->gid;
    *pstage = in_group;
	*pgid = 0;
    if(gid < 0)
        gid = 0;
	for(;gid<num_group;gid++)
    {
	//	reset_var(gid);
	//	reset_group(gid);
		active_group(gid);
	}
    printf("transfer:active_group\n");
}
void transfer_block(TRANS *tr)
{
    int i = 0,j;
    int numthread;
	GROUP_INFO *group_info = (GROUP_INFO*)((dfs_group)->group_info);
    printf("%s: ",tr->n.tr->tp);
    for(; i<tr->n.tr->gid ;i++)
    {
        numthread = group[i]->numthread;
        for(j = 0; j < numthread; j++)
        {
            group_info[i].blocked[j] = 1;
            printf("group_info[%d].blocked[[%d] = 1\n",i,j);
        }
    }
    for(i = tr->n.tr->gid; i<num_group;i++)
    {
	//	reset_var(i);
		reset_group(i);
    }
    printf("transfer_block!\n");
}

void transfer_unlock(TRANS *tr)
{
    int i,j;
    int numthread;
    int gid = tr->n.tr->gid;
    printf("@@@@@@@@********************transfer_unlock\n");
	GROUP_INFO *group_info = (GROUP_INFO*)((dfs_group)->group_info);
    
    if(((dfs_group + 1)) && (dfs_group+1)->flag == 1)
    {
	    GROUP_INFO *Bgroup_info = (GROUP_INFO*)((dfs_group + 1)->group_info);
        for(i = tr->n.tr->gid+1; i < num_group; i++)
        {
            numthread = group[i]->numthread;
            for(j = 0; j < numthread; j++)
            {
                group_info[i].blocked[j] = Bgroup_info[i].blocked[j];
                printf("group_info[%d].blocked[%d] = %d\t",i,j,group_info[i].blocked[j]);
            }
        }
        dfs_group->flag = 1;
    }

    printf("\n");
    if(gid+1 < num_group)
    {
        if(tr->n.tr == group[gid+1]->indicator)
            for(i = tr->n.tr->gid+1; i < num_group; i++)
            {
                numthread = group[i]->numthread;
                for(j = 0; j < numthread; j++)
                {
                    group_info[i].blocked[j] = 1;
                    printf("group_info[%d].blocked[%d] = 1\t",i,j);
                }
            }
        dfs_group->flag = 1;
    }

    printf("\n");
}
//wyy_end

int get_index(int gid, int pid)
{
	int numthread = group[gid]->numthread;
	int i = 0;
	for(;i<numthread;i++)
	{
		if(group[gid]->pid[i]==pid)
			return i;
	}
}
void increase_redundant()
{
	if((dfs_group-1)->compressed)
	{
		state_redundant++;
	}
}
void amend_redundant()
{
	if((dfs_group-1)->compressed)
	{
		state_redundant -= 2;
	}
}
void print()
{
    printf("********");
}
//void print_stpt(TRANS_DEF **trans_def)
//{
//    int i=0;
//    for(;i < num_thread; i++)
//        printf("%s",trans_def[i][0].tr.n.tr.tp);
//    printf("\n");
//    for(i = 0;i < num_thread; i++)
//        printf("%s",trans_def[i][1].tr.n.tr.tp);
//
//}
