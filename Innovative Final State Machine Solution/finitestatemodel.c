#include<stdio.h>
#include<malloc.h>
#define MAXREPEATITION 25
#define MAXNODES 27      //size of english alphbet for instance
#define STEPSIZE 1
#define LENGTH 10000

struct source_character
                {
                unsigned char character;
                struct source_character *next;
                }*schead;

struct FSMState	
	{
	char label;

	int counter[MAXREPEATITION];
					int counterindextracker;//--------to track index of counter array	

	struct FSMState *nextstate[MAXNODES];
					int nextstatecounter;//to keep track of nextstate array of self referential pointers

	char prechar[MAXREPEATITION];    // step size is 1 in this case, for step2--> 4 array should be used
	char postchar[MAXREPEATITION];
					int prepostcharcounter;//---------to track index of prechar and postchararrays 

	struct FSMState *next;      // for construction and display purpose
	}*fsmhead;

void displayentiremachine();
void displaystatecontents(struct FSMState *fsmtemp);
void constructFSMforstring();
void addsourcecharacter(char a);
struct FSMState* createState(char label,int initialcounter,char prechar,char postchar);
void constructorupdatestatemachine(int index,int unique);
void substringmatch();
int transitforacceptance(int index,struct FSMState *fsmtemp);

char string[LENGTH],cstring[LENGTH];

int main()
	{
	system("clear");	
	while(1)
		{
		printf("\n\t\t\tFINITE STATE MODEL FOR PATTERN MATCHING\n");
		int choice;
		printf("\n Enter 1 to construct the FSM for the given string");
		printf("\n Enter 2 to display the machine");
		printf("\n Enter 3 to search for a substring match");
		printf("\n Enter 4 to Quit\n\t");
		scanf("%d",&choice);
		system("clear");
		switch(choice)
			{
			case 1:	printf("\nCONSTRUCTION OF FSM\n\t");
				scanf("%s",&string);
				if(fsmhead!=NULL)  //freeing the machine memmory for new machine construction
                			{
                			struct FSMState *fk;
                			while(fsmhead!=NULL)
                        			{
                        			fk=fsmhead;
                        			fsmhead=fsmhead->next;
                        			free(fk);
                        			}
					struct source_character *sc;
					while(schead!=NULL)
						{
						sc=schead;
						schead=schead->next;
						free(sc);
						}
                			printf("\nvenkiiiiiiiiiiii");
                			}
				constructFSMforstring(); 	
				break;
			case 2: printf("\nFSM for %s\n",string);
				displayentiremachine();
				break;
			case 3: printf("\n Enter the String to search for a match\n\t"); 
				scanf("%s",&cstring);
				substringmatch();
				break;
			case 4: printf("\n\t\t\tTHANK Q\n");
				//exit(0);
				goto VENKATNAIDU;
			default: printf("\n InCorrect Choice! Enter a correct one");
				 break;	
			}
		}
VENKATNAIDU: return(0);
	}

void substringmatch()
	{
	struct FSMState *fsmtemp=fsmhead;
	int i=0,success=0;
	int j=0;
	while( j < fsmtemp->nextstatecounter )
		{
		//printf("\nstate label = %c, in substring match  %c == %c ",fsmtemp->label,cstring[i],
			//				(fsmtemp->nextstate[j])->label);
		if(cstring[i]==(fsmtemp->nextstate[j])->label)
			{
			//printf("\n\t label %c \n",cstring[i]);
			success=transitforacceptance(i,fsmtemp->nextstate[j]);	
			break;	
			}
		else
			j++;
		}
	//---------------------Processing complete print results---------------------------------------------------
	if(success==0)
		printf("\n\t\t SUBSTRING does not exist\n");
	else
		printf("\n\t\t SUBSTRING Exists\n");
	}

