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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "po.h"
#include "pallpo.h"
#include "trace_2.h"
#include "print_tran.h"
#include "init_2.h"
#include "omp.h"

#ifdef GROUP
#include "group_2.h"
#endif
#define Version "SPIN 1.6.5 + Partial Order Package 3.1.3"
//extern char *emalloc();
void

wrapup()
{
    double a, b;

    printf("(%s)\n", Version);
    fprintf(result,"(%s)\n", Version);
#ifdef BITSTATE
    printf("bit statespace search ");
    fprintf(result,"bit statespace search ");
#else
    printf("full statespace search ");
    fprintf(result,"full statespace search ");
#endif
#if !(defined BITSTATE) && !(defined PROV)
    printf("for:\n	invalid endstates");
    fprintf(result,"for:\n	invalid endstates");
#else
    printf("for:\n	assertion violations and invalid endstates");
    fprintf(result,"for:\n	assertion violations and invalid endstates");
#endif
    if (!done)
    {
        printf("\nsearch was not completed");
        fprintf(result,"\nsearch was not completed");
    }
    printf("\n\nvector %d byte, depth reached %d", hmax, mreached);
    fprintf("result,\n\nvector %d byte, depth reached %d", hmax, mreached);

    printf(", errors: %d\n", errors);
    fprintf(result,", errors: %d\n", errors);

#ifdef BITSTATE
    printf("%8d states, visited\n", nstates);
    fprintf(result,"%8d states, visited\n", nstates);

    printf("%8d states, matched	   total: %8d\n",truncs, nstates+truncs);
    fprintf(result,"%8d states, matched	   total: %8d\n",truncs, nstates+truncs);
#else
    printf("%8d states, stored\n", nstates);
    fprintf(result,"%8d states, stored\n", nstates);

    if(ncleared)
    {
        printf("%8d states, cleared\n", ncleared);
        fprintf(result,"%8d states, cleared\n", ncleared);
    }

    printf("%8d states, matched	   total: %8d\n",truncs, nstates+ncleared+truncs);
    fprintf(result,"%8d states, matched	   total: %8d\n",truncs, nstates+ncleared+truncs);
#endif

    a = (double) (1<<ssize);
    b = (double) nstates_max+1.;

    printf("hash factor: %f ", a/b);
    fprintf(result,"hash factor: %f ", a/b);

#ifdef BITSTATE
    printf("(best coverage if >100)\n");
    fprintf(result,"(best coverage if >100)\n");

#else
    printf("hash conflicts: %d (resolved) (%3.1f %%)\n",hcmp,((double)hcmp/(double)nstates)*100);
    fprintf(result,"hash conflicts: %d (resolved) (%3.1f %%)\n",hcmp,((double)hcmp/(double)nstates)*100);

#endif
    printf("(max size 2^%d states, ", ssize);
    fprintf(result,"(max size 2^%d states, ", ssize);

#ifdef HASHC
    printf("%d bits stored, ", bsize);
    fprintf(result,"%d bits stored, ", bsize);

#endif
#ifdef MEMCNT
    printf("memory used: %d\n", memcnt);
    fprintf(result,"memory used: %d\n", memcnt);

#endif
    extern_wrapup(done);
    exit(0);
}

    char *
emalloc(n)
{	char *tmp = (char *) malloc(n);
    if (!tmp)
    {	printf("pan: out of memory\n");
        wrapup();
        exit(0);
    }
#ifdef MEMCNT
    memcnt += n;
#endif
    memset(tmp, 0, n);
    return tmp;
}

    char *
emalloc_wc(n)
{	char *tmp = (char *) malloc(n);
    if (!tmp)
    {	printf("pan: out of memory\n");
        wrapup();
        exit(0);
    }
    memset(tmp, 0, n);
    return tmp;
}

    char *
erealloc(p,n,on)
    void *p;
    unsigned n,on;
{	char *tmp = (char *) realloc(p,n);
    if (tmp)
    {
#ifdef MEMCNT
        memcnt += n-on;
#endif
        memset(tmp+on, 0, n-on);
    }
    return tmp;
}

efree(p)
    void *p;
{
    if(!p)
    {
        Uerror("cannot free");
    }
    else
    {
        free(p);
    }
}





d_hash(src, ns, dest, nd, d)
    unsigned char *src, *dest;
{
    register unsigned char qq;
    register unsigned char *q, *q2;
    register m = 0L, n = 0L;
    register ind = 0;
    static unsigned char oldkey[MAXSTSZ],hn[8];
    static unsigned long oldm, oldn;
    static int ft = 1;
    int h,i=0;
    unsigned char mask;

    if (ns>=MAXSTSZ)
        Uerror("MAXSTSZ too small, try a greater value ... (see decl.h)\n");
    h = ns;
    q = (unsigned char *) src;

    if (!d)
    {
        do {
            mask = 1;
            for (i=ind; i<ind+interval;i+=2)
            {
                if (*q&mask)
                {
                    m ^= hashmatrix[i];
                    n ^= hashmatrix[i+1];
                }
                mask = mask<<1;
            }
            ind += interval;
            q++;
        } while (--h > 0);
    }
    else if (ft)
    {
        ft = 0;
        q2 = oldkey;
        do {
            mask = 1;
            for (i=ind; i<ind+interval;i+=2)
            {
                if (*q&mask)
                {
                    m ^= hashmatrix[i];
                    n ^= hashmatrix[i+1];
                }
                mask = mask<<1;
            }
            ind += interval;
            *q2++ = *q++;
        } while (--h > 0);
        oldm = m;
        oldn = n;
    }
    else
    {
        m = oldm;
        n = oldn;
        q2 = (unsigned char *) oldkey;
        do {
            if (qq = *q2 ^ *q)
            {
                mask = 1;
                for (i=ind; i<ind+interval;i+=2)
                {
                    if (qq & mask)
                    {
                        m ^= hashmatrix[i];
                        n ^= hashmatrix[i+1];
                    }
                    mask = mask<<1;
                }
                *q2 = *q;
            }
            q++; q2++;
            ind += interval;
        } while (--h > 0);
        oldm = m;
        oldn = n;
    }
    if (nd == 32)
    {
        *((long *)dest) = m;
    }
    else if (nd == 64)
    {
        *((unsigned *)dest) = m;
        *(((unsigned *)dest)+1) = n;
    }
    else
    {
        *((unsigned *)hn) = m;
        *(((unsigned *)hn)+1) = n;
        for (i = (nd-1)>>3; i>=0; i--)
            dest[i] = hn[i];
        if (nd%8)
            dest[0] &= (1<<(nd%8))-1;
    }
}

#ifdef RANDH
#include <sys/time.h>
#endif

hinit()
{
#ifdef RANDH
    struct timeval tp;
    struct timezone tzp;
#endif
    int i;
#ifdef RANDH
    gettimeofday(&tp, &tzp);
    srandom(tp.tv_usec & 0xFFL);
#else
    srandom(1);
#endif
    hashmatrix = (unsigned long *)
        malloc(MAXSTSZ*sizeof(unsigned long)*interval);
    for (i=0; i<(MAXSTSZ*interval); i++)
        hashmatrix[i] = (unsigned long)random();
    /*
       hashmatrix = (unsigned long *)
       malloc(MAXSTSZ*sizeof(unsigned long)*16);
       for (i=0; i<(MAXSTSZ*16); i++)
       hashmatrix[i] = (unsigned long)random();
       */
}



/* #ifdef PROV */
no_more_in_stack(p)
    void *p;
{
    if(stack_flag)
        return;
    n_++;
    if(((struct H_el *)p)->tagged!=0 )
    {
        printf("bad stack-state\n");
        fprintf(result,"bad stack-state\n");
    }
    ((struct H_el *)p)->tagged =1;
}
/* #endif */

    uchar
is_in_stack()
{
    int j1;
#ifdef BITSTATE
    return 1;
#else
    register struct H_el *tmp;
    char *v;
    short n;

    v=addr_current_state();
    n=vsize;
#ifndef HASHC
    d_hash((char *)v, n, dest, 32, 1);
    j1=*(int *)dest & mask;
#else
    d_hash((char *)v, n, dest, 64, 1);
    n = tw;
    j1 = *((int *)dest+1) & mask;
    v = (char *)dest;
#endif
    tmp = H_tab[j1];
    if (!tmp)
        return 0;
    for (;tmp;tmp=tmp->nxt)
    {
        if (memcmp(&(tmp->state), v, n) == 0)

            if (!tmp->tagged)
                return 1;
    }
    return 0;
#endif
}



#ifdef SLEEP
add_tr_to_sleep(t)
    TRANS *t;
{
    if(nb_trans_in_sleep>= (MAX_SLEEP-1))
        if(!double_sleep())
            Uerror("MAX_SLEEP too small, not enough memory available\n");
    cur_sleep[nb_trans_in_sleep++]=t;
}

remove_from_sleep(t)
    TRANS *t;
{
    short i;

    for(i=0;i<nb_trans_in_sleep;i++)
        if(same_trans(t,cur_sleep[i]))
            break;
    if(i==nb_trans_in_sleep)
        return;
    for(;i<nb_trans_in_sleep;i++)
        cur_sleep[i]=cur_sleep[i+1];
    nb_trans_in_sleep--;
}

    TRANS *
get_sleep(c)
    short c;
{
    static short ind;
    if(c)
        ind=nb_trans_in_sleep;
    if (ind==0)
        return (0);
    return (cur_sleep[--ind]);
}

    uchar
is_sleeping(t)
    TRANS *t;
{
    short i;

    for(i=0;i<nb_trans_in_sleep;i++)
        if(same_trans(t,cur_sleep[i]))
            return 1;
    return 0;
}

print_sleep(f)
    FILE *f;
{
    short i;
    if(NULL!=f)
    {
        if(nb_trans_in_sleep>0)
        {
            printf("%d in s:",nb_trans_in_sleep);
            fprintf(result,"%d in s:",nb_trans_in_sleep);
            for(i=0;i<nb_trans_in_sleep;i++)
                PTRANS(cur_sleep[i]);
        }else
        {
            return;
        }
    }
}

