#include"main.h"
char **external_commands=NULL;
void extract_external_commands(char ***external_commands)
{
    //open the file(common.txt)
	FILE *file = fopen("command.txt", "r");
    if (file == NULL) {
        perror("Error: Could not open file.\n");
        return;
    }
    
    // validated file is opened or not
      // Allocate memory for the array of pointers
    *external_commands = (char **)malloc(MAX_COMMANDS * sizeof(char *));
    if (*external_commands == NULL) {
        perror("Error: Memory allocation for external_commands failed.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    int row=0,col=0;
    char temp[50];
    //run loop till the reaches the EOF		
    while (fgets(temp, sizeof(temp), file)) {
        // Remove the newline character, if present
        temp[strcspn(temp, "\n")] = '\0';

        // Allocate memory for the command and copy it to the array
        (*external_commands)[row] = (char *)malloc((strlen(temp)+1)*sizeof(char));
        if ((*external_commands)[row]== NULL) {
            perror("Error: Memory allocation failed.\n");
	    fclose(file);
		exit(EXIT_FAILURE);
            //fclose(file);
            //return;
        }
        strcpy((*external_commands)[row], temp);
        row++;

        // Stop if we've reached the maximum number of commands
        if (row >= MAX_COMMANDS) {
            break;
        }
    }
    //check the new line is there or not
    //if it is there 
    	// allocate memory for that row external_commands[row]
    	//steore the content in the row
    	// row++

    //if it is not the new line
    	//store the charcter in the current col posistion
    	//in the temp buffer
    (*external_commands)[row] = NULL; // Null-terminate the array
    fclose(file);
}

int check_command_type(char *command)
{
   
	char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
	//  check the given command is present in the buitltin array or not
		//if present returmn .>BUITIN
extract_external_commands(&external_commands);
		  for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(command, builtins[i]) == 0) {
            return BUILTIN;
        }
    }
	

	//  check the given command is present in the external_commands array or not
		//if present returmn ->B EXTERNAL
		  for(int i=0;external_commands[i]!=NULL;i++)
		  {
		      if(strcmp(command,external_commands[i])==0)
		      {
		return EXTERNAL;
		}
		  }
	// return NO_COMMAND
		return NO_COMMAND;
}
