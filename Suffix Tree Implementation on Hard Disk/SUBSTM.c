#include<string.h>
#include<stdio.h>
#define NODEOFFSET 300
void displaysuffixtree1(int block,int tab);	
void substringmatch(int block);
int headoffset=NODEOFFSET,substringindex=0;
char substring[1000];
int matchedornot=0;
FILE *ifp,*fpx;
int main()
	{
	char choice=0;	
	system("clear");
	while(1)
		{
		printf("\n\tBUILDING SUFFIX TREE ON HARDDISK DIRECTLY AND PROCESSING ON HARDDISK\n");
		printf("\n\t\t Enter 1 to Construct Tree on Harddisk");
		printf("\n\t\t Enter 2 to Display the Tree");
		printf("\n\t\t Enter 3 to Match Substring");
		printf("\n\t\t Enter 4 to EXIT\n\t\t\t");
		scanf("%d",&choice);
		system("clear");
		switch(choice)
			{
			case 1: printf("\n\t\t   Choose the Form of Input to Construct suffixtree");
				printf("\n\t\t\t Enter 1 to enter input file name");
				printf("\n\t\t\t Enter 2 to enter input string\n\t\t\t\t\t");	
				char f[1000],fs[1000];
				scanf("%d",&choice);
				switch(choice)
					{
					case 1:
						printf("\n\t\t\t\tEnter the Input File Name\n\t\t\t\t\t");
						scanf("%s",&f);
						strcat(fs,"./suffixtreehdd ");
						strcat(fs,f);
						system(fs);
						ifp=fopen(f,"r");
						break;
					case 2: 
						printf("\n\t\t\t\tEnter the String \n\t\t\t\t\t");
						scanf("%s",&f);
						ifp=fopen("input","w");
						fprintf(ifp,"%s",f);
						fclose(ifp);
						system("./suffixtreehdd input");
						ifp=fopen("input","r");
						break;
					}	
				fpx=fopen("vst.stod","r+w");
				break;
			case 2:	
				if(fpx==NULL)
					{
					printf("\n\t\t\tChoose 1 to construct the tree on harddisk\n\n");
					break;
					}
				displaysuffixtree1(headoffset,0);
				break;
			case 3:	
				printf("\n Enter the Substring to test whether it matches or not\n\t");
				scanf("%s",&substring);
				matchedornot=0;
				int stindexx1,edindexx1,nuosx1,i1=0;
        			fseek(fpx,NODEOFFSET,SEEK_SET);
        			fscanf(fpx,"%d%d%d",&stindexx1,&edindexx1,&nuosx1);
				//printf("\n %d %d %d",stindexx1,edindexx1,nuosx1);
				for(i1=0;i1<nuosx1;i1++)
	        			{
					int blocky1,xy1;
					fscanf(fpx,"%d",&blocky1);
					xy1=ftell(fpx);
					substringmatch(blocky1);
					fseek(fpx,xy1,SEEK_SET);
					if(matchedornot==1)
						{
						printf("\n\t\tGiven string is a Substring!\n\n");
						break;
						}
        				else  if(matchedornot==2)
						{
	        				printf("\n\t\tString doesnot match!\n\n");
						break;
						}
					}
				if(matchedornot==0)
					{
					printf("\n\t\tString doesnot match!iii\n\n");	
					}
				break;
			case 4:
				printf("\n\n\t\t\t\t\tTHANK Q\n\n");
				goto VENKATNAIDU;
			default:
				printf("\nEnter Correct Choice\n");
				break;	
			}
		}		
VENKATNAIDU:
	return(0);
	}

int tabcount1=0;
void displaysuffixtree1(int block,int tab) // 0 initially
        {
        int z=0;
        FILE *llp=fpx;
        char c;
        int stindexx,edindexx,nuosx;
        fseek(llp,block,SEEK_SET);
        fscanf(llp,"%d%d%d",&stindexx,&edindexx,&nuosx);
        if(nuosx!=0)
                {
                if(block==headoffset)
                        printf("\nSUFFIX ROOT at LEVEL 0");
                else
                        {
                        for(z=0;z<tab;z++) printf("\t");
                        printf("%d PARENT at %d LEVEL: ",block,tab);
                        rewind(ifp);
                        fseek(ifp,stindexx,SEEK_SET);
                        for(z=stindexx;z<=edindexx;z++)
                                {
                                fscanf(ifp,"%c",&c);
                                printf("%c",c);
                                }
                        }
                printf("\n");
                }
      if(nuosx==0)
                {
                for(z=0;z<tab;z++) printf("\t");
                printf("%d LEAF at %d LEVEL: ",block,tab);
                rewind(ifp);
                        fseek(ifp,stindexx,SEEK_SET);
                for(z=stindexx;z<=edindexx;z++)
                                {
                                fscanf(ifp,"%c",&c);
                                printf("%c",c);
                                }
                printf("\n");
                return;
                }
        else
                {
                tabcount1+=1;
                for(z=0;z<nuosx;z++)
                        {
                        int blocky,xy;
                        fscanf(llp,"%d",&blocky);
                        xy=ftell(llp);
                        displaysuffixtree1(blocky,tabcount1);
                        fseek(llp,xy,SEEK_SET);
                        }
                tabcount1-=1;
                return;
                }
        printf("\n");
        }

void substringmatch(int block)
	{
	FILE *llp=fpx;
        char c;
	int stindexx,edindexx,nuosx,i=0,j=0;
	fseek(llp,block,SEEK_SET);
	fscanf(llp,"%d%d%d",&stindexx,&edindexx,&nuosx);
	//printf("\n%d %d %d",stindexx,edindexx,nuosx);			
	rewind(ifp);
        fseek(ifp,stindexx,SEEK_SET);
	fscanf(ifp,"%c",&c);
	if(c==substring[substringindex])
		{
			//printf("wat");
		substringindex++;
		if(substring[substringindex]=='\0')
	  	      	{//printf("\nhere");
	        	matchedornot=1;
	        	return;
	        	}
		if((stindexx-edindexx)==0)
			{
			for(i=0;i<nuosx;i++)
				{
				int blocky,xy;
		                fscanf(llp,"%d",&blocky);
		                xy=ftell(llp);
		                substringmatch(blocky);
		                fseek(llp,xy,SEEK_SET);
			 	if(matchedornot==1 || matchedornot==2)
                                	return;
				}
			}
		else
			{
			for(j=stindexx+1;j<=edindexx;j++)
				{
				rewind(ifp);
			        fseek(ifp,j,SEEK_SET);
			        fscanf(ifp,"%c",&c);
				if(c==substring[substringindex++])
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
			 for(i=0;i<nuosx;i++)
			        {
			        int blocky,xy;
			        fscanf(llp,"%d",&blocky);
			        xy=ftell(llp);
			        substringmatch(blocky);
			        fseek(llp,xy,SEEK_SET);
			        if(matchedornot==1 || matchedornot==2)
				        return;
				}
			}		
		//printf("\nvenkiiiiiiiii");
		}
		






























		
	}
