#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for Linux system calls like access(), fork(), getcwd()
#include <sys/types.h> // includes the definition of various data types used in system calls, such as pid_t
#include <sys/wait.h> // for wait() system call
#include <limits.h> // For PATH_MAX is a system-defined constant that specifies the maximum length, in bytes, of a fully qualified path name, including the null terminator (\0)
#include <ctype.h> // For manipulation of charecter


// function to return fully qualified path
char* get_path(char *cmd){

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
	  // ex - C:\\Windows\\System32\\echo.exe or /bin/ls
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

	  dir_path= strtok(NULL,":"); // returns next token/dir_path until it becomes null
	}
    
	free(path_copy); // free copied path
	return NULL; // if it access method return -1 it will not return full_path 
	// so this will get return eventually to caller
	
}


// char* remove_extra_spaces(char *str){

//     char *start = strchr(str, '\''); // Find the first single quote
//     char *end = strrchr(str, '\'');  // Find the last single quote
//     char *source = NULL;             // Source to extract text
//     size_t len = 0;

//     // Determine the source of the text
//     if (start != NULL && end != NULL && end > start) {
//         source = start + 1;          // Text inside single quotes
//         len = end - start - 1;
//     } else {
//         source = str;                // Copy string to source
//         len = strlen(str);
//     }

//     char *text = (char *) malloc((len + 1) * sizeof(char)); // Allocate memory
    
//     int j = 0;          // Index for the text array
//     int space_seen = 0; // Flag to handle 1st consecutive space

//     for (size_t i = 0; i < len; i++) {
//         char current = *(source + i);

//         if (current == ' ') {
//             if (!space_seen) {
//                 text[j++] = current;
//                 space_seen = 1;
//             }
//         } else {

//             text[j++] = current;
// 			space_seen=0;

//         }
//     }
	
//     text[j] = '\0'; // Null-terminate the string
//     return text;    

// }


// Function to process echo with escape sequences inside double quotes
char* process_echo(char *str) {
    char *result = (char *)malloc(1000 * sizeof(char)); // Allocate large buffer
    result[0] = '\0'; // Initialize result string

    int in_single_quotes = 0;
    int in_double_quotes = 0;
    char buffer[1000];
    int buffer_index = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];

        // Toggle single quotes
        if (current == '\'' && !in_double_quotes) {
            in_single_quotes = !in_single_quotes;
            continue;
        }
        // Toggle double quotes
        if (current == '"' && !in_single_quotes) {
            in_double_quotes = !in_double_quotes;
            continue;
        }

        // Handle backslashes
        if (current == '\\') {
            if (in_single_quotes) {
                // Literal backslashes in single quotes
                buffer[buffer_index++] = current;
            } else if (in_double_quotes) {
                // Handle escape sequences in double quotes
                if (str[i + 1] != '\0' && str[i+1] != '\"') {
                    i++; // Skip the backslash
                    if (str[i] == '"' || str[i] == '\\' || str[i] == '$') {
                        buffer[buffer_index++] = str[i]; // Valid escape sequences
                    } else {
                        buffer[buffer_index++] = '\\'; // Keep backslash
                        buffer[buffer_index++] = str[i]; // Append the next character
                    }
                }
                // If backslash is the last character in double quotes, ignore it
            } else {
                // Outside of quotes, keep the backslash
                buffer[buffer_index++] = current;
            }
            continue;
        }

        // Handle spaces outside of quotes
        if (isspace(current) && !in_single_quotes && !in_double_quotes) {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                strcat(result, buffer);
                strcat(result, " ");
                buffer_index = 0;
            }
            continue;
        }

        // Append regular characters to the buffer
        buffer[buffer_index++] = current;
    }

    // Append remaining buffer to result
    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        strcat(result, buffer);
    }

    // Remove trailing space
    if (strlen(result) > 0 && result[strlen(result) - 1] == ' ') {
        result[strlen(result) - 1] = '\0';
    }

    return result;
}


void cat_file(char *files){    // print file content

	FILE *f = fopen(files,"r"); // open file of given path

	if (f == NULL){
		
		perror("failed to open file"); // if file not found
		
	} else {
            
		char c;

		while ((c=fgetc(f)) != EOF) { // get character pointed by file pointer f and store it in c until f reaches end of file
			putchar(c);
		}

		fclose(f);

	}

}

