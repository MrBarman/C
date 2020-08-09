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
void object_code(FILE *,char [],char [],symtab [],opcd [],int *);
void object_program(symtab [],int ,int );
void zero_adder(char [],int );
void write_text(FILE *,char [],int *,int *,int *);
void pass2(symtab s[],opcd op[],int start)
{
		FILE *fp,*objfp;
		char opc[10], instruction[50],operand[10];
		int i,j,k,f,space,location=start;
		printf("\n-----------------------\n\tPass_2\n-----------------------\n");
		fp=fopen("tokenized.txt","r");
		objfp = fopen("ObjectCode.txt","w");
		printf("\t");
    while(!feof(fp))
    {
		fgets(instruction,50,fp);//taking one line at a time to the string "op"
		i=0;
        //space=0;
        while(instruction[i]!=' ')
        {
        	i++;
        }
        k=0;
        i++;
        opc[k]='\0';
        while(instruction[i]!='\0'&&instruction[i]!='\n'&&instruction[i]!=' ')
        {
        	opc[k++] = instruction[i++];
        }
        opc[k]='\0';
        if(strcmp(opc,"END")==0)
        {
            printf("%s",instruction);
            fprintf(objfp,"%c",'#');
            break;
        }
        i++;
        k=0;
        while(instruction[i]!='\0'&&instruction[i]!='\n'&&instruction[i]!=' ')
        {
        	operand[k++] = instruction[i++];
        }
        operand[k]='\0';
        for(k=0;k<strlen(instruction)-1;k++)
        {
            printf("%c",instruction[k]);
        }
        printf("\t\t");
        object_code(objfp,opc,operand,s,op,&location);
        printf("%X\t",location);
    }
		fclose(objfp);
		object_program(s,start,location-start);
		return;
}
void object_code(FILE *objfp,char opc[],char operand[],symtab st[],opcd op[],int *location)
{
    int i,j,addr,k,flag=0;
    char hex[10],ch[5],adr[8];
    if(objfp==NULL)
    {
        printf("Error opening file..");
        return;
    }
    for(i=0;i<60;i++)
    {
        if(strcmp(opc,op[i].opcode)==0)
        {
            strcpy(hex,op[i].hexcode);
            break;
        }
    }
    k=strlen(operand);
    for(i=0;i<60;i++)
    {
        //k=strlen(operand);
        if(operand[k-2]==',')
        {
            //k=strlen(operand);
            operand[k-2]='\0';
            flag=1;
        }
        if(strcmp(operand,st[i].label)==0)
        {
            addr = st[i].loc;
            if(flag==1)
            {
                addr+=32768;
                //flag=0;
            }
            break;
        }
    }
    if(strcmp(opc,"RSUB")==0)
    {
        addr = 0;
        printf("%s0000\n",hex);
        fprintf(objfp,"%s0000\n",hex);
        *location += 3;
        return;
    }
    if(strcmp(opc,"START")==0||strcmp(opc,"END")==0)
    {
        printf("\n");
        if(strcmp(opc,"END")==0)
        {
            fprintf(objfp,"%c",'#');
        }
        return;
    }
    if(strcmp(opc,"WORD")==0)
    {
        //sprintf(adr,"%X",atoi(operand));
        zero_adder(operand,6);
        printf("%s\n",operand);
        fprintf(objfp,"%s\n",operand);
        *location += 3;
        return ;
    }
    if(strcmp(opc,"RESW")==0||strcmp(opc,"RESB")==0)
    {
            printf("\n");
            fprintf(objfp,"%s","$ ");
            if(strcmp(opc,"RESW")==0)
            {
                *location += atoi(operand)*3;
                fprintf(objfp,"%X\n",atoi(operand)*3);
            }
            else if(strcmp(opc,"RESB")==0)
            {
                *location += atoi(operand);
                fprintf(objfp,"%X\n",atoi(operand));
            }
            return;
    }
    if(strcmp(opc,"BYTE")==0)
    {
        j=2;
        if(operand[0]=='C')
        {
            k=0;
            ch[k]='\0';
            while(operand[j]!=39)
            {
                ch[k++] = operand[j++];
            }
            ch[k]='\0';
            for(i=0;i<k;i++)
            {
                printf("%X",ch[i]);
                fprintf(objfp,"%X",ch[i]);
            }
            fprintf(objfp,"%c",'\n');
            *location += i;
            printf("\n");
            return ;
        }
        if(operand[0]=='X')
        {
            k=0;
            ch[k]='\0';
            while(operand[j]!=39)
            {
                ch[k++] = operand[j++];
            }
            ch[k]='\0';
            printf("%s\n",ch);
            fprintf(objfp,"%s\n",ch);
            *location += k/2;
            return ;
        }
    }
    itoa(addr,adr,16);
    zero_adder(adr,4);
    *location += 3;
    printf("%s\n",strcat(hex,adr));
    fprintf(objfp,"%s\n",hex);
    return ;
}
void object_program(symtab s[],int start,int p_size)
{
    int i=0,length=0,j=0,position=start,num=0,k=0,location=start,flag=0;
    FILE *fp,*fp1;
    char temp[8],loc[8],mix[20],temp2[10],text[200];
    fp = fopen("ObjectCode.txt","r");
    fp1 = fopen("ObjectProgram.txt","w");
    zero_adder((itoa(start,loc,16)),6);
    zero_adder((itoa(p_size,temp,16)),6);
    printf("\n---------------------------------\n\tObject_Program\n---------------------------------\n");
    printf("\nH^%6s^%s^%s\n\n",s[0].label,loc,temp);
    fprintf(fp1,"H^%6s^%s^%s\n",s[0].label,loc,temp);
    while(!feof(fp))
    {
        fgets(temp,15,fp);
        i=0;
        j=0;
        while(temp[j]!='\n'&&temp[j]!='\0')
        {
            //temp[j++]=mix[i++];
            j++;
        }
        temp[j]='\0';
        if(temp[0]!='$')
        {
            i=0;
            if(length>=28||flag==1||temp[0]=='#')
            {
                flag=0;
                text[k-1]='\0';
                write_text(fp1,text,&length,&location,&num);
                k=0;
                text[k]='\0';
            }
            if(length<=30)
            {
                length+=(strlen(temp)+1)/2;
                while(temp[i]!='\0')
                {
                    text[k++]=temp[i++];
                }
                text[k++]='^';
            }
        }
        if(temp[0]=='#'||temp[0]=='$')
        {
            if(temp[0]=='$')
            {
                i=0;
                j=2;
                while(temp[j]!='\0')
                {
                    temp2[i++] = temp[j++];
                }
                temp2[i] = '\0';
                num += strtol(temp2,NULL,16);
                flag=1;
            }
        }
        if(temp[0]=='#')
            break;
    }
    zero_adder((itoa(start,loc,16)),6);
    printf("\nE^%s",loc);
    fprintf(fp1,"E^%s",loc);
    fclose(fp1);
    return;
}
void zero_adder(char operand[],int no)
{
    int i,j;
    char tmp_operand[20];
    strcpy(tmp_operand,operand);
    for(i=0;tmp_operand[i]!='\0';i++);
    for(j=0;j<no-i;j++)
        operand[j]='0';
    operand[j]='\0';
    strcat(operand,tmp_operand);
    return ;
}
void write_text(FILE *fp1,char text[],int *length,int *location,int *num)
{
    char loc[8];
    int t_loc=*location;
    zero_adder((itoa(t_loc,loc,16)),6);
    printf("T^%s^%2X^%s\n",loc,*length,text);
    fprintf(fp1,"T^%s^%2X^%s\n",loc,*length,text);
    *location+=*length+*num;
    *length=0;
    *num=0;
    return;
}
