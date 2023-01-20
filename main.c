#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <windows.h>
#define maximum_size_of_input 1000
bool is_cut = false , is_find = true;
char pathes[maximum_size_of_input];

void main_function(); // this function is the input receiver of the program
void createfile(char *); // this function creates a .txt file with specified path
FILE *open_or_create_file(char * , char* , int); // this function open an existing file or creates a new file by supposed path
void insertstr(char * , char * , char *); // command 2
FILE *find_path(char * , char * , int * , int); // this function find path of a command
bool find_string(char * , char * , int *); // this function find string after --str and saves it ro second argument
bool find_position(char * , int * , int *); // this function find positon after --pos and if it has bad syntax return 0
bool go_to_position(FILE * , int , int , char * ,int *); // this function moves the file pointer to the supposed line and column
void save_text(FILE *, char *); // this function copies from file pointer to EOF in second argument
void back_the_text(FILE * , char * , int , char *); // this function pastes the copied text to the file pointer, third argument is required spaces
void cat(char *); // command 3
void removestr(char * , char *); // command 4
bool find_size(char * , int *); // this function converts a string to an integer
void find_first_position_with_size(FILE * , int , int , int , int); // this function finds the first position (according to -b and -f) and pushes the file pointer to that position
// file pointer , line , col , size , mode (-b or -f)
void save_text_from_first(FILE * , char *); // this function copies some texts that starts from first of file
void append(FILE * , char *); // this function appends a text to the end of file
void copystr(char *); // command 5
void cutstr(char *); // command 6
void pastestr(char *); // command 7
void grep(char *); //command 8
void find_grep(FILE * , char * , int , int , int *); // this function use for printing a line (by another function) that has some text (pattern)
void print_the_line(char * , int , int * , int); // this function use for printing a line that starts after last_enter and push the index to next enter

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
        insertstr(input , "unnormal" , "");
    } else if (strcmp (input , "cat") == 0){
        cat(input);
    } else if (strcmp (input , "removestr") == 0) {
        removestr(input , "nothing");
    } else if (strcmp (input , "copystr") == 0) {
        copystr(input);
    } else if (strcmp (input ,  "cutstr") == 0) {
        cutstr(input);
    } else if (strcmp (input , "pastestr") == 0) {
        pastestr(input);
    } else if (strcmp (input , "grep") == 0) {
        grep(input);
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
    FILE *file = open_or_create_file(path , "w" , -1);
    if (file != NULL) {
        fclose(file);
    }
}

FILE *open_or_create_file(char *path , char *type , int id)
{
    if (path[0] == '\0')
    {
        printf("invalid command\n");
        return NULL;
    }
    if (path[0] == '/') {
        if (strlen(path) < 11) {
            if (id == -1)
                printf("invalid address\n");
            else
                printf("Address of file %d is invalid\n" , id+1);
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
                if (id == -1) {
                    printf("This file doesn't exist\n");
                } else {
                    printf("File %d doesn't exists\n" , id+1);
                }
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
            strcpy(pathes , virual_path);
            return fopen(virual_path , type);
        } else {
            if (id == -1)
                printf("invalid address\n");
            else
                printf("Address of file %d is invalid\n" , id+1);
            return NULL;
        }
    } else if (path[0] == '"' && path[strlen(path)-1] == '"') {
        path++;
        path[strlen(path)-1] = '\0';
        return open_or_create_file(path , type , id);
    } else {
        if (id == -1)
            printf("invalid address\n");
        else
            printf("Address of file %d is invalid\n" , id+1);
        return NULL;
    }
}

