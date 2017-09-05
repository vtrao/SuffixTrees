#include<malloc.h>
#include<stdio.h>
#include<string.h>
#define NODEOFFSET 300
#define FILEE "vst.stod"
#define MAXNODES 35

	struct source_character
                {
                unsigned char character;
                struct source_character *next;
                }*schead;

void SuffixTree();
void addsourcecharacter(char a);
void constructorupdatetree(int index,int unique);
void formChainTillEndOfString(int);
void socarecursive(int ,int);
void displaysuffixtree(int block,int tab,int parentoff);
void compactandoptimize();
void recursivilycompactsubtree(int blockto);

char S[25],substring[200];
FILE *ifp;
FILE *hddfp,*tempfp,*fpx;
int ch,nodeoffset=NODEOFFSET,dummy,headoffset=NODEOFFSET;
int nos[MAXNODES],substringindex=0,check=0,matchedornot=0;

int main(int argc, char *argv[])
        {
	if(argc==1)
                {
                printf("\n Enter the filename to construct the Suffix tree for the Same\n");
                scanf("%s",S);
                }
        else
                strcpy(S,argv[1]);
        ifp=fopen(S,"r");               // reading the source file on which suffix tree is to be built
        hddfp=fopen("vst.stod","w");
        fclose(hddfp);
        SuffixTree();
        compactandoptimize();
        return(0);
	}

