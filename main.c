#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#define maximum_size_of_input 1000000

void main_function(); // this function is the input receiver of the program
void createfile(char *); // this function creates a .txt file with specified path
FILE *open_or_create_file(char * , char*); // this function open an existing file or creates a new file by supposed path
void insertstr(char *); // command 2
FILE *find_path(char * , char * , int *); // this function find path of a command
void find_string(char * , char * , int *); // this function find string after --str and saves it ro second argument
bool find_position(char * , int * , int *); // this function find positon after --pos and if it has bad syntax return 0
bool go_to_position(FILE * , int , int , char *); // this function moves the file pointer to the supposed line and column
void save_text(FILE *, char *); // this function copies from file pointer to EOF in second argument
void back_the_text(FILE * , char * , int , char *); // this function pastes the copied text to the file pointer, third argument is required spaces
void cat(char *); // command 3
void removestr(char *); // command 4

/*
 * first Name: Farzam
 * last Name: Koohi Ronaghi
 * student Id: 401106403
 */

int main()
{
    mkdir("root");
    while (1)
    {
        main_function();
    }
}

void main_function()
{
    char *input = (char *)calloc(maximum_size_of_input , sizeof(char));
    gets(input);
    strtok(input , " ");
    // checker is the first word of command
    if (strcmp(input , "createfile") == 0){
        createfile(input);
    } else if (strcmp(input , "insertstr") == 0) {
        insertstr(input);
    } else if (strcmp (input , "cat") == 0){
        cat(input);
    } else if (strcmp (input , "removestr") == 0) {
        removestr(input);
    }
    else {
        printf("invalid command\n");
    }
    free(input);
}

void createfile(char *command) {
    command = strtok(NULL, " ");
    if (command == NULL || strcmp(command, "--file") != 0) {
        printf("invalid command\n");
        return;
    }
    char *path = (char *) calloc(maximum_size_of_input, sizeof(char));
    path[0] = '\0';
    command = strtok(NULL , "");
    strcpy(path , command);
    FILE *file = open_or_create_file(path , "w");
    if (file != NULL) {
        fclose(file);
    }
}

FILE *open_or_create_file(char *path , char *type)
{
    if (path[0] == '\0')
    {
        printf("invalid command\n");
        return NULL;
    }
    if (path[0] == '/') {
        if (strlen(path) < 11) {
            printf("invalid address\n");
            return NULL;
        }
        if (path[1] == 'r' && path[2] == 'o' && path[3] == 'o' && path[4] == 't' && path[5] == '/' && path[strlen(path)-1] == 't' && path[strlen(path)-2] == 'x' && path[strlen(path)-3] == 't' && path[strlen(path)-4] == '.' && path[strlen(path)-5] != '/') {
            char *virual_path = (char *)calloc(strlen(path) , sizeof(char));
            virual_path[0] = '\0';
            path++;
            if (strcmp(type , "w") == 0 && access(path, F_OK) == 0) {
                printf("This file exists\n");
                return NULL;
            } else if (strcmp(type , "r+") == 0 && access(path, F_OK) == -1) {
                printf("This file doesn't exist\n");
                return NULL;
            }
            strtok(path , "/");
            while (path != NULL)
            {
                strcat(virual_path , path);
                path = strtok(NULL , "/");
                if (path != NULL) {
                    mkdir(virual_path);
                    strcat(virual_path, "/");
                }
            }
            return fopen(virual_path , type);
        } else {
            printf("invalid address\n");
            return NULL;
        }
    } else if (path[0] == '"' && path[strlen(path)-1] == '"') {
        path++;
        path[strlen(path)-1] = '\0';
        return open_or_create_file(path , type);
    } else {
        printf("invalid address\n");
        return NULL;
    }
}

