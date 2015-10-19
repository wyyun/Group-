extern char *emalloc();
void PTrans(Trans *Tr)
{
	if(NULL==Tr)
		return;
	printf("proc_name=%s,tp=%s,pid=%d,tid=%d,st=%d,forw=%d,back=%d\n",\
	Tr->proc_name,Tr->tp,Tr->pid,Tr->tid,Tr->st,Tr->forw,Tr->back);
}
void PTRANS(TRANS *TR)
{
	if(NULL==TR)
		return;
	Trans *Tr = (*TR).n.tr;
	PTrans(Tr);
}
void Pstpt_n(TRANS **stpt, int n)
{
	if(NULL==stpt)
		return;
	int i = 0;
	while(i<n)
	{
		PTRANS(*(stpt-i));
		i++;
	}
}
void Pstpt(TRANS **stpt)
{
	if(NULL==stpt)
		return;
	int i = 0;
	while(NULL!=*(stpt-i))
	{
		PTRANS(*(stpt-i));
		i++;
	}
}
