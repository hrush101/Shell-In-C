#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for Linux system calls like access()


// function to return fully qualified path
char *get_path(char *cmd){

	// returning pointer pointing to full_path array
	 char *r_path = NULL;  
	   
	// this will return ';' seprated executable dir paths in envirnoment variable 'PATH'
	char *path = getenv("PATH"); 

	// check if path is null and retun if true
	if (path == NULL){
		return NULL;
	}

	char *path_copy = strdup(path); // create a copy of ENV var PATH as we should not modify orignal path variable
    
	// this splits the path_copy string into individual directories using ';' as a delimiter
	char *dir_path = strtok(path_copy, ":"); 

	char full_path[2000]; // array to store fully qualified path from root ex - /usr/bin/ls
    
	// travel tokens / paths seprated by delimiter ';' 
	while (dir_path != NULL)
	{
	  // concatinate dir and cmd to fully qualified path from root 
	  // ex - C:\\Windows\\System32\\echo.exe
	  snprintf(full_path , sizeof(full_path) , "%s/%s" , dir_path , cmd);

	  // Check if C:\\Windows\\System32\\echo.exe exists as executable
	  if( access(full_path,X_OK)== 0 ){
		    r_path=strdup(full_path); // will create a copy of path array and return pointer to caller 
			free(path_copy);// if call returns from here free allocated path_copy
		    return r_path;
	
		// parameter taken by _access 1st-fully qualified path of cmd bin and 2nd-mode using int - 0,1,2,4 or bitwise OR
		// 1 or X_OK Checks if the file is executable.
		// 0 or F_OK Checks if the file exits.
		// 2 or R_OK Checks if the file is readable.
        // 4 or W_OK Checks if the file is writable.

	  }

	  dir_path= strtok(NULL,":"); // returns next token/dir path until it becomes null
	}
    
	free(path_copy); // free copied path
	return NULL; // if it access method return -1 it will not return full_path 
	// so this will get return eventually to caller
	
};

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
 	 // print promt and flush stdout to print it on screen
 	 printf("$ ");
 	 fflush(stdout);

 	 // cmd input
 	 char input[100];
	 fgets(input, 100, stdin);
  

 	 // remove trailling newline ('\n') as user enter cmds the array of character
 	 // by adding null terminator to last index that point last index to '\0'
 	 input[strlen(input)-1]='\0';

 	 if (!strcmp(input,"exit 0")) {
        	exit(0);
         }else if (!strncmp(input,"echo",strlen("echo"))){
               char *str = &input[(strlen("echo")+1)];
	       printf("%s\n",str);
	 }else if (!strncmp(input,"type",strlen("type"))){
               		char *ptr[] = {

         		"echo",
         		"type",
         		"exit",

         		};

        	       char *cmd = &input[(strlen("type")+1)];
              	       int found = 0;       
	      	        for(int i=0;i<3;i++){
         
        	           if(!strcmp(ptr[i],cmd)){
                	      printf("%s is a shell builtin\n",cmd);
		   	        found = 1;
		              break;
		  	 }
	       	}
            
			// If not a shell builtin, search in PATH env variable
			if(found != 1){
				char *cmd_path=get_path(cmd);

				if (cmd_path != NULL){
					printf("%s is %s\n", cmd, cmd_path);
					free(cmd_path);
				}
				else{
					printf("%s: not found\n", cmd);
				}
			}
	       
	 }else{
               printf("%s: command not found\n",input);
         }
	
  }
  return 0;
}