void insertstr(char *command)
{
    command = strtok(NULL , "");
    char *resume = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(resume , command);
    strtok(command , " ");
    int skip;
    if (command == NULL || strcmp(command, "--file") != 0) {
        printf("invalid command\n");
        return;
    }
    FILE *file = find_path(command , "r+" , &skip);
    if (file == NULL) {
        return;
    }
    resume += (7 + skip);
    if (resume == NULL)
    {
        printf("invalid command\n");
        return;
    }
    resume++;
    if (resume == NULL) {
        printf("invalid command\n");
        return;
    }
    char *string = (char *)calloc(strlen(resume) + 1 , sizeof(char));
    char *help = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(help , resume);
    find_string(resume , string , &skip);
    strcpy(resume , help);
    resume += (skip + 6);
    if (resume == NULL || resume[1] == '\0') {
        printf("invalid command\n");
        return;
    }
    resume++;
    int line = 0 , col = 0;
    bool flag = find_position(resume , &line , &col);
    if (!flag) {
        printf("invalid command\n");
        return;
    }
    go_to_position(file , line , col , "insert");
    go_to_position(file , line , col , "insert");
    char *copy = (char *)calloc(maximum_size_of_input , sizeof(char));
    save_text(file , copy);
    fseek(file , 0 , SEEK_SET);
    FILE *file2 = file;
    go_to_position(file2 ,line , col , "insert");
    FILE *file3;
    fseek(file2 , ftell(file2), SEEK_SET);
    file3 = file2;
    back_the_text(file3 , string , 0 , "second_time");
    FILE *file4;
    fseek(file3 , ftell(file3) , SEEK_SET);
    file4 = file3;
    back_the_text(file4 , copy ,0 , "second_time");
    fclose(file);
}

FILE *find_path(char *resume, char *type , int *skip)
{
    // this function starts with --file
    if (resume == NULL) {
        printf("invalid command\n");
        return NULL;
    }
    resume = strtok(NULL , "");
    if (resume == NULL)
    {
        printf("invalid command\n");
        return NULL;
    }
    if (resume[0] == '/') {
        strtok(resume , " ");
        char *path = (char *) calloc(strlen(resume)+1 , sizeof(char));
        strcpy(path , resume);
        *skip = strlen(path);
        return open_or_create_file(path , type);
    }
    if (resume[0] == '\"') {
        char *path = (char *) calloc(maximum_size_of_input , sizeof(char));
        int i = 0;
        for (; ; i++)
        {
            path[i] = resume[i];
            if (path[i] == '\"' && i != 0)
                break;
        }
        path[i+1] = '\0';
        *skip = strlen(path);
        return open_or_create_file(path , type);
    }
}

void find_string(char *resume , char *string , int *skip)
{
    // this function starts with --str
    char *help = (char *)calloc(maximum_size_of_input , sizeof(char));
    strtok(resume , " ");
    if (resume == NULL || strcmp(resume , "--str") != 0) {
        printf("invalid command\n");
        return;
    }
    resume = strtok(NULL , "");
    strcpy(help , resume);
    if (resume == NULL){
        printf("invalid command\n");
        return;
    }
    if (resume[0] != '\"') {
        strtok(resume , " ");
        int j = 0 , i = 0;
        for (; i != strlen(resume); i++)
        {
            if (resume[i] == '\\') {
                if (resume[i+1] == 'n') {
                    i++;
                    string[j] = '\n';
                    j++;
                } else {
                    string[j] = resume[i+1];
                    i++;
                    j++;
                }
            } else {
                string[j] = resume[i];
                j++;
            }
        }
        *skip = i;
        strcpy(resume , help);
        return;
    }
    else {
        int j = 0 , i = 0;
        for (; i != strlen(resume); i++) {
            if (resume[i+1] == '\\') {
                if (resume[i+2] == 'n'){
                    string[j] = '\n';
                    j++;
                    i++;
                } else {
                    string[j] = resume[i+2];
                    i++;
                    j++;
                }
            } else if (resume[i+1] == '\"') {
                break;
            } else {
                string[j] = resume[i+1];
                j++;
            }
        }
        *skip = i+2;
        strcpy(resume , help);
        return;
    }
    return;
}

bool find_position(char *pos , int *line , int *col)
{
    // this function starts with --pos x:y
    strtok(pos , " ");
    if (pos == NULL || strcmp(pos , "--pos") != 0) {
        return false;
    }
    pos = strtok(NULL , "");
    *line = *col = 0;
    int i;
    bool is_two_dots = false;
    for (i = 0; i != strlen(pos); i++) {
        if ((pos[i] < '0' || pos[i] > '9') && pos[i] != ':') {
            return false;
        }
        if (i == 0 && pos[i] == ':') {
            return false;
        }
        if (pos[i] == ':') {
            i++;
            is_two_dots = true;
            break;
        }
        *line *= 10;
        *line += (pos[i] - '0');
    }
    if (i == strlen(pos) || !is_two_dots)
        return false;
    while (i != strlen(pos)) {
        if (pos[i] < '0' || pos[i] > '9') {
            return false;
        }
        *col *= 10;
        *col += (pos[i] - '0');
        i++;
    }
    return true;
}

