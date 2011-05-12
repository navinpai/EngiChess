/* Game Tree Implementation*/



struct tnode 
{	Board  cbrd;
	int turn;
	struct tnode	*son;
	struct tnode	*next;
};


typedef struct tnode tnode;


tnode* getnode(Board b)
{	
	tnode *temp=NULL;
	temp= new tnode;
	if(temp==NULL)
	{	cout<<endl<<"ERROR : MEMORY  INSUFFICIENT.";	getch();	exit(0);	}
	temp->cbrd=b;
	temp->turn=1;
	temp->son=NULL;
	temp->next=NULL;
	return temp;
}


tnode* append(tnode* list,tnode* x)
{	
	tnode *temp=NULL;
	tnode *p=NULL;
	if(list==NULL)
	{	
		list=x;		
	}
	else
	{	
		temp=list;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=x;
	}
	/*
	cout<<endl<<"Printing in Append.\n";
	getch();
	p=list;
	while(p!=NULL)
	{	p->cbrd.printboard();
		getch();
		p=p->next;
	}
    */
	return list;
}


tnode* generate(Board b)
{
	int i,j;
	tnode *list=NULL;
	tnode *aux=NULL;
	Board tem;

	for(i=0;i<64;i++)
	{	
		for(j=0;j<64;j++)
		{
			tem=b;
			if((tem.checkvalid(i,j)==1) && (tem.checkturn(i)==1))
			{
				//cout<<endl<<"i:"<<i<<endl;
				//cout<<endl<<"j:"<<j<<endl;
				tem.makemove(i,j);
                //cout<<endl<<"Changing list:"<<endl;
				aux=getnode(tem);
				list=append(list,aux);
				//cout<<endl<<"out of append"<<endl;
				//aux->cbrd.printboard();
				//getch();
			}
		}
	}
	//cout<<endl<<"Out of List"<<endl;

	/*cout<<endl<<"Printing LIST"<<endl;
	tnode *p=NULL;
	p=list;
	while(p!=NULL)
	{	p->cbrd.printboard();
		getch();
		p=p->next;
	}
	getch();*/
   // cout<<endl<<"Finished printing list so Leaving Generate"<<endl;
	return list;
}
				

			
void expand(tnode* p,int plevel,int depth)
{
	tnode *q;
	//cout<<endl<<"In Expand"<<endl;getch(); 
	if(plevel<depth)
	{	
        //cout<<endl<<"In Expand giving call to generate"<<endl;getch(); 
		q=generate(p->cbrd);
		//cout<<endl<<"Returning from generate to expand"<<endl;getch(); 
		p->son=q;
		while(q!=NULL)
		{	
			if(p->turn==1)
			q->turn=-1;
			else
			q->turn=1;
			q->son=NULL;
			expand(q,plevel+1,depth);
			q=q->next;
		}
	}
	//cout<<endl<<"Leaving Expand"<<endl;getch(); 
}


tnode* buildtree(Board brd,int depth)
{	
	cout<<endl<<"In Build tree"<<endl;getch(); 
	tnode *ptree;
	ptree=getnode(brd);
	expand(ptree,0,depth);
	return ptree;
}


void bestbranch(tnode* pnd,tnode **pbest,int *pvalue)
{		
		Evaluation eval;

		tnode *p=NULL;	tnode *pbest2=NULL;
		int val;

		if(pnd->son==NULL)
		{	*pvalue=eval.EvaluateBoardScore(pnd->cbrd);
			//cout<<endl<<"Value="<<*pvalue<<"\nFor board:\n";
			*pbest=pnd;
			//pnd->cbrd.printboard();
			//getch();
		}
		else
		{	p=pnd->son;
			bestbranch(p,pbest,pvalue);
			*pbest=p;
			if(pnd->turn ==-1)
				*pvalue=-*pvalue;
			p=p->next;
			while(p!=NULL)
			{	bestbranch(p,&pbest2,&val);
				if(pnd->turn==-1)
					val=-val;
				if(val > *pvalue)
				{	*pvalue=val;
					*pbest=p;
				}
				p=p->next;
			}
			if(pnd->turn==-1)
				*pvalue=-*pvalue;
		}
}

			
void printtree(tnode* tree)
{	if(tree)
	{	tree->cbrd.printboard();	getch();
		if(tree->next)	printtree(tree->next);
		if(tree->son)	printtree(tree->son);
	}
}


Board nextmove(Board brd,int depth)
{	tnode* ptree;	tnode *best;
	int value;

	ptree=buildtree(brd,depth);
	bestbranch(ptree,&best,&value);
	return best->cbrd;
}







/*--------------------------------------------------*/