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
