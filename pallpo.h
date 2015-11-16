//int stpt_ind_para[2]={0,0};
int *stpt_ind_para;
FILE *result;
//TRANS **stpt_para[num_group];

void open_result_file(FILE *result)
{
    if(!(result = fopen("result.txt","w")))
    {
        printf("result.txt cannot open result file");
        exit(1);
    }
}

/* void deleted_trail(int flag_par)
{
    int k = 0;
    while(trpt_par[flag_par]->o_t)
    {
        k++;
        trpt_cp[flag_par]++;
        trpt_cp[flag_par] = trpt_par[flag_par];
        printf("trpt_cp:%s, trpt_cp=%p,trpt_par=%p\n",trpt_cp[flag_par]->o_t->tp,trpt_cp[flag_par],trpt_par[flag_par]);
        trpt_par[flag_par]--;
        trpt--;
    }
    printf("deleted_trail:copy loop  = %d\n",k);
}

void copy_trail(int groupnum)
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
            printf("copy_trail:%s=%p\n",trpt_cp[i]->o_t->tp,trpt_cp[i]->oval);
            trpt->prog = trpt_cp[i]->prog;
            printf("trpt:%s trpt=%p,trpt_cp=%p\n",trpt->o_t->tp,trpt,trpt_cp[i]);
            trpt_cp[i]++;
        }
    }
    printf("copy_trail:number of loop = %d\n",k);
} */
