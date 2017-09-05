#include<malloc.h>
#include<stdio.h>
#include<string.h>
#define MAXNODES 27
#define LENGTH 10000
#define NODESIZE 100
#define FILENAME "st.stod"

struct suffixtree_node
        {
        int startindex;
        int endindex;
        int numberofsons;
        struct suffixtree_node *son[MAXNODES]; //#define MAXNODES To some 27 or maximum possible length of
        }*sthead;

struct tandem
        {
        char data[LENGTH];
        struct tandem *next;
        }*head,*curr;

void tandemRepeat(char *s);
void border(char *s);
char * strrev(char * str);
void retrievesuffixtree();
void buildtree(struct suffixtree_node *temp,int offset);
void displaysuffixtree(struct suffixtree_node *treenode,int tab);
void printstringcontents(int start,int end);
void substringmatch(struct suffixtree_node *temp);
void longestpossiblesubstring();
void finddeepestforknode(struct suffixtree_node*);
void finddeepestforknode1(struct suffixtree_node*);
void findpalindrome();
void longestrepeatedsubstring();

int tabcount;
int substringindex=0,matchedornot=0;
int check=0;
char string[LENGTH],substring[LENGTH];

int main()
	{
	system("clear");
	while(1)
                {
		int choice;
		printf("\n------------------------------Main Menu---------------Suffix Tree Construction Part----");
                printf("\n Enter 1 to BUILD the SUFFIX Tree and make it PERSISTENT");
                printf("\n Enter 2 to DISPLAY the SUFFIX Tree of the String");
		printf("\n Enter 3 to Search for a Substring match");
		printf("\n Enter 4 to Run Applications LMS,LRS,PALINDROME,tANDEMrEPEATS,bORDERrEPEATS");
                printf("\n Enter 5 to Exit From the Application\n\t");
		scanf("%d",&choice);
		system("clear");
		switch(choice)
			{
			case 1:		{
					char command[LENGTH+20];
					printf("\n Enter String to build the Suffix tree and make it persistant\n\t\t");
					scanf("%s",string);
					strcpy(command,"./suffixtree ");
					strcat(command,string); 
					system(command);
				       printf("\n\tSUFFIX TREE for %s is BUILD SUCCESSFULLY! SELECT 2 to view the tree");
					}
					break;

			case 2:		{
					printf("\nSUFFIX TREE FOR THE STRING %s\n",string);
					retrievesuffixtree();
					}
					break;

			case 4:		{
					while(1)
						{		
						printf("\n\t------------------------------Sub Menu---------------Suffix Tree Application Part----");
						printf("\n\t Enter 1 to find Longest Repeated Substring");
			                	printf("\n\t Enter 2 to find Longest Possible SUBSTRING");
			                	printf("\n\t Enter 3 palindrome");
			                	printf("\n\t Enter 4 for Tandem Repeats");
			                	printf("\n\t Enter 5 for Border Repeats");
						printf("\n\t Enter 6 to DISPLAY the SUFFIX Tree of the String Formed");	
						printf("\n\t Enter 7 to Exit from this submenu\n\t\t");
						scanf("%d",&choice);
				                system("clear");
					        switch(choice)
							{
							case 1: {
								printf("\n\t Longest repeated substring\n");
								printf("\n\tEnter the String1 to find Longest repeated substring it \n\t\t");
								scanf("%s",string);
								longestrepeatedsubstring();
								}
							        break;
							case 2: {
								printf("\n\tEnter the String1 to find Longest Possible SUBSTRING match \n\t\t");
								scanf("%s",substring);
								printf("\n\tEnter the String2 to find Longest Possible SUBSTRING match \n\t\t");
								scanf("%s",string);
								longestpossiblesubstring();
								}
								break;
							case 3: {
								printf("\n\tPalindrome \n");
								printf("\n\tEnter the String1 to find Longest Possible Palindrome in it \n\t\t");
								scanf("%s",string);
								findpalindrome();
								}
							        break;
												       
							case 4: {
								printf("\n -- Tandem Repeat\n");
							        printf("\n\tEnter the String1 to find Tandem Repeats in it \n\t\t");
						                scanf("%s",string);
								tandemRepeat(string);
								}
								break;
							case 5: {
							        printf("\n --- Border Repeat\n");
							        printf("\n\tEnter the String1 to find Longest Possible Palindrome in it \n\t\t");
								scanf("%s",string);
							        border(string);
							        }
							        break;
							case 6:	{
								printf("\n\tSUFFIX TREE FOR THE STRING %s\n",string);
								retrievesuffixtree();
								}                                        
								break;
							case 7: goto VENKI;
								break;		
							default : {
								  printf("\n InCorrect choice entry! Try again\n");fflush(stdin);
								  }
							          break;
							}			 
						}
					}
					break;	
			case 3:		{
					int printed=0;	
					printf("\n\t\t\tSUBSTRING MATCHING\n");
					if(sthead==0)
						printf("\n Tree is not yet Constructed,select 1 and 2 \n");
					else
						{
						int var=0;		
					printf("\nEnter the String to find whether it matches or not(SUBSTRING)\n\t\t");
						scanf("%s",substring);
						if(strlen(string)<strlen(substring))
                                                        {
                                                        printf("\n\t\t String doesnot match!\n\n");
                                                        break;
                                                        }
						for(var=0;var<sthead->numberofsons;var++)	
							{
							substringmatch(sthead->son[var]);	
							if(matchedornot==1)
								{
								printf("\n\t\t Given string is a Substring!\n\n");
								break;
								}
							else if(matchedornot==2)
                                                                {
                                                                printf("\n\t\t String doesnot match!\n\n");
                                                                break;
                                                                }
							if(check==1)
                                                                {
								printed=1;	
                                                                printf("\n\t\t String doesnot match!\n\n");
                                                                break;
                                                                }
							}
						}
					if(matchedornot==0 && printed==0)
                                                printf("\n\t\t String doesnot match!\n\n");
					substringindex=0;
					matchedornot=0;
					check=0;
					}
					break;

			case 5:		{
					printf("\n\t\t\t Thank U\n");	
					goto NAIDU;
					}


			default : 	{
					printf("\n InCorrect choice entry! Try again\n");fflush(stdin);
					}
					break;
			}
		VENKI: printf("\t");	
		}
	NAIDU: printf("\t\n");
	}