bool go_to_position(FILE *file , int line , int col , char *mode)
{
    fseek(file , 0 , SEEK_SET);
    int now_line = 1 , now_col = 0;
    char now_char = '\0';
    while (line != 1 && now_char != EOF) {
        now_char = fgetc(file);
        if (now_char == EOF)
            break;
        if (now_char == '\n')
            now_line++;
        if (now_line == line)
            break;
    }
    if (now_line < line) {
        if (strcmp(mode , "insert") != 0)
            return false;
        while (line > now_line) {
            fputc('\n' , file);
            now_line++;
        }
    }
    if (strcmp(mode , "help") == 0)
        return true;
    if (now_char == EOF) {
        if (strcmp(mode , "insert") != 0) {
            return false;
        }
        fseek(file , 0 , SEEK_END);
        for (int i = 0; i != col; i++)
            fputc(' ' , file);
    } else {
        while (now_col < col) {
            now_char = fgetc(file);
            if (now_char != '\n' && now_char != EOF) {
                now_col++;
            }
            else
                break;
        }
        if (now_col < col) {
            if (strcmp(mode , "insert") != 0) {
                return false;
            }
            char *copy = (char *) calloc(maximum_size_of_input , sizeof(char));
            save_text(file , copy);
            go_to_position(file , line , col , "help");
            for (int i = 0; i != now_col; i++) {
                now_char = fgetc(file);
            }
            if (now_char == EOF) {
                fseek(file , 0 , SEEK_END);
            }
            fseek(file , ftell(file) , SEEK_SET);
            FILE *file2 = file;
            back_the_text(file2 , copy , col - now_col , "first_time");
        }
    }
}

void save_text(FILE *file , char *copy)
{
    int i = 0;
    char now = '\0';
    while (now != EOF) {
        now = fgetc(file);
        if (now != EOF) {
            copy[i] = now;
            i++;
        }
    }
    copy[i] = '\0';
    return;
}

void back_the_text(FILE *file , char *copy , int spaces , char *mode)
{
    if (strcmp(mode , "first_time") == 0) {
        for (int i = 0; i != spaces; i++)
            fputc(' ', file);
        fputc('\n', file);
    }
    for (int i = 0; i != strlen(copy); i++)
       fputc(copy[i] , file);
    return;
}

void cat(char *command)
{
    command = strtok(NULL , " ");
    if (command == NULL) {
        printf("invalid command\n");
        return;
    }
    if (strcmp(command , "--file") != 0) {
        printf("invalid command\n");
        return;
    }
    int skip;
    FILE *file = find_path(command , "r+" , &skip);
    if (file == NULL) {
        return;
    }
    char now_char = '\0';
    while (now_char != EOF) {
        now_char = fgetc(file);
        if (now_char != EOF) {
            printf("%c" , now_char);
        }
        else {
            printf("\n");
            break;
        }
    }
    fclose(file);
}

void removestr(char *command)
{
    command = strtok(NULL , "");
    char *resume = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(resume , command);
    strtok(command , " ");
    if (command == NULL || strcmp(command , "--file") != 0) {
        printf("invalid command\n");
        return;
    }
    int skip;
    FILE *file = find_path(command , "r+", &skip);
    if (file == NULL) {
        printf("invalid command\n");
        return;
    }
    resume += 7 + skip;
    if (resume == NULL || resume[0] != ' ') {
        printf("invalid command\n");
        return;
    }
    resume += 1;
    if (resume == NULL) {
        printf("invalid command\n");
        return;
    }
    char *pos = (char *)calloc(strlen(resume) + 1 ,sizeof(char));
    char *pos2 = (char *)calloc(strlen(resume) + 1 ,sizeof(char));
    strcpy(pos , resume);
    pos2[0] = '\0';
    int line , col;
    strtok(pos , " ");
    if (pos == NULL) {
        printf("invalid command\n");
        return;
    }
    strcat(pos2 , pos);
    pos = strtok(NULL , " ");
    if (pos == NULL) {
        printf("invalid command\n");
        return;
    }
    strcat(pos2 , " ");
    strcat(pos2 , pos);
    bool flag = find_position(pos2 , &line , &col);
    if (!flag) {
        printf("invalid command\n");
        return;
    }
    strcpy(resume , pos);
    resume += strlen(pos2);
    if (resume == NULL || resume[0] != ' ') {
        printf("invalid command\n");
        return;
    }
    resume++;
    if (resume == NULL) {
        printf("invalid command\n");
        return;
    }
    printf("%s\n" , resume);
}

// invalid inputs must check