void insertstr(char *command , char *mode , char *paste)
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
    FILE *file = find_path(command , "r+" , &skip , -1);
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
    if (strcmp(mode , "unnormal") == 0) {
        char *help = (char *) calloc(maximum_size_of_input, sizeof(char));
        strcpy(help, resume);
        bool flag = find_string(resume, string, &skip);
        if (!flag)
            return;
        strcpy(resume, help);
        resume += (skip + 6);
        if (resume == NULL || resume[1] == '\0') {
            printf("invalid command\n");
            return;
        }
        resume++;
    } else {
        strcpy(string , paste);
    }
    int line = 0 , col = 0;
    bool flag = find_position(resume , &line , &col);
    if (!flag) {
        printf("invalid command\n");
        return;
    }
    int null;
    go_to_position(file , line , col , "insert" , &null);
    go_to_position(file , line , col , "insert" , &null);
    char *copy = (char *)calloc(maximum_size_of_input , sizeof(char));
    save_text(file , copy);
    fseek(file , 0 , SEEK_SET);
    FILE *file2 = file;
    go_to_position(file2 ,line , col , "insert" , &null);
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

FILE *find_path(char *resume, char *type , int *skip , int id)
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
        return open_or_create_file(path , type , id);
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
        return open_or_create_file(path , type ,id);
    }
}