void retrievesuffixtree()
	{
	FILE *fp;
        int stringoffset,headoffset,i=0,l;
        fp=fopen(FILENAME,"r");
        fscanf(fp,"%d %d",&stringoffset,&headoffset);
        fseek(fp,stringoffset,SEEK_SET);
        fscanf(fp,"%s",&string);
        fseek(fp,headoffset,SEEK_SET);
	sthead=(struct suffixtree_node *)malloc(sizeof(struct suffixtree_node));
        fscanf(fp,"%d%d%d",&sthead->startindex,&sthead->endindex,&sthead->numberofsons);
        for(i=0;i<sthead->numberofsons;i++)
                {
		struct suffixtree_node *temp=(struct suffixtree_node *)malloc(sizeof(struct suffixtree_node));
		sthead->son[i]=temp;
		fscanf(fp,"%d",&l);
		buildtree(temp,l);
                }
	displaysuffixtree(sthead,0);
	}

void buildtree(struct suffixtree_node *temp,int offset)
	{
	FILE *tfp;
	int i=0,l=0;
	tfp=fopen(FILENAME,"r");
	fseek(tfp,offset,SEEK_SET);
	fscanf(tfp,"%d%d%d",&temp->startindex,&temp->endindex,&temp->numberofsons);
	for(i=0;i<temp->numberofsons;i++)
		{
		struct suffixtree_node *sttemp=(struct suffixtree_node *)malloc(sizeof(struct suffixtree_node));
                temp->son[i]=sttemp;
                fscanf(tfp,"%d",&l);
                buildtree(sttemp,l);
		}	
	}

void displaysuffixtree(struct suffixtree_node *treenode,int tab)
        {
	int z=0;
        if(treenode->numberofsons!=0)
                {
                if(treenode==sthead)
                        printf("\nSUFFIX ROOT at LEVEL 0");
                else
                        {
                        for(z=0;z<tab;z++) printf("\t");
                        printf("PARENT at %d LEVEL: ",tab);
                        printstringcontents(treenode->startindex,treenode->endindex);
                        }
		printf("\n");
                }
        if(treenode->numberofsons==0)
                {
                for(z=0;z<tab;z++) printf("\t");
                printf("LEAF at %d LEVEL: ",tab);
                printstringcontents(treenode->startindex,treenode->endindex);
                printf("\n");
                return;
                }
        else
                {
                tabcount+=1;
                for(z=0;z<treenode->numberofsons;z++)
                        {
                        displaysuffixtree(treenode->son[z],tabcount);
                        }
                tabcount-=1;
                }
        printf("\n");
        }

void printstringcontents(int start,int end)
        {
        int z;
        for(z=start;z<=end;z++)
                printf("%c",string[z]);
        }