// Function to handle the `cat` command with multiple quoted file paths
void handle_cat(char *str) {

    char *start = str;

    while (*start) {
        // Detect the type of quotes
        char quote_type = '\0';
		
        if (*start == '\'') {
            quote_type = '\'';
        } else if (*start == '"') {
            quote_type = '"';
        }

        if (quote_type) {
            char *end = strchr(start + 1, quote_type);
            if (!end) {
                fprintf(stderr, "Error: Unmatched %c in input.\n", quote_type);
                return;
            }

            // Extract the file path between the quotes
            char file_path[300];
            int length = end - start - 1; // Length of the file path
            if (length >= sizeof(file_path)) {
                fprintf(stderr, "Error: File path too long.\n");
                return;
            }

            strncpy(file_path, start + 1, length);
            file_path[length] = '\0';

            if (quote_type == '"') {
                // Process escape sequences in double-quoted file paths
                char processed_path[300];
                int j = 0;
                for (int i = 0; file_path[i] != '\0'; i++) {
                    if (file_path[i] == '\\' && (file_path[i + 1] == '\\' || file_path[i + 1] == '"' || file_path[i + 1] == '$')) {
                        i++;
                    }
                    processed_path[j++] = file_path[i];
                }
                processed_path[j] = '\0';
                cat_file(processed_path);
            } else {
                cat_file(file_path); // Handle single-quoted file paths
            }

            start = end + 1;
        } else {
            start++;
        }
    }
}

void pwd(){

	char path[1000];

	if (getcwd(path,sizeof(path)) != NULL) {
		
		printf("%s \n",path);
	
	}

}

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1) {

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
        } else if (!strncmp(input,"echo",strlen("echo"))) {

            char *str = &input[(strlen("echo")+1)];
	       				
			char *final_text = process_echo(str);// remove extra spaces and print the string

			if(final_text != NULL ) {

				printf("%s \n",final_text);

			}

			free(final_text);

	    } else if (!strncmp(input,"type",strlen("type"))){
               	char *ptr[] = {"pwd","echo","type","exit"};

        	        char *cmd = &input[(strlen("type")+1)];
              	    int found = 0;       
	      	        for( int i=0 ; i < sizeof(ptr) / sizeof(ptr[0]) ; i++ ){
         
        	           	if(!strcmp(ptr[i],cmd)){
                	      printf("%s is a shell builtin\n",cmd);
		   	        	  found = 1;
		                  break;
		  	            }
	       	        }
            
			// If not a shell builtin, search in PATH env variable
			if(!found){
				char *cmd_path=get_path(cmd);

				if (cmd_path != NULL){
					printf("%s is %s\n", cmd, cmd_path);
					free(cmd_path);
				}
				else{
					printf("%s: not found\n", cmd);
				}
			}

	    } else if (!strcmp(input,"pwd")) { // print current working directory

			    pwd();

		} else if (!strncmp(input,"cat", strlen("cat"))) {

			char *files = &input[(strlen("cat")+1)];
			handle_cat(files);


		} else if (!strncmp(input,"cd", strlen("cd")) ) { // change dir both for absolute and relative path
            char *path = &input[(strlen("cd")+1)];
            int status;

			if (!strcmp(path,"~")){
				char *home_dir = getenv("HOME"); // to get home directory
				if (home_dir != NULL){
                   
				   status = chdir(home_dir);

				}else {
					perror("enable to get HOME \n");
				}
				
			}
			else {
				status = chdir(path);
			}

	       	if(status == -1)
			{
				printf("cd: %s: No such file or directory \n",path);
			}


		} else {   // here we are seprating cmd and executing it with the arguments passed with it ex - ls -l
   
				// Parse command and arguments i.e seprate cmd and argument passed with cmd
				char *args[10]; // array to hold cmd and it argument
				int argc=0;        // argument count
				char *token = strtok(input," "); // split the input string into space seprated token

				
				while ( token != NULL && argc < 10 ) // keep parsing until no more tokens left
				{

					args[argc++] = token; // store each token cmd + arguments in an array
					token = strtok(NULL, " "); // get next token till reaches null
					
				}

				args[argc] = NULL; // Null-terminate the array to mark the end of array

				char *cmd_path = get_path(args[0]); // find the full path of the command executable / binary
				// here args[0] is cmd

				if (cmd_path != NULL) { 

					// Fork and execute the command
					pid_t pid = fork(); // Create a child process

					// Here we create a new process using fork() because executing a command with execv replaces the current process image with the new program. 
					// If we didn't fork, the shell itself would be replaced by the command, and the shell would no longer be available to accept further commands
				
					if(pid == 0) // as fork return 0 if child process is created
					{
						
						// here child process executes the cmd using execv system call which is use to execute binary / executable of program
						execv(cmd_path,args); // execv replaces current program / process with new process 
						free(cmd_path);       // free cmd_path as its dynamic memory
						perror("execv failed"); // print error if execv fails
						exit(1);                //  exit 1 indicates the program is terminating due to an error in execv

					}
					else if (pid > 0) // in parent as child has not finished its execution
					{ 
						int status;
						waitpid(pid ,&status, 0); // parent waits for a child process to finish its execution

						// pid - process id of child that is 0
						// status - pointer pointing to exit status of the child process will be stored 1 or 0
						// 0 - the default behavior the parent process will wait until the child finishes executing

					}
					else                       // if fork fails
					{
						perror("fork failed"); // Print an error message fork failed to create child
					}

					free(cmd_path);

				}else {
            		printf("%s: command not found\n",input);
        		}
				
			}  
		
	
  }

  return 0;
}