bool find_string(char *resume , char *string , int *skip)
{
    // this function starts with --str
    char *help = (char *) calloc(maximum_size_of_input, sizeof(char));
    strtok(resume, " ");
    if (resume == NULL || strcmp(resume, "--str") != 0) {
        printf("invalid command\n");
        return false;
    }
    resume = strtok(NULL, "");
    strcpy(help, resume);
    if (resume == NULL) {
        printf("invalid command\n");
        return false;
    }
    if (resume[0] != '\"') {
        strtok(resume, " ");
        int j = 0, i = 0;
        for (; i != strlen(resume); i++) {
            if (resume[i] == '\\') {
                if (resume[i + 1] == 'n') {
                    i++;
                    string[j] = '\n';
                    j++;
                } else {
                    string[j] = resume[i + 1];
                    i++;
                    j++;
                }
            } else {
                if (is_find) {
                    if (resume[i] == '*') {
                        string[j] = 254;
                    }
                    else
                        string[j] = resume[i];
                    j++;
                } else {
                    string[j] = resume[i];
                    j++;
                }
            }
        }
        *skip = i;
        strcpy(resume, help);
        return true;
    } else {
        int j = 0, i = 0;
        for (; i != strlen(resume); i++) {
            if (resume[i + 1] == '\\') {
                if (resume[i + 2] == 'n') {
                    string[j] = '\n';
                    j++;
                    i++;
                } else {
                    string[j] = resume[i + 2];
                    i++;
                    j++;
                }
            } else if (resume[i + 1] == '\"') {
                break;
            } else {
                if (is_find) {
                    if (resume[i+1] == '*') {
                        string[j] = 254;
                    } else
                        string[j] = resume[i+1];
                    j++;
                } else {
                    string[j] = resume[i + 1];
                    j++;
                }
            }
        }
        *skip = i + 2;
        strcpy(resume, help);
        return true;
    }
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

bool go_to_position(FILE *file , int line , int col , char *mode , int *moved)
{
    *moved = 0;
    fseek(file , 0 , SEEK_SET);
    int now_line = 1 , now_col = 0;
    char now_char = '\0';
    while (line != 1 && now_char != EOF) {
        now_char = fgetc(file);
        if (now_char == EOF) {
            *moved = -1;
            break;
        }
        (*moved)++;
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
        *moved = -1;
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
                *(moved)++;
                now_col++;
            }
            else
                break;
        }
        if (now_char == EOF)
            *moved = -1;
        if (now_col < col) {
            if (strcmp(mode , "insert") != 0) {
                return false;
            }
            char *copy = (char *) calloc(maximum_size_of_input , sizeof(char));
            save_text(file , copy);
            int null;
            go_to_position(file , line , col , "help" , &null);
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
    FILE *file = find_path(command , "r+" , &skip ,-1);
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

void removestr(char *command , char *mode)
{
    char *path = calloc(maximum_size_of_input , sizeof(char));
    command = strtok(NULL , "");
    strcpy(path , command);
    char *resume = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(resume , command);
    strtok(command , " ");
    if (command == NULL || strcmp(command , "--file") != 0) {
        printf("invalid command\n");
        return;
    }
    int skip;
    FILE *file = find_path(command , "r+", &skip , -1);
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
    strtok(resume , " ");
    resume = strtok(NULL , " ");
    if (resume == NULL || strcmp(resume , "-size") != 0) {
        printf("invalid command\n");
        return;
    }
    resume = strtok(NULL , " ");
    if (resume == NULL) {
        printf("invalid command\n");
        return;
    }
    int size;
    flag = find_size(resume , &size);
    if (!flag) {
        printf("invalid command\n");
        return;
    }
    resume = strtok(NULL , " ");
    if (resume == NULL) {
        printf("invalid command\n");
        return;
    }
    int direction;
    if (strcmp(resume , "-b") == 0)
        direction = -1;
    else if (strcmp(resume , "-f") == 0)
        direction = 1;
    else {
        printf("invalid command\n");
        return;
    }
    find_first_position_with_size(file , line , col , size , direction);
    FILE *file2 = file;
    char *copy1 = (char *)calloc(maximum_size_of_input , sizeof(char));
    save_text_from_first(file2 , copy1);
    char *copy2 = (char *)calloc(maximum_size_of_input , sizeof(char));
    char clipboard[size+1];
    int i;
    for (i = 0; i != size; i++) {
        clipboard[i] = fgetc(file2);
    }
    clipboard[i] = '\0';
    if (strcmp(mode , "cut") == 0) {
        const size_t len = strlen(clipboard) + 1;
        HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
        memcpy(GlobalLock(hMem), clipboard, len);
        GlobalUnlock(hMem);
        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
        is_cut = true;
    }
    save_text(file2 , copy2);
    fclose(file2);
    fclose(file);
    strtok(path , " ");
    int null;
    FILE *file3 = find_path(path , "w+" , &null , -1);
    append(file3 , copy1);
    append(file3 , copy2);
    fclose(file3);
}

bool find_size(char *size , int *result){
    int res = 0;
    for (int i = 0; i != strlen(size); i++) {
        if (size[i] < '0' || size[i] > '9') {
            return false;
        }
        res *= 10;
        res += (size[i] - '0');
    }
    *result = res;
    return true;
}

void find_first_position_with_size(FILE *file , int line , int col , int size , int type) {
    int moved = 0;
    if (type == 1) {
        go_to_position(file , line , col , "remove" , &moved);
    } else {
        go_to_position(file , line , col , "remove" , &moved);
        moved = ftell(file);
        fseek(file , 0 , SEEK_SET);
        int chars = 0;
        char check;
        while (ftell(file) <= moved) {
            check = fgetc(file);
            chars++;
            if (check == EOF)
                fputc('\0' , file);
        }
        fseek(file, 0, SEEK_SET);
        for (int i = 0; i != chars - size - 1; i++)
            fgetc(file);
    }
}

void save_text_from_first(FILE *file , char *copy) {
    int now = ftell(file) , i = 0;
    fseek(file , 0 , SEEK_SET);
    char now_char;
    while (ftell(file) < now) {
        now_char = fgetc(file);
        copy[i] = now_char;
        i++;
    }
    copy[i] = '\0';
}

void append(FILE *file , char *text){
    for (int i = 0; i != strlen(text); i++)
        fputc(text[i] , file);
}

void copystr(char *command) {
    command = strtok(NULL , "");
    char *resume = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(resume , command);
    strtok(command , " ");
    if (command == NULL || strcmp(command , "--file") != 0) {
        printf("invalid command\n");
        return;
    }
    int skip;
    FILE *file = find_path(command , "r+", &skip , -1);
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
    strtok(resume , " ");
    resume = strtok(NULL , " ");
    if (resume == NULL || strcmp(resume , "-size") != 0) {
        printf("invalid command\n");
        return;
    }
    resume = strtok(NULL , " ");
    if (resume == NULL) {
        printf("invalid command\n");
        return;
    }
    int size;
    flag = find_size(resume , &size);
    if (!flag) {
        printf("invalid command\n");
        return;
    }
    resume = strtok(NULL , " ");
    if (resume == NULL) {
        printf("invalid command\n");
        return;
    }
    int direction;
    if (strcmp(resume , "-b") == 0)
        direction = -1;
    else if (strcmp(resume , "-f") == 0)
        direction = 1;
    else {
        printf("invalid command\n");
        return;
    }
    find_first_position_with_size(file , line , col , size , direction);
    FILE *file2 = file;
    char *copy = (char *)calloc(maximum_size_of_input , sizeof(char));
    int i;
    for (i = 0; i != size; i++)
        copy[i] = fgetc(file2);
    copy[i] = '\0';
    const size_t len = strlen(copy) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), copy, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    fclose(file);
    fclose(file2);
    is_cut = false;
}

void cutstr(char *command) {
    removestr(command , "cut");
    // har chi talash kardam ke tarkibi az copy va remove beshe, nashod :(
}

void pastestr(char *command) {
    HANDLE h;
    OpenClipboard(NULL);
    h = GetClipboardData(CF_TEXT);
    char *paste = (char *)h;
    CloseClipboard();
    insertstr(command , "normal" , paste);
    if (is_cut == true) {
        OpenClipboard(NULL);
        EmptyClipboard();
        CloseClipboard();
        is_cut = false;
    }
    return;
}

void grep(char *command) {
    int mode = 0 , lines = 0;
    // mode == 0 normal -- mode == 1 -- -c -- mode == 2 -- -l
    command = strtok(NULL , "");
    char *newcommand = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(newcommand , "\0");
    strcpy(newcommand , command);
    char *pattern = (char *) calloc(maximum_size_of_input , sizeof(char));
    char *help = (char *) calloc(maximum_size_of_input , sizeof(char));
    int skip;
    strcpy(pattern , "\0");
    strcpy(help , "\0");
    strcpy(help , newcommand);
    strtok(help , " ");
    if (strcmp(help , "--str") != 0) {
        if (strcmp(help , "-c") == 0) {
            mode = 1;
        } else if (strcmp(help , "-l") == 0){
            mode = 2;
        }
        strtok(newcommand , " ");
        newcommand = strtok(NULL , "");
    }
    find_string(newcommand, pattern , &skip);
    command += (7 + skip);
    if (mode != 0)
        command += 3;
    strcpy(newcommand , command);
    int i = 0;
    while (newcommand != NULL) {
        strtok(newcommand , " ");
        if (newcommand == NULL)
            break;
        FILE *file = find_path(newcommand , "r+" , &skip , i);
        i++;
        int before = lines;
        find_grep(file , pattern , i , mode , &lines);
        if (before != lines && mode == 2)
            printf("%s\n" , pathes);
        fclose(file);
        file = NULL;
        command += 8+skip;
        if (command == NULL || command[0] == '\0')
            break;
        command++;
        strcpy(help , "--files ");
        strcat(help , command);
        strcpy(command , help);
        strcpy(newcommand , command);
    }
    if (mode == 1)
        printf("%d\n" , lines);
}

void find_grep(FILE *file , char *pattern , int id , int mode , int *lines) {
    char *text = (char *) calloc(1000000 , sizeof(char));
    fseek(file , 0 , SEEK_END);
    save_text_from_first(file , text);
    int last_enter = -1 , j = 0;
    for (int i = 0; i < strlen(text); i++) {
        if (j == strlen(pattern)) {
            if (mode == 0) {
                printf("%s: ", pathes);
                print_the_line(text, last_enter, &i , mode);
            } if (mode == 1 || mode == 2)
                print_the_line(text, last_enter, &i , mode);
            (*lines)++;
            j = 0;
            // i go to eof or \n
            // and print the line after the last_enter
        }
        if (text[i] == '\n')
            last_enter = i;
        if (text[i] == pattern[j])
            j++;
        else {
            i -= j;
            j = 0;
        }
    }
}

void print_the_line(char *text , int last_enter , int *ind , int mode) {
    int i = last_enter;
    if (last_enter == -1)
        i = 0;
    while (text[i] == '\n' || text[i] == '\0')
        i++;
    while (text[i] != '\n' && text[i] != '\0') {
        if (mode == 0)
            printf("%c" , text[i]);
        i++;
    }
    if (mode == 0)
        printf("\n");
    *ind = i;
}


// invalid inputs must check
// eof error in removestr
// checking pos for all :(
// .txt is not important :(
// backslash gheir mortabet
// enter and eof is ignored in find