void substringmatch(struct suffixtree_node *temp) // substring index is a global variable to keep track of substring
        {
        int i=0,j=0;
        if(string[temp->startindex]==substring[substringindex])
                {
                check=1;
                substringindex++;
                if(substring[substringindex]=='\0')
                        {
                        matchedornot=1;
                        return;
                        }
                if((temp->startindex - temp->endindex)==0)
                        {
                        for(i=0;i<temp->numberofsons;i++)
                                {
                                substringmatch(temp->son[i]);
                                if(matchedornot==1 || matchedornot==2)
                                        return;
                                }
                        }
                else
                        {
                        for(j=temp->startindex+1;j<=temp->endindex;j++)
                                {
                        	if(string[j]==substring[substringindex++])
                                        {
                                        if(substring[substringindex]=='\0')
                                                {
                                                matchedornot=1;
                                                return;
                                                }
                                        }
                                else
                                        {
                                        matchedornot=2;
                                        return;   
                                        }
                                }
			for(i=0;i<temp->numberofsons;i++)
                        	{
                        	substringmatch(temp->son[i]);
                       		if(matchedornot==1 || matchedornot==2)
                                	return;
                                }
                        }
                }
        }

char buf[100];
int depth=0,initialdepth=0,onlyonce=0,flag=0,yes=0;
void longestpossiblesubstring()
	{
	depth=0,initialdepth=0,onlyonce=0,flag=0,yes=0;
	strcat(string,"^");
	strcat(string,substring);
	strcat(string,"*");
	char command[LENGTH+20];
        strcpy(command,"./suffixtree ");
        strcat(command,string);
        system(command);
	printf("\nSUFFIX TREE FOR THE STRING %s\n",string);
        retrievesuffixtree();
	int i1;
	for(i1=0;i1<sthead->numberofsons;i1++)
		{
		finddeepestforknode(sthead->son[i1]);
		}
	
	if(strlen(buf)!=0)
		printf("\n--->Longest Possible Substring :\n\t %s \n",buf);
	else
		printf("\n\t No Match found");
	}


void reverse()
	{
	int i=0,j=0;
	for(i=strlen(string)-1;i>=0;i--,j++)
		substring[j]=string[i];
	substring[j]='\0';	
	}
	
void findpalindrome()
	{
	reverse();
	depth=0,initialdepth=0,onlyonce=0,flag=0,yes=0;
	strcat(string,"^");
	strcat(string,substring);
	strcat(string,"*");
	char command[LENGTH+20];
        strcpy(command,"./suffixtree ");
        strcat(command,string);
        system(command);
        printf("\nSUFFIX TREE FOR THE STRING %s\n",string);
        retrievesuffixtree();
        int i1;
        for(i1=0;i1<sthead->numberofsons;i1++)
                {
                finddeepestforknode(sthead->son[i1]);
                }
	printf("\n---> Palindrome");
	if(strlen(buf)!=0)
                printf("\n\t %s \n",buf);
        else
                printf("\n\t No Match found");
	}

void finddeepestforknode(struct suffixtree_node *treenode)
	{
	int z=0;
        if(treenode->numberofsons!=0)
                {
                for(z=treenode->startindex;z<=treenode->endindex;z++)
			{
			substring[depth]=string[z];
			depth+=1;
			//printf("\t%c",string[z]);	
			}
                }
        if(treenode->numberofsons==0)
                {
		//printf("\n");
		substring[depth]='\0';
                for(z=treenode->startindex;z<=treenode->endindex;z++)
			{
			if(string[z]=='^')
				{
				flag=1;//printf("\tcame\n");
				}
			if(flag==1 && string[z]=='*')
				{
				//printf("\n what\n");
				if(onlyonce==0)
					{	
					onlyonce=1;
					//printf(" ONLYONCE ");
					}	
				yes=1;			
				break;
				}
			//printf("%c",string[z]);
			}

		if(yes==1)
			{
			if(onlyonce==1)
				{
				onlyonce++;
				strcpy(buf,substring);
				initialdepth=depth;
				}
			else 
				{
				if(depth>initialdepth)
					{
					initialdepth=depth;
					strcpy(buf,substring);
					
					}
				}
			yes=0;flag=0;depth=0;
			//printf(" dindex char =%c,deepest fork=%d",string[dindex],depth);
			}
                return;
                }
        else
                {
                for(z=0;z<treenode->numberofsons;z++)
                        {
                        finddeepestforknode(treenode->son[z]);
                        }
                }
	}

