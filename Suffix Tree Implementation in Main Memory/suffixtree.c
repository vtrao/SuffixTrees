#include<stdio.h>
#include<string.h>
#define MAXNODES 27	
#define LENGTH 1000
#define NODESIZE 100


	struct source_character
        	{
        	unsigned char character;
        	struct source_character *next;
        	}*schead;

	struct suffixtree_node
        	{
        	int startindex;
        	int endindex; 
                int numberofsons;
        	struct suffixtree_node *son[MAXNODES]; //#define MAXNODES To some 27 or maximum possible length of
        	}*sthead;

void SuffixTree(char *s);
void addsourcecharacter(char a);
void constructorupdatetree(int index,int unique);
void formChainTillEndOfString(int startposition,struct suffixtree_node *father);
void socarecursive(int index,struct suffixtree_node *sttracer);
void compactandoptimize();
void recursivilycompactsubtree(struct suffixtree_node *temp);
void showsuffixtree(struct suffixtree_node*,int tab);
void showsuffixesofgivenstring(struct suffixtree_node *treenode);
void writenodetoharddisk(struct suffixtree_node *,int child[]);

int globalindex;
char S[LENGTH];
FILE *stfp;
int tabcount=0;
int curpos=0;
int headposition;

int main(int argc, char *argv[])
	{
	if(argc==1)
		{
		printf("\n Enter the String to construct the Suffix tree for the Same\n");
		scanf("%s",S);
		}
	else
                {
                strcpy(S,argv[1]);
		}	
       	SuffixTree(S);
	compactandoptimize();
	stfp=fopen("st.stod","w");			// to persistence
	showsuffixtree(sthead,0);
	curpos=ftell(stfp);
	fprintf(stfp,"%s ",S);
	rewind(stfp);
	fprintf(stfp,"%d ",curpos);
	fprintf(stfp,"%d ",headposition);
	fclose(stfp);					//	showsuffixesofgivenstring(sthead);
	return(0);
	}


void SuffixTree(char *S)
	{
	int distinctornot=0,i=0;// 1 means distinct and 0 means same
	//Creation of rootnode of tree-------------------------------------------------
        sthead=(struct suffixtree_node*)malloc(sizeof(struct suffixtree_node));
        sthead->startindex=0;
        sthead->endindex=0;
        sthead->numberofsons=0;
        sthead->son[0]=NULL;
        while(S[i]!='\0')
                {
                if(i==0)
                        {
                        addsourcecharacter(S[i]);
			constructorupdatetree(i,1);
                        }
                else
                        {
                        struct source_character *sctemp=schead;
                        while(sctemp!=NULL)
                                {
                                if(sctemp->character!=S[i])
                                        distinctornot=1;
                                else
                                        {
                                        distinctornot=0;
                                        break;
                                        }
                                sctemp=sctemp->next;
                                }
                        if(distinctornot==1) // new one or distinct one
                                {
                                addsourcecharacter(S[i]);
				constructorupdatetree(i,1);
                                }
                        else
                                {               //add to each and every entry of pointerlist
				constructorupdatetree(i,0);
                                }
                        }
                i++;
                distinctornot=0;
                }
	}

void addsourcecharacter(char a)
        {
        if(schead!=NULL)
                {
                struct source_character *sctemp=schead;
                while(sctemp->next!=NULL)
                        sctemp=sctemp->next;
                struct source_character *sctracer=(struct source_character*)malloc(sizeof(struct source_character));
                sctracer->character=a;
                sctracer->next=NULL;
                sctemp->next=sctracer;
                }
        else
                {
                schead=(struct source_character*)malloc(sizeof(struct source_character));
                schead->character=a;
                schead->next=NULL;
                }
        }