void SuffixTree()
	{
	int distinctornot=0,counter=0;// 1 means distinct and 0 means same
        //Creation of rootnode of tree-------------------------------------------------
	hddfp=fopen("vst.stod","r+w");
	fpx=hddfp;
	fprintf(hddfp,"%8d ",NODEOFFSET); 
	rewind(hddfp);
	fseek(hddfp,NODEOFFSET,SEEK_SET);
	nodeoffset+=NODEOFFSET;
	fprintf(hddfp,"%8d %8d %8d ",0,0,0);
	rewind(ifp);
	while((ch=getc(ifp))!=EOF)
		{
		if(ch==10) break;
		//printf("\nCharacter: %c ",ch);
		if(ftell(ifp)==1)
			{
			addsourcecharacter(ch);
			constructorupdatetree(ftell(ifp)-1,1);
			}
		else
			{
			struct source_character *sctemp=schead;
                        while(sctemp!=NULL)
                                {
                                if(sctemp->character!=ch)
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
                                addsourcecharacter(ch);
                                constructorupdatetree(ftell(ifp)-1,1);
                                }
                        else
                                {               //add to each and every entry of pointerlist
                                constructorupdatetree(ftell(ifp)-1,0);
                                }
                        }
                distinctornot=0;
		counter++;
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
	//printf("\n\t\t\t\t distinct character encountered\t%c",a);
        }

void constructorupdatetree(int index,int unique)
        {
	int numberofsons,i=0;
        if(unique==1) // a new character arrived
                {
		rewind(hddfp);
		tempfp=hddfp;
		fseek(tempfp,nodeoffset,SEEK_SET);       //tempfp contains the current address of new node
		fprintf(tempfp,"%8d %8d %8d ",index,index,0);
		fseek(hddfp,headoffset,SEEK_SET);
		fscanf(hddfp,"%d%d%d",&dummy,&dummy,&numberofsons);	// first two are for dummy purpose 	
		for(i=0;i<numberofsons;i++)
			fscanf(hddfp,"%d",&nos[i]);
		nos[i]=nodeoffset;
		numberofsons+=1;
		rewind(hddfp);
		fseek(hddfp,headoffset,SEEK_SET);            // cleaning for 100 bytes should be followed to clear garbage
		fprintf(hddfp,"%8d %8d %8d ",0,0,numberofsons);
		for(i=0;i<numberofsons;i++)
			fprintf(hddfp,"%8d ",nos[i]);
	//printf("\n\tUNIcharacter = %c, input offset is = %d",ch,ftell(ifp)-1);
		nodeoffset+=NODEOFFSET;
		formChainTillEndOfString(ftell(ifp)-1);
                }
        else         // same old character arrived, so compare with existing 1st levels symbols
                {
		FILE *sfp,*inputfp;
		char ich,curchar;       //to store input character for comparision
		inputfp=fopen("input","r");       
		fseek(inputfp,index,SEEK_SET);
		fscanf(inputfp,"%c",&curchar);		 
		rewind(hddfp);
		tempfp=hddfp;
		fseek(tempfp,headoffset,SEEK_SET);
		fscanf(tempfp,"%d%d%d",&dummy,&dummy,&numberofsons);
		//printf("\n\tSame character = %c, inputoffset = %d",ch,ftell(ifp)-1);
                int k,blockaddress=0,ab=0;
                for(k=0;k<numberofsons;k++)
                        {
			fscanf(tempfp,"%d",&blockaddress);
			ab=ftell(tempfp);
			fseek(tempfp,blockaddress,SEEK_SET);
			fscanf(tempfp,"%d",&dummy);         // retrieve startindex of distinct child of root
			fseek(inputfp,dummy,SEEK_SET);
			fscanf(inputfp,"%c",&ich);
                        if(ich==curchar)
				{
				//printf("\n\t\tcharacter = %c,index = %d,%d Curchar = %c",ich,dummy,ab,curchar);
				socarecursive(blockaddress,index+1);
				return;   // note ittttttttttttttttttttttttttttttttttttttttttttttttt
				}	
			fseek(tempfp,ab,SEEK_SET);
                        }
                }
        }

void socarecursive(int currentpointer,int index)
        {
	int numberofsons1;
	char ich1,curchar1;
	rewind(hddfp);
	FILE *socafp=hddfp,*infp;
	infp=fopen("input","r");
	fseek(infp,index,SEEK_SET);
	fscanf(infp,"%c",&curchar1);
	fseek(socafp,currentpointer,SEEK_SET);
	fscanf(socafp,"%d%d%d",&dummy,&dummy,&numberofsons1);
	int k=0,blockadd=0,ab1=0;
        for(k=0;k<numberofsons1 && curchar1!=10;k++)
                {
		fscanf(socafp,"%d",&blockadd);
		ab1=ftell(socafp);
		fseek(socafp,blockadd,SEEK_SET);
		fscanf(socafp,"%d",&dummy);
		fseek(infp,dummy,SEEK_SET);
		fscanf(infp,"%c",&ich1);
                //printf("\n\t in for k=%d,nos=%d %c==or!= %c",k,numberofsons1,curchar1,ich1);
		if(ich1==curchar1)
			{
			//printf("\n\t\t\t %c = %c ",ich1,curchar1);
			socarecursive(blockadd,index+1);
			return;
			}
		fseek(socafp,ab1,SEEK_SET);	
                }
	if(curchar1!=10)      //if(S[index]!='\0')
                {
		//printf("\n\t\t\t %c != %c ",ich1,curchar1);
		//printf("\n\t\t\t create node here \n\n\n");
		tempfp=hddfp;
		rewind(tempfp);
		FILE *vfp=tempfp;
		fseek(tempfp,nodeoffset,SEEK_SET);
		fprintf(tempfp,"%8d %8d %8d ",index,index,0);
		fseek(tempfp,currentpointer,SEEK_SET); // current pointer is the father node
		fscanf(tempfp,"%d%d%d",&dummy,&dummy,&numberofsons1);
		for(k=0;k<numberofsons1;k++)
			fscanf(tempfp,"%d",&nos[k]);
		nos[k]=nodeoffset;
		numberofsons1+=1;
		rewind(tempfp);
		fseek(tempfp,currentpointer,SEEK_SET);
		fprintf(tempfp,"%8d %8d %8d ",dummy,dummy,numberofsons1);
		for(k=0;k<numberofsons1;k++)
			fprintf(tempfp,"%8d ",nos[k]);
		//printf("\n\t\t\te s rcharacter = %c, input offset is = %d",curchar1,index);
		nodeoffset+=NODEOFFSET;
                formChainTillEndOfString(index);
                }
        }

void formChainTillEndOfString(int index) // nodeoffset gives current off set position
        {
	FILE *father=hddfp;
	rewind(father);
        FILE *tfp=fopen("input","r");
	fseek(tfp,index+1,SEEK_SET);
	char h;
	int i=0,numberofsons=0;
	while((h = getc(tfp))!=EOF)
		{
		if(h==10) break;
		tempfp=hddfp;
		rewind(tempfp);
		fseek(tempfp,nodeoffset,SEEK_SET);
		fprintf(tempfp,"%8d %8d %8d ",ftell(tfp)-1,ftell(tfp)-1,0);

		fseek(father,nodeoffset-NODEOFFSET,SEEK_SET);
                fscanf(father,"%d%d%d",&dummy,&dummy,&numberofsons);     // first two are for dummy purpose tttttttt
                for(i=0;i<numberofsons;i++)
                        fscanf(father,"%d",&nos[i]);
                nos[i]=nodeoffset;
                numberofsons+=1;
                rewind(father);
                fseek(father,nodeoffset-NODEOFFSET,SEEK_SET);  // cleaning for 100 bytes should be followed to clear garbage
                fprintf(father,"%8d %8d %8d ",ftell(tfp)-2,ftell(tfp)-2,numberofsons);
                for(i=0;i<numberofsons;i++)
                        fprintf(father,"%8d ",nos[i]);
		//printf("\n\t\t\tcharacter = %c, input offset is = %d",h,ftell(tfp)-1);
		nodeoffset+=NODEOFFSET;
                }
        }

void compactandoptimize()
        {
        int numberofsons,block,dum1,dum2;
        rewind(hddfp);
        fseek(hddfp,headoffset,SEEK_SET);
        fscanf(hddfp,"%d%d%d",&dum1,&dum2,&numberofsons);
        int k,m=0;
        for(k=0;k<numberofsons;k++)
                {
                fscanf(hddfp,"%d",&block);
                m=ftell(hddfp);
                //printf("\n-----------------------%d\n",block);
                recursivilycompactsubtree(block);
                fseek(hddfp,m,SEEK_SET);
                }
        }

int a[20],retainstartindex=0,startindexr=0;
void recursivilycompactsubtree(int blockto)
        {
        FILE *rfp=tempfp;
	FILE *rcp=fpx;
        int sindex,eindex,numberofsons2;
        char tch;
        fseek(rfp,blockto,SEEK_SET); // rfp to point to father block 
        fscanf(rfp,"%d%d%d",&sindex,&eindex,&numberofsons2);
        //printf("\n\tsindex=%d,eindex=%d,numberofsons=%d",sindex,eindex,numberofsons2);
        fseek(ifp,sindex,SEEK_SET);
        fscanf(ifp,"%c",&tch);
        //printf("\t\n %c",tch);
        if(numberofsons2==1)
                {
		if(retainstartindex==0)
			{
			//printf("\n\t %c setted %d",tch,sindex);
			startindexr=sindex;
			retainstartindex=1;
			}	
		//printf("\n\t\t\tcompacted %c with ",tch);
                int block1,stindexx,edindexx,nosx,p,block2,po1,po2;
                fscanf(rfp,"%d",&block1);
		fclose(rfp);
		rfp=fopen("vst.stod","r+w");
		rcp=rfp;
		rewind(rcp);
		fseek(rcp,block1,SEEK_SET);
		fscanf(rcp,"%d%d%d",&stindexx,&edindexx,&nosx);
		//printf(" %d %d %d ",stindexx,edindexx,nosx);
		for(p=0;p<nosx;p++)
			{
			fscanf(rcp,"%d",&block2); //printf("\n venki %d",block2);
			a[p]=block2; //printf("\n venki %d",a[p]);
			}
		fseek(rcp,blockto,SEEK_SET);
		if(nosx>=0)
			{
			//printf("\n %c resetted %d",tch,nosx);
			fprintf(rcp,"%8d %8d %8d ",startindexr,edindexx,nosx);
			retainstartindex=0;
			}
		else
			fprintf(rcp,"%8d %8d %8d ",stindexx,edindexx,nosx);
		for(p=0;p<nosx;p++)
			{
			//printf("\n 1:  %d",a[p]);
			fprintf(rcp,"%8d ",a[p]); //printf("\n%d here %d ",o,block2);
			}
                recursivilycompactsubtree(blockto);
                }
        else if(numberofsons2==0)
                {
                }
        else
                {
                int m,block3,lp;
                for(m=0;m<numberofsons2;m++)
                        {
                        fscanf(rfp,"%d",&block3);
                        lp=ftell(rfp);
                        recursivilycompactsubtree(block3);
                        fseek(rfp,lp,SEEK_SET);
                        }
                }
        //printf("\n-------------Venki");
        }
