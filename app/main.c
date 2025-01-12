#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(-1){
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
         }
         

	 if (!strncmp(input,"echo",strlen("echo"))){
               char *str = &input[(strlen("echo")+1)];
	       printf("%s\n",str);
	 }else{
               printf("%s: command not found\n",input);

	 }
         
	 if (!strncmp(input,"type",strlen("type"))){
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
	       
	       if(found != 1){
            	  printf("%s: not found\n", cmd);                
	       }
	       
	 }
	
  }
  return 0;
}