void constructorupdatetree(int index,int unique)
        {
	struct suffixtree_node *sttemp;
        if(unique==1) // a new character arrived
                {
                sttemp=(struct suffixtree_node*)malloc(sizeof(struct suffixtree_node));
                sttemp->startindex=index;
                sttemp->endindex=index;
                sttemp->numberofsons=0;
                sttemp->son[0]=NULL;
                sthead->son[sthead->numberofsons]=sttemp;
                sthead->numberofsons+=1;
                formChainTillEndOfString(index+1,sttemp);
                }
        else         // same old character arrived, so compare with existing 1st levels symbols
                {
                int k;
                struct suffixtree_node *sttracer;
                sttemp=sthead;
                for(k=0;k<sttemp->numberofsons;k++)
                        {
                        sttracer=sttemp->son[k];
                        if(S[sttracer->startindex]==S[index])
                                socarecursive(index+1,sttracer);
                        }
                }
	}


void socarecursive(int index,struct suffixtree_node *sttracer)
        {
	struct suffixtree_node *sttemp=sttracer;
        int k;
        for(k=0;k<sttemp->numberofsons;k++)
                {
                sttracer=sttemp->son[k];
                if(S[sttracer->startindex]==S[index])
                        {
                        socarecursive(index+1,sttracer);//straceris to sent but is global so no need
                        return;
                        }
                }
        if(S[index]!='\0')
                {
                struct suffixtree_node *newnode=(struct suffixtree_node*)malloc(sizeof(struct suffixtree_node));
                newnode->startindex=index;
                newnode->endindex=index;
                newnode->numberofsons=0;
                newnode->son[0]=NULL;
                sttemp->son[sttemp->numberofsons]=newnode;
                sttemp->numberofsons+=1;
                formChainTillEndOfString(index+1,newnode);
                }
        }

void formChainTillEndOfString(int startposition,struct suffixtree_node *father)
        {
	int i;
	struct suffixtree_node *temp;
	for(i=startposition;S[i]!='\0';i++)
		{
		temp=(struct suffixtree_node*)malloc(sizeof(struct suffixtree_node));
	        temp->startindex=i;
        	temp->endindex=i;
        	temp->numberofsons=0;
        	temp->son[0]=NULL;
		father->son[father->numberofsons]=temp;
        	father->numberofsons+=1;
		father=temp;	
		temp=NULL;
		}	
        }

void compactandoptimize()
        {
        struct suffixtree_node *sttemp=sthead;
        int k;
        for(k=0;k<sthead->numberofsons;k++)
                {
                sttemp=sthead->son[k];
                recursivilycompactsubtree(sttemp);
                }
        }

void recursivilycompactsubtree(struct suffixtree_node *temp)
        {
        if(temp->numberofsons==1)
                {
                struct suffixtree_node *child;
                child=temp->son[0];
                temp->numberofsons=child->numberofsons;
                int j=0;
                for(j=0;j<child->numberofsons;j++)
                        temp->son[j]=child->son[j];
                temp->endindex+=1;
                free(child);
                recursivilycompactsubtree(temp);
                }
        else if(temp->numberofsons==0)
                {
                }
        else
                {
                int l;
                for(l=0;l<temp->numberofsons;l++) 
			{
                        recursivilycompactsubtree(temp->son[l]);
			}
                }
        }

void showsuffixtree(struct suffixtree_node *treenode,int tab)
	{
	int z=0,children[27],m=0;
	if(treenode->numberofsons==0)
		{
		writenodetoharddisk(treenode,children);
		return;
		}
	else
		{
		tabcount+=1;
        	for(z=0;z<treenode->numberofsons;z++)
                	{
			showsuffixtree(treenode->son[z],tabcount);	
			children[m++]=curpos;
			}
		tabcount-=1;
		}
	if(treenode->numberofsons!=0)
                {
                if(treenode==sthead)
                        {
			headposition=ftell(stfp)+NODESIZE;
                        writenodetoharddisk(treenode,children);
                        }
                else
                        {
			writenodetoharddisk(treenode,children);
                        }
                }
	}

void writenodetoharddisk(struct suffixtree_node *temp,int child[])
        {
	int i;
        fseek(stfp,NODESIZE,SEEK_CUR);
        curpos=ftell(stfp);
        fprintf(stfp,"%d %d %d ",temp->startindex,temp->endindex,temp->numberofsons);
	for(i=0;i<temp->numberofsons;i++)
		fprintf(stfp,"%d ",child[i]);
        }