int transitforacceptance(int index,struct FSMState *fsmtemp)
	{
	int flag1=0,flag2=0;	
	if(cstring[index+1]=='\0')
		{
		//printf("\n here -------");	
		return(1);
		}
	//printf("\t%c",cstring[index]);
	int i;
	if(index!=0)
		{
		for(i=0;i<fsmtemp->prepostcharcounter;i++)
			{
   	      		//printf("\ninpp %c==%c and %c==%c",cstring[index-1],fsmtemp->prechar[i],
			//				  cstring[index+1],fsmtemp->postchar[i]);
			if( cstring[index-1]==fsmtemp->prechar[i] && cstring[index+1]==fsmtemp->postchar[i] )
				{
				//printf("\n matched");
				flag1=1;
				break;
				}	
			}
		}
	
	if(flag1==1 || index==0)
		{
		if(cstring[index]==cstring[index+1])
			{
			int u=index+1,h=0; 
			while(cstring[index]==cstring[u])
				{u++;h++;}
			//printf("\n %c here counter %d",cstring[index],h);
			for(i=0;i<fsmtemp->counterindextracker;i++)
				{
				if(h==fsmtemp->counter[i])
					{
					//printf("\n here successfully set flag2");
					flag2=1;
					break;
					}
				}
			if(flag2==1)
				{
				index=index+h;
				//printf("\n%d and %c",index,cstring[index]);
				if(cstring[index+1]=='\0') return(1);
				}
			else
				return(0);
			}
		for(i=0;i<fsmtemp->nextstatecounter;i++)
			{
			//printf("\ntransit here if %c==%c",cstring[index+1],(fsmtemp->nextstate[i])->label);
			if(cstring[index+1]==(fsmtemp->nextstate[i])->label)
				{
				//printf("\n\t success here if %c==%c",cstring[index+1],(fsmtemp->nextstate[i])->label);
				return(transitforacceptance(index+1,fsmtemp->nextstate[i]));
				}
			}
		return(0);	
		}
	else 
	return(0);
	}