void longestrepeatedsubstring()
        {
        depth=0,initialdepth=0,onlyonce=0,yes=0;
        strcat(string,"^");
        char command[LENGTH+20];
        strcpy(command,"./suffixtree ");
        strcat(command,string);
        system(command);
        printf("\nSUFFIX TREE FOR THE STRING %s\n",string);
        retrievesuffixtree();
        int i1;
        for(i1=0;i1<sthead->numberofsons;i1++)
                {
                finddeepestforknode1(sthead->son[i1]);
                }
	if(strlen(buf)!=0)
                printf("\n--->Longest Repeated Substring:\n\t %s \n",buf);
        else
                printf("\n\t No Match found");
        }

void finddeepestforknode1(struct suffixtree_node *treenode)
	{
	int z=0;
        if(treenode->numberofsons!=0)
                {
                for(z=treenode->startindex;z<=treenode->endindex;z++)
                        {
                        substring[depth]=string[z];
                        depth+=1;
                        //printf("\t%c",string[z]);
                        }
                }
        if(treenode->numberofsons==0)
                {
                //printf("\n");
                substring[depth]='\0';
                for(z=treenode->startindex;z<=treenode->endindex;z++)
                        {
                        if(string[z]=='^')
                                {
                                //printf("\n what\n");
                                if(onlyonce==0)
                                        {
                                        onlyonce=1;
                                        //printf(" ONLYONCE ");
                                        }
                                yes=1;
                                break;
                                }
                        //printf("%c",string[z]);
                        }

	        if(yes==1)
                        {
                        if(onlyonce==1)
                                {
                                onlyonce++;
                                strcpy(buf,substring);
                                initialdepth=depth;
                                }
                        else
                                {
                                if(depth>initialdepth)
                                        {
                                        initialdepth=depth;
                                        strcpy(buf,substring);

                                        }
                                }
                        yes=0;depth=0;
                        //printf(" dindex char =%c,deepest fork=%d",string[dindex],depth);
                        }
                return;
                }
        else
                {
                for(z=0;z<treenode->numberofsons;z++)
                        {
                        finddeepestforknode1(treenode->son[z]);
                        }
                }
        }

void tandemRepeat(char *s)
	{       
	char *tandemArray[LENGTH];
        char string1[LENGTH]="";string1[0]='\0';
	int i,j,same,ctr,ctr1;
	same=0,ctr=0;
        int tag;
        tag=0;
	char *string2 = (char*)malloc(LENGTH*sizeof(char));
	strcpy(string2,string);
	char *s2;
	for(i=2;i<=8;i++)
		{    
		char *s=string;
		for(;*s !='\0';s++)
			{
			memmove( substring, s, i );
			if(strlen(substring)>=i)
				{
				strcpy(string1,substring);	
				char command[LENGTH+20];
				strcpy(command,"./suffixtree ");
				strcat(command,string1); 
				system(command);
				same=0;
				for(s2=string2;*s2 !='\0';s2++)
					{	
					memmove( substring, s2, i );
					if(strlen(substring)>=i)
						{
						if(!strcmp(string1,substring))
							same++;
						}
					}
				strcpy(substring,"");	
				if(same>1)
					{
					curr=head;
					while(curr)
						{
						if(!(strcmp(curr->data,string1)))
							{
							tag=1;
							break;
							}
						curr=curr->next;
						}								
					if(tag==0)
						{
						printf(" \nString %s appears %d times \n",string1,same);
						curr = (struct tandem *)malloc(sizeof(struct tandem));
						strcpy(curr->data,string1);
						curr->next=head;
						head=curr;
						curr=head;
						}
					}tag=0;		
				}
			}
         	
		}
	free(string2);
	}


char * strrev(char * str)
        {
        int length = strlen(str);
        char * result = (char *)malloc(length+1);
        if( result != NULL )
                {
                int i,j;
                result[length] = '\0';
                for ( i = length-1, j=0;   i >= 0;   i--, j++ )
                        result[j] = str[i];
                }
       return result;
        }

void border(char *s)
        {
        int i;
        char substr[LENGTH]="";substr[0]='\0';
        char *sub=substr;
        char revstring[LENGTH]="";revstring[0]='\0';
        char *rev=revstring;
        char revmain[LENGTH]="";revmain[0]='\0';
        char *rmain=revmain;
        char finalstring[LENGTH]="";finalstring[0]='\0';
        char *final=finalstring;
        for(i=0;i<strlen(string);i++)
	        {
	        memcpy(sub,s,i);
	        rev=strrev(s);
	        memcpy(revmain,rev,i);
	        char *final=strrev(revmain);
	        if(!(strcmp(final,sub)))
		        printf("\n%s\n",final);
		}
	}
