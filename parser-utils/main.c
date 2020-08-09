/** 
 *  @file    myheader.h
 *  @author  manash( barmanmanash005@gmail.com.com)
 *  @date    28/05/2018
 *  @version 1.0 
 *  
 *  @brief Parser-utils main function
 *
 *  @section DESCRIPTION
 *  
 *  This is the main program that
 *  contains the function to access the
 *  feature of parser utility
 *
 */

/* Declaring the header files */
#include<stdio.h>
#include<ctype.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"myheader.h"

/**
 * This is the main function that calls 
 * the services of parser utiity.
 *
 * Return value: 0 (success)
 *              -1 (failure)
 */
int main(int argc,char *argv[])
{
	CFILE *cread_ptr=NULL;
	char path[SIZE]="/home/linux/programs/ek-parser-utils/demo.ini";
	char *result=NULL;
	int ret_val;

	/*calling function for parsing*/
	cread_ptr=ini_read(path); 
	if(cread_ptr==NULL)
	{
		printf("\n\t Unable to open file\n\t Recheck filename or path\n");
		return -1;
	}
	
	// function for get value for file with section 
	if(cread_ptr->is_section==TRUE)
	{
		result=get_sect_key_val(cread_ptr,"FTP","RunFTP");
		if(result==NULL)
		{
			return -1;
		}
		get_all(cread_ptr);
		ret_val=set_sect_key_value(cread_ptr,"settings","Data","7",path);
		if(ret_val==-1)
		{
			return -1;
		}
	}

	/* function for get value for file without section */
	else if(cread_ptr->is_section==FALSE)
	{
		result=get_key_val(cread_ptr,"BACKUP");	
		if(result==NULL)
		{
			return -1;
		}
		get_all_key_val(cread_ptr);
		ret_val=set_key_value(cread_ptr,"NEWKEY","OK",path);
		if(ret_val==-1)
		{
			return -1;
		}
	}
free(cread_ptr);
return 0;
} 