void constructFSMforstring()
	{
	int distinctornot=0,i=0;// 1 means distinct and 0 means same
        //Creation of Start State of FSM-------------------------------------------------
	fsmhead=createState('$',0,'$','$');
        while(string[i]!='\0')
                {
                if(i==0)
                        {
                        addsourcecharacter(string[i]);
                        constructorupdatestatemachine(i,1);
                        }
                else
                        {
                        struct source_character *sctemp=schead;
                        while(sctemp!=NULL)
                                {
                                if(sctemp->character!=string[i])
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
                                addsourcecharacter(string[i]);
                                constructorupdatestatemachine(i,1);
		                }
                        else
                                constructorupdatestatemachine(i,0); //add to each and every entry of pointerlist
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

void displayentiremachine()
	{
	printf("\n\n\t-----------------------------Finite State Machine Contents--------------------------------\n");
	struct FSMState *fsmtemp=fsmhead;
	while(fsmtemp!=NULL)
		{
		displaystatecontents(fsmtemp);
		fsmtemp=fsmtemp->next;
		}
	printf("\n\t------------------------------------------------------------------------------------------\n");
	}


void displaystatecontents(struct FSMState *fsmtemp)
	{
	if((fsmtemp->label)=='$')
		printf("\n\tState : \t'(%c)' START STATE ",fsmtemp->label);
	else
		printf("\n\tState : \t'(%c)'",fsmtemp->label);
	printf("\n\t\tCounter or Repeatition Counter Contents:\n\t\t\t");
	int i=0;
	for(i=0;i<fsmtemp->counterindextracker;i++)
		printf("%d\t",fsmtemp->counter[i]);
	printf("\n\t\tLabels of Transitions to other States of Automata\n\t\t\t");
	for(i=0;i<fsmtemp->nextstatecounter;i++)
		printf("%c\t",(fsmtemp->nextstate[i])->label);
	printf("\n\t\tPreCharacter -------- PostCharacter\n\t\t");
	for(i=0;i<fsmtemp->prepostcharcounter;i++)
		printf("%c\t\t\t\t%c\n\t\t",fsmtemp->prechar[i],fsmtemp->postchar[i]);
	printf("\n");
	}

void constructorupdatestatemachine(int index,int unique)
	{
	struct FSMState *fsmtracer;	
	if(unique==1)
		{
		struct FSMState *fsmtemp;
		int a=index+1,k=0;
		//----------------State Counter determinator-------------------------------------------------------
                while(string[index]==string[a])
                        {
                        //printf("\n\t a=%d,index=%d string[i]=%c",a,index,string[a]);
                        a++;k++;
                        if(string[a]=='\0') break;
                        }
		//----------------State Construction---------------------------------------------------------------
		if(index==0)	
			{
			if(string[index+1]=='\0')
				fsmtemp=createState(string[index],k,'$','\0');
			else
				fsmtemp=createState(string[index],k,'$',string[index+1]);
			}
		else
			{
			if(string[index+1]=='\0')
				fsmtemp=createState(string[index],k,string[index-1],'\0');
			else
				fsmtemp=createState(string[index],k,string[index-1],string[index+1]);
			
			//----------------determine the next state transition of previous symbol-------------------
			struct FSMState *fsmstatetrans=fsmhead;
			while(fsmstatetrans!=NULL)
				{
				if(fsmstatetrans->label==string[index-1]) 
					{
					int h=0,set=0;
                                        for(h=0;h<fsmstatetrans->nextstatecounter;h++)
                                        	{
                                        	if((fsmstatetrans->nextstate[h])->label==string[index])
                                        		set=1;
                                        	}
                                        if(set==0)
						fsmstatetrans->nextstate[fsmstatetrans->nextstatecounter++]=fsmtemp;	
					//printf("\nvenki");
					}
				fsmstatetrans=fsmstatetrans->next;
				}
			}
		fsmtracer=fsmhead;
		while(fsmtracer->next!=NULL)
			fsmtracer=fsmtracer->next;
		fsmtracer->next=fsmtemp;
		fsmhead->nextstate[fsmhead->nextstatecounter++]=fsmtemp;
		}
	else 
		{
		fsmtracer=fsmhead->next;
		struct FSMState *fsmnextstate=fsmhead->next;
		while(fsmtracer!=NULL)
			{
			if(fsmtracer->label==string[index])
				{
				//printf("\nchar %c",string[index]);
				int flag=0,i1=0;
				//----------------Step size=1---prepostchar updation-------------------------------	
				for(i1=0;i1<fsmtracer->prepostcharcounter;i1++)
					{
					if(fsmtracer->prechar[i1]==string[index-1] &&
					   fsmtracer->postchar[i1]==string[index+1])
						flag=1;	
					}
				if(flag==0)
					{
					fsmtracer->prechar[fsmtracer->prepostcharcounter]=string[index-1];
					if(string[index+1]!='\0')
						fsmtracer->postchar[fsmtracer->prepostcharcounter++]=string[index+1];	
					else
						fsmtracer->postchar[fsmtracer->prepostcharcounter++]='\0';
					}
				//----------------State Counter determinator---------------------------------------
				int a=index+1,k=0;
				while(string[index]==string[a])
					{
					//printf("\n\t a=%d,index=%d string[i]=%c",a,index,string[a]); 
					a++;k++;
					if(string[a]=='\0') break;
					}				
				if(k!=0)
					{
					int j=0,doit=0;
					for(j=0;j<fsmtracer->counterindextracker;j++)
						if(k==fsmtracer->counter[j])
							doit=1;
					if(doit==0)
						fsmtracer->counter[fsmtracer->counterindextracker++]=k;	
					}	
				//----------------determine the next state transition of previous symbol-----------
				struct FSMState *fsmstatetrans=fsmhead;
				if(string[index]!=string[index-1])
	                        	while(fsmstatetrans!=NULL)
        	                        	{
                	                	if(fsmstatetrans->label==string[index-1])
                        	                	{
							int h=0,set=0;
							for(h=0;h<fsmstatetrans->nextstatecounter;h++)
								{	
								if((fsmstatetrans->nextstate[h])->label==string[index])
									set=1;
								}
							if(set==0)		
                                	             		fsmstatetrans->nextstate[fsmstatetrans->nextstatecounter++]
										=fsmtracer;
                                        		//printf("\nvenki");
                                        		}
                                		fsmstatetrans=fsmstatetrans->next;
                                		}

				break;
				}
                        fsmtracer=fsmtracer->next;
			}
		}
	}

struct FSMState* createState(char label,int initialcounter,char prechar,char postchar)//initial counter will be 0
        {
        struct FSMState *newstate=(struct FSMState*)malloc(sizeof(struct FSMState));
        newstate->label=label;
        newstate->counter[0]=initialcounter;
	newstate->counterindextracker=1;
        newstate->nextstatecounter=0;
        newstate->prechar[0]=prechar;
        newstate->postchar[0]=postchar;
        newstate->prepostcharcounter=1;
        newstate->next=NULL;
        return(newstate);
        }

