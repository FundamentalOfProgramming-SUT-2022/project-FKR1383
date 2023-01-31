#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <windows.h>
#include <dirent.h>
#define maximum_size_of_input 1000000
bool is_cut = false , is_find = false , is_replace = false , is_arman = false , is_arman2 = false;
char pathes[maximum_size_of_input] , pathes2[maximum_size_of_input] , pathes3[maximum_size_of_input];
unsigned char arman_string[maximum_size_of_input];

void main_function(); // this function is the input receiver of the program
void createfile(char *); // this function creates a .txt file with specified path
FILE *open_or_create_file(char * , char* , int); // this function open an existing file or creates a new file by supposed path
void insertstr(char * , char * , unsigned char *); // command 2
FILE *find_path(char * , char * , int * , int); // this function find path of a command
bool find_string(char * , char * , int *); // this function find string after --str and saves it ro second argument
bool find_position(char * , int * , int *); // this function find positon after --pos and if it has bad syntax return 0
bool go_to_position(FILE * , int , int , char * ,int *); // this function moves the file pointer to the supposed line and column
void save_text(FILE *, char *); // this function copies from file pointer to EOF in second argument
void back_the_text(FILE * , char * , int , char *); // this function pastes the copied text to the file pointer, third argument is required spaces
void cat(char *); // command 3
void removestr(char * , char *); // command 4
bool find_size(char * , int *); // this function converts a string to an integer
bool find_first_position_with_size(FILE * , int , int , int , int); // this function finds the first position (according to -b and -f) and pushes the file pointer to that position
// file pointer , line , col , size , mode (-b or -f)
void save_text_from_first(FILE * , char *); // this function copies some texts that starts from first of file
void append(FILE * , char *); // this function appends a text to the end of file
void copystr(char *); // command 5
void cutstr(char *); // command 6
void pastestr(char *); // command 7
void grep(char *); //command 10
void find_grep(FILE * , char * , int , int , int *); // this function use for printing a line (by another function) that has some text (pattern)
void print_the_line(char * , int , int * , int); // this function use for printing a line that starts after last_enter and push the index to next enter
void create_backup_file(); // this function creates a backupfile after creating file
void create_dot_file(); // this function creates ..txt and update it with last file (before action)
void update_backup_file(); // this function update backup file after an action and removes ..txt file
void undo(char *); // command 11
void find(char * , char *); // command 8 -- second argument is for replace
int word_finding(char * ,int); // this command found that the n-th character of file is for result-th word of the text
void replace(char *); // command 9
int replacing_the_word(char * ,int , int , char *); // this function replaces the word on the supposed position with removing the current phrase
void auto_indent(char *); // command 12
void shifting(char * , int , int); // this function shifting the text from one position for some cells
void left_shifting(char * , int , int); // this function shifting the text to left from one position for some cells
void text_comparator(char *); // command 13
void tree (char *); // command 14
void show_tree(char * , int , int , bool * , int); // this function shows directories and files to some depth
void arman(char *); // command 15

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
    // checking is the first word of command :
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
    } else if (strcmp (input , "undo") == 0) {
        undo(input);
    } else if (strcmp (input , "find") == 0) {
        find(input , NULL);
    } else if (strcmp (input , "replace") == 0) {
        replace(input);
    } else if (strcmp (input , "auto-indent") == 0) {
        auto_indent(input);
    } else if (strcmp (input , "compare") == 0) {
        text_comparator(input);
    } else if (strcmp (input , "tree") == 0) {
        tree(input);
    } else {
        printf("invalid command\n");
    }
    free(input);
}

void createfile(char *command) {
    command = strtok(NULL, " ");
    char *path = (char *) calloc(maximum_size_of_input, sizeof(char));
    path[0] = '\0';
    command = strtok(NULL , "");
    strcpy(path , command);
    FILE *file = open_or_create_file(path , "w" , -1);
    if (file != NULL) {
        fclose(file);
        create_backup_file();
    }
}

