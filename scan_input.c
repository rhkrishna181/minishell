#include "main.h"
extern char **external_commands;
char *external_command[200];

void my_sigaction(int signum, siginfo_t *info, void *ucontext){
    if(signum==SIGINT)
    {
	printf("\n");
    }
    fflush(stdout);
}
char *get_command(char *input_string)
{

    static char command[20];
    // stoe the first word till the input_string raeches space or null charcter
    int i=0;
    while(*input_string!=' ' && *input_string!='\0')
    {
	{
	    command[i++]=*input_string++;
	}
    }
    command[i]='\0';
    return command;
}
void echo(char *input_string, int status) {
    if (status == 0) {
	printf("%s\n", input_string);
    } else {
	fprintf(stderr, "Error: %s\n", input_string);
    }
}
void scan_input(char *prompt,char *input_string)
{

    struct sigaction var;
    memset(&var,0,sizeof(var));
    var.sa_sigaction=my_sigaction;
    var.sa_flags=SA_SIGINFO;
    sigaction(SIGINT,&var,NULL);
    int status;
    char *cmd=NULL;
    char **external_command=NULL;
    int ret=-3;
    while(1)
    {
	//print the prompt
	//	extract_external_commands(&external_command);
	__fpurge(stdin);
	printf("%s",prompt);
	fflush(stdout);
	//read the input
	// clear buffer before read
	memset(input_string, 0, sizeof(input_string)); // Clear buffer
	__fpurge(stdin);
	//scanf("%[^\n]",input_string);
	if (!fgets(input_string, 1024, stdin)) continue;
	input_string[strcspn(input_string,"\n")]='\0';
	// validate the PS! variable passed or not
	// if it is passed & valid => change the prompt array content
	if ((strlen(input_string) == 0) ) continue; // Ignore empty input

	/*
	   if(strstr(input_string,"PS1=")==NULL && flag) 
	   {
	   printf("PS1: Command not Found1\n");
	   flag=0;
	   continue;
	   }
	 */

	cmd=get_command(input_string);
	ret=check_command_type(cmd);
	//printf("command type is %d\n",ret);
	if(ret!=EXTERNAL && ret!=BUILTIN)
	{
	    // no command 
	    //print the error for cmd not found
	    //perror("the error for cmd not found\n");
	    if(strncmp("PS1=",input_string,4)==0)
	    {
		//if (strlen(input_string) == 0) continue; // Ignore empty input

		if (input_string[4]==' ') 
		{
		    int a=4;
		    while(input_string[a]==' ')
		    {	
			a++;
		    }    
		    int b=a;
		    while(input_string[b]!=' ')
		    {
			b++;
		    }
		    input_string[b]='\0';
		    printf("%s: Command not found\n",&input_string[a]);
		    continue; // Ignore empty input
		}
		if(input_string[4]!='\0' || input_string[4]!=' ')
		{
		    strcpy(prompt,&input_string[4]);
		    continue;
		}
		else
		{
		    printf("%s: Command not found\n",&input_string[4]);
		    continue;
		}
	    }

	    if(strstr(input_string,"PS1=")==NULL) 
	    {
		printf("%s: Command not Found\n",input_string);
		continue;
	    }

	}

	else if(ret==BUILTIN)
	{
	    //internal
	    execute_internal_commands(input_string);
	    free(external_commands); // Free memory after use
	    external_commands = NULL; // Reset pointer for next iteration
	}
	else if (ret==EXTERNAL)
	{
	    execute_external_command(input_string);
	    free(external_commands); // Free memory after use
	    external_commands = NULL; // Reset pointer for next iteration
	}
	

    }
}

void execute_internal_commands(char *input_string) {
    char *cmd = get_command(input_string);
    if (cmd == NULL) {
	echo("No command entered", 1);
	return;
    }
    // check cmd exit or not
    // => exit()    -->exit from the mini shell

    // cd cmd is cd cmd or not
    // chdir()  change working directory

    // check cmd  pwd or not
    // getcwd()    get current working directory
    // echo command
    // Parse the command into arguments
    if (strcmp(cmd, "exit") == 0) {
	exit(0);
    } 
    else if (strcmp(cmd, "cd") == 0) {
	char *path = strtok(NULL, " ");
	if (path == NULL || chdir(path) != 0) {
	    perror("cd failed");
	}
    } 
    else if (strcmp(cmd, "pwd") == 0) {
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	    printf("%s\n", cwd);
	} else {
	    perror("pwd failed");
	}
    } else if (strncmp(cmd, "echo", 4) == 0) {
	echo(&input_string[5], 0); // Pass everything after "echo"
    } 
    else {
	echo("Unknown command", 1);
    }
}



