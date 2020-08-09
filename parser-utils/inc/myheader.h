/** 
 *  @file    myheader.h
 *  @author  manash( barmanmanash005@gmail.com)
 *  @date    28/05/2018
 *  @version 1.0 
 *  
 *  @brief Parser-utils  Header
 *
 *  @section DESCRIPTION
 *  
 *  This is  header file for parser utility
 *
 */
#ifndef __MY_HEADER_H__
#define __MY_HEADER_H__

#define SIZE 500
#define TRUE 1
#define FALSE 0
#define LIMIT 2
#define SMALL_SIZE 200

/**
 * Following are the declarartion and initialization
 * of static variables.   
 *
 **/
static int is_section=FALSE;
static char *delim="=";
static int is_file_without_section=FALSE;

/**
 * Following are the definition of  structures  
 * used in the parser utility
 * 
 **/
	typedef struct name_value{
	    char name[SIZE];
	    char value[SIZE];
   		struct name_value *next_name_val;
	}name_value;
 
	typedef struct section{
		char section_name[SIZE];
    	name_value *name_val;
    	name_value *head_name_val;
    	struct section *next_section;
	}section;

typedef struct config_file{
	char *path;
	int is_section;
	int cfile_desc;
	section *sect_ptr;
	name_value *name_value;
	FILE *cfp;
}CFILE;

/**
 * Below are the declaration of functions used in
 * parser utility.
 *
 **/
CFILE* ini_read(char *);
CFILE* ini_write(char *);
void ini_parse(char [],section ***,section **,int,name_value ***,name_value **);
void ini_lexical_analysis(FILE *,section **,int,name_value **);
void ini_print(section *);
int frequency_check(char []);
int is_key_val_pair(char []);
void key_val_check(char[],int);
void ini_print_without_sect(name_value *);
void replace_delimiter(FILE **,int);
char* get_sect_key_val(CFILE *,char[],char[]);
char* get_key_val(CFILE *,char[]);
void get_all(CFILE *);
void get_all_key_val(CFILE *);
int set_key_value(CFILE *,char[],char[],char[]);
int set_sect_key_value(CFILE *,char[],char[],char[],char []);

#endif