double_sleep()
{
    TRANS **tmppt;
    int newsize;

    newsize=2*MAX_SLEEP;

#ifdef DYNSTRUCTCONT
    printf("doubling sleep, newsiz=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
    fprintf(result,"doubling sleep, newsiz=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
#endif
    tmppt = (TRANS **)erealloc(cur_sleep,newsize*sizeof(TRANS *),
            MAX_SLEEP*sizeof(TRANS *));

    if (tmppt)
    {
        cur_sleep = tmppt;
        MAX_SLEEP = newsize;
        if(NULL!=fo)
        {
            fprintf(fo,"*DSleep*\n");
        }
        return 1;
    }
    else
        return 0;
}

restore_sleep()
{
    TRANS  **mlst, **llst;

    for(mlst=dfs_stpt->m;mlst!=mptr;mlst++)
        remove_from_sleep(*mlst);

    mptr=dfs_stpt->m;


    for(llst=dfs_stpt->l;llst!=lptr;llst++)
        add_tr_to_sleep(*llst);
    lptr=dfs_stpt->l;
}

rm_tr_confl(t)
    TRANS *t;
{
    TRANS *ts;

    update_conflig_tags(t);

    for(ts=get_sleep(1);ts;ts=get_sleep(0))
    {
        update_conflig_tags(ts);
        if(confl(t,ts))
        {
            remove_from_sleep(ts);
            //printf("remove from sleep\n");
            *lptr++=ts;
            if(lptr==lptr_max)
                if (!double_lptr())
                    Uerror("MAX_L_LIST is too small, not enough memory available\n");
        }
    }
}

sleep_compare(pt,nb_hs)
    uchar *pt;
    short nb_hs;
{
    TRANS **hs;
    uchar *marked = pt+nb_hs*sizeof(TRANS *);
    short i,flag=0;

    for(hs=(TRANS **)pt,i=0;i<nb_hs;i++,hs++)
    {
        if(!*hs)
            continue;
        if(!is_sleeping(*hs))
        {
            bset(marked,i);
            flag=1;
        }
    }
    return flag;
}

confl(t1,t2)
    TRANS *t1,*t2;
{
    ACC acc1,acc2;

    if(same_proc(t1,t2))
        return 1;

    for(acc1=get_acc(t1,1,0);valid(acc1);acc1=get_acc(t1,0,0))
        for(acc2=get_acc(t2,1,1);valid(acc2);acc2=get_acc(t2,0,1))
            if(depend(acc1,acc2))
                return 1;
    return 0;
}
#endif






#ifdef BITSTATE
caching(n)
    int n;
{
    static int i1=0;
    static short i2=1;

    if(!caching_flag)
    {
        printf("State Space Caching...\n");
        fprintf(result,"State Space Caching...\n");

        printf("WARNING: With bitstate, states of the depth first stack can be deleted,\n");
        fprintf(result,"WARNING: With bitstate, states of the depth first stack can be deleted,\n");

        printf("termination is no more guaranteed...\n");
        fprintf(result,"termination is no more guaranteed...\n");

        caching_flag=1;
    }

    for(;n;)
    {
        if(SS[i1]&i2)
        {
            SS[i1] &= ~i2;
            nb_bits--;
            n--;
            ncleared++;
        }
        i2=i2<<1;
        if(i2==256)
        {
            i2=1;
            i1= (i1+1)&bitst_addr_mask;
        }
    }
}

#else

caching()
{
    static int j=0;
    register struct H_el *tmp, *tmp1;
    char found=0;

    if(!caching_flag)
    {
        printf("State Space Caching...\n");
        fprintf(result,"State Space Caching...\n");
        caching_flag=1;
    }
    if(n_< nstates/2)
        if(!stack_flag)
        {
            stack_flag=1;
            printf("WARNING: Most of the stored states are in the depth-first search stack,\n");
            fprintf(result,"WARNING: Most of the stored states are in the depth-first search stack,\n");

            printf("and won't be removed from the cache...\n");
            fprintf(result,"and won't be removed from the cache...\n");

            printf("If possible, use a larger <cache_size> in pan -n<cache_size>\n");
            fprintf(result,"If possible, use a larger <cache_size> in pan -n<cache_size>\n");

            printf("In any case, the search continues...\n");
            fprintf(result,"In any case, the search continues...\n");
        }
    if(n_==0)
    {
        Uerror("Cache too small -- if possible, use a larger <cache_size> in pan -n<cache_size>\n");
    }

    while (found==0)
    {
        j=(j+1)&mask;
        tmp = H_tab[j];
        if(!tmp)
            continue;
        for(tmp1=0;tmp;tmp=tmp->nxt)
        {
            if(tmp->tagged)
            {
                found=1;
                break;
            }
            tmp1=tmp;
        }
    }



    nstates--;
    n_--;
    ncleared++;
    if(!tmp1)
    {
        H_tab[j]=tmp->nxt;
        efree(tmp);
    }
    else
    {
        tmp1->nxt=tmp->nxt;
        efree(tmp);
    }
}
#endif

#ifdef BITSTATE
    short
hstore(v, n)
    char *v;
    short n;
{
    int j1,j2,J2,J1,j3,j4;
    hmax=max(hmax,n);
    d_hash((char *)v, n,dest,64,1);
    /*if(match_now(1,1,1)&&(n==44))
      {
      int i=0;
      for(;i<954868;i++)
      {
      d_hash((char *)v, n,dest,64,1);
      }
      exit(0);
      }*/
    J1=*(unsigned *)dest & mask ;
    J2=*((unsigned *)dest+1) & mask ;
    j3 = (1<<(J1&7)); j1 = J1>>3;
    j4 = (1<<(J2&7)); j2 = J2>>3;
#ifdef DXBITS
    if((SS[j2]&j3) && (SS[j1]&j4))
        return 1;
    if(SS[j2]& j3)
        nb_bits++;
    else
        if(SS[j1]& j4)
            nb_bits++;
        else
            nb_bits += 2;
    SS[j2] |= j3; SS[j1] |= j4;
    return 0;
#else
    if(SS[j2]&j3)
        return 1;
    else
    {
        nb_bits++;
        SS[j2] |= j3;
        return 0;
    }
#endif
}

#else

    short
hstore(vv, nn)
    char *vv;
    short nn;
{
    int j1;
    short cnt=1;
    short n;
    char *v;
    TRANS **tmp_pt;
#ifdef SLEEP
    TRANS *ts;
    short nb_bytes=((nb_trans_in_sleep-1)>>BYTE_PWR)+1;
#endif
#ifdef HASHC
    uchar **w, *Hel, *hc;
#endif
    register struct H_el *tmp;

    hmax=max(hmax,nn);
#ifndef HASHC
    v = vv; n = nn;
    d_hash((char *)v, n, dest, 32, 1);
    j1=*(int *)dest & mask;
#else
    d_hash(vv, nn, dest, 64, 1);
    n = tw;
    j1 = *((int *)dest+1) & mask;
    v = (char *)dest;
#endif
    tmp = H_tab[j1];
    if (!tmp)
    {
#if (defined HASHC) || !(defined SLEEP)
        if (!caching_flag)
            tmp = (struct H_el *)emalloc(sizeof(struct H_el)+n-sizeof(unsigned));
        else
            tmp = (struct H_el *)emalloc_wc(sizeof(struct H_el)+n-sizeof(unsigned));
#else
        if (!caching_flag)
            tmp = (struct H_el *)emalloc(sizeof(struct H_el)+n-sizeof(unsigned)+
                    nb_trans_in_sleep*sizeof(TRANS *)+nb_bytes);
        else
            tmp = (struct H_el *)emalloc_wc(sizeof(struct H_el)+n-sizeof(unsigned)+
                    nb_trans_in_sleep*sizeof(TRANS *)+nb_bytes);
#endif
        H_tab[j1] = tmp;
    } else
    { for (;;cnt++)
        {
            if (memcmp(&(tmp->state), v, n) == 0)

            {
                if(NULL!=fo)
                    fprintf(fo,"*M*\n");
                if (!tmp->tagged) {
                    if(NULL!=fo)
                        fprintf(fo,"*C*\n");
#ifndef HASHC
#ifdef SLEEP
                    if(sleep_compare(((uchar *)&(tmp->state))+n,
                                tmp->nb_sleep))
                    {
#ifdef HASHC
                        if (!all_sleeps_enable
                                (((uchar *)&(tmp->state))+n, tmp->nb_sleep))
                        {
                            if(NULL!=fo)
                                fprintf(fo,"*UC*\n");
                            goto nl;
                        }
#endif
                        tmp->sl_unmatch=1;
                        if(NULL!=fo)
                            fprintf(fo,"*SM*\n");
                    }
#endif
#endif
                    return 1;
                }
                else
                {
#ifndef HASHC
#ifdef SLEEP
                    if(sleep_compare(((uchar *)&(tmp->state))+n,
                                tmp->nb_sleep))
                    {
#ifdef HASHC
                        if (!all_sleeps_enable
                                (((uchar *)&(tmp->state))+n, tmp->nb_sleep))
                        {
                            if(NULL!=fo)
                                fprintf(fo,"*UC*\n");
                            goto nl;
                        }
#endif
#ifdef OUT
                        fprintf(fo,"*SM*\n");
#endif
                        (dfs_stpt-1)->hashp=tmp;
                        tmp->tagged=0;
                        return 2;
                    }
#endif
#endif
                    return 1;
                }
            }
nl:
            if (!tmp->nxt) break;
            tmp = tmp->nxt;
        }
        hcmp++;

#if (defined HASHC) || !(defined SLEEP)
        if (!caching_flag)
            tmp->nxt = (struct H_el *)emalloc(sizeof(struct H_el)+n-sizeof(unsigned));
        else
            tmp->nxt = (struct H_el *)emalloc_wc(sizeof(struct H_el)+n-sizeof(unsigned));
#else
        if (!caching_flag)
            tmp->nxt = (struct H_el *)emalloc(sizeof(struct H_el)+n-sizeof(unsigned)+
                    nb_trans_in_sleep*sizeof(TRANS *)+nb_bytes);
        else
            tmp->nxt = (struct H_el *)emalloc_wc(sizeof(struct H_el)+n-sizeof(unsigned)+
                    nb_trans_in_sleep*sizeof(TRANS *)+nb_bytes);
#endif
        tmp = tmp->nxt;
    }
    memcpy(((char *)&(tmp->state)), v, n);
    (dfs_stpt-1)->hashp=tmp;
#ifndef HASHC
#ifdef SLEEP
    tmp->nb_sleep=nb_trans_in_sleep;
    tmp_pt = (TRANS **)(((uchar *)&(tmp->state))+n);
    for(ts=get_sleep(1);ts;ts=get_sleep(0))
        *tmp_pt++ =ts;
#endif
#endif

    return 0;
}
#endif





init()
{
    short NB_proc_short;
    short i;

    if (maxdepth == 0)
        maxdepth=10000;
    mask = ((1<<ssize)-1);
    hinit();
#ifdef BITSTATE
    SS = (uchar *) emalloc(1<<(ssize-3));
    bitst_addr_mask = ((1<<(ssize-3))-1);
#else
    H_tab = (struct H_el **)
        emalloc((1<<ssize)*sizeof(struct H_el *));
#endif

#ifdef SLEEP
    cur_sleep=(TRANS **)emalloc(MAX_SLEEP*sizeof(TRANS *));
    m_list_table=mptr=(TRANS  **)emalloc(MAX_M_LIST*sizeof(TRANS *));
    mptr_max=mptr+MAX_M_LIST;
    l_list_table=lptr=(TRANS  **)emalloc(MAX_L_LIST*sizeof(TRANS *));
    lptr_max=lptr+MAX_L_LIST;
#endif
    trans_def=(TRANS_DEF **)emalloc(MAX_proc*sizeof(TRANS_DEF *));
    for(i=0;i<MAX_proc;i++)
        trans_def[i]=(TRANS_DEF *)emalloc(MAX_TRANS_DEF*sizeof(TRANS_DEF));
    dfs_stack=dfs_stpt= (DFS_stack *) emalloc(maxdepth*sizeof(DFS_stack));
    //wyy_beg
    dfs_stpt_par[0]=(DFS_stack *) emalloc(maxdepth*sizeof(DFS_stack));
    dfs_stpt_par[1]=(DFS_stack *) emalloc(maxdepth*sizeof(DFS_stack));
    dfs_stpt_cp = (DFS_stack *) emalloc(maxdepth*sizeof(DFS_stack));
    stpt_par[0]=(TRANS **)emalloc(MAX_Stack_tr_size*sizeof(TRANS *));
    stpt_par[1]=(TRANS **)emalloc(MAX_Stack_tr_size*sizeof(TRANS *));
    //wyy_end
    stpt=stpt_org=(TRANS **)emalloc(MAX_Stack_tr_size*sizeof(TRANS *));
    NB_proc_short=i=(MAX_proc-1)/SHORT+1;
    P=(short *)emalloc(i*sizeof(short));
    new_P=(short *)emalloc(i*sizeof(short));
    Tab=(short **)emalloc(MAX_proc*sizeof(short *));
    for(i=0;i<=MAX_proc;i++)
        Tab[i]=(short *)emalloc(NB_proc_short*sizeof(short));
    P_2=(short *)emalloc(i*sizeof(short));
    enabled=(short *)emalloc(i*sizeof(short));
    proviso=(short *)emalloc(i*sizeof(short));
    P_visited=(short *)emalloc(i*sizeof(short));
    P_set=(short *)emalloc(i*sizeof(short));
    P_size=(short *)emalloc(MAX_proc*sizeof(short));
    Pst=(short *)emalloc(MAX_proc*sizeof(short));
}




#ifdef GROUP
int blocked(TRANS_DEF trans_def)
{
    TRANS *t1 = trans_def.tr;
    Trans *t2 = ((*t1).n).tr;
    int gid = t2->gid;
    int pid = t2->pid;
    if(gid>-1)
    {
        int ind = get_index(gid,pid);
        GROUP_INFO *group_info = (GROUP_INFO*)(dfs_group->group_info);
        if(group_info[gid].blocked[ind])
            return 1;
    }
    return 0;
}
#endif
select_transitions(int select_flag,short *k_par,int flag_par)
{
    short i,X,K,I;
    short k = 0;
    // short k_par[2]={0,0};
    int ggid;
    short size,old_size;

#ifndef HASHC
#ifdef SLEEP
    TRANS **hs,**m,*ts;
    uchar *pt,*marked;

    if (sleep_mismatch)

    {
        sleep_mismatch=0;
        pt=((uchar *)&(dfs_stpt->hashp->state))+vsize;
        marked=pt+dfs_stpt->hashp->nb_sleep*sizeof(TRANS *);


        for(ts=get_sleep(1);ts;ts=get_sleep(0))

        {
            for(i=0,hs=(TRANS **)pt;i<dfs_stpt->hashp->nb_sleep;i++,hs++)

                if(*hs && same_trans(ts,*hs))
                {
                    if(bis_set(marked,i))
                    {
                        for(m=dfs_stpt->m;m!=mptr;m++)
                            if(same_trans(*m,ts))
                                break;
                        if(m==mptr)
                        {
                            *lptr++=ts;
                            if(lptr==lptr_max)
                                if (!double_lptr())
                                    Uerror("MAX_L_LIST is too small, not enough memory available\n");
                        }
                        remove_from_sleep(ts);
                    }
                    break;
                }

            if(i==dfs_stpt->hashp->nb_sleep)

            {
                for(m=dfs_stpt->m;m!=mptr;m++)
                    if(same_trans(*m,ts))
                        break;
                if(m==mptr)
                {
                    *lptr++=ts;
                    if(lptr==lptr_max)
                        if (!double_lptr())
                            Uerror("MAX_L_LIST is too small, not enough memory available\n");
                }
                remove_from_sleep(ts);
            }
        }

        for(i=0,hs=(TRANS **)pt;i<dfs_stpt->hashp->nb_sleep;i++,hs++)
        {
            if(! *hs)
                continue;
            if(bis_set(marked,i))
            {
                if(stpt_ind>=MAX_Stack_tr_size-1)
                    if (!double_stpt())
                        Uerror("MAX_Stack_tr_size too small, not enough memory available\n");
                k++;
                *(++stpt)=*hs;
                *hs=0;
                stpt_ind++;
            }
        }

        dfs_stpt++;
        return k;
    }

#endif
#endif
    if(!trans_def_in(flag_par))//置trans_def[][]元素值
        return 0;
    //wyy_beg
    // printf("\nnb=%d\n",nb_proc()-1);
    //wyy_end
    nb=((nb_proc()-1)>>SHORT_PWR)+1;
    //wyy_beg
    // printf("nb=%hu\n",nb);
    Ppt=0;
    k=0;
    k_par[0]=0;k_par[1]=0;//related to num_group
    //wyy_end
#ifdef OVER
    memset((char *)P_2, 0, nb*sizeof(short));
    memset((char *)P_visited, 0, nb*sizeof(short));
    memset((char *)enabled, 0, nb*sizeof(short));
    memset((char *)proviso, 0, nb*sizeof(short));
    old_size=0;

    for(I=nb_proc()-1;I>=0;I--)
    {
        for(i=0;i<MAX_TRANS_DEF && trans_def[I][i].tr;i++)
        {
            update_conflig_tags(trans_def[I][i].tr);
#ifdef SLEEP
            if((trans_def[I][i].enabled)
                    && !trans_def[I][i].in_sleep)
                set(enabled,I);
#else
            if((trans_def[I][i].enabled))
                set(enabled,I);
#endif

#ifdef PROV
#ifdef SLEEP
            if(trans_def[I][i].enabled
                    &&!trans_def[I][i].in_stack
                    && !trans_def[I][i].in_sleep)
                set(proviso,I);
#else
            if(trans_def[I][i].enabled
                    &&!trans_def[I][i].in_stack)
                set(proviso,I);
#endif
#endif
        }
    }

    for(i=0;i<nb;i++)
        P_set[i]= ~ 0;

    for(I=nb_proc()-1;I>=0;I--)
    {
        if(is_set(P_visited,I) || !is_set(enabled, I))

            continue;

#ifdef PROV
        if(!is_set(proviso,I))

            continue;
#endif

        Pst[Ppt++]=I;
        memset((char *)P,0,nb*sizeof(short));
        size=0;
        //wyy_beg
        printf("2222\n");
        fprintf(result,"2222\n");

        do{
            II=Pst[--Ppt];
            //wyy_beg
            printf("Pst[%d]=%hd\t",Pst,II);
            fprintf(result,"Pst[%d]=%hd\t",Pst,II);
            P_1=Tab[II];
            //wyy_beg
            printf("Tab[%d]=%hd\n",II,Tab[II]);
            fprintf(result,"Tab[%d]=%hd\n",II,Tab[II]);

            memset((char *)P_1,0,nb*sizeof(short));

            set(P,II);

            size+=P_size[II];
            if(II==0)
                continue;
            for(i=0;i<MAX_TRANS_DEF && trans_def[II][i].tr;i++)
                addprocess(trans_def[II][i]);

            for(i=0;i<nb;i++)
            {
                P_2[i] = P_1[i] & ~P[i];
                P[i] |= P_1[i];
                if(P_2[i])
                    for(K=i*SHORT;K<(i+1)*SHORT && K<nb_proc();K++)
                        if(is_set(P_2,K))
                            Pst[Ppt++]=K;
            }
        }while(Ppt);

        if(!old_size || size<old_size)
        {
            for(i=0;i<nb;i++)
                P_set[i]=P[i];
            old_size=size;
        }

        for(i=0;i<nb;i++)
            P_visited[i] |= P[i];

        for(X=I-1;X>=0;X--)
        {
            if(!is_set(P,X) || !(is_set(enabled,X)))

                continue;

#ifdef PROV
            if(!(is_set(proviso,X)))
                continue;
#endif

            Pst[Ppt++]=X;
            memset((char *)new_P,0,nb*sizeof(short));

            size=0;
            do{
                II=Pst[--Ppt];
                set(new_P,II);
                size+=P_size[II];

                for(i=0;i<nb;i++)
                {
                    P_2[i] = Tab[II][i] & ~new_P[i];
                    new_P[i] |= Tab[II][i];


                    if(P_2[i])
                        for(K=i*SHORT;K<(i+1)*SHORT && K<nb_proc();K++)
                            if(is_set(P_2,K))
                                if((K>X) && is_set(enabled,K))
                                    goto End_;
                                else Pst[Ppt++]=K;

                }
            }while(Ppt);

            if(!old_size || size<old_size)
            {
                for(i=0;i<nb;i++)
                    P_set[i]=new_P[i];
                old_size=size;
            }
End_:
            Ppt=0;
        }
    }
#else
    for(i=0;i<nb;i++)
        P_set[i]= ~ 0;
#endif
Sel:
    for(II=0;II<nb_proc();II++)//选择各线程中的transitions存储在stpt栈中
    {
        if(!is_set(P_set,II))
        {
            continue;
        }
        for(i=0;i<MAX_TRANS_DEF && trans_def[II][i].tr;i++)
            if(trans_def[II][i].enabled)
            {
#ifdef SLEEP
                if(!trans_def[II][i].in_sleep)
#endif
                {	if(stpt_ind=max_stpt(stpt_ind,stpt_ind_para)>=MAX_Stack_tr_size-1)
                    //if(stpt_ind >= MAX_Stack_tr_size-1)
                    if (!double_stpt())
                        Uerror("MAX_Stack_tr_size too small\n");
#ifdef GROUP
                    if(!blocked(trans_def[II][i]))
                    {
#endif
                        if(!select_flag)
                        {
                            *(++stpt)=trans_def[II][i].tr;
                            printf("trans_def[%d][%d]:%s\t",II,i,trans_def[II][i].tr->n.tr->tp);
                            fprintf(result,"trans_def[%d][%d]:%s\t",II,i,trans_def[II][i].tr->n.tr->tp);

                            printf("stpt:%s,stpt_ind:%d\t",(*stpt)->n.tr->tp,stpt_ind);
                            fprintf(result,"stpt:%s,stpt_ind:%d\t",(*stpt)->n.tr->tp,stpt_ind);

                            printf("type:%d,",(*stpt)->n.type);
                            fprintf(result,"type:%d,",(*stpt)->n.type);

                            printf("select_flag=0\n");
                            fprintf(result,"select_flag=0\n");
                            k++;
                            stpt_ind++;
                        }
                        else
                        {
                            ggid = (trans_def[II][i]).tr->n.tr->gid;
                            //   printf("%s,ggid:%d\n",trans_def[II][i].tr->n.tr->tp,ggid);
                            if(ggid >= 0)
                            {
                                stpt_par[ggid]++;


                                // stpt_par[ggid] +=sizeof(int);
                                *(stpt_par[ggid]) = trans_def[II][i].tr;
                                /*     if(nb_proc()-1 == sum)
                                       {
                                       if(ggid != kid)
                                       {
                                       printf("deleted stpt:%s,",(*stpt)->n.tr->tp);
                                       stpt--;
                                //    printf("stpt:%s,kid = %d\n",(*stpt)->n.tr->tp,kid);
                                }
                                 *(++stpt)=trans_def[II][i].tr;//instore and use during the Up time
                                 printf("stpt:%s,",(*stpt)->n.tr->tp);
                                 kid = ggid;
                                 }
                                 */
                                printf("trans_def[%d][%d]:%s, ",II,i,trans_def[II][i].tr->n.tr->tp);
                                fprintf(result,"trans_def[%d][%d]:%s, ",II,i,trans_def[II][i].tr->n.tr->tp);

                                printf("stpt_par[%d]:%s, ",ggid,(*(stpt_par[ggid]))->n.tr->tp);
                                fprintf(result,"stpt_par[%d]:%s, ",ggid,(*(stpt_par[ggid]))->n.tr->tp);

                                printf("type:%d, ",(*(stpt_par[ggid]))->n.type);
                                fprintf(result,"type:%d, ",(*(stpt_par[ggid]))->n.type);
                                k_par[ggid]++;
                                printf("k=%d, ",k_par[ggid]);
                                fprintf(result,"k=%d, ",k_par[ggid]);
                                stpt_ind_para[ggid]++;
                                printf("stpt_ind_para[%d]:%d, ",ggid,stpt_ind_para[ggid]);
                                fprintf(result,"stpt_ind_para[%d]:%d, ",ggid,stpt_ind_para[ggid]);
                                printf("select_flag=1\n");
                                fprintf(result,"select_flag=1\n");
                            }
                        }
#ifdef GROUP
                    }
#endif
                }
            }
    }
    // print_stpt(trans_def);//print the trans_def's elements
    // print();
    printf("\n");
    fprintf(result,"\n");
    if(!select_flag)
        return k;
    return k;
    /*    else
          return k_par[ggid];
          */
}

select_transitions_par(int select_flag,short *k_par,int flag_par)
{
    short i,X,K,I;
    short k = 0;
    //short k_par[2]={0,0};

    int ggid;
    short size,old_size;

#ifndef HASHC
#ifdef SLEEP
    TRANS **hs,**m,*ts;
    uchar *pt,*marked;
    /*
       if (sleep_mismatch)

       {
       sleep_mismatch=0;
       pt=((uchar *)&(dfs_stpt->hashp->state))+vsize;
       marked=pt+dfs_stpt->hashp->nb_sleep*sizeof(TRANS *);


       for(ts=get_sleep(1);ts;ts=get_sleep(0))

       {
       for(i=0,hs=(TRANS **)pt;i<dfs_stpt->hashp->nb_sleep;i++,hs++)

       if(*hs && same_trans(ts,*hs))
       {
       if(bis_set(marked,i))
       {
       for(m=dfs_stpt->m;m!=mptr;m++)
       if(same_trans(*m,ts))
       break;
       if(m==mptr)
       {
     *lptr++=ts;
     if(lptr==lptr_max)
     if (!double_lptr())
     Uerror("MAX_L_LIST is too small, not enough memory available\n");
     }
     remove_from_sleep(ts);
     }
     break;
     }

     if(i==dfs_stpt->hashp->nb_sleep)

     {
     for(m=dfs_stpt->m;m!=mptr;m++)
     if(same_trans(*m,ts))
     break;
     if(m==mptr)
     {
     *lptr++=ts;
     if(lptr==lptr_max)
     if (!double_lptr())
     Uerror("MAX_L_LIST is too small, not enough memory available\n");
     }
     remove_from_sleep(ts);
     }
     }

     for(i=0,hs=(TRANS **)pt;i<dfs_stpt->hashp->nb_sleep;i++,hs++)
     {
     if(! *hs)
     continue;
     if(bis_set(marked,i))
     {
     if(stpt_ind>=MAX_Stack_tr_size-1)
     if (!double_stpt())
     Uerror("MAX_Stack_tr_size too small, not enough memory available\n");
     k++;
     *(++stpt)=*hs;
     *hs=0;
     stpt_ind++;
     }
     }

     dfs_stpt++;
     return k;
     }
     */
#endif
#endif
    k_par[0]=0;k_par[1]=0;
    printf("*****************before******************\n");
    fprintf(result,"*****************before******************\n");

    if(!trans_def_in(flag_par))//置trans_def[][]元素值
        return 0;
    printf("****************end**********************\n");
    fprintf(result,"****************end**********************\n");

    //wyy_beg
    // printf("\nnb=%d\n",nb_proc()-1);
    //wyy_end
    nb=((nb_proc()-1)>>SHORT_PWR)+1;
    //wyy_beg
    // printf("nb=%hu\n",nb);
    Ppt=0;
    k=0;
    /*
    int kid = 0;//whether instore in the stpt;
    int sum = 0;
    for(i = 0; i<num_group; i++)
        sum += group[i]->numthread;
    printf("sum = %d\n",sum);*/
    //    k_par[0]=0;k_par[1]=0;
    //wyy_end
#ifdef OVER
    memset((char *)P_2, 0, nb*sizeof(short));
    memset((char *)P_visited, 0, nb*sizeof(short));
    memset((char *)enabled, 0, nb*sizeof(short));
    memset((char *)proviso, 0, nb*sizeof(short));
    old_size=0;

    for(I=nb_proc()-1;I>=0;I--)
    {
        for(i=0;i<MAX_TRANS_DEF && trans_def[I][i].tr;i++)
        {
            update_conflig_tags(trans_def[I][i].tr);
#ifdef SLEEP
            if((trans_def[I][i].enabled)
                    && !trans_def[I][i].in_sleep)
                set(enabled,I);
#else
            if((trans_def[I][i].enabled))
                set(enabled,I);
#endif

#ifdef PROV
#ifdef SLEEP
            if(trans_def[I][i].enabled
                    &&!trans_def[I][i].in_stack
                    && !trans_def[I][i].in_sleep)
                set(proviso,I);
#else
            if(trans_def[I][i].enabled
                    &&!trans_def[I][i].in_stack)
                set(proviso,I);
#endif
#endif
        }
    }

    for(i=0;i<nb;i++)
        P_set[i]= ~ 0;

    for(I=nb_proc()-1;I>=0;I--)
    {
        if(is_set(P_visited,I) || !is_set(enabled, I))

            continue;

#ifdef PROV
        if(!is_set(proviso,I))

            continue;
#endif

        Pst[Ppt++]=I;
        memset((char *)P,0,nb*sizeof(short));
        size=0;
        //wyy_beg
        printf("2222\n");
        fprintf(result,"2222\n");
        do{
            II=Pst[--Ppt];
            //wyy_beg
            printf("Pst[%d]=%hd\t",Pst,II);
            fprintf(result,"Pst[%d]=%hd\t",Pst,II);
            P_1=Tab[II];
            //wyy_beg
            printf("Tab[%d]=%hd\n",II,Tab[II]);
            fprintf(result,"Tab[%d]=%hd\n",II,Tab[II]);
            memset((char *)P_1,0,nb*sizeof(short));

            set(P,II);

            size+=P_size[II];
            if(II==0)
                continue;
            for(i=0;i<MAX_TRANS_DEF && trans_def[II][i].tr;i++)
                addprocess(trans_def[II][i]);

            for(i=0;i<nb;i++)
            {
                P_2[i] = P_1[i] & ~P[i];
                P[i] |= P_1[i];
                if(P_2[i])
                    for(K=i*SHORT;K<(i+1)*SHORT && K<nb_proc();K++)
                        if(is_set(P_2,K))
                            Pst[Ppt++]=K;
            }
        }while(Ppt);

        if(!old_size || size<old_size)
        {
            for(i=0;i<nb;i++)
                P_set[i]=P[i];
            old_size=size;
        }

        for(i=0;i<nb;i++)
            P_visited[i] |= P[i];

        for(X=I-1;X>=0;X--)
        {
            if(!is_set(P,X) || !(is_set(enabled,X)))

                continue;

#ifdef PROV
            if(!(is_set(proviso,X)))
                continue;
#endif

            Pst[Ppt++]=X;
            memset((char *)new_P,0,nb*sizeof(short));

            size=0;
            do{
                II=Pst[--Ppt];
                set(new_P,II);
                size+=P_size[II];

                for(i=0;i<nb;i++)
                {
                    P_2[i] = Tab[II][i] & ~new_P[i];
                    new_P[i] |= Tab[II][i];


                    if(P_2[i])
                        for(K=i*SHORT;K<(i+1)*SHORT && K<nb_proc();K++)
                            if(is_set(P_2,K))
                                if((K>X) && is_set(enabled,K))
                                    goto End_;
                                else Pst[Ppt++]=K;

                }
            }while(Ppt);

            if(!old_size || size<old_size)
            {
                for(i=0;i<nb;i++)
                    P_set[i]=new_P[i];
                old_size=size;
            }
End_:
            Ppt=0;
        }
    }
#else
    for(i=0;i<nb;i++)
        P_set[i]= ~ 0;
#endif
Sel:
    for(II=0;II<nb_proc();II++)//选择各线程中的transitions存储在stpt栈中
    {
        if(!is_set(P_set,II))
        {
            continue;
        }
        for(i=0;i<MAX_TRANS_DEF && trans_def[II][i].tr;i++)
            if(trans_def[II][i].enabled)
            {
#ifdef SLEEP
                if(!trans_def[II][i].in_sleep)
#endif
                {	if(stpt_ind=max_stpt(stpt_ind,stpt_ind_para)>=MAX_Stack_tr_size-1)
                    //if(stpt_ind >= MAX_Stack_tr_size-1)
                    if (!double_stpt())
                        Uerror("MAX_Stack_tr_size too small\n");
#ifdef GROUP
                    if(!blocked(trans_def[II][i]))
                    {
#endif
                        /*   if(!select_flag)
                             {
                             printf("select_flag=1\n");
                         *(++stpt)=trans_def[II][i].tr;
                         printf("trans_def[%d][%d]:%s\t",II,i,trans_def[II][i].tr->n.tr->tp);
                         printf("stpt:%s,stpt_ind:%d\t",(*stpt)->n.tr->tp,stpt_ind);
                         k++;
                         stpt_ind++;
                         }
                         else
                         {
                         */
                        ggid = (trans_def[II][i]).tr->n.tr->gid;
                     //   printf("%s,ggid:%d\n",trans_def[II][i].tr->n.tr->tp,ggid);
                        if(ggid >= 0)
                        {
                            stpt_par[ggid]++;


                           // stpt_par[ggid] +=sizeof(int);
                            *(stpt_par[ggid]) = trans_def[II][i].tr;
                      /*     if(nb_proc()-1 == sum)
                           {
                               if(ggid != kid)
                               {
                                    printf("deleted stpt:%s,",(*stpt)->n.tr->tp);
                                    stpt--;
                                   //    printf("stpt:%s,kid = %d\n",(*stpt)->n.tr->tp,kid);
                               }
                               *(++stpt)=trans_def[II][i].tr;//instore and use during the Up time
                               printf("stpt:%s,",(*stpt)->n.tr->tp);
                               kid = ggid;
                           }
                        */
                            printf("trans_def[%d][%d]:%s, ",II,i,trans_def[II][i].tr->n.tr->tp);
                            fprintf(result,"trans_def[%d][%d]:%s, ",II,i,trans_def[II][i].tr->n.tr->tp);
                            printf("stpt_par[%d]:%s, ",ggid,(*(stpt_par[ggid]))->n.tr->tp);
                            fprintf(result,"stpt_par[%d]:%s, ",ggid,(*(stpt_par[ggid]))->n.tr->tp);
                            printf("type:%d, ",(*(stpt_par[ggid]))->n.type);
                            fprintf(result,"type:%d, ",(*(stpt_par[ggid]))->n.type);
                            k_par[ggid]++;
                            printf("k=%d, ",k_par[ggid]);
                            fprintf(result,"k=%d, ",k_par[ggid]);
                            stpt_ind_para[ggid]++;
                            printf("stpt_ind_para[%d]:%d, ",ggid,stpt_ind_para[ggid]);
                            fprintf(result,"stpt_ind_para[%d]:%d, ",ggid,stpt_ind_para[ggid]);
                            printf("select_flag=1\n");
                            fprintf(result,"select_flag=1\n");
                    }
#ifdef GROUP
                    }
#endif
                }
            }
    }
    printf("\n");
    fprintf(result,"\n");
    /*
    if(nb_proc()-1 == sum)
    {
        printf("deleted stpt:%s,",(*stpt)->n.tr->tp);
        stpt--;//whether use the stpt to instore tr
    }*/
    /*if(!select_flag)
      return k;
      else  */
    //        return k_par;
}

copy_stpt()
{
    int i = 0;
    for(; i < num_group; i++ )
    {
        while(stpt_par[i])
        {
            *(++stpt) = *stpt_par[i];
            printf("stpt:,%d\n",*stpt,stpt);
            fprintf(result,"stpt:,%d\n",*stpt,stpt);
            stpt_ind++;
            stpt_par[i]++;
        }
    }
}

#ifdef OVER
addprocess(trans)
    TRANS_DEF trans;
{
    ACC acc;

    if(trans.enabled)
        for(acc=get_acc(trans.tr,1,0);valid(acc);acc=get_acc(trans.tr,0,0))
            addpr(acc);
    else
        for(acc=get_acc_dis(trans.tr,1);valid(acc);	acc=get_acc_dis(trans.tr,0))
            addpr(acc);
}
#endif

new_state_PO()
{
        int state_count = 0;
        TRANS **tt;
        TRANS *t;
        short n;
        short n_par[NUM_THREADS];
        //short *n_par;
        short i;
        int j;
        uchar depth_flag=0;
        char cache_flag=1;
        int nid;//get the thread id;
        int select_flag = 0;
        int select_par = 1;//select_par = 0,parallel;select_flag = 1,schedule;
    #ifdef OUT
    #ifdef STDOUT
        fo=stdout;
    #else
        if (!(fo=fopen("foutput","w")))
        {
            printf("cannot create foutput");
            fprintf(result,"cannot create foutput");
            exit(1);
        }
    #endif
#else
        fo=NULL;
#endif
        init();
        init_zjr();
Down:
        if (depth >= maxdepth)
        {
            if(!double_dfs_stack())
            {
                if(!depth_flag)
                {
                    printf("maxdepth is too small, the search will be truncated\n");
                    fprintf(result,"maxdepth is too small, the search will be truncated\n");
                    depth_flag=1;
                }
                truncs++;
                goto Up;
            }
        }
        printf("Down\n");
        fprintf(result,"Down\n");
        int temp = hstore(addr_current_state(), vsize);
        /*#ifdef GROUP
          if(match_now(53,1,1))
          {
          printf("state count %d, depth %d: temp=%d:\n",state_count, depth,temp);
          print_now();
        //temp = 0;
        }
#endif
*/
        state_count++;
        switch(temp)
        {
            case 1:
                truncs++;
                trace_count_equivalent++;
                //insert_trace("hstore return 1",depth,dfs_stpt);
#ifdef DB
                printf("depth%d: trace is truncated by state equivalence:\n",depth);
                fprintf(result,"depth%d: trace is truncated by state equivalence:\n",depth);
                print_now();
#endif
                printf("Down:case1\n");
                fprintf(result,"Down:case1\n");
                goto Up;
                break;
            case 2: //this case hasn't happen till now
                truncs++;
#ifdef SLEEP
                sleep_mismatch=1;
#endif
                break;
            case 0:
                nstates++; //nstates is unique during dfs
                printf("Down:case0\n");
                fprintf(result,"Down:case0\n");

#ifdef GROUP
                increase_redundant();
#endif
                if(MAX_STATES && nstates > MAX_STATES)
                    caching();
                if(nstates > nstates_max)
                    nstates_max = nstates;
                break;
            default:
                Uerror("Bad type returned by hstore\n");
        }
        if (depth > mreached)
            mreached = depth;
#ifdef GROUP
#ifdef TIME
        if(dfs_group->compressed)
        {
            printf("Down:dfs_group->compressed\n");
            fprintf(result,"Down:dfs_group->compressed\n");
            goto Up;
        }
#endif
#endif
Again:
        n=select_transitions(select_flag,n_par,-1);
#ifdef GROUP
        if(n==0)
        {
            if(nb_trans_in_sleep==0)
            {
                if(dfs_group->compressed==0)
                {
                    switch(dfs_group->active_gid)
                    {
                        case 0:
                            endstate_count[0]++;
                            break;
                        case 1:
                            endstate_count[1]++;
                            break;
                        case 2:
                            endstate_count[2]++;
                            break;
                    }
                }
                if(select_par)//select_par = 0,parallel;select_flag = 1,schedule;
                {
                    transfer_par(t);
                    select_flag = 1;
                    select_transitions_par(select_flag,n_par,-1);
                    if(n_par[0] == 0)
                        goto End;
                    else
                    {
                    }
                }
                else
                {
                    /*    transfer();
                          n = select_transitions();
                          if(n == 0)
                          {
                          goto End;
                          }else
                          {}
                    // copy_stpt();*/
                    select_flag = 0;
                    goto End;
                }
            }else
            {
                goto End;
            }
        }
#else
        if(n==0)
        {
            goto End;
        }
#endif
        //print_pset(fo,n,stpt);
        if(!select_flag)//if select_flag=0
        {
Downagain:
            printf("Downagain\n");
            fprintf(result,"Downagain\n");
#ifdef SHOW
            /*if(++step>NB_STEP)
              {
              step=0;
              printf("depth reached %d\n", mreached);
              printf("%8d states, stored\n", nstates);
              printf("%8d states, cleared\n", ncleared);
              printf("%8d states, matched	   total: %8d\n",
              truncs, nstates+ncleared+truncs);
              printf("%8d errors found\n", errors);
              printf("memory used=%d bytes\n",memcnt);
              fflush(stdout);
              }*/
#endif
            t = *stpt;
            stpt--;
            stpt_ind--;
#ifdef SLEEP
            dfs_stpt->l = lptr;
            dfs_stpt->m = mptr;
            rm_tr_confl(t);
#endif
            execute(t,fo,-1);
            printf("\nexecute:%s\n",t->n.tr->tp);
            fprintf(result,"\nexecute:%s\n",t->n.tr->tp);
#ifdef GROUP
            switch(dfs_group->active_gid)
            {
                case 0:
                    visitedstate_count[0]++;
                    break;
                case 1:
                    if(dfs_group->compressed == 0)
                        visitedstate_count[1]++;
                    break;
                case 2:
                    if(dfs_group->compressed == 0)
                        visitedstate_count[2]++;
                    break;
            }
#endif
#ifdef GROUP
            refresh(t,dfs_stpt);
#endif
            cache_flag =1;
            depth++;
            dfs_stpt->tr = t;
            dfs_stpt->hashp = 0;
#ifdef SLEEP
#ifdef OUT
            print_sleep(fo);
#endif
#endif

            dfs_stpt->branch = n - 1;
            //wyy_beg
            printf("Downagain:dfs_stpt:%s,branch=%d\n",dfs_stpt->tr->n.tr->tp,dfs_stpt->branch);
            fprintf(result,"Downagain:dfs_stpt:%s,branch=%d\n",dfs_stpt->tr->n.tr->tp,dfs_stpt->branch);
            //wyy_end
            dfs_stpt++;
            goto Down;
        }//endif select_flag=0



        if(select_flag)//select_flag=1
        {
            omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for reduction(+:depth,truncs,state_count,trace_count_equivalent,nstates,trace_count_sleep,trace_count_end)
            for(nid = 0 ; nid < NUM_THREADS ; nid++)
            {

Downagain_par:
                //nid = omp_get_thread_num();
                printf("Downagain_par:nid = %d\n",nid);
                fprintf(result,"Downagain_par:nid = %d\n",nid);
                if(stpt_par[nid])
                {
                    tt=stpt_par[nid];
                    t = *tt;
                    (stpt_par[nid])--;
                    stpt_ind_para[nid]--;
#ifdef SLEEP
                    dfs_stpt_par[nid]->l=lptr;
                    dfs_stpt_par[nid]->m=mptr;

                    //rm_tr_confl(t);
#endif
                    execute(t,fo,nid);
                    //execute(t,fo,-1);
                    printf("\nexecute:%s\n",t->n.tr->tp);
                    fprintf(result,"\nexecute:%s\n",t->n.tr->tp);
                    printf("stpt_ind_para[%d] = %d\n",nid,stpt_ind_para[nid]);
                    fprintf(result,"stpt_ind_para[%d] = %d\n",nid,stpt_ind_para[nid]);
#ifdef GROUP
                    switch(dfs_group->active_gid)
                    {
                        case 0:
                            visitedstate_count[0]++;
                            break;
                        case 1:
                            if(dfs_group->compressed==0)
                                visitedstate_count[1]++;
                            break;
                        case 2:
                            if(dfs_group->compressed==0)
                                visitedstate_count[2]++;
                            break;
                    }
#endif
                    cache_flag=1;

                    depth++;
                    dfs_stpt_par[nid]->tr=t;
                    dfs_stpt_par[nid]->hashp=0;

#ifdef SLEEP
#ifdef OUT
                    //print_sleep(fo);
#endif
#endif

                    dfs_stpt_par[nid]->branch = n_par[nid]-1;
                    //        printf("dfs_stpt_par[%d]:%d\n",dfs_stpt_par[nid]);
                    printf("dfs_stpt_par[%d]:%s,branch=%d\n",nid,dfs_stpt_par[nid]->tr->n.tr->tp,dfs_stpt_par[nid]->branch);
                    fprintf(result,"dfs_stpt_par[%d]:%s,branch=%d\n",nid,dfs_stpt_par[nid]->tr->n.tr->tp,dfs_stpt_par[nid]->branch);
                    dfs_stpt_par[nid]++;
                    goto Down_par;

Down_par:
                    printf("\nDown_par:nid = %d\n",nid);
                    fprintf(result,"\nDown_par:nid = %d\n",nid);
                    if (depth >= maxdepth)
                    {
                        int double_dfs = double_dfs_stack();
                        if(!double_dfs)
                        {
                            if(!depth_flag)
                            {
                                printf("maxdepth is too small, the search will be truncated\n");
                                fprintf(result,"maxdepth is too small, the search will be truncated\n");
                                depth_flag=1;
                            }
                            truncs++;
                            goto Up_par;
                        }
                    }

                    int temp = hstore(addr_current_state(), vsize);
                    state_count++;
                    switch(temp)
                    {
                        case 1:
                            truncs++;
                            trace_count_equivalent++;
                            //insert_trace("hstore return 1",depth,dfs_stpt);
#ifdef DB
                            printf("depth%d: trace is truncated by state equivalence:\n",depth);
                            fprintf(result,"depth%d: trace is truncated by state equivalence:\n",depth);
                            print_now();
#endif
                            goto Up_par;
                            break;
                        case 2: //this case hasn't happen till now
                            truncs++;
#ifdef SLEEP
                            sleep_mismatch=1;
#endif
                            break;
                        case 0:
                            nstates++; //nstates is unique during dfs

#ifdef GROUP
                            increase_redundant();
#endif
                            if(MAX_STATES && nstates > MAX_STATES)
                                caching();
                            if(nstates > nstates_max)
                                nstates_max = nstates;
                            break;
                        default:
                            Uerror("Bad type returned by hstore\n");
                    }
                    if (depth > mreached)
                        mreached = depth;
#ifdef GROUP
#ifdef TIME
                    if(dfs_group->compressed)
                        goto Up_par;
#endif
#endif
                }//end if(stpt_par[nid]if()

Again_par:
                    printf("\nAgain_par:nid = %d\n",nid);
                    fprintf(result,"\nAgain_par:nid = %d\n",nid);
#pragma omp barrier
#pragma omp master
                    //                if(nid == 0)
                    {
#pragma omp critical
                        {
                            printf("before select_transitions_par,nid == %d\n",nid);
                            fprintf(result,"before select_transitions_par,nid == %d\n",nid);
                            //select_transitions_par(select_flag,n_par,nid);
                            select_transitions_par(select_flag,n_par,-1);
                            printf("end select_transitions_par,nid == %d\n",nid);
                            fprintf(result,"end select_transitions_par,nid == %d\n",nid);

                        }
                    }
            //}//end if(stpt_par[nid])


#pragma omp barrier
#ifdef GROUP
            if(n_par[nid]==0)
            {
                if(nb_trans_in_sleep==0)
                {
                    if(dfs_group->compressed==0)
                    {
                        switch(dfs_group->active_gid)
                        {
                            case 0:
                                endstate_count[0]++;
                                break;
                            case 1:
                                endstate_count[1]++;
                                break;
                            case 2:
                                endstate_count[2]++;
                                break;
                        }
                    }
                }
                goto End_par;
            }
#else
            if(n_par[nid]==0)
                goto End_par;
#endif
            goto Downagain_par;
End_par:
            //wyy_beg
            // char *add = addr_current_state();
            printf("\nEnd_par:nid = %d\n",nid);
            fprintf(result,"\nEnd_par:nid = %d\n",nid);
            if(nb_trans_in_sleep>0)
            {
                trace_count_sleep++;
#ifdef DB
                printf("depth%d: trace is truncated by sleep set:\n",depth);
                fprintf(result,"depth%d: trace is truncated by sleep set:\n",depth);
                print_now();
#endif
            }else
            {
                trace_count_end++;
                // #ifdef GROUP
                //  amend_redundant();
                //#endif
                //
#ifdef DB
                printf("depth%d: trace reaches end:\n",depth);
                fprintf(result,"depth%d: trace reaches end:\n",depth);
                print_now();
#endif
            }
#ifdef SLEEP
            if(nb_trans_in_sleep > 0)
            {
                printf("nb_trans_in_sleep>0\t");
                fprintf(result,"nb_trans_in_sleep>0\t");
                goto Up_par;
            }
#endif
            //  goto Up_par;
Up_par:
            printf("\nUp_par:nid = %d\n",nid);
            fprintf(result,"\nUp_par:nid = %d\n",nid);

            /* nid = omp_get_thread_num(); */

            //printf("************End_par:nid=%d\n",nid);
            //                if(nid == 0)
#pragma omp barrier
#pragma omp master
            {
                if (depth > 0)
                {
                    int i;
                    select_flag = 0;//select_flag = 0;
                    select_par = 0;
                    for(i = 0 ; i<NUM_THREADS ; i++)//copy the group dfs_stpt_par to the dfs_stpt
                    {
                        int k = 0;
                        dfs_stpt_par[i]--;
                        while(dfs_stpt_par[i]->tr)
                        {
                            dfs_stpt_cp++;
                            k++;
                            dfs_stpt_cp = dfs_stpt_par[i];
                            printf("k = %d,dfs_stpt_par:%s,branch = %d\n",k,dfs_stpt_par[i]->tr->n.tr->tp,dfs_stpt_par[i]->branch);
                            fprintf(result,"k = %d,dfs_stpt_par:%s,branch = %d\n",k,dfs_stpt_par[i]->tr->n.tr->tp,dfs_stpt_par[i]->branch);
                            dfs_stpt_par[i]--;
                        }
                        while(k)//instore the dfs_stpt_par[i],and fresh thr sleep set
                        {
                            *dfs_stpt = *dfs_stpt_cp;
                            dfs_stpt->tr = dfs_stpt_cp->tr;
                            dfs_stpt->l = dfs_stpt_cp->l;
                            dfs_stpt->m = dfs_stpt_cp->m;
                            dfs_stpt->branch = dfs_stpt_cp->branch;
                            dfs_stpt->hashp = dfs_stpt_cp->hashp;
                            t = (dfs_stpt)->tr;
                            printf("loopdfs = %d,End_par:dfs_stpt:%s\t",k,t->n.tr->tp);
                            fprintf(result,"loopdfs = %d,End_par:dfs_stpt:%s\t",k,t->n.tr->tp);
                            rm_tr_confl(t);
#ifdef GROUP

                            refresh(t,dfs_stpt);
#endif
                            dfs_stpt_cp++;
                            k--;
                            dfs_stpt++;
                        }
                        printf("\n");
                        fprintf(result,"\n");

                        k = 0;
                        while((*stpt_par[i]))//instore the left elements in stpt_par[i]
                        {
                            k++;
                            stpt++;
                            printf("stpt_par[%d]:%p,%s\n",i,stpt_par[i],(*stpt_par[i])->n.tr->tp);
                            fprintf(result,"stpt_par[%d]:%p,%s\n",i,stpt_par[i],(*stpt_par[i])->n.tr->tp);
                            // (*stpt)->n = (*stpt_par[i])->n;
                            (*stpt) = (*stpt_par[i]);
                            //
                            //     (*stpt)->n.tr = (*stpt_par[i])->n.tr;
                            //     (*stpt)->n.pr = (*stpt_par[i])->n.pr;
                            //     (*stpt)->n.type = (*stpt_par[i])->n.type;

                            printf("loopstpt = %d,End_par:stpt:%s,",k,(*stpt)->n.tr->tp);
                            fprintf(result,"loopstpt = %d,End_par:stpt:%s,",k,(*stpt)->n.tr->tp);
                            stpt_par[i]--;
                        }
                        printf("\n");
                        fprintf(result,"\n");
                        //wyy_beg
                        char *add = addr_current_state();
                        deleted_trail(i);
                    }//end for
                    copy_trail(num_group);
                }//end depth>0
            }//end critical
        }//end for
/* goto Up;
   break; */
}//endif select_flag = 1

/* {
Downagain_par:
nid = omp_get_thread_num();
printf("Downagain_par:nid = %d\n",nid);
fprintf(result,"Downagain_par:nid = %d\n",nid);
if(stpt_par[nid])
{
tt=stpt_par[nid];
t = *tt;
(stpt_par[nid])--;
stpt_ind_para[nid]--;
#ifdef SLEEP
dfs_stpt_par[nid]->l=lptr;
dfs_stpt_par[nid]->m=mptr;

//rm_tr_confl(t);
#endif
execute(t,fo,nid);
//execute(t,fo,-1);
printf("\nexecute:%s\n",t->n.tr->tp);
fprintf(result,"\nexecute:%s\n",t->n.tr->tp);
printf("stpt_ind_para[%d] = %d\n",nid,stpt_ind_para[nid]);
fprintf(result,"stpt_ind_para[%d] = %d\n",nid,stpt_ind_para[nid]);
#ifdef GROUP
switch(dfs_group->active_gid)
{
case 0:
visitedstate_count[0]++;
break;
case 1:
if(dfs_group->compressed==0)
visitedstate_count[1]++;
break;
case 2:
if(dfs_group->compressed==0)
visitedstate_count[2]++;
break;
}
#endif */
/*
#ifdef GROUP
refresh(t,dfs_stpt_par[nid]);
#endif
*/
/* cache_flag=1;

   depth++;
   dfs_stpt_par[nid]->tr=t;
   dfs_stpt_par[nid]->hashp=0;

#ifdef SLEEP
#ifdef OUT
//print_sleep(fo);
#endif
#endif

dfs_stpt_par[nid]->branch = n_par[nid]-1;
//        printf("dfs_stpt_par[%d]:%d\n",dfs_stpt_par[nid]);
printf("dfs_stpt_par[%d]:%s,branch=%d\n",nid,dfs_stpt_par[nid]->tr->n.tr->tp,dfs_stpt_par[nid]->branch);
fprintf(result,"dfs_stpt_par[%d]:%s,branch=%d\n",nid,dfs_stpt_par[nid]->tr->n.tr->tp,dfs_stpt_par[nid]->branch);
dfs_stpt_par[nid]++;
goto Down_par;

Down_par:
printf("\nDown_par:nid = %d\n",nid);
fprintf(result,"\nDown_par:nid = %d\n",nid);
if (depth >= maxdepth)
{
int double_dfs = double_dfs_stack();
if(!double_dfs)
{
if(!depth_flag)
{
printf("maxdepth is too small, the search will be truncated\n");
fprintf(result,"maxdepth is too small, the search will be truncated\n");
depth_flag=1;
}
truncs++;
goto Up_par;
}
}

int temp = hstore(addr_current_state(), vsize);
state_count++;
switch(temp)
{
case 1:
truncs++;
trace_count_equivalent++;
//insert_trace("hstore return 1",depth,dfs_stpt);
#ifdef DB
printf("depth%d: trace is truncated by state equivalence:\n",depth);
fprintf(result,"depth%d: trace is truncated by state equivalence:\n",depth);
print_now();
#endif
goto Up_par;
break;
case 2: //this case hasn't happen till now
truncs++;
#ifdef SLEEP
sleep_mismatch=1;
#endif
break;
case 0:
nstates++; //nstates is unique during dfs

#ifdef GROUP
increase_redundant();
#endif
if(MAX_STATES && nstates > MAX_STATES)
caching();
if(nstates > nstates_max)
nstates_max = nstates;
break;
default:
Uerror("Bad type returned by hstore\n");
}
if (depth > mreached)
    mreached = depth;
#ifdef GROUP
#ifdef TIME
if(dfs_group->compressed)
    goto Up_par;
#endif
#endif
    //}//end if(stpt_par[nid]if()

Again_par:
printf("\nAgain_par:nid = %d\n",nid);
fprintf(result,"\nAgain_par:nid = %d\n",nid);
#pragma omp barrier
#pragma omp master
//                if(nid == 0)
{
#pragma omp critical
    {
        printf("before select_transitions_par,nid == %d\n",nid);
        fprintf(result,"before select_transitions_par,nid == %d\n",nid);
        //select_transitions_par(select_flag,n_par,nid);
        select_transitions_par(select_flag,n_par,-1);
        printf("end select_transitions_par,nid == %d\n",nid);
        fprintf(result,"end select_transitions_par,nid == %d\n",nid);

    }
}
}//end if(stpt_par[nid]if()


#pragma omp barrier
#ifdef GROUP
if(n_par[nid]==0)
{
    if(nb_trans_in_sleep==0)
    {
        if(dfs_group->compressed==0)
        {
            switch(dfs_group->active_gid)
            {
                case 0:
                    endstate_count[0]++;
                    break;
                case 1:
                    endstate_count[1]++;
                    break;
                case 2:
                    endstate_count[2]++;
                    break;
            }
        }
    }
    goto End_par;
}
#else
if(n_par[nid]==0)
    goto End_par;
#endif
    goto Downagain_par;
End_par:
//wyy_beg
// char *add = addr_current_state();
printf("\nEnd_par:nid = %d\n",nid);
fprintf(result,"\nEnd_par:nid = %d\n",nid);
if(nb_trans_in_sleep>0)
{
    trace_count_sleep++;
#ifdef DB
    printf("depth%d: trace is truncated by sleep set:\n",depth);
    fprintf(result,"depth%d: trace is truncated by sleep set:\n",depth);
    print_now();
#endif
}else
{
    trace_count_end++;
    // #ifdef GROUP
    //  amend_redundant();
    //#endif
    //
#ifdef DB
    printf("depth%d: trace reaches end:\n",depth);
    fprintf(result,"depth%d: trace reaches end:\n",depth);
    print_now();
#endif
}
#ifdef SLEEP
if(nb_trans_in_sleep > 0)
{
    fprintf(result,"nb_trans_in_sleep>0\t");
    goto Up_par;
}
#endif
//  goto Up_par;
Up_par:
printf("\nUp_par:nid = %d\n",nid);
fprintf(result,"\nUp_par:nid = %d\n",nid);
nid = omp_get_thread_num();
//printf("************End_par:nid=%d\n",nid);
//                if(nid == 0)
#pragma omp barrier
#pragma omp master
{
    if (depth > 0)
    {
        int i;
        select_flag = 0;//select_flag = 0;
        select_par = 0;
        for(i = 0 ; i<NUM_THREADS ; i++)//copy the group dfs_stpt_par to the dfs_stpt
        {
            int k = 0;
            dfs_stpt_par[i]--;
            while(dfs_stpt_par[i]->tr)
            {
                dfs_stpt_cp++;
                k++;
                dfs_stpt_cp = dfs_stpt_par[i];
                printf("k = %d,dfs_stpt_par:%s,branch = %d\n",k,dfs_stpt_par[i]->tr->n.tr->tp,dfs_stpt_par[i]->branch);
                fprintf(result,"k = %d,dfs_stpt_par:%s,branch = %d\n",k,dfs_stpt_par[i]->tr->n.tr->tp,dfs_stpt_par[i]->branch);
                dfs_stpt_par[i]--;
            }
            while(k)//instore the dfs_stpt_par[i],and fresh thr sleep set
            {
                *dfs_stpt = *dfs_stpt_cp;
                dfs_stpt->tr = dfs_stpt_cp->tr;
                dfs_stpt->l = dfs_stpt_cp->l;
                dfs_stpt->m = dfs_stpt_cp->m;
                dfs_stpt->branch = dfs_stpt_cp->branch;
                dfs_stpt->hashp = dfs_stpt_cp->hashp;
                t = (dfs_stpt)->tr;
                printf("loopdfs = %d,End_par:dfs_stpt:%s\t",k,t->n.tr->tp);
                fprintf(result,"loopdfs = %d,End_par:dfs_stpt:%s\t",k,t->n.tr->tp);
                rm_tr_confl(t);
#ifdef GROUP

                refresh(t,dfs_stpt);
#endif
                dfs_stpt_cp++;
                k--;
                dfs_stpt++;
            }
            printf("\n");
            fprintf(result,"\n");

            k = 0;
            while((*stpt_par[i]))//instore the left elements in stpt_par[i]
            {
                k++;
                stpt++;
                printf("stpt_par[%d]:%p,%s\n",i,stpt_par[i],(*stpt_par[i])->n.tr->tp);
                fprintf(result,"stpt_par[%d]:%p,%s\n",i,stpt_par[i],(*stpt_par[i])->n.tr->tp);
                // (*stpt)->n = (*stpt_par[i])->n;
                (*stpt) = (*stpt_par[i]);
                //
                //     (*stpt)->n.tr = (*stpt_par[i])->n.tr;
                //     (*stpt)->n.pr = (*stpt_par[i])->n.pr;
                //     (*stpt)->n.type = (*stpt_par[i])->n.type;

                printf("loopstpt = %d,End_par:stpt:%s,",k,(*stpt)->n.tr->tp);
                fprintf(result,"loopstpt = %d,End_par:stpt:%s,",k,(*stpt)->n.tr->tp);
                stpt_par[i]--;
            }
            printf("\n");
            fprintf(result,"\n");
            //wyy_beg
            char *add = addr_current_state();
            deleted_trail(i);
        }//end for
        copy_trail(num_group);
    }//end depth>0
}//end critical
}//end pragma
goto Up;
}//endif select_flag = 1 */
Up:
printf("Up\n");
fprintf(result,"Up\n");
if(depth == 0)
{
#ifdef WARAPUP
    trace_count_total = trace_count_equivalent + trace_count_sleep + trace_count_end;
#ifdef GROUP
    printf("state redundant = %d\n",state_redundant);
    fprintf(result,"state redundant = %d\n",state_redundant);
    expectedtrace_count += trace_count_total;

    //calculate expected state count
    int i = 0;
    int j;
    for(; i<num_group; i++)
    {
        expectedtrace_count += visitedstate_count[i];
        int series = 1;
        if(i < (num_group - 1))
        {
            series = endstate_count[i] - 1;
            for(j = i+1;j<num_group;j++)
            {
                series *= endstate_count[j];
            }
            printf("i = %d,series = %d\n",i,series);
            fprintf(result,"i = %d,series = %d\n",i,series);
            expectedtrace_count += series * (num_group-i-1);
            expectedtrace_count += series;
            trace_count_compressed += series;
        }
    }
    printf("expectedtrace_count = %d\n",expectedtrace_count);
    fprintf(result,"expectedtrace_count = %d\n",expectedtrace_count);
    printf("trace count:total=%d,equivalent=%d,sleep=%d,end=%d,compressed=%d\n\n", \
            trace_count_total,trace_count_equivalent,trace_count_sleep,trace_count_end \
            trace_count_compressed);
    fprintf(result,"trace count:total=%d,equivalent=%d,sleep=%d,end=%d,compressed=%d\n\n", \
            trace_count_total,trace_count_equivalent,trace_count_sleep,trace_count_end \
            trace_count_compressed);
    printf("visited state count:");
    fprintf(result,"visited state count:");
    for(i = 0;i<num_group;i++)
    {
        printf("\nvisitedstate_count[%d]=%d ",i,visitedstate_count[i]);
        fprintf(result,"\nvisitedstate_count[%d]=%d ",i,visitedstate_count[i]);
        total_visitedstate_count += visitedstate_count[i];
    }
    printf("\ntotal visited state count=%d\n",total_visitedstate_count);
    fprintf(result,"\ntotal visited state count=%d\n",total_visitedstate_count);
    printf("\nendstate count:");
    fprintf(result,"\nendstate count:");
    for(i = 0; i<num_group; i++)
    {
        printf("\nendstate_count[%d] = %d ",i,endstate_count[i]);
        fprintf(result,"\nendstate_count[%d] = %d ",i,endstate_count[i]);
    }
    printf("\nexpected state count=%d\n",expectedstate_count);
    fprintf(result,"\nexpected state count=%d\n",expectedstate_count);
#else
    printf("trace count:total = %d,equivalent=%d,sleep=%d, end=%d\n\n", \
            trace_count_total,trace_count_equivalent,trace_count_sleep, \
            trace_count_end);
    fprintf(result,"trace count:total = %d,equivalent=%d,sleep=%d, end=%d\n\n", \
            trace_count_total,trace_count_equivalent,trace_count_sleep, \
            trace_count_end);
#endif

#endif
    goto End1;
}
dfs_stpt--;
//wyy_beg
printf("Up:dfs_stpt:%s,branch=%d\n",dfs_stpt->tr->n.tr->tp,dfs_stpt->branch);
fprintf(result,"Up:dfs_stpt:%s,branch=%d\n",dfs_stpt->tr->n.tr->tp,dfs_stpt->branch);
// char *add = addr_current_state();
//wyy_end
#ifdef GROUP
dfs_group--;
#endif
#if !(defined HASHC) && (defined SLEEP)
if(dfs_stpt->hashp && dfs_stpt->hashp->sl_unmatch)
{
    dfs_stpt->hashp->sl_unmatch = 0;
    sleep_mismatch = 1;
    goto Again;
}
#endif
n = dfs_stpt->branch;

/*#ifdef PROV*/
if(dfs_stpt->hashp)
    no_more_in_stack(dfs_stpt->hashp);
    /*#endif*/
    t = dfs_stpt->tr;
    printf("backtr:%s\n",t->n.tr->tp);
    fprintf(result,"backtr:%s\n",t->n.tr->tp);
    backtr(fo,-1);
    /*
       printf("Back depth=%d",depth);
       PTRANS(t);
       */
#ifdef GROUP
    set_nmitl(fo,dfs_stpt,depth);
#endif

#ifdef SLEEP
    *lptr++ = t;
    if(lptr == lptr_max)
if(!double_lptr())
    Uerror("MAX_L_LIST is too small,not enough memory available\n");
    restore_sleep();
#endif

    depth--;

#ifdef SLEEP
    *mptr++=t;
    if(mptr == mptr_max)
if(!double_mptr())
    Uerror("MAX_M_LIST is too small,not enough memory available\n");
#endif
    transfer_unlock(t);//if t == group[gid+1].indicator,then lock the gid+1~num_group's group_blocked
if(n>0)
{
    transfer_block(t);//blocked the 0~gid-1 group_blocked
    //wyy_beg
    char *add = addr_current_state();
    goto Downagain;
}
if(n==0)
{
    printf("Up:n==0\t");
    fprintf(result,"Up:n==0\t");
    //transfer_unlock(t);
    goto Up;
}
goto Downagain;

End:
printf("End\n");
fprintf(result,"End\n");
if(nb_trans_in_sleep>0)
{
    trace_count_sleep++;
#ifdef DB
    printf("depth%d: trace is truncated by sleep set:\n",depth);
    fprintf(result,"depth%d: trace is truncated by sleep set:\n",depth);
    print_now();
#endif
}else
{
    trace_count_end++;
    /*#ifdef GROUP
      amend_redundant();
#endif
*/
#ifdef DB
    printf("depth%d: trace reaches end:\n",depth);
    fprintf(result,"depth%d: trace reaches end:\n",depth);
    print_now();
#endif
}
//insert_trace("select_transition return 0",depth,dfs_stpt);
#ifdef SLEEP
if(nb_trans_in_sleep > 0)
{
    printf("nb_trans_in_sleep>0\t");
    fprintf(result,"nb_trans_in_sleep>0\t");
    goto Up;
}
#endif
/*if (!endstate() && nb_proc()
  &&  depth < maxdepth-1)
  {
  printf("trace count=%d\n",trace_count_total);
  uerror("invalid endstate");
  }*/
if(depth > 0)
{
    printf("depth>0\t");
    fprintf(result,"depth>0\t");
    goto Up;
}
goto End1;
End1:printf("\n\nnew_state_PO:return\n\n");
fprintf(result,"\n\nnew_state_PO:return\n\n");
// return;
}

#ifdef SLEEP
double_lptr()
{
    TRANS **tmppt;
    int newsize, i;
    register DFS_stack *p;

    if (MAX_L_LIST >= 10000)
        newsize = MAX_L_LIST + 10000;
    else
        newsize = MAX_L_LIST * 2;
#ifdef DYNSTRUCTCONT
    printf("doubling lptr, newsiz=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
    fprintf(result,"doubling lptr, newsiz=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
#endif
    tmppt = (TRANS **)erealloc(l_list_table,newsize*sizeof(TRANS *),
            MAX_L_LIST*sizeof(TRANS *));

    if (tmppt)
    {
        lptr = tmppt + (lptr - l_list_table);
        lptr_max=tmppt+newsize;
        p = dfs_stack;
        for (i=0; i<=depth; i++, p++)
            p->l = tmppt + (p->l - l_list_table);
        l_list_table = tmppt;
        MAX_L_LIST = newsize;
        if(NULL!=fo)
        {
            fprintf(fo,"*DLlist*\n");
        }
        return 1;
    }
    else
        return 0;
}

double_mptr()
{
    TRANS **tmppt;
    int newsize, i;
    register DFS_stack *p;

    if (MAX_M_LIST >= 100000)
        newsize = MAX_M_LIST + 100000;
    else
        newsize = MAX_M_LIST * 2;
#ifdef DYNSTRUCTCONT
    printf("doubling mptr, newsiz=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
    fprintf(result,"doubling mptr, newsiz=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
#endif

    tmppt = (TRANS **)erealloc(m_list_table,newsize*sizeof(TRANS *),
            MAX_M_LIST*sizeof(TRANS *));

    if (tmppt)
    {
        mptr = tmppt + (mptr - m_list_table);
        mptr_max=tmppt+newsize;
        p = dfs_stack;
        for (i=0; i<=depth; i++, p++)
            p->m = tmppt + (p->m - m_list_table);
        m_list_table = tmppt;
        MAX_M_LIST = newsize;
        if(NULL!=fo)
        {
            fprintf(fo,"*DMlist*\n");
        }
        return 1;
    }
    else
        return 0;
}
#endif

double_dfs_stack()
{
    DFS_stack *tmppt;
    int ndpth;

    if(static_stack)
        return 0;
    if (maxdepth<100000)
        ndpth = maxdepth*2;
    else
        ndpth = maxdepth+50000;

#ifdef DYNSTRUCTCONT
    printf("doubling dfs_stack, newsiz=%d\tmemory=%d\n",ndpth,ndpth*sizeof(DFS_stack));
    fprintf(result,"doubling dfs_stack, newsiz=%d\tmemory=%d\n",ndpth,ndpth*sizeof(DFS_stack));
#endif
    tmppt = (DFS_stack *)erealloc(dfs_stack,ndpth*sizeof(DFS_stack),
            maxdepth*sizeof(DFS_stack));

    if (tmppt)
    {
        dfs_stpt= tmppt + (dfs_stpt - dfs_stack);
        dfs_stack=tmppt;
        maxdepth=ndpth;
        if(NULL!=fo)
        {
            fprintf(fo,"*Ddfs stack*\n");
        }
        return 1;
    }
    else
        return 0;
}

double_stpt()
{
    TRANS **tmppt;
    int newsize;
    //wyy_beg
    TRANS **tmppt0,**tmppt1;
    //wyy_end

    if (MAX_Stack_tr_size >= 200000)
        newsize = MAX_Stack_tr_size + 100000;
    else
        newsize = MAX_Stack_tr_size * 2;
#ifdef DYNSTRUCTCONT
    printf("doubling stpt, newsize=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
    fprintf(result,"doubling stpt, newsize=%d\tmemory=%d\n",newsize,newsize*sizeof(TRANS *));
#endif

    tmppt = (TRANS **)erealloc(stpt_org,newsize*sizeof(TRANS *),
            MAX_Stack_tr_size*sizeof(TRANS *));

    if (tmppt)
    {
        stpt = tmppt + (stpt - stpt_org);
        stpt_org = tmppt;

        //wyy_beg
        tmppt0 = (TRANS **)erealloc(stpt_org,newsize*sizeof(TRANS *),
                MAX_Stack_tr_size*sizeof(TRANS *));
        if(tmppt0)
        {
            stpt_par[0] = tmppt0 + (stpt_par[0] - stpt_org);
            stpt_org = tmppt0;
        }
        else
            return 0;

        stpt_org = tmppt0;
        tmppt1 = (TRANS **)erealloc(stpt_org,newsize*sizeof(TRANS *),
                MAX_Stack_tr_size*sizeof(TRANS *));
        if(tmppt1)
        {
            stpt_par[1] = tmppt1 + (stpt_par[1] - stpt_org);
        }
        else
            return 0;
        //wyy_end
        MAX_Stack_tr_size = newsize;
        if(NULL!=fo)
        {
            fprintf(fo,"*DTrans Stack*\n");
        }
        return 1;
    }
    else
        return 0;
}





main(argc, argv)
    char *argv[];
{
    while (argc > 1 && argv[1][0] == '-')
    {
        switch (argv[1][1])
        {
            default :
                if(!extern_option(argv[1][1]))
                {
                    usage();
                    exit(1);
                }
                break;
            case 'c': upto  = atoi(&argv[1][2]); break;
            case 'm': maxdepth = atoi(&argv[1][2]); static_stack=1;break;
            case 'w': ssize = atoi(&argv[1][2]); break;
#if (defined HASHC) || (defined BITSTATE)
#endif
#ifndef BITSTATE
            case 'n': MAX_STATES = atoi(&argv[1][2]); break;
#endif
#ifdef HASHC
            case 'b': bsize = atoi(&argv[1][2]);
                      if (bsize>64 || bsize<8)
                      {
                          bsize = 64;
                          fprintf(stderr, "b must be between 8 and 64. b set to %d.\n",bsize);
                      }
                      break;
#endif
        }
        argc--; argv++;
    }
    if (ssize>32)
    {
        fprintf(stderr, "w must be less than 32. w set to %d.\n",32);
        ssize=32;
    }
#ifdef HASHC
    if ((((bsize-1)>>3)+1)*8+(((ssize-1)>>3)+1)*8>64)
    {
        fprintf(stderr, "b set to %d.\n",64-(((ssize-1)>>3)+1)*8);
        bsize=64-(((ssize-1)>>3)+1)*8;
    }
    tw=bsize/8;
#endif

    /* wyy_beg */
    open_result_file(result);
    /* wyy_end */

    signal(SIGINT, wrapup);
    run();
    //print_traces(fo);
    done = 1;
#ifdef WRAPUP
    wrapup();
#endif
    exit(0);
}

usage()
{
    fprintf(stderr, "unknown option\n");
    fprintf(stderr, "-cN stop at Nth error ");
    fprintf(stderr, "(default=1)\n");
    fprintf(stderr, "-mN max depth N (default=10k)\n");
    fprintf(stderr, "-wN hashtable of 2^N entries ");
#ifdef HASHC
    fprintf(stderr, "-bN states stored on N bits (default=64)\n");
#endif
#ifndef BITSTATE
    fprintf(stderr, "-nN max states stored N\n");
#endif
}

Uerror(str)
    char *str;
{

    printf("pan: %s (at depth %d)\n", str,
            (depthfound==-1)?depth:depthfound);
    putrail();
    wrapup();
    exit(0);
}

uerror(str)
    char *str;
{
    if(NULL!=fo)
    {
        fprintf (fo,"*ERR*\n");
    }
    if (++errors == upto)
    {
        printf("pan: %s (at depth %d)\n", str,
                (depthfound==-1)?depth:depthfound);
        putrail();
        wrapup();
        exit(0);
    }
    return 1;
}

void print_stpt(TRANS_DEF **trans_def)
{
    int i=0;
    for(;i < num_thread; i++)
    {
        printf("%s",trans_def[i][0].tr->n.tr->tp);
        fprintf(result,"%s",trans_def[i][0].tr->n.tr->tp);
    }
    printf("\n");
    fprintf(result,"\n");
    //    for(i = 0;i < num_thread; i++)
    //      printf("%s",trans_def[i][1].tr->n.tr->tp);
}

int max_stpt(int stpt,int *stpt_ind)
{
    // int max_stpt_ind = stpt_ind[0];
    int  i = 0;
    while(i<num_thread)
    {
        if(stpt < stpt_ind[i])
            stpt = stpt_ind[i];
        i++;
    }
    return stpt;
}
