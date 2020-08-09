/** 
*  @file    parser.c
*  @author  manash( barmanmanash005@gmail.com)
*  @date    28/05/2018
*  @version 1.0 
*  
*  @brief Parser-utils main function
*
*  @section DESCRIPTION
*  
*  This is the parser utility program that
*  contains the implementations of the parsing
*  features (accessed by main)
*
*/


/* declaring the required header files */
#include<stdio.h>
#include<ctype.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"myheader.h"

/** 
 * Below function opens the file(ini) for parsing
 * return value: pointer of type structure "CFILE" (success)
 *   			 NULL(failure)
 * argument: a charcter pointer, contains the path of the 
 *       	 file to be opened.
 * 
 **/
CFILE* ini_read(char *file)
{
		FILE *file_ptr;
		int file_desc,ret_val;
		struct flock lock;
		section *head_sect=NULL;
		name_value *head_key_val=NULL;

		/* Initializing the flock structure with zero */
		memset(&lock,0,sizeof(lock));
		lock.l_type=F_RDLCK;

		/* Below function opens a file for reading and also checks for error  */
		if((file_desc=open(file,O_RDWR))<0)
		{
				return NULL;
		}
		/* Below function obtains the file pointer from file descriptor  */
		if((file_ptr=fdopen(file_desc,"r+"))==NULL)
		{
				printf("\n Unable to open file\n Error code: %s\n\n",strerror(errno));
				return (NULL);
		}

		/* Obtaining Readlock for file */
		if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
		{
				printf("\n File LOCKING Failed\n Error code: %s\n\n",strerror(errno));
				return	(NULL);
		}

		/* assigning important info to structure members */
		CFILE *cfile=(CFILE *)malloc(sizeof(CFILE));
		if(cfile==NULL)
		{
			printf("\nMemory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		cfile->path=file;
		cfile->cfile_desc=file_desc;
		cfile->cfp=file_ptr;

		/* calling function for lexical analysis */
		ini_lexical_analysis(file_ptr,&head_sect,file_desc,&head_key_val);
		cfile->is_section=is_section;
		
		/* if SECTION present in the file  */
		if(is_section==TRUE) 
		{
				cfile->sect_ptr=head_sect;
				cfile->name_value=head_key_val;
		}

		/* if no SECTION present in the file */
		else if(is_section==FALSE)
		{
				section *sec =(section *)malloc(sizeof(section));
				if(sec==NULL)
				{
					printf("\nMemory allocation failed\n");
					exit(EXIT_FAILURE);
				}
				sec=NULL;
				cfile->sect_ptr=sec;
				cfile->name_value=head_key_val;
		}

		/* Releasing the obtained lock */
		lock.l_type=F_UNLCK;
		if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
		{
				printf("\n File UNLOCKING Failed\n Error code: %s\n\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		/* closing the file pointers and descriptors */
		fclose(file_ptr);
		close(file_desc);
		return cfile;
}

/** 
 * Below function does the lexical anlysis of the file(ini)
 * opened by the calling function.
 *
 * In case of failure,it terminates the prgram with an error message
 * 
 * arguments-   			 
 * argument-1: a file pointer of the opened file. 
 * argument-2: a pointer of type structure "section"
 * argument-3: an int contatining the file descriptor of the opened file
 * argument-4: a pointer of type structure "name_value"
 *
 **/
void ini_lexical_analysis(FILE *file_ptr,section **head_sect,int file_desc,name_value **head_key_val)
{
		int c,str_index=0,str_len=0,line_no=1;
		char str[SIZE],option;
		section *sect;
		name_value *no_sect;

		/* calling function for replacing conflicting delimiters */	
		replace_delimiter(&file_ptr,file_desc);
		rewind(file_ptr);
		/* Below piece of code checks for comments */
		c=fgetc(file_ptr);
		while(c!=EOF)
		{

				/* Ignoring the comments */
				if((c=='#' || c==';')&& c!=EOF)
				{
						while((c=fgetc(file_ptr))!='\n')
								;
				}

				/* Below piece of code checks for space characters */
				if(isspace(c))
				{
						if(c=='\n')
						{
								line_no++;
						}
						while(isspace((c=fgetc(file_ptr))))
						{
								if(c=='\n')
								{
										line_no++;
								}
						}
				}

				/* Below piece of code tokenizes a string and sends it to the 
				   ini_parse() function */
				if(c!='#' && c!=';'&& !isspace(c) &&  c!=EOF)
				{
						str[str_index++]=c;
						while((c=fgetc(file_ptr))!='\n')
						{
								str[str_index++]=c;
						}
						str[str_index]='\0';
						str_len=(strlen(str)-1);//storing the length of the string

						/* Below piece of code removes the trailing whitesspaces */
						while(isspace(str[str_len]))
						{
								str_len--;
						}
						str[str_len+1]='\0';

						/* reinitialising the variables for reuse */
						str_index=0;
						str_len=0;

						/* calling function for parsing */
						ini_parse(str,&head_sect,&sect,line_no,&head_key_val,&no_sect);
				}
		}
}


/** 
 * Below function does the parsing of the tokenized strings
 * sent to it by lexical analysis function.
 * If parsing is succcessfull SECTION,KEY and VALUE extracted 
 * from the tokens and stored as a linked list.
 *
 * In case of failure,it terminates the prgram with an error message
 * 
 * arguments-   			 
 * argument-1: a character array the will contain the tokens sent by the 
 * 			   calling function. 
 * argument-2: a pointer of type structure "section"
 * argument-3: a pointer of type structure "section"
 * argument-4: an int contatining the line number of the opened file from which 
 *			   the string is tokenized
 * argument-5: a pointer of type structure "name_value"
 * argument-6: a pointer of type structure "name_value"
 *
 **/
void ini_parse(char string[SIZE],section ***head_sect,section **sect,int line_no,name_value ***head_key_val,name_value **no_sect)
{
		int name_index=0,val_index=0,str_index=0,sec_index=0;
		int section_flag=FALSE,str_len;
		char *token,*last;

		str_len=(strlen(string)-1);

		/* Below piece of code does the syntax analysis if section is found */
		if(string[str_index]=='[')
		{
				is_section=TRUE;
				if(is_file_without_section==TRUE)
				{
						printf("\nERROR at line: %d\n",line_no);
						printf("\nERROR: Section not supposed to be in this file\n");
						printf("\nString with section: %s\n\n",string);
						exit(EXIT_FAILURE);
				}
				while(string[str_index]!='\0')
				{
						if((string[str_index])==']')// end of a section
						{
								section_flag=TRUE;
						}

						/* Below code checks for syntax error in a section */
						if((section_flag==TRUE) && (str_index!=str_len))
						{
								printf("\nSyntax error on section at line no: %d\n",line_no);
								printf("section: \"%s\"\n\n",string);
								exit(EXIT_FAILURE);
						}
						str_index++;
				}
				str_index=0;

				/* Below code checks for empty SECTION */
				if(string[str_index]=='[' && string[str_index+1]==']')
				{
						printf("\nEmpty SECTION found at line no: %d\n\n",line_no);
						printf("section: \"%s\"\n",string);
						exit(EXIT_FAILURE);
				}
				str_index=1;

				/* Below code stores the section name into section
				   if syntax analysis is successful */
				(*sect)=(section *)malloc(sizeof(section));
				if((*sect)==NULL)
				{
					printf("\nMemory allocation failed\n");
					printf("\nError occured in line: %d\n",line_no);
					exit(EXIT_FAILURE);
				}
				while(string[str_index]!=']')
				{
						(*sect)->section_name[sec_index]=string[str_index];
						str_index++;
						sec_index++;
				}
				(*sect)->section_name[sec_index]='\0';
				(*sect)->next_section=(**head_sect);
				(**head_sect)=(*sect);
				(*sect)->head_name_val=NULL;
				str_index=0;
		}

		/* Below piece of code does the syntax analysis 
		   if the sent token is not section */
		   else
		   {

				   /* below code executes if file format does not contain section  */
				   if(is_section==FALSE)
				   {
						   is_file_without_section=TRUE;

						   /* calling function for syntax analysis of the string  */
						   key_val_check(string,line_no);

						   /* below code tokenizes the string */
						   token=strtok(string,delim);
						   last=(token+strlen(token)-1);

						   /* Below code removes the trailing spaces from the token obtained 
							  separated by delimeter '='  */
						   while((last>token) && isspace(*last))
						   {
								   last--;
						   }
						   *(last+1)='\0';

						   /* Below code stores the syntactically analysed token into respective KEY(name) */
						   (*no_sect)=(name_value *)malloc(sizeof(name_value));
							if((*no_sect)==NULL)
							{
								printf("\nMemory allocation failed\n");
								printf("\nError occured in line: %d\n",line_no);
								exit(EXIT_FAILURE);
							}
						   while(*token!='\0')
						   {
								   (*no_sect)->name[name_index++]=(*token);
								   *(token++);
						   }
						   (*no_sect)->name[name_index]='\0';
						   token=strtok(NULL,delim);

						   /* Below code removes the preceding spaces from the token obtained 
							  separated by delimeter '='  */
						   while(isspace(*token))
						   {
								   token++;
						   }

						   /* Below code stores the analysed token into respective VALUE */
						   while(*token!='\0')
						   {
								   (*no_sect)->value[val_index++]=(*token);
								   *(token++);
						   }
						   (*no_sect)->value[val_index]='\0';
						   (*no_sect)->next_name_val=(**head_key_val);
						   (**head_key_val)=(*no_sect);
				   }

				   /* Below code executes if file format contains section */
				   if(is_file_without_section==FALSE)
				   {
						   /* calling function to check if delimiter is present in the string */
						   key_val_check(string,line_no);

						   /* below code tokenizes the string */
						   token=strtok(string,delim);
						   if(token==NULL)
						   {
								   printf("\nERROR while tokenizing the string at line: %d\n\n",line_no);
								   exit(EXIT_FAILURE);
						   }
						   last=(token+strlen(token)-1);

						   /* Below code removes the trailing spaces from the token obtained 
							  separated by delimeter '='  */
						   while((last>token) && isspace(*last))
						   {
								   last--;
						   }
						   *(last+1)='\0';

						   /* Below code stores the analysed token into respective KEY(name) */
						   (*sect)->name_val=(name_value *)malloc(sizeof(name_value));
						   while(*token!='\0')
						   {
								   (*sect)->name_val->name[name_index++]=(*token);
								   *(token++);
						   }
						   (*sect)->name_val->name[name_index]='\0';
						   token=strtok(NULL,delim);

						   /* Below code removes the preceding spaces from the token obtained 
							  separated by delimeter '='  */
						   while(isspace(*token))
						   {
								   token++;
						   }

						   /* Below code stores the analysed token into respective VALUE */
						   while(*token!='\0')
						   {
								   (*sect)->name_val->value[val_index++]=(*token);
								   *(token++);
						   }
						   (*sect)->name_val->value[val_index]='\0';
						   (*sect)->name_val->next_name_val=(*sect)->head_name_val;
						   (*sect)->head_name_val=(*sect)->name_val;
				   }
		   }
}

/**
 * Below function checks the frequency of occurances of delimiter
 *  in a string.
 * argument- A character array that will contain the string sent by 
 * 			 the calling function.
 *
 * return value: count of the number of occurances of the delimiter
 *				 in the tokenized string.
 */
int frequency_check(char string[SIZE])
{
		int count=0,index=0;
		while(string[index]!='\0')
		{
				/* counting the occurance of delimiters  */
				if(string[index]==(*delim))
				{
						count++;
				}
				index++;
		}
		return count;
}

/**
 * Below function checks if one of KEY or VALUE is missing 
 * in the string.
 *
 * argument- A character array that will contain the string sent by 
 * 			 the calling function.
 * 
 * return_value: FALSE(if KEY or VALUE is missing) 
 * 				 TRUE (if no KEY or VALUE is missing)
 *
 **/
int is_key_val_pair(char string[SIZE])
{
		int  index=0,str_len;
		str_len=(strlen(string)-1);

		/* checking the position of delimiter. If delimiter present in
		   the starting or end of the string, then either KEY or VALUE
		   is missing*/
		if((string[index]==(*delim))||(string[str_len]==(*delim)))
		{
				return FALSE;
		}
		else
		{
				return TRUE;
		}
}

/**
 * Below function checks for syntax of KEY/VALUE 
 *
 * arguments-
 * argument-1: A character array containing the string sent 
 *			   by calling function.
 * argument-2: The line no in the file from which the string
 *			   is extracted.
 *
 * In case if wrong syntax,program is terminated
 * with an error message
 *
 * */
void key_val_check(char string[SIZE],int line_no)
{
		char *is_delim;
		int delim_frequency,check_res;

		/* below code checks if delimiter is present in the string
		   if not then generate error  */
		is_delim=strchr(string,*delim);
		if(is_delim==NULL)
		{
				printf("\nError: delimiter not found on string at line:  %d\n",line_no);
				printf("String: \"%s\"\n\n",string);
				exit(EXIT_FAILURE);
		}

		/* below code checks if more than one delimiter is present in the file
		   if present then generate error  */
		if((check_res=is_key_val_pair(string))==FALSE)
		{
				printf("\nError: Key or Value is missing in the string at line: %d\n",line_no);
				printf("String: \"%s\"\n\n",string);
				exit(EXIT_FAILURE);
		}

		/* calling function to  checks if more than one delimiter 
		   is present in the file, if present then generate error  */
		if((delim_frequency=frequency_check(string))>1)
		{
				printf("\nError: More than one delimiter present in the string at line: %d\n",line_no);
				printf("String: \"%s\"\n\n",string);
				exit(EXIT_FAILURE);
		}
}

/**
 * Below function checks for different delimiters in different lines. 
 * If present then converts them into a single delimiter.
 *
 * In case of error,program is terminated with an error message.
 *
 *
 **/
void replace_delimiter(FILE **file_ptr,int file_desc)
{
		int c,ret_val,count=0;
		struct flock lock;

		/* Initializing the flock structure */
		memset(&lock,0,sizeof(lock));
		lock.l_type=F_WRLCK;

		/* checking for replacable delimiters */
		while((c=fgetc(*file_ptr))!=EOF)
		{

				/* ignoring the comments */
				if(c=='#' || c==';')
				{
						while((c=fgetc(*file_ptr))!='\n')
								;
				}
				if(c=='\n')
				{
						count=0;
				}

				/* ignore if delimiter is not different */
				if(count==0 && c=='=')
				{
						while((c=fgetc(*file_ptr))!='\n')
								;
				}

				/* Below code replaces delimiter "=" with "=" */
				if(c==':')
				{
						count++;
						if(count<LIMIT)
						{
								/* Below code sets write lock on file */
								if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
								{
										printf("\n File LOCKING Failed\n Error code: %s\n\n",strerror(errno));
										exit(EXIT_FAILURE);
								}

								/* replacing delimiter */
								fseek(*file_ptr, -1, SEEK_CUR);
								fputc('=',*file_ptr);
								fseek(*file_ptr, 0, SEEK_CUR);

								/* releasing the lock */
								lock.l_type=F_UNLCK;
								if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
								{
										printf("\n File UNLOCKING Failed\n Error code: %s\n\n",strerror(errno));
										exit(EXIT_FAILURE);
								}
						}
				}
		}
}

/**
 * Below function prints the stored result
 * for file with section. 
 * argument- A pointer of type structure "section" 
 *
 **/
void ini_print(section *head_sect)
{

		/* printing the SECTIONS  */
		while(head_sect!=NULL)
		{
				printf("file");
				printf("\n%s\n\n",head_sect->section_name);

				/* printing the KEY and VALUE inside SECTION */
				while(head_sect->head_name_val!=NULL)
				{
						printf("KEY: %s\t\tVALUE: %s\n",head_sect->head_name_val->name,head_sect->head_name_val->value);
						head_sect->head_name_val=head_sect->head_name_val->next_name_val;
				}
				head_sect=head_sect->next_section;
		}
}

/**
 * Below function prints the stored result for file without SECTION 
 * argument- A pointer of type structure "name_value"
 *
 **/
void ini_print_without_sect(name_value *head_key_val)
{
		printf("\n\n\t=================RESULT==================\n\n");
		while(head_key_val!=NULL)
		{
				printf("\n\t KEY: %s\t\tVALUE: %s\n",head_key_val->name,head_key_val->value);
				head_key_val=head_key_val->next_name_val;
		}
}

/**
 * Below  funcion  searches the KEY/VALUE requested by the 
 * calling function in the structure and displays it. 
 * arguments-
 * argument-1: A pointer of type structure "CFILE"
 * argument-2: a character array which will contain the 
 * 			   KEY or VALUE to be searched
 * return value: A pointer of type character
 * 				 NULL if result not found. 
 *
 * */
char* get_key_val(CFILE *head_key_val,char string[SIZE])
{
		name_value *nvl=head_key_val->name_value;	
		printf("\n\n\t=================SEARCH RESULT==================\n\n");
		while(nvl!=NULL )
		{
				if(!strcmp(nvl->name,string))
				{
						printf("\n\t\tKEY: %s\t\tVALUE: %s\n",nvl->name,nvl->value);
						return nvl->value;
				}
				nvl=nvl->next_name_val;
		}
		printf("\n");
		printf("\n\t\tRequested VALUE not found\n\n");
		return NULL;
}

/** 
 * Below funcion  displays all KEY and VALUE. 
 * argument: A pointer of type CFILE.
 *
 **/
void get_all_key_val(CFILE *head_key_val)
{
		name_value *nvl=head_key_val->name_value;	
		printf("\n\n\t=================SEARCH RESULT==================\n\n");
		while(nvl!=NULL )
		{
				{
						printf("\n\t\tKEY: %s\t\tVALUE: %s\n",nvl->name,nvl->value);
				}
				nvl=nvl->next_name_val;
		}
}

/**
 * Below function prints the content of the  SECTION requested by the 
 * calling function.
 *
 * arguments-
 * argument-1: A pointer of type structure "CFILE"
 * argument-2: a character array which will contain the 
 * 			   section name
 * argument-3: a character array which will contain the 
 * 			   KEY 
 * return value: A pointer of type character if success
 *				 NULL result not found.
 *
 * */
char* get_sect_key_val(CFILE *head_sect,char section_arr[SMALL_SIZE],char key[SMALL_SIZE])
{
		section *sect=head_sect->sect_ptr;
		name_value* nvl;
		printf("\n\n\t=================SEARCH RESULT==================\n\n");
		// printing the SECTIONS  
		while(sect!=NULL)
		{
				nvl=sect->head_name_val;
				// if SECTION and KEY is matched  
				if(!strcmp(sect->section_name,section_arr))
				{
						while(nvl!=NULL)
						{
							if(!strcmp(nvl->name,key))
							{
								printf("\n\n\t    SECTION: %s\n\n",sect->section_name);
								printf("\t    KEY: %s\t\tVALUE: %s\n",nvl->name,nvl->value);
								return nvl->value;
							}
							nvl=nvl->next_name_val;
						}
				}
				sect=sect->next_section;
		}
		printf("\n");
		printf("\n\t\tRequested value not found\n\n");
		return NULL;
}

/** 
 * Below function prints the all SECTION,KEY and VALUE 
 * argument: a pointer of type structure "CFILE"
 *
 */
void get_all(CFILE *head_sect)
{
		section *sect=head_sect->sect_ptr;
		name_value *nvl;
		  
		while(sect!=NULL)
		{
			printf("\n\n\t    SECTION: %s\n\n",sect->section_name);
			nvl=sect->head_name_val;
			while(nvl!=NULL)
			{
					printf("\t    KEY: %s\t\tVALUE: %s\n",nvl->name,nvl->value);
					nvl=nvl->next_name_val;
			}
				sect=sect->next_section;
		}
}

/**
 * Below function sets the value for file without SECTION 
 * arguments-
 * argument-1: A pointer of type structure "CFILE"
 * argument-2: A charcter array that will contain the KEY
 * argument-3: A charcter array that will contain the VALUE
 * argument-4: A charcter array that will contain the file path
 *			   for which key and value to be set.
 *
 * return value: 0(success)
 *				-1(failure)
 */
int set_key_value(CFILE *data,char key[SIZE],char value[SIZE],char file[SIZE])
{
		int ret_val,key_found=FALSE,file_desc;
		struct flock lock;
		FILE *file_ptr;

		name_value* nmvl = data->name_value;

		/* initializing the lock structure  */
		memset(&lock,0,sizeof(lock));
		lock.l_type=F_WRLCK;

		/* Below code searches for KEY of the VALUE to be set */
		while(data->name_value!=NULL )
		{
				if(!strcmp(data->name_value->name,key))
				{
						strcpy(data->name_value->value,value);
						key_found=TRUE;	//set the flag to TRUE if KEY is found
						break;
				}
				data->name_value=data->name_value->next_name_val;
		}

		/* Below function opens a file for reading and also checks for error  */
		if((file_desc=open(file,O_WRONLY | O_TRUNC))<0)
		{
				printf("\n Unable to open file\n Error code: %s\n\n",strerror(errno));
				return -1;
		}

		/* Below function obtaines the file pointer from file descriptor  */
		if((file_ptr=fdopen(file_desc,"w"))==NULL)
		{
				printf("\n Unable to open file\n Error code: %s\n\n",strerror(errno));
				return -1;
		}

		/* Obtaining writelock for file */
		if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
		{
				printf("\n File LOCKING Failed\n Error code: %s\n\n",strerror(errno));
				return	-1;
		}

		/* Below code will write the KEY and VALUE into the file */
		while(nmvl!=NULL )
		{
				fprintf(file_ptr,"%s = %s\n",nmvl->name,nmvl->value);
				nmvl=nmvl->next_name_val;
		}

		/* below code will write into the file only if KEY is not found in the structure  */
		if(key_found==FALSE)
		{
				fprintf(file_ptr,"%s = %s\n",key,value);
		}

		/* releasing locks after successfull write */
		lock.l_type=F_UNLCK;
		if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
		{
				printf("\n File UNLOCKING Failed\n Error code: %s\n\n",strerror(errno));
				return -1;
		}

		fclose(file_ptr);
		close(file_desc);
return 0;
}

/*
 * Below function sets the value for file with SECTION 
 * arguments-
 * argument-1: A pointer of type structure "CFILE"
 * argument-2: A charcter array  that will contain the secion
 * argument-3: A charcter array  that will contain the KEY
 * argument-4: A charcter array  that will contain the VALUE
 * argument-5: A charcter array  that will contain the file path
 *			   for which section key and value to be set.
 *
 * return value: 0(success)
 *				-1(failure)
 */
int set_sect_key_value(CFILE *data,char sectionArray[SIZE],char key[SIZE],char value[SIZE],char file_path[SIZE])
{
		int ret_val,section_found=FALSE,key_found=FALSE,file_desc;
		struct flock lock;
		FILE *file_ptr;

		section* sect=data->sect_ptr;
		name_value* nmvl;

		// Initializing the flock structure with zero 
		memset(&lock,0,sizeof(lock));
		lock.l_type=F_WRLCK;

		/* Below code searches for SECTION and KEY to be changed in the structure  */
		while(sect!=NULL)
		{
				nmvl=sect->head_name_val;
				if(!strcmp(sect->section_name,sectionArray))
				{
					section_found=TRUE;
					if(section_found)
					{
						while(nmvl!=NULL)
						{
							if(!strcmp(nmvl->name,key))
							{
									key_found=TRUE;
									strcpy(nmvl->value,value);
									break;
							}
							nmvl=nmvl->next_name_val;
						}
					}
				}
				if(key_found==TRUE)
				{
					break;
				}
				sect=sect->next_section;
		}

		/* Below piece of code opens a file for reading and also checks for error  */
		if((file_desc=open(file_path,O_WRONLY | O_TRUNC))<0)
		{
				printf("\n Unable to open file\n Error code: %s\n\n",strerror(errno));
				return -1;
		}

		/* Below piece of code gets the file pointer from file descriptor  */
		if((file_ptr=fdopen(file_desc,"w"))==NULL)
		{
				printf("\n Unable to open file\n Error code: %s\n\n",strerror(errno));
				return -1;
		}

		/* Obtaining writelock for file */
		if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
		{
				printf("\n File LOCKING Failed\n Error code: %s\n\n",strerror(errno));
				return	-1;
		}

		/* Below code will write the KEY and VALUE into the file */
		while(data->sect_ptr!=NULL)
		{
				fprintf(file_ptr,"\n[%s]\n\n",data->sect_ptr->section_name);
				while(data->sect_ptr->head_name_val!=NULL)
				{
						fprintf(file_ptr,"%s = %s\n",data->sect_ptr->head_name_val->name,data->sect_ptr->head_name_val->value);
						data->sect_ptr->head_name_val=data->sect_ptr->head_name_val->next_name_val;
				}
				/* below code adds new KEY and VALUE in the corresponding SECTION to the file */
				if(key_found==FALSE)
				{
					if(!strcmp(data->sect_ptr->section_name,sectionArray))
					{
						fprintf(file_ptr,"%s = %s\n",key,value);
					}
				}
				data->sect_ptr=data->sect_ptr->next_section;
		}

		/* below code will write into the file only if new section,key and value is encountered  */
		if(section_found==FALSE)
		{
				fprintf(file_ptr,"\n[%s]\n\n",sectionArray);
				fprintf(file_ptr,"%s = %s\n",key,value);
		}

		/* releasing locks after successfull write */
		lock.l_type=F_UNLCK;
		if((ret_val=fcntl(file_desc,F_SETLKW,&lock))<0)
		{
				printf("\n File UNLOCKING Failed\n Error code: %s\n\n",strerror(errno));
				return -1;
		}

		fclose(file_ptr);
		close(file_desc);
return 0;
}

