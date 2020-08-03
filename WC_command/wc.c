#include<stdio.h>
#include<string.h>

int charcount(FILE *);
int line(FILE *);
int maxline(FILE *);
int wrd(FILE *);
int disp(char[]);
static int siz=1000;

int main(int argc,char *argv[])
{
	FILE *fp,*fp2;
	int character=0,byte=0,newline=0,max=0,word=0,flag=0,i=0,j=0,ind=0,c;
	char half[siz],full[siz];
	if(argv[1]==NULL)			/* When there is no argument in command line after executable filename */
	{
		while((c=getchar())!=EOF)
		{
			character++;
			if(c=='\n')
				newline++;
			else
			{
				if(c==' ' ||c=='\n'|| c=='\t')
					flag=0;
				else if(flag==0)
				{
					word++;
					flag=1;

				}

			}
		}
		printf("\tlines: %d word: %d character: %d\n",newline,word,character);
	}
	else if(argv[1]!=NULL && argv[2]!=NULL) /* If there are two arguments after executable filename */
	{

		fp=fopen(argv[2],"r");
		if(fp==NULL)
		{
			ind=0;
			fp=fopen(argv[1],"r");
			if(fp!=NULL)
			{
				ind=1;
			}
			else if(fp==NULL)
			{
				printf(" No such file or directory\n");
				return -1;
			}
		}
		if(fp!=NULL)
		{


			if(!strcmp(argv[1],"-m")||!strcmp(argv[1],"--chars")||!strcmp(argv[2],"-m")||!strcmp(argv[2],"--chars"))
			{
				character=charcount(fp);
				printf("Character: %d ",character);
				if(ind==3)
					printf("\tFile: %s\n",argv[2]);
				else if(ind==1)
					printf("\tFile: %s\n",argv[1]);
			}
			else if(!strcmp(argv[1],"-c")||!strcmp(argv[1],"--bytes")||!strcmp(argv[2],"c")||!strcmp(argv[2],"--bytes"))
			{
				byte=charcount(fp);
				printf("Bytes: %d ",byte);
				if(ind==3)
					printf("\tFile: %s\n",argv[2]);
				else if(ind==1)
					printf("\tFile: %s\n",argv[1]);
			}
			else if(!strcmp(argv[1],"-L")||!strcmp(argv[1],"--max-line-length")||!strcmp(argv[2],"-L")||!strcmp(argv[2],"--max-line-length"))
			{
				max=maxline(fp);
				printf("Longest line length: %d ",max);
				if(ind==3)
					printf("\tFile: %s\n",argv[2]);
				else if(ind==1)
					printf("\tFile: %s\n",argv[1]);
			}
			else if(!strcmp(argv[1],"-l")||!strcmp(argv[1],"--lines")||!strcmp(argv[2],"-l")||!strcmp(argv[2],"--lines"))
			{
				newline=line(fp);
				printf("Newline: %d",newline);
				if(ind==3)
					printf("\tFile: %s\n",argv[2]);
				else if(ind==1)
					printf("\tFile: %s\n",argv[1]);
			}
			else if((!strcmp(argv[1],"-w"))||!strcmp(argv[1],"--words")||!strcmp(argv[2],"-w")||!strcmp(argv[2],"--words"))
			{
				word=wrd(fp);
				printf("Word: %d ",word);
				if(ind==3)
					printf("\tFile: %s\n",argv[2]);
				else if(ind==1)
					printf("\tFile: %s\n",argv[1]);
			}
			else
			{
				printf("wc: unrecognized option \nTry 'wc --help' for more information.\n");
			}

		}


		fclose(fp);
	}
	else if(argv[1]!=NULL && argv[2]==NULL) /* If there  is only one argument in commandline after executable filename(File to be read) */
	{
		
		while(argv[1][i]!='=') // copying the content of argv[] 
		{
			half[i]=argv[1][i];
			i++;
		}
		half[i++]='=';
		half[i]='\0';
		if(!strcmp(argv[1],"--version"))
		{
			fp2=fopen("version.txt","r");
			while((c=fgetc(fp2))!=EOF)
				putchar(c);
			fclose(fp2);			
		}
		else if(!strcmp(argv[1],"--help"))
		{
			fp2=fopen("help.txt","r");
			while((c=fgetc(fp2))!=EOF)
				putchar(c);
			fclose(fp2);			
		}
		else if(!strcmp(half,"--files0-from="))
		{
			while(argv[1][j]!='\0') // copying the content of argv[1] to a character array
			{
				full[j]=argv[1][j];
				j++;
			}
			disp(full);
		}
		else 
		{

			fp=fopen(argv[1],"r");
			if(fp==NULL)
			{
				printf("No such file or directory\n");
				return -1;
			}

			else
			{

				character=charcount(fp);
				rewind(fp);
				//	printf("%d",character);
				newline=line(fp);
				rewind(fp);
				//	printf("%d",newline);
				word=wrd(fp);
				rewind(fp);
				printf("\n\t%d\t %d \t%d\t %s\n\n",newline,word,character,argv[1]);

			}
			fclose(fp);
		}
	}
	//	printf("\n Newline: %d Word:%d character: %d byte: %d\n",newline,word,character,byte);

	return 0;
}

int charcount(FILE *f1) // Function to count the number of characters in the file
{
	int c,ch=0;
	while((c=fgetc(f1))!=EOF)
	{
		ch++;
	}

	return ch;		
}

int line(FILE *f3) //Function to count the number of lines in the file
{
	int c,nl=0;
	while((c=fgetc(f3))!=EOF)
	{
		if(c=='\n')
			nl++;
	}
	return nl;
}

int wrd(FILE *f4) //Function to count the number of lines in the file
{
	int w=0,c,flag=0;
	while((c=fgetc(f4))!=EOF)
	{
		if(c!=' ' && c!='\n' && c!='\t')
			flag=0;
		else if(flag==0)
		{
			flag=1;
			w++;
		}
	}
	return w;
}

int maxline(FILE *f5) //Function to calculate the length of the longest line in the file
{
	int maxlen=0,len=0,c;
	while((c=fgetc(f5))!=EOF)
	{
		if(c!='\n')
			len++;
		else
		{
			if(len>maxlen)
			{
				maxlen=len;
				len=0;
			}
		}


	}
	return maxlen;

}

int disp(char s1[siz]) //Funcion for option "--files0-from=F"
{
	FILE *fp;
	int i=14,j=0,c;
	char s2[siz];
	while(s1[i]!='\0')
	{
		s2[j++]=s1[i++];
	}
	s2[j]='\0';
	fp=fopen(s2,"r");
	if(fp==NULL)
	{
		printf("No such file or directory\n");
		return -1;
	}
	else
	{
		while((c=fgetc(fp))!=EOF)
			putchar(c);
	}
	return 0;

}