FILE *open_or_create_file(char *path , char *type , int id)
{
    if (path[0] == '/') {
        if (strlen(path) < 7) {
            if (id == -1)
                printf("invalid address\n");
            else
                printf("Address of file %d is invalid\n" , id+1);
            return NULL;
        }
        if (path[1] == 'r' && path[2] == 'o' && path[3] == 'o' && path[4] == 't' && path[5] == '/') {
            char *virual_path = (char *)calloc(strlen(path) , sizeof(char));
            virual_path[0] = '\0';
            path++;
            bool flag = false;
            if (strcmp(type , "w") == 0 && access(path, F_OK) == 0) {
                printf("This file exists\n");
                return NULL;
            } else if (strcmp(type , "r+") == 0 && access(path, F_OK) == -1) {
                flag = true;
                char *checker = (char *)calloc(maximum_size_of_input , sizeof(char));
                strcpy(checker , path);
                char *virtual_checker = (char *)calloc(maximum_size_of_input , sizeof(char));
                virtual_checker[0] = '\0';
                strtok(checker , "/");
                while (checker != NULL)
                {
                    strcat(virtual_checker , checker);
                    checker = strtok(NULL , "/");
                    if (checker != NULL) {
                        DIR* dir = opendir(virtual_checker);
                        if (ENOENT == errno) {
                            flag = false;
                        }
                        strcat(virtual_checker, "/");
                    }
                }
                if (id == -1) {
                    if (flag)
                        printf("This file doesn't exist\n");
                    else
                        printf("This directory doesn't exist\n");
                } else {
                    if (flag)
                        printf("File %d doesn't exist\n" , id+1);
                    else
                        printf("Directory of file %d doesn't exist\n" , id+1);
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

void insertstr(char *command , char *mode , unsigned char *paste)
{
    command = strtok(NULL , "");
    char *resume = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(resume , command);
    strtok(command , " ");
    int skip;
    FILE *file = find_path(command , "r+" , &skip , -1);
    if (file == NULL) {
        return;
    }
    fclose(file);
    create_dot_file();
    file = fopen(pathes , "r+");
    resume += (8 + skip);
    unsigned char *string = (unsigned char *)calloc(maximum_size_of_input , sizeof(char));
    if (strcmp(mode , "unnormal") == 0) {
        char *help = (char *) calloc(maximum_size_of_input, sizeof(char));
        strcpy(help, resume);
        bool flag = find_string(resume, string, &skip);
        strcpy(resume, help);
        resume += (skip + 7);
    } else {
        strcpy(string , paste);
    }
    int line = 0 , col = 0;
    bool flag = find_position(resume , &line , &col);
    if (!flag) {
        printf("invalid position\n");
        fclose(file);
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
    update_backup_file();
}

FILE *find_path(char *resume, char *type , int *skip , int id)
{
    // this function starts with --file
    resume = strtok(NULL , "");
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
    resume = strtok(NULL, "");
    strcpy(help, resume);
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
                        string[j] = (char)127;
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
                        string[j] = (char)127;
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
    if ((*line) == 0)
        return false;
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
        if (strcmp(mode , "insert") != 0) {
            return false;
        }
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
    return true;
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
    command = strtok(NULL , "");
    char *save = (char *)calloc(maximum_size_of_input , sizeof(char));
    strcpy(save , command);
    strtok(command , " ");
    int skip;
    FILE *file = find_path(command , "r+" , &skip ,-1);
    save += (7 + skip);
    if (save[0] != '\0') {
        save += 4;
        is_arman = true;
    }
    if (file == NULL) {
        return;
    }
    char now_char = '\0';
    while (now_char != EOF) {
        now_char = fgetc(file);
        if (now_char != EOF) {
            if (is_arman) {
                char a[2] = {now_char , '\0'};
                strcat(arman_string , a);
            } else
                printf("%c" , now_char);
        }
        else {
            if (!is_arman)
                printf("\n");
            break;
        }
    }
    fclose(file);
    if (is_arman) {
        is_arman = false;
        arman(save);
    }
}

void removestr(char *command , char *mode) {
    char *path = calloc(maximum_size_of_input , sizeof(char));
    command = strtok(NULL , "");
    strcpy(path , command);
    char *resume = (char *) calloc(maximum_size_of_input , sizeof(char));
    strcpy(resume , command);
    strtok(command , " ");
    int skip;
    FILE *file = find_path(command , "r+", &skip , -1);
    if (file == NULL) {
        return;
    }
    fclose(file);
    create_dot_file();
    file = fopen(pathes , "r+");
    resume += 8 + skip;
    char *pos = (char *)calloc(strlen(resume) + 1 ,sizeof(char));
    char *pos2 = (char *)calloc(strlen(resume) + 1 ,sizeof(char));
    strcpy(pos , resume);
    pos2[0] = '\0';
    int line , col;
    strtok(pos , " ");
    strcat(pos2 , pos);
    pos = strtok(NULL , " ");
    strcat(pos2 , " ");
    strcat(pos2 , pos);
    bool flag = find_position(pos2 , &line , &col);
    if (!flag) {
        printf("invalid command\n");
        fclose(file);
        return;
    }
    strcpy(resume , pos);
    resume += strlen(pos2);
    resume++;
    strtok(resume , " ");
    resume = strtok(NULL , " ");
    resume = strtok(NULL , " ");
    int size;
    flag = find_size(resume , &size);
    if (!flag) {
        printf("invalid size\n");
        fclose(file);
        return;
    }
    resume = strtok(NULL , " ");
    int direction;
    if (strcmp(resume , "-b") == 0)
        direction = -1;
    else if (strcmp(resume , "-f") == 0)
        direction = 1;
    else {
        printf("invalid command\n");
        fclose(file);
        return;
    }
    if (!find_first_position_with_size(file , line , col , size , direction)) {
        printf("invalid position!\n");
        fclose(file);
        return;
    }
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
    update_backup_file();
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

bool find_first_position_with_size(FILE *file , int line , int col , int size , int type) {
    int moved = 0;
    bool flag;
    if (type == 1) {
        flag = go_to_position(file , line , col , "remove" , &moved);
        if (!flag) {
            return false;
        }
        int i;
        for (i = 0; i != size; i++) {
            if (fgetc(file) == EOF)
                return false;
        }
        fseek(file , 0 , SEEK_SET);
        go_to_position(file , line , col , "remove" , &moved);
    } else {
        flag = go_to_position(file , line , col , "remove" , &moved);
        if (!flag)
            return false;
        moved = ftell(file);
        fseek(file , 0 , SEEK_SET);
        int chars = 0;
        char check;
        while (ftell(file) < moved) {
            check = fgetc(file);
            chars++;
            if (check == EOF)
                fputc('\0' , file);
        }
        fseek(file, 0, SEEK_SET);
        if (chars - size < 0)
            return false;
        for (int i = 0; i < chars - size; i++)
            fgetc(file);
    }
    return true;
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
    int skip;
    FILE *file = find_path(command , "r+", &skip , -1);
    if (file == NULL) {
        return;
    }
    fclose(file);
    create_dot_file();
    file = fopen(pathes , "r+");
    resume += 7 + skip;
    resume += 1;
    char *pos = (char *)calloc(strlen(resume) + 1 ,sizeof(char));
    char *pos2 = (char *)calloc(strlen(resume) + 1 ,sizeof(char));
    strcpy(pos , resume);
    pos2[0] = '\0';
    int line , col;
    strtok(pos , " ");
    strcat(pos2 , pos);
    pos = strtok(NULL , " ");
    strcat(pos2 , " ");
    strcat(pos2 , pos);
    bool flag = find_position(pos2 , &line , &col);
    if (!flag) {
        printf("invalid position\n");
        fclose(file);
        return;
    }
    strcpy(resume , pos);
    resume += strlen(pos2);
    resume++;
    strtok(resume , " ");
    resume = strtok(NULL , " ");
    resume = strtok(NULL , " ");
    int size;
    flag = find_size(resume , &size);
    if (!flag) {
        printf("invalid size\n");
        fclose(file);
        return;
    }
    resume = strtok(NULL , " ");
    int direction;
    if (strcmp(resume , "-b") == 0)
        direction = -1;
    else if (strcmp(resume , "-f") == 0)
        direction = 1;
    else {
        printf("invalid command\n");
        fclose(file);
        return;
    }
    flag = find_first_position_with_size(file , line , col , size , direction);
    if (!flag) {
        fclose(file);
        printf("invalid position!\n");
        return;
    }
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
    update_backup_file();
}

void cutstr(char *command) {
    removestr(command , "cut");
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
    char *save = (char *)calloc(maximum_size_of_input , sizeof(char));
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
    if (!is_arman2) {
        find_string(newcommand, pattern, &skip);
        command += (7 + skip);
        strcpy(arman_string , "\0");
    } else {
        strcpy(pattern , arman_string);
        is_arman2 = false;
    }
    if (mode != 0)
        command += 3;
    strcpy(newcommand , command);
    int i = 0;
    while (newcommand != NULL) {
        strtok(newcommand , " ");
        if (newcommand == NULL)
            break;
        FILE *file = find_path(newcommand , "r+" , &skip , i);
        if (file == NULL) {
            i++;
            command += 8+skip;
            if (command == NULL || command[0] == '\0')
                break;
            command++;
            if (command[0] == '=')
                break;
            strcpy(help , "--files ");
            strcat(help , command);
            strcpy(command , help);
            strcpy(newcommand , command);
            continue;
        }
        i++;
        int before = lines;
        find_grep(file , pattern , i , mode , &lines);
        if (before != lines && mode == 2) {
            strcat(arman_string , pathes);
            strcat(arman_string , "\n");
        }
        fclose(file);
        file = NULL;
        command += 8+skip;
        if (command == NULL || command[0] == '\0')
            break;
        command++;
        if (command[0] == '=')
            break;
        strcpy(help , "--files ");
        strcat(help , command);
        strcpy(command , help);
        strcpy(newcommand , command);
    }
    if (mode == 1) {
        char num[100];
        sprintf(num , "%d" , lines);
        strcat(arman_string , num);
        strcat(arman_string , "\n");
    }
    if (command[0] == '=') {
        command += 3;
        is_arman = true;
        strcpy(save , "\0");
        strcat(save , command);

    }
    if (!is_arman)
        printf("%s", arman_string);
    else {
        arman_string[strlen(arman_string)-2] = '\0';
        arman(save);
    }
    is_arman = false;
}

void find_grep(FILE *file , char *pattern , int id , int mode , int *lines) {
    char *text = (char *) calloc(1000000 , sizeof(char));
    fseek(file , 0 , SEEK_END);
    save_text_from_first(file , text);
    int last_enter = -1 , j = 0;
    for (int i = 0; i <= strlen(text); i++) {
        if (j == strlen(pattern)) {
            if (mode == 0) {
                strcat(arman_string , pathes);
                strcat(arman_string , ": ");
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
        if (mode == 0) {
            char x[2] = {text[i] , '\0'};
            strcat(arman_string , x);
        }
        i++;
    }
    if (mode == 0) {
        strcat(arman_string , "\n");
    }
    *ind = i;
}

void create_backup_file() {
    char *backup_address = (char *)calloc(maximum_size_of_input , sizeof(char));
    int last , i;
    for (last = strlen(pathes)-1; last >= 0; last--) {
        if (pathes[last] == '/')
            break;
    }
    for (i = 0; i <= last; i++) {
        backup_address[i] = pathes[i];
    }
    backup_address[i] = '$';
    while (i != strlen(pathes)) {
        backup_address[i+1] = pathes[i];
        i++;
    }
    backup_address[i+1] = '\0';
    FILE *file = fopen(backup_address , "w");
    fclose(file);
    int attr = GetFileAttributes(backup_address);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(backup_address, attr | FILE_ATTRIBUTE_HIDDEN);
    }
    file = fopen(backup_address , "r+");
    fclose(file);
}

void create_dot_file() {
    strcpy(pathes2 , pathes);
    char *dot_file = (char *)calloc(maximum_size_of_input , sizeof(char));
    int last , i;
    for (last = strlen(pathes)-1; last >= 0; last--) {
        if (pathes[last] == '/')
            break;
    }
    for (i = 0; i <= last; i++)
        dot_file[i] = pathes[i];
    dot_file[i] = '\0';
    strcat(dot_file , "..txt");
    strcpy(pathes3 , dot_file);
    FILE *file = fopen(dot_file , "w");
    fclose(file);
    int attr = GetFileAttributes(dot_file);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(dot_file, attr | FILE_ATTRIBUTE_HIDDEN);
    }
    file = fopen(pathes , "r+");
    char *all_text = (char *)calloc(maximum_size_of_input , sizeof(char));
    fseek(file , 0 , SEEK_END);
    save_text_from_first(file , all_text);
    fclose(file);
    file = NULL;
    file = fopen(dot_file , "r+");
    for (int i = 0; i != strlen(all_text); i++)
        fputc(all_text[i] , file);
    fseek(file , 0 , SEEK_SET);
    fclose(file);
}

void update_backup_file() {
    char *dollar_file = (char *)calloc(maximum_size_of_input , sizeof(char));
    char *dot_file = (char *)calloc(maximum_size_of_input , sizeof(char));
    strcpy(dot_file , pathes3);
    int last , i;
    for (last = strlen(pathes2)-1; last >= 0; last--) {
        if (pathes2[last] == '/')
            break;
    }
    for (i = 0; i <= last; i++) {
        dollar_file[i] = pathes2[i];
    }
    dollar_file[i] = '$';
    while (i != strlen(pathes2)) {
        dollar_file[i+1] = pathes2[i];
        i++;
    }
    dollar_file[i+1] = '\0';
    char *all_dot = (char *)calloc(maximum_size_of_input , sizeof(char));
    FILE *file = fopen(dot_file , "r+");
    fseek(file , 0 , SEEK_END);
    save_text_from_first(file , all_dot);
    fclose(file);
    FILE *file2 = fopen(dollar_file , "w");
    fclose(file2);
    int attr = GetFileAttributes(dollar_file);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(dollar_file, attr | FILE_ATTRIBUTE_HIDDEN);
    }
    file2 = fopen(dollar_file , "r+");
    for (i = 0; i != strlen(all_dot); i++)
        fputc(all_dot[i] ,file2);
    fclose(file2);
    remove(dot_file);
}

void undo(char *command) {
    command = strtok(NULL , " ");
    int null , null2;
    FILE *file = find_path(command , "r+" , &null , null2);
    fseek(file , 0 , SEEK_END);
    char *all_text = (char *)calloc(maximum_size_of_input , sizeof(char));
    all_text[0] = '\0';
    save_text_from_first(file , all_text);
    char *all_text_dollar = (char *)calloc(maximum_size_of_input , sizeof(char));
    char *dollar_file = (char *)calloc(maximum_size_of_input , sizeof(char));
    all_text_dollar[0] = '\0';
    int last , i;
    for (last = strlen(pathes); last >= 0; last--) {
        if (pathes[last] == '/')
            break;
    }
    for (i = 0; i <= last; i++)
        dollar_file[i] = pathes[i];
    dollar_file[i] = '$';
    while (i != strlen(pathes)) {
        dollar_file[i+1] = pathes[i];
        i++;
    }
    dollar_file[i+1] = '\0';
    fclose(file);
    FILE *file1 = fopen(dollar_file , "r+");
    fseek(file1 , 0 , SEEK_END);
    save_text_from_first(file1 , all_text_dollar);
    fclose(file1);
    file = fopen(pathes , "w");
    for (int i = 0; i != strlen(all_text_dollar); i++)
        fputc(all_text_dollar[i] , file);
    fclose(file);
    FILE *file2 = fopen(dollar_file , "w");
    fclose(file2);
    int attr = GetFileAttributes(dollar_file);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(dollar_file, attr | FILE_ATTRIBUTE_HIDDEN);
    }
    file2 = fopen(dollar_file , "r+");
    for (i = 0; i != strlen(all_text); i++)
        fputc(all_text[i] ,file2);
    fclose(file2);
}

void find(char *command , char *replace) {
    bool options[4] = {false};
    int at;
    // 0 is count -- 1 is at -- 2 is byword -- 3 is all
    char *newcommand = (char *) calloc(maximum_size_of_input , sizeof(char));
    char *newcommand2 = (char *) calloc(maximum_size_of_input , sizeof(char));
    char *save = (char *)calloc(maximum_size_of_input , sizeof(char));
    command = strtok(NULL , "");
    strcpy(newcommand , command);
    strcpy(newcommand2 , command);
    char *string = (char *)calloc(maximum_size_of_input , sizeof(char));
    char *all_text = (char *)calloc(maximum_size_of_input , sizeof(char));
    int skip;
    if (!is_arman2) {
        is_find = true;
        find_string(command, string, &skip);
        is_find = false;
        newcommand += (7 + skip);
        newcommand2 += (7 + skip);
    } else {
        strcpy(string , arman_string);
        is_arman2 = false;
    }
    strtok(newcommand , " ");
    int null;
    FILE *file = find_path(newcommand , "r+" , &skip , null);
    newcommand2 += (7 + skip);
    strcpy(command , newcommand2);
    if (command[0] != '\0') {
        command++;
        strtok(command , " ");
        while (command != NULL && command[0] != '\0' && command[0] != '=') {
            if (strcmp(command, "-count") == 0)
                options[0] = true;
            if (strcmp(command , "-at") == 0) {
                options[1] = true;
                command = strtok(NULL , " ");
                at = atoi(command);
            }
            if (strcmp(command , "-byword") == 0)
                options[2] = true;
            if (strcmp(command , "-all") == 0)
                options[3] = true;
            if (command == NULL || command[0] == '\0' || command[0] == '=')
                break;
            command = strtok(NULL , " ");
            if (command == NULL || command[0] == '\0' || command[0] == '=')
                break;
        }
        if (command != NULL && command[0] != '\0' && command[0] == '=') {
            command = strtok(NULL , "");
            strcpy(save , "\0");
            strcpy(save , command);
            is_arman = true;
        } else {
            is_arman = false;
        }
    }
    int *first_pos = (int *) calloc(maximum_size_of_input , sizeof(int));
    int *second_pos = (int *) calloc(maximum_size_of_input , sizeof(int));
    bool successful_replace = false;
    int counter = 0;
    save_text(file , all_text);
    int j = 0, start = 0;
    for (int i = 0; i <= strlen(all_text); i++) {
        if (j == strlen(string)) {
            if (string[0] == 127) {
                int l = start;
                while (l >= 0 && all_text[l] != ' ' && all_text[l] != ' ' && all_text[l] != '\n')
                    l--;
                *(first_pos + counter) = l+1;
                *(second_pos + counter) = i;
                int end = start;
                while (all_text[end] != '\n' && all_text[end] != '\0' && all_text[end] != ' ')
                    end++;
                i = end + 1;
                j = 0;
            } else if (string[strlen(string) - 1] == 127) {
                *(first_pos + counter) = start;
                int end = start;
                while (all_text[end] != '\n' && all_text[end] != '\0' && all_text[end] != ' ')
                    end++;
                i = end + 1;
                *(second_pos + counter) = end;
                j = 0;
            } else {
                *(first_pos + counter) = start;
                *(second_pos + counter) = i;
                j = 0;
                i = start + 1;
            }
            if (is_replace) {
                if (options[1]) {
                    if (at-1 == counter) {
                        successful_replace = true;
                        i = replacing_the_word(all_text , *(first_pos + counter) , *(second_pos + counter)-1 , replace);
                    }
                } else {
                    successful_replace = true;
                    i = replacing_the_word(all_text , *(first_pos + counter) , *(second_pos + counter)-1 , replace);
                }
            }
            counter++;
        }
        if (i >= strlen(all_text)) {
            if (string[j] == 127 && j == strlen(string) - 1) {
                *(first_pos + counter) = start;
                *(second_pos + counter) = i;
                if (is_replace) {
                    if (options[1]) {
                        if (at-1 == counter) {
                            successful_replace = true;
                            i = replacing_the_word(all_text , *(first_pos + counter) , *(second_pos + counter)-1 , replace);
                        }
                    } else {
                        successful_replace = true;
                        i = replacing_the_word(all_text , *(first_pos + counter) , *(second_pos + counter)-1 , replace);
                    }
                }
                counter++;
            }
            break;
        }
        if (j == 0)
            start = i;
        if (string[j] == 127) {
            if (j == 0 || string[j - 1] == ' ') {
                j++;
                while (string[j] != ' ' && string[j] != '\0')
                    j++;
                j--;
                while (all_text[i] != ' ' && all_text[i] != '\n' && all_text[i] != '\0') {
                    i++;
                }
                i--;
                bool flag = false;
                int started_j = j , started_i = i;
                while (i != -1 && all_text[i] != ' ' && all_text[i] != '\n' && all_text[i] != '\0') {
                    if (all_text[i] == string[j]) {
                        if (!flag)
                            started_i = i;
                        flag = true;
                        j--;
                    } else if (string[j] == 127) {
                        break;
                    } else if (flag) {
                        flag = false;
                        j = started_j;
                        i = started_i;
                    }
                    i--;
                }
                while (i != 0 && all_text[i] != ' ' && all_text[i] != '\n' && all_text[i] != '\0')
                    i--;
                if (string[j] == 127) {
                    i++;
                    while (all_text[i] != ' ' && all_text[i] != '\n' && all_text[i] != '\0')
                        i++;
                    i--;
                    while (string[j] != ' ' && string[j] != '\0')
                        j++;
                }
                if (string[j] != ' ' && string[j] != '\0') {
                    i = start;
                    j = 0;
                }
            } else {
                j++;
                while (all_text[i] != ' ' && all_text[i] != '\n' && all_text[i] != '\0') {
                    i++;
                }
                i--;
            }
        } else {
            if (all_text[i] == string[j])
                j++;
            else {
                i = start;
                j = 0;
            }
        }
    }
    if (!is_replace) {
        if (options[0]) {
            if (options[1] || options[2] || options[3]) {
                if (is_arman)
                    strcat(arman_string , "These options cannot come together!");
                else
                    printf("These options cannot come together!\n");
            }
            else {
                if (is_arman) {
                    char num[100];
                    sprintf(num, "%d", counter);
                    strcat(arman_string, num);
                } else
                    printf("%d\n", counter);
            }
        } else {
            if (options[1]) {
                if (options[3]) {
                    if (is_arman)
                        strcat(arman_string , "These options cannot come together!");
                    else
                        printf("These options cannot come together!\n");
                }
                else {
                    if (options[2]) {
                        if (at > counter) {
                            if (is_arman)
                                strcat(arman_string , "-1");
                            else
                                printf("-1\n");
                        }
                        else {
                            int x = word_finding(all_text, first_pos[at - 1]);
                            if (is_arman) {
                                char num[100];
                                sprintf(num , "%d" , x);
                                strcat(arman_string , num);
                            } else {
                                printf("%d\n", word_finding(all_text, first_pos[at - 1]));
                            }
                        }
                    } else {
                        if (at > counter) {
                            if (is_arman)
                                strcat(arman_string , "-1");
                            else
                                printf("-1\n");
                        }
                        else {
                            if (is_arman) {
                                char num[100];
                                sprintf(num , "%d" , first_pos[at - 1]);
                                strcat(arman_string , num);
                            } else
                                printf("%d\n", first_pos[at - 1]);
                        }
                    }
                }
            } else {
                if (options[2]) {
                    if (options[3]) {
                        if (counter == 0) {
                            if (is_arman)
                                strcat(arman_string , "-1");
                            else
                                printf("-1\n");
                        }
                        else
                            for (int i = 0; i != counter; i++) {
                                int x = word_finding(all_text, first_pos[i]);
                                char num[100];
                                sprintf(num, "%d" , x);
                                if (is_arman) {
                                    strcat(arman_string , num);
                                    strcat(arman_string , " ");
                                } else {
                                    printf("%d ", x);
                                }
                            }
                        if(!is_arman)
                            printf("\n");
                    } else {
                        if (counter == 0) {
                            if (is_arman)
                                strcat(arman_string , "-1");
                            else
                                printf("-1\n");
                        }
                        else {
                            int x = word_finding(all_text , first_pos[0]);
                            if (is_arman) {
                                char num[100];
                                sprintf(num , "%d" , x);
                                strcat(arman_string , num);
                            } else {
                                printf("%d\n", word_finding(all_text, first_pos[0]));
                            }
                        }
                    }
                } else {
                    if (options[3]) {
                        if (counter == 0) {
                            if (is_arman)
                                strcat(arman_string , "-1");
                            else
                                printf("-1\n");
                        }
                        else
                            for (int i = 0; i != counter; i++) {
                                char num[100];
                                sprintf(num , "%d" , first_pos);
                                if (is_arman) {
                                    strcat(arman_string , num);
                                    strcat(arman_string , " ");
                                } else
                                    printf("%d ", first_pos[i]);
                            }
                        if (!is_arman)
                            printf("\n");
                    } else {
                        if (1 > counter) {
                            if (is_arman)
                                strcat(arman_string , "-1");
                            else
                                printf("-1\n");
                        }
                        else {
                            if (is_arman) {
                                char num[100];
                                sprintf(num , "%d" , first_pos[0]);
                                strcat(arman_string , num);
                            } else
                                printf("%d\n", first_pos[0]);
                        }
                    }
                }
            }
        }
    } else {
        if (successful_replace)
            printf("replace successfully done!\n");
        else
            printf("replace failed\n");
    }
    fclose(file);
    if (successful_replace) {
        file = fopen(pathes, "w");
        for (int i = 0; i != strlen(all_text); i++) {
            fputc(all_text[i],file);
        }
        fclose(file);
        update_backup_file();
    }
    if(is_arman) {
        is_arman = false;
        arman(save);
    }
}



int word_finding(char *text , int noc) {
    // noc stands for number of character
    int j = 0;
    for (int i = 0; i != strlen(text); i++) {
        if (i == noc)
            break;
        if (text[i] == ' ' || text[i] == '\n')
            j++;
    }
    return j+1;
}

void replace(char *command) {
    char *newcommand = (char *)calloc(maximum_size_of_input , sizeof(char));
    char *str1 = (char *)calloc(maximum_size_of_input , sizeof(char));
    char *str2 = (char *)calloc(maximum_size_of_input , sizeof(char));
    int skip;
    command = strtok(NULL , "");
    if (!is_arman2) {
        strcpy(newcommand, command);
        find_string(newcommand, str1, &skip);
        command += (7 + skip);
    } else {
        is_arman2 = false;
        strcpy(str1 , arman_string);
    }
    strcpy(newcommand , command);
    find_string(newcommand , str2 , &skip);
    command += (7 + skip);
    strcpy(newcommand , command);
    strtok(command , " ");
    FILE *file = find_path(command,  "r+" , &skip , 0);
    if (file == NULL)
        return;
    fclose(file);
    create_dot_file();
    bool options[2] = {false , false};
    int at;
    newcommand += (8 + skip);
    char *newcommand2 = (char *)calloc(maximum_size_of_input , sizeof(char));
    strcpy(newcommand2 , newcommand);
    strtok(newcommand2 , " ");
    while (newcommand2 != NULL) {
        if (strcmp(newcommand2 , "-at") == 0) {
            options[0] = true;
            newcommand2 = strtok(NULL , " ");
            at = atoi(newcommand2);
        }
        if (strcmp(newcommand2 , "-all") == 0)
            options[1] = true;
        if (newcommand2[0] == '\0' || newcommand2 == NULL)
            break;
        newcommand2 = strtok(NULL , " ");
    }
    if (options[0] && options[1]) {
        printf("invalid mix of options!\n");
        return;
    }
    strcpy(newcommand , "\0");
    strcat(newcommand , "find --str \"");
    strcat(newcommand , str1);
    strcat(newcommand , "\" --file \"/");
    strcat(newcommand , pathes);
    strcat(newcommand , "\"");
    if (options[0]) {
        strcat(newcommand , " -at ");
        char *num = (char *)calloc(maximum_size_of_input , sizeof(char));
        sprintf(num , "%d" , at);
        strcat(newcommand , num);
    } else {
        strcat(newcommand , " -all");
    }
    char *find_command = (char *)calloc(maximum_size_of_input , sizeof(char));
    strcpy(find_command , newcommand);
    strtok(find_command , " ");
    is_replace = true;
    find(find_command , str2);
    is_replace = false;
}

int replacing_the_word(char *text , int first , int second , char *string) {
    char *saved = (char *)calloc(maximum_size_of_input , sizeof(char));
    for (int i = second + 1; i != strlen(text); i++) {
        *(saved + i - second - 1) = *(text + i);
    }
    *(saved + strlen(text) - 1 - second) = '\0';
    int j = first;
    for (int i = 0; i != strlen(string); i++) {
        *(text + j) = *(string + i);
        j++;
    }
    int result = j;
    for (int i = 0; i != strlen(saved); i++) {
        *(text + j) = *(saved + i);
        j++;
    }
    *(text + j) = '\0';
    return result;
}

void auto_indent(char *command) {
    command = strtok(NULL , " ");
    int null , null2;
    FILE *file = find_path(command , "r+" , &null , null2);
    fseek(file , 0 , SEEK_END);
    char *all_text = (char *)calloc(maximum_size_of_input , sizeof(char));
    save_text_from_first(file , all_text);
    fclose(file);
    create_dot_file();
    int outer_open_acolad , outer_close_acolad , tabs = 0 , c , o;
    bool flag = false;
    for (int i = 0; i != strlen(all_text); i++) {
        if (all_text[i] == '{') {
            outer_open_acolad = i;
            if (c < outer_open_acolad) {
                tabs = 0;
                flag = false;
            } else if (outer_close_acolad < outer_open_acolad)
                tabs--;
            outer_close_acolad = -1;
            int counter = 0;
            for (int j = i+1; j != strlen(all_text); j++) {
                if (all_text[j] == '{')
                    counter++;
                if (all_text[j] == '}') {
                    if (counter == 0) {
                        outer_close_acolad = j;
                        break;
                    } else {
                        counter--;
                    }
                }
            }
            if (outer_close_acolad == -1) {
                printf("invalid text for auto indent!\n");
                return;
            }
            int starting_of_line = 0;
            for (int j = i; j != 0; j--) {
                if (all_text[j] == '\n') {
                    starting_of_line = j + 1;
                    break;
                }
            }
            int j , now_tabs = 0;
            for (j = starting_of_line; all_text[j] == '\t' && now_tabs != tabs; j++) {
                now_tabs++;
            }
            if (now_tabs < tabs) {
                shifting(all_text , j , tabs-now_tabs);
                c += tabs-now_tabs;
                outer_open_acolad += tabs-now_tabs;
                outer_close_acolad += tabs-now_tabs;
                i = outer_open_acolad;
                int l = j;
                for (int k = 0; k != tabs-now_tabs; k++) {
                    all_text[l] = '\t';
                    l++;
                }
                j = l;
            }
            int k;
            for (k = j; k != outer_open_acolad && (all_text[k] == ' ' || all_text[k] == '\t'); k++);
            if (k-j != 0) {
                left_shifting(all_text, k, k - j);
                c -= (k-j);
                outer_open_acolad -= (k-j);
                outer_close_acolad -= (k-j);
            }
            if (j != outer_open_acolad) {
                j = outer_open_acolad-1;
                while (all_text[j] == ' ' || all_text[j] == '\t')
                    j--;
                if (outer_open_acolad - j == 1) {
                    shifting(all_text , outer_open_acolad,  1);
                    c++;
                    all_text[outer_open_acolad] = ' ';
                    outer_open_acolad++;
                    outer_close_acolad++;
                    j = outer_open_acolad;
                } else {
                    j++;
                    all_text[j] = ' ';
                    j++;
                    if (outer_open_acolad - j != 0) {
                        left_shifting(all_text, outer_open_acolad, outer_open_acolad - j);
                        c -= outer_open_acolad - j;
                        outer_close_acolad -= (outer_open_acolad - j);
                        outer_open_acolad -= (outer_open_acolad - j);
                    }

                }
            }
            j++;
            if (all_text[j] != '\n') {
                while(all_text[j] != '\n' && all_text[j] != '\0' && (all_text[j] == ' ' || all_text[j] == '\t'))
                    j++;
                if (j - outer_open_acolad - 1 != 0) {
                    left_shifting(all_text , j , j - outer_open_acolad - 1);
                    c -= j - outer_open_acolad - 1;
                    outer_close_acolad -= (j - outer_open_acolad - 1);
                    j = outer_open_acolad + 1;
                }
                if (all_text[j] != '\n') {
                    shifting(all_text , j , 1);
                    c++;
                    all_text[j] = '\n';
                    outer_close_acolad += 1;
                    outer_open_acolad += 1;
                }
            }
            j++;
            shifting(all_text , j , tabs+1);
            c += (tabs+1);
            outer_close_acolad += tabs+1;
            for (int k = 0; k != tabs+1; k++) {
                all_text[j] = '\t';
                j++;
            }
            if (all_text[j] != '\t' && j != outer_close_acolad) {
                j = outer_close_acolad-1;
                while (all_text[j] == ' ' || all_text[j] == '\t')
                    j--;
                j++;
                left_shifting(all_text , outer_close_acolad , outer_close_acolad - j);
                c -= outer_close_acolad - j;
                outer_close_acolad -= (outer_close_acolad - j);
            }
            j = outer_close_acolad;
            shifting(all_text , j , 1);
            c++;
            outer_close_acolad++;
            all_text[j] = '\n';
            j++;
            shifting(all_text , j , tabs);
            c += tabs;
            for (int k = 0; k != tabs; k++) {
                all_text[j+k] = '\t';
            }
            outer_close_acolad += tabs;
            j = outer_close_acolad+1;
            if (all_text[j] != '\n') {
                while (all_text[j] != '\n' && all_text[j] != '\0' && (all_text[j] == ' ' || all_text[j] == '\t'))
                    j++;
                left_shifting(all_text , j , j-outer_close_acolad-1);
                c -= j-outer_close_acolad-1;
            }
            j = outer_close_acolad + 1;
            if (all_text[j] != '\n' && all_text[j] != '\0') {
                shifting(all_text , j , 1);
                c++;
                all_text[j] = '\n';
                j++;
                shifting(all_text , j , tabs);
                c += tabs;
                for (int k = 0; k != tabs; k++)
                    all_text[j+k] = '\t';
            }
            tabs++;
            i = outer_open_acolad;
            if (!flag) {
                o = outer_open_acolad;
                c = outer_close_acolad;
                flag = true;
            }
        }
    }
    file = fopen(pathes , "w");
    for (int i = 0; i != strlen(all_text); i++) {
        fputc(all_text[i] , file);
    }
    fclose(file);
    update_backup_file();
}

void shifting(char *text , int ind , int size) {
    for (int i = strlen(text); i >= ind; i--) {
        text[i+size] = text[i];
    }
}

void left_shifting(char *text , int ind , int size) {
    for (int i = ind; i <= strlen(text); i++) {
        text[i-size] = text[i];
    }
}

void text_comparator(char *command) {
    command = strtok(NULL, "");
    char *save = (char *)calloc(maximum_size_of_input , sizeof(char));
    strcpy(save , command);
    char *newcommand = (char *) calloc(maximum_size_of_input, sizeof(char));
    char *all_text1 = (char *) calloc(maximum_size_of_input, sizeof(char));
    char *all_text2 = (char *) calloc(maximum_size_of_input, sizeof(char));
    strcpy(newcommand, command);
    strtok(command, " ");
    int skip;
    FILE *file = find_path(command, "r+", &skip, 0);
    save += (8 + skip);
    fseek(file, 0, SEEK_END);
    save_text_from_first(file, all_text1);
    fclose(file);
    newcommand += 8 + skip;
    strcpy(command, newcommand);
    strcpy(newcommand, "--file ");
    strcat(newcommand, command);
    strtok(newcommand, " ");
    file = find_path(newcommand, "r+", &skip, 0);
    save += skip;
    if (save[0] != '\0') {
        save += 4;
        is_arman = true;
    }
    fseek(file, 0, SEEK_END);
    save_text_from_first(file, all_text2);
    fclose(file);
    char *line1 = (char *) calloc(maximum_size_of_input, sizeof(char));
    char *line2 = (char *) calloc(maximum_size_of_input, sizeof(char));
    int j = 0, i = 0, x = 0, y = 0, counter = 1;
    while (1) {
        if (strlen(all_text1) == 0 || strlen(all_text2) == 0)
            break;
        while (all_text1[i] != '\n' && all_text1[i] != '\0') {
            line1[x] = all_text1[i];
            i++;
            x++;
        }
        line1[x] = '\0';
        while (all_text2[j] != '\n' && all_text2[j] != '\0') {
            line2[y] = all_text2[j];
            j++;
            y++;
        }
        line2[y] = '\0';
        if (strcmp(line1, line2) != 0) {
            if (is_arman) {
                strcat(arman_string , "============ #");
                char num[100];
                sprintf(num , "%d" , counter);
                strcat(arman_string , num);
                strcat(arman_string , " ============\n");
                strcat(arman_string , line1);
                strcat(arman_string , "\n");
                strcat(arman_string , line2);
                strcat(arman_string , "\n");
            } else {
                printf("============ #%d ============\n", counter);
                printf("%s\n%s\n", line1, line2);
            }
        }
        counter++;
        if (all_text1[i] == '\0')
            break;
        if (all_text2[j] == '\0')
            break;
        i++;
        j++;
        x = y = 0;
    }
    if (all_text1[i] != '\0' ^ all_text2[j] != '\0') {
        if (all_text1[i] == '\0') {
            j++;
            int lines = 0;
            for (int k = j; k != strlen(all_text2); k++)
                if (all_text2[k] == '\n')
                    lines++;
            if (is_arman) {
                strcat(arman_string , ">>>>>>>>>>>> #");
                char num[100];
                sprintf(num , "%d" , counter);
                strcat(arman_string , num);
                strcat(arman_string , " - #");
                sprintf(num , "%d" , lines + counter);
                strcat(arman_string , num);
                strcat(arman_string, " >>>>>>>>>>>>\n");
                for (int k = j; k != strlen(all_text2); k++) {
                    char x[2] = {all_text2[k] , '\0'};
                    strcat(arman_string , x);
                }
                strcat(arman_string , "\n");
            } else {
                printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", counter, counter + lines);
                for (int k = j; k != strlen(all_text2); k++)
                    printf("%c", all_text2[k]);
                printf("\n");
            }
        } else {
            i++;
            int lines = 0;
            for (int k = i; k != strlen(all_text1); k++)
                if (all_text1[k] == '\n')
                    lines++;
            if (is_arman) {
                strcat(arman_string , "<<<<<<<<<<<< #");
                char num[100];
                sprintf(num , "%d" , counter);
                strcat(arman_string , num);
                strcat(arman_string , " - #");
                sprintf(num , "%d" , lines + counter);
                strcat(arman_string , num);
                strcat(arman_string, " <<<<<<<<<<<<\n");
                for (int k = i; k != strlen(all_text1); k++) {
                    char x[2] = {all_text1[k] , '\0'};
                    strcat(arman_string , x);
                }
                strcat(arman_string , "\n");
            } else {
                printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", counter, counter + lines);
                for (int k = i; k != strlen(all_text1); k++)
                    printf("%c", all_text1[k]);
                printf("\n");
            }
        }
    }
    if (is_arman) {
        arman_string[strlen(arman_string)-2] = '\0';
        is_arman = false;
        arman(save);
    } else {
        printf("%s" , arman_string);
    }
}

void tree(char *command){
    command = strtok(NULL , " ");
    int depth = atoi(command);
    if (depth < -1) {
        printf("invalid depth!\n");
        return;
    }
    command = strtok(NULL , " ");
    if (command != NULL) {
        command = strtok(NULL , "");
        strcpy(arman_string, "\0");
        is_arman = true;
    }
    char *path = (char *)calloc(maximum_size_of_input , sizeof(char));
    strcpy(path , "root");
    bool *last_l = (bool *)calloc(maximum_size_of_input , sizeof(bool));
    show_tree(path , 0 , 0 , last_l , depth);
    if (is_arman) {
        arman(command);
        is_arman = false;
    }
}

void show_tree(char *path , int root ,int level , bool last_l[] , int depth) {
    if (depth < level && depth != -1)
        return;
    int counter = 0 , counter2 = 0;
    char *new_path = (char *)calloc(2000 , sizeof(char));
    DIR *dir = opendir(path);
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, dp->d_name);
        int attr = GetFileAttributes(new_path);
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && ((attr & FILE_ATTRIBUTE_HIDDEN) == 0))
            counter++;
    }
    closedir(dir);
    dir = opendir(path);
    if (!dir)
        return;
    while ((dp = readdir(dir)) != NULL) {
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, dp->d_name);
        int attr = GetFileAttributes(new_path);
        if (strcmp(dp->d_name , ".") != 0 && strcmp(dp->d_name , "..") != 0  && ((attr & FILE_ATTRIBUTE_HIDDEN) == 0)) {
            for (int i = 0; i < root; i++) {
                if (i % 4 == 0 && i >= 4 &&!last_l[(i/4)-1]) {
                    unsigned char a[2] = {179 , 0};
                    if (is_arman)
                        strcat(arman_string , a);
                    else
                        printf("%c", 179);
                } else {
                    char a[2] = {' ' , 0};
                    if (is_arman)
                        strcat(arman_string , a);
                    else
                        printf(" ");
                }
            }
            if (level != 0) {
                unsigned char a[6] = {192 , 196 , 196 , 196 , '\0'};
                unsigned char b[6] = {195 , 196 , 196 , 196 , '\0'};
                if (counter2 == counter-1) {
                    last_l[level] = true;
                    if (is_arman)
                        strcat(arman_string , a);
                    else
                        printf("%c%c%c%c", 192, 196 , 196 , 196);
                } else {
                    if (is_arman)
                        strcat(arman_string , b);
                    else
                        printf("%c%c%c%c", 195, 196 , 196 , 196);
                }
            } else {
                if (counter2 == counter-1)
                    last_l[level] = true;
            }
            if (is_arman) {
                strcat(arman_string , dp->d_name);
                strcat(arman_string , "\n");
            } else {
                printf("%s\n", dp->d_name);
            }
            counter2++;
            show_tree(new_path , root + 4, level+1 , last_l , depth);
        }
    }
    closedir(dir);
}

void arman(char *command) {
    is_arman2 = true;
    is_arman = false;
    strtok(command , " ");
    if (strcmp(command , "find") == 0) {
        find(command , NULL);
    } else if (strcmp(command , "insertstr") == 0) {
        insertstr(command , "normal" , arman_string);
    } else if (strcmp(command , "grep") == 0) {
        grep(command);
    } else if (strcmp(command , "replace") == 0) {
        replace(command);
    }
    strcpy(arman_string , "\0");
    is_arman2 = false;
}

// Tree bug
// Find bug
