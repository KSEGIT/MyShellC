/*
###### MyShell Project #######
CST2555 – Operating System and Computer Networks Coursework 1 – Operating System

Authors: 
Daniel Kiska 
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"
#define BUFFER_LENGTH 256

// ###### VARIABLES #######
    char buffer[BUFFER_LENGTH] = { 0 };
    char command[BUFFER_LENGTH] = { 0 };
    char arg[BUFFER_LENGTH] = { 0 };
    char arg2[BUFFER_LENGTH] = { 0 };
    char cwd[BUFFER_LENGTH] = { 0 };
    char line[BUFFER_LENGTH] = { 0 };
    char output[BUFFER_LENGTH] = { 0 };
    char shell[BUFFER_LENGTH] = { 0 };

//TODO use pointers
void runCommandLoop(char command[BUFFER_LENGTH], char arg[BUFFER_LENGTH], char arg2[BUFFER_LENGTH]){
//###### Starting command list #######
    // COMMAND -- cd -- change directory
    if (strcmp(command, "cd") == 0)
    {
        if (strcmp(arg,"")!= 0){
            //system changing dir 
            chdir(arg);
        }
        else{
            //getting current dir
            system("pwd");
        }
    }
    // COMMAND -- dir -- directory listing
    else if (strcmp(command, "dir") == 0)
    {
        getcwd(cwd, sizeof(cwd)); //store cwd
        strcpy(command,"cd"); 
        strcat(command," ");
        strcat(command,arg);
        system(command);
        system("ls");
        strcat(command," ");
        strcat(command,cwd);
        system(command);
    }
    //COMMAND -- environ -- enviorment variables
    else if (strcmp(command, "environ") == 0 && strcmp(arg, "") == 0)
    {
        system("printenv"); // using linux library
    }
    //COMMAND -- cls -- clear terminal command
    else if (strcmp(command,"clr")== 0 && strcmp(arg, "") == 0){
        system("reset"); // using linux library
    }
    //COMMAND -- cls -- clear terminal command
    else if (strcmp(command,"copy")== 0 && strcmp(arg, "") != 0 && strcmp(arg2, "") != 0){
        //preparing command for system
        strcpy(command,"cp"); 
        strcat(command," ");
        strcat(command,arg);
        strcat(command," ");
        strcat(command,arg2);
        system(command);
    }
    //COMMAND -- rm -- removing directory
    else if (strcmp(command,"rd")== 0 && strcmp(arg, "") != 0){
        //preparing command for system
        strcpy(command,"rmdir"); 
        strcat(command," ");
        strcat(command,arg);
        system(command);
    }
    //COMMAND -- md -- creating directory
    else if (strcmp(command,"md")== 0 && strcmp(arg, "") != 0){
        //preparing command for system
        strcpy(command,"mkdir"); 
        strcat(command," ");
        strcat(command,arg);
        system(command);
    }
    //COMMAND -- quit -- exit the shell
    else if (strcmp(command, "quit") == 0 && strcmp(arg, "") == 0)
    {
        //end program
        exit(1);
    }
    // COMMAND -- echo -- repeat text
    else if (strcmp(command, "echo") == 0 && strcmp(arg, "") != 0)
    {
        //strcat(arg,"\n");
        fputs("\n",stderr);
        fputs(arg,stderr);
        fputs("\n",stderr);
    }
    // COMMAND -- help -- printing help for the user
    else if (strcmp(command, "help") == 0 && strcmp(arg, "") == 0){
        fputs("List of available commands, for more help visit README.md\n\n",stderr);
        fputs("1. cd <directory> - changes the current default directory to <directory>.\n",stderr);
        fputs("2. clr - clear screen\n",stderr);
        fputs("3. dir <directory> - lists the contents of directory <directory>. \n",stderr);
        fputs("4. copy <source> <destination> - Copy the <source> folder to <destination>.\n",stderr);
        fputs("5. echo <comment> - Display <comment> on the display followed by a new line - (remember to put massage in double quotes like \"text here\").\n",stderr);
        fputs("6. md <directory> - Create the folder <directory>.\n",stderr);
        fputs("7. rd <directory> - Remove the folder <directory>.\n",stderr);
        fputs("8. help - shows all available commands \n",stderr);
        fputs("9. quit - Quit the shell.\n",stderr);
        fputs("\n",stderr);
    }
    // wrong command (not mentioned in the list)
    else
    {
        fputs("Wrong command - use only command provided in help\n", stderr);
    }
    //reseting values
    getcwd(shell, sizeof(shell));
    strcat(shell,": ");
    fputs(shell,stderr);
}
//function for setting enviroment
void setMyShellEnv(){
  char home_path[1024];
  getcwd(home_path, 1024);
  strcat(home_path, "/myshell");
  setenv("shell", home_path, 1);
}

//main function
int main(int argc, char *argv[])
{
    //setting shell enviroment
    setMyShellEnv();
    // checking if script runned with file input
    if (argv[1] != NULL){
            FILE *fp;
            fp = fopen(argv[1], "r");
            //starting loop
            if (fp == NULL){
            //if batchfile doesnt exist
            printf("Batchfile %s was not found\n", argv[1]);
            exit(1);
            }
            else{ 
            //File is open successfully 
                //scaning lines
                char line [BUFFER_LENGTH];
                while(fgets(line,sizeof line, fp) != NULL){
                    //run here for every line
                    sscanf(line,"%s %s %s", command, arg, arg2);
                    runCommandLoop(command, arg, arg2);
                    } 
                }
            fclose(fp);
            exit(1);
            printf("Batchfile %s was executed\n", argv[1]);
            return EXIT_SUCCESS;
        }

  // starting infinite loop for getting command input from the user
  getcwd(shell, sizeof(shell));
  strcat(shell,": ");
  fputs(shell,stderr);
  while (fgets(buffer, BUFFER_LENGTH, stdin) != NULL)
  {
    //getting tokens from the string and:
    //saving argument into arg variable
    strcpy(arg,"");
    //saving command into command variable
    strcpy(command,"");
    //second variable for example for copying
    strcpy(arg2,"");
    //reading input and getting tokens from string
    sscanf(buffer,"%s %s %s", command, arg, arg2);
    // for echo we need to check first character of the argument
    char a = arg[0];
    char b = '"';
    if ( a == b ){
         //fputs("checking for echo parameter\n",stderr);
         char forEcho[BUFFER_LENGTH];
         strcpy(forEcho, buffer);
        //runing loop with long argument
        runCommandLoop(command, forEcho, arg2);
    }else{
        runCommandLoop(command, arg, arg2);
    } 
  }
return EXIT_SUCCESS;
}