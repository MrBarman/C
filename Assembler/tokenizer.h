void tokenize()
{
    FILE *fpc,*fpcn;
    int ic=0,jc=0,space;
    char temp[50],store[30],str2[30];
    fpc = fopen("source2.asm","r");
    if(fpc==NULL)
		return;
    fpcn = fopen("sic.txt","w");
    if(fpcn==NULL)
		return;
    //printf("The source file is : \n");
    while(!feof(fpc))
    {
        ic=0;
        jc=0;
        fgets(temp,50,fpc);
        //printf("%s",temp);//printing the source file
        //store[0] = '\0';
        //if(temp[0] != '!')
        //{
        while(temp[ic] != '\0')
        {
            while(isspace(temp[ic]))//checking blank space at starting of the line
            {
                ic++;
            }
            if(temp[ic] == '!')
                break;
            while(!isspace(temp[ic]) && temp[ic] != '\0' && temp[ic] != '!')//continue until next space not found or end of the line
            {
                store[jc] = temp[ic];//storing first word
                ic++;
                jc++;
            }
            if(isspace(temp[ic]) && temp[ic] != '\0')//enter if there is a space after first word and it is not end
            {
                while(isspace(temp[ic]))//continue until there is a white space character
                {
                    ic++;
                }
                if(temp[ic] != '\0')//if it is not the end of the line
                        store[jc++] = ' ';//inserting first space
                while(!isspace(temp[ic]) && temp[ic] != '\0' && temp[ic] != '!')//continue if it is not a white space or end of line
                {
                    store[jc] = temp[ic];//store the second word
                    ic++;
                    jc++;
                }
                if(isspace(temp[ic]) && temp[ic] != '\0')//enter if there is a space after second word and it is not end
                {
                    while(isspace(temp[ic]))//skipping all spaces
                    {
                        ic++;
                    }
                    if(temp[ic] != '\0')
                        store[jc++] = ' ';//inserting second space if it is not end of the line
                    while(!isspace(temp[ic]) && temp[ic] != '\0' && temp[ic] != '!')//continue if it is not a white space and not end end
                    {
                        store[jc] = temp[ic];//store the third word
                        ic++;
                        jc++;
                    }
                }
                store[jc++]='\n';//printing a next line after every line
                store[jc] = '\0';
                jc=0;
                //adding space to the front of the line which don't have label
                space = 0;
                while(store[jc]!='\0')//continue until end of the line
                {
                    if(store[jc] == ' ')//enter if it is a space
                    {
                        space++;//counting space in one line
                    }
                    jc++;
                }
                if(space < 2)//if number of space is less than 2 then enter a white space in front of the line
                {
                    strcpy(str2, " ");
                    strcat(str2,store);
                    fputs(str2,fpcn);
                }
                else
                {
                    fputs(store,fpcn);
                }   //fputs(store,fpcn);
            }
        }
    }
    fclose(fpc);
    fclose(fpcn);
}
