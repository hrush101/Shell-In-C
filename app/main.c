#include <stdio.h>
#include <string.h>
int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  printf("$ ");

  // Wait for user input and exit when user inputs
  char input[100];
  fgets(input, 100, stdin);
  

  // remove trailling newline ('\n') as user enter cmds the array of character
  // by adding null terminator to last index that point last index to '\0'
  input[strlen(input)-1]='\0';
  printf("%s: command not found\n",input);

  return 0;
}
