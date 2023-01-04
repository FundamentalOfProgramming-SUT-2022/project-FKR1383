#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

void program(); // This function is the command giver in our VIM
void decide(char * , char *); // This function is the processor of our commands
void createfile(char *);

/*
 * first Name: Farzam
 * last Name: Koohi Ronaghi
 * student Id: 401106403
 * number of attempt: 1
 * date : 1401/10/14
 */

int main()
{
    // creating root folder after being executed
    // this folder is created beside of .exe file
    mkdir("root");
    while (1)
    {
        program();
    }
}

void program()
{
    // this use for saving commands
    char *command = calloc(2000000000 , sizeof(char));
    gets(command);
    // help is an extra string that is equal to command
    char *help = calloc(strlen(command) + 1 ,  sizeof(char));
    strcpy(help , command);
    char *operation = strtok(help , " ");
    decide(command , operation);
    return;
}

void decide(char *command , char *operation)
{
    // checking all situations for our commands
    if (strcmp(operation , "createfile") == 0){
        // new command is our command without first word
        char *newcommand = strchr(command, ' ');
        if(newcommand != NULL)
            command = newcommand + 1;
        else {
            printf("invalid command\n");
            return;
        }
        createfile(command);
        return;
    }
    /*else if()
    {

    }*/
}

void createfile(char *command)
{
    // first, We must check that our command is null or not
    char *help = calloc(strlen(command)+1 , sizeof(command));
    strcpy(help , command);
    char *file = strtok(help , " ");
    if (file == NULL)
    {
        printf("invalid command\n");
        return;
    }
    if (strcmp(file , "--file") == 0)
    {
        char *newcommand = strchr(command, ' ');
        if(newcommand != NULL)
            command = newcommand + 1;
        else {
            printf("invalid command\n");
            return;
        }
        // now, command will show us the path (directories) and file name
        if (command[0] == '"' && command[strlen(command)-1] == '"')
        {
            // minimum length of command must be /root/a.txt
            if (strlen(command) < 11)
            {
                printf("invalid command\n");
                return;
            }
            if (command[1] == '/' && command[2] == 'r' && command[3] == 'o' && command[4] == 'o' && command[5] == 't' && command[strlen(command)-2] == 't' && command[strlen(command)-3] == 'x' && command[strlen(command)-4] == 't' && command[strlen(command)-5] == '.')
            {
                //finding the end /
                int last = strlen(command)-1;
                while(command[last] != '/')
                    last--;
                char *dir = calloc(strlen(command) , sizeof(char));
                int j = 0;
                for (int i = 2; i <= last; i++)
                {
                    if (command[i] == '/')
                    {
                        dir[j] = '\0';
                        mkdir(dir);
                    }
                    dir[j] = command[i];
                    j++;
                }
                for (int i = last+1; i != strlen(command)-1; i++)
                {
                    dir[j] = command[i];
                    j++;
                }
                dir[j] = '\0';
                if (access(dir, F_OK) == 0) {
                    printf("This file name exists!\n");
                    return;
                } else {
                    FILE *fptr;
                    fptr = fopen(dir, "w");
                    fclose(fptr);
                    return;
                }
            }
            else
            {
                printf("invalid command\n");
                return;
            }
        }
        else if (command[0] == '/')
        {
            if (command[1] == 'r' && command[2] == 'o' && command[3] == 'o' && command[4] == 't' && command[strlen(command)-1] == 't' && command[strlen(command)-2] == 'x' && command[strlen(command)-3] == 't' && command[strlen(command)-4] == '.')
            {
                // finding the end //
                int last = strlen(command)-1;
                while(command[last] != '/')
                    last--;
                char *dir = calloc(strlen(command) , sizeof(char));
                int j = 0;
                for (int i = 1; i <= last; i++)
                {
                    if (command[i] == '/')
                    {
                        dir[j] = '\0';
                        mkdir(dir);
                    }
                    dir[j] = command[i];
                    j++;
                }
                for (int i = last+1; i != strlen(command); i++)
                {
                    dir[j] = command[i];
                    j++;
                }
                dir[j] = '\0';
                if (access(dir, F_OK) == 0) {
                    printf("This file name exists!\n");
                    return;
                } else {
                    FILE *fptr;
                    fptr = fopen(dir, "w");
                    fclose(fptr);
                    return;
                }
            }
            else
            {
                printf("invalid command\n");
                return;
            }
        }
        else
        {
            printf("invalid command\n");
            return;
        }
    }
    else
    {
        printf("invalid command\n");
        return;
    }
}

/*
 * Remember!
 * 1 - invalid commands must place at the end of the decide function!
 */