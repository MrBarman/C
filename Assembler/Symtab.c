#include<stdio.h>
#include<string.h>
#include<math.h>
#include "tokenizer.h"
//#include "pass2.h"
struct ss{
	char label[10];
	int loc;
};
typedef struct ss symtab;
struct op{
        char opcode[10];
        char hexcode[5];
};

typedef struct op opcd;
int update_loc(char [],char [],int);
int pass1(FILE *,symtab [],int *,opcd []);
void print(symtab [],int);
//void check_opcode(char [],opcd []);
//void pass2(symtab [],opcd [],int);
//void read_opcode(FILE *,opcd []);
int hex_to_dec(int);
int main()
{
	FILE *fp,*fp2;
	int locctr=0,start;
	char ch,opcode[10];
	symtab st[60];
	opcd op[60];
	tokenize();//calling tokenize function from tokenizer.c
	fp = fopen("tokenized.txt","r");
	if(fp==NULL)
		return 0;
	fp2 = fopen("opcode.txt","r");
	if(fp2==NULL)
		return 0;
	//read_opcode(fp2,op);
	start = pass1(fp,st,&locctr,op);
	//pass2(st,op,start);
	fclose(fp);
	return(0);
}
int pass1(FILE *fp, symtab s[],int *locctr,opcd opc[])
{
	int i=0,j=0,k=0,l=0,f,temp=0;
	char op[100],opcode[20];
	while(feof(fp)==0)
	{
		f=0; //reset the flag to zero at start of every line
		fgets(op,50,fp);//taking one line at a time to the string "op"
		i=0;
		//if(op[0]!='!')//if it is not a comment line
		//{
        while(op[i]!=' ')//label found, loop until next space not found,
        {                   //means label of the structure will contain the label now
            s[j].label[i]=op[i];
            i++;
        }
        if(i>0)
        {
            s[j].label[i]='\0';//end the label with null character
            f=1;//location counter need to be inserted if flag is set
        }
        i++;
        k=0;
        while(op[i] != ' ' && op[i] != '\0' && op[i] != '\n')//checking for opcode part of the instruction
        {
            opcode[k++]=op[i++];//Putting the opcode to a string
        }
        opcode[k]='\0';//ending the opcode with a null character
        if(!((strcmp(opcode,"START"))!=0||(strcmp(opcode,"RESB")!=0)||(strcmp(opcode,"RESW")!=0)||(strcmp(opcode,"END")!=0)||(strcmp(opcode,"BYTE")!=0)||(strcmp(opcode,"WORD")!=0)))
        	check_opcode(opcode,opc);
        if(f==1)
        {
            for(l=0;l<j;l++)
            {
                if(strcmp(s[l].label,s[j].label) == 0)//checking if the label already appeared previously or not
                {
                    printf("\n\nLabel \"%s\" has already appeared.\nCheck the Source code\n",s[j].label);
                    return;
                }
            }
            s[j].loc = *locctr;//Because flag is set so we need to update the Structures location part
            j++;//incrementing the structure array
        }
        *locctr = *locctr + update_loc(opcode,op,i);//updating the location counter
    }
	//}
	printf("\n-----------------------\n\tSYMTAB\n-----------------------\n");
	print(s,j);
	//printf("hgh %d",s[1].loc);
	return s[1].loc;
}
int update_loc(char temp[],char op[],int pos)
{
	char addr[10];
	int i=0,blngth=0;
    if(strcmp(temp,"START")==0)
	{
		pos++;//checking the next position after space
		while(op[pos] !=' ' && op[pos] !='\0')
		{
			addr[i++] = op[pos++];
		}

		addr[i] = '\0';
		//hex = atoi(addr);//atoi converts a string to integer
		//dec = hex_to_dec(hex);
		//printf("%d,%d\n",hex,dec);
		return hex_to_dec(atoi(addr));
	}
	else if(strcmp(temp,"RESW")==0)
	{
		pos++;
		while(op[pos] !=' ' && op[pos] !='\0')
		{
			addr[i++] = op[pos++];
		}
		return 3*atoi(addr);

	}
	else if(strcmp(temp,"RESB")==0)
	{

		pos++;
		while(op[pos] !=' ' && op[pos] !='\0')
		{
			addr[i++] = op[pos++];
		}
		return atoi(addr);
	}
	else if(strcmp(temp,"BYTE")==0)
	{
		if(op[pos+1]=='C')
		{
            pos= pos+3;//skipping first three character eg. space C'
            while(op[pos]!=' ' && op[pos]!='\0')
            {
                blngth++;
                pos++;
            }
            return blngth-2;
		}
		if(op[pos+1]=='X')
		{
            pos=pos+3;////skipping first three character eg. space X'
            while(op[pos]!=' ' && op[pos]!='\0')
            {
                blngth++;
                pos++;
            }
            blngth--;
            blngth = blngth/2;
			return blngth;
		}
	}
	else
		return 3;
}
void print(symtab s[],int j)
{
    FILE *symtabfp;
	int i=0;
	symtabfp = fopen("SYMTAB.txt","w");
	for(;i<j;i++)
	{
		printf("%s\t%X\n",s[i].label,s[i].loc);
		fprintf(symtabfp,"%s\t%X\n",s[i].label,s[i].loc);//writing the Symtab into a file
	}
	fclose(symtabfp);
	return;
}
int hex_to_dec(int hex)
{
    int dec=0,i=0,rem;
    while(hex!=0)
    {
        rem = hex%10;
        dec = dec + rem*pow(16,i);
        hex = hex/10;
        i++;
    }
    return dec;
}
void check_opcode(char temp[],opcd op[])
{
	int i;
    for(i=0;i<60;i++)
    {
    	if(strcmp(temp,op[i].opcode)==0)
    		return;
    }
    printf("%s is not an opcode.\n",temp);
    return;
}
void read_opcode(FILE *fp,opcd op[])
{
	//opcd op[60];
	int i,j,k;
	char temp[20];
	j=0;
	while(!feof(fp))
	{
		fgets(temp,20,fp);
		i=0;
		while(temp[i]!=' ')
		{
			op[j].opcode[i]=temp[i];
			i++;
		}
		op[j].opcode[i]='\0';
		k=0;
		i++;
		while(temp[i]!='\n'&&temp[i]!=' '&&temp[i]!='\0')
		{
			op[j].hexcode[k++]=temp[i++];
		}
		op[j].hexcode[k]='\0';
		j++;
	}
	return;
}
