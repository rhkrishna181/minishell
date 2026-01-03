/*############################################################################################### 
 *
 *           Author         :Hari krishna R 
 *           Date           : 26-01-2025
 *           File           : main.c©
 *           Descriptions   : minishell program capable of executing commands and job controll
 *           Objective      :  To implement a basic shell that can interpret user input, execute commands,
 *                             and manage job control signals 
 *           Usage          :
 *           Output         :
 *
###############################################################################################*/
#include<stdio.h>
#include "main.h"
int main()
{
    char input_string[100];
    char prompt[100]="minishell$ ";
    system("clear");
    scan_input(prompt,input_string);
	return 0;
}


