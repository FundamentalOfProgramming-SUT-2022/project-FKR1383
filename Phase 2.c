#include ncurses.h
#include conio.h
#include stdio.h
#include string.h
#include stdlib.h
#include stdbool.h
#include sysstat.h
#include unistd.h
#define maximum_size_of_input 1000000
#define length_of_design 1000


FILE file = NULL;
int now_mode = 0 , counter = 1 , lines = 1 , length_of_lines[length_of_design] , y_coor = 1,  x_coor = 0 , y_majazi_coor = 1 , x_majazi_coor = 0 ,first_line = 1 , first_character = 0 , last_character = 0 , first_selected , second_selected , first_real_selected , second_real_selected , x_majazi_first_selected , y_majazi_first_selected , x_majazi_second_selected , y_majazi_second_selected , x_second_selected , y_second_selected , x_first_selected , y_first_selected, first_character_before_visual , first_line_before_visual , real_length_of_lines[length_of_design] , first_pos[maximum_size_of_input] ,second_pos[maximum_size_of_input] , number_of_finds , which_find;
char command_args[length_of_design] = command args , mode[3][7] = {NORMAL , VISUAL , INSERT} , is_saved = '-' , file_name[length_of_design] =  , text[maximum_size_of_input] , now_text[length_of_design] , cb[maximum_size_of_input] , address[length_of_design] , dot_text[maximum_size_of_input] , dollar_text[maximum_size_of_input];
bool first_visual = false , is_visual = true , do_visual = false , is_cut = false , is_saveas = false , do_auto_indent = false , is_insert = false , do_undo = false , is_find = false , is_replace = false , one_word = false , is_find3 = false;
WINDOW win1 , win2 , win3 , win4 , main_win;
void starting_program();
void starting_settings();
void create_dot_text();
void create_dollar_text();
int go_to_position(int , int);

int replacing_the_word(char text2 , int first , int second , char string) {
    char saved = (char )calloc(maximum_size_of_input , sizeof(char));
    for (int i = second + 1; i != strlen(text2); i++) {
        (saved + i - second - 1) = (text2 + i);
    }
    (saved + strlen(text2) - 1 - second) = '0';
    int j = first;
    for (int i = 0; i != strlen(string); i++) {
        (text2 + j) = (string + i);
        j++;
    }
    int result = j;
    for (int i = 0; i != strlen(saved); i++) {
        (text2 + j) = (saved + i);
        j++;
    }
    (text2 + j) = '0';
    return result;
}

bool find_string(char resume , char string , int skip)
{
     this function starts with --str
    char help = (char ) calloc(maximum_size_of_input, sizeof(char));
    strtok(resume,  );
    resume = strtok(NULL, );
    strcpy(help, resume);
    if (resume[0] != '') {
        one_word = true;
        strtok(resume,  );
        int j = 0, i = 0;
        for (; i != strlen(resume); i++) {
            if (resume[i] == '') {
                if (resume[i + 1] == 'n') {
                    i++;
                    string[j] = 'n';
                    j++;
                } else {
                    string[j] = resume[i + 1];
                    i++;
                    j++;
                }
            } else {
                if (is_find3) {
                    if (resume[i] == '') {
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
        skip = i;
        strcpy(resume, help);
        return true;
    } else {
        one_word = false;
        int j = 0, i = 0;
        for (; i != strlen(resume); i++) {
            if (resume[i + 1] == '') {
                if (resume[i + 2] == 'n') {
                    string[j] = 'n';
                    j++;
                    i++;
                } else {
                    string[j] = resume[i + 2];
                    i++;
                    j++;
                }
            } else if (resume[i + 1] == '') {
                break;
            } else {
                if (is_find3) {
                    if (resume[i+1] == '') {
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
        skip = i + 2;
        strcpy(resume, help);
        return true;
    }
}

void find3(char command , char rep) {
    
    bool options[4] = {false ,false , false , false};
    int at;
     0 is count -- 1 is at -- 2 is byword -- 3 is all
    char newcommand = (char ) calloc(maximum_size_of_input , sizeof(char));
    char newcommand2 = (char ) calloc(maximum_size_of_input , sizeof(char));
    char save = (char )calloc(maximum_size_of_input , sizeof(char));
    command = strtok(NULL , );
    strcpy(newcommand , command);
    strcpy(newcommand2 , command);
    char string = (char )calloc(maximum_size_of_input , sizeof(char));
    char all_text = (char )calloc(maximum_size_of_input , sizeof(char));
    strcpy(all_text , text);
    int skip;
        is_find3 = true;
        find_string(command, string, &skip);
        is_find3 = false;
        newcommand += (7 + skip);
        newcommand2 += (7 + skip);
    strtok(newcommand ,  );
    strcpy(command , newcommand2);
    if (command[0] != '0') {
        strtok(command ,  );
        while (command != NULL && command[0] != '0' && command[0] != '=') {
            if (strcmp(command, -count) == 0)
                options[0] = true;
            if (strcmp(command , -at) == 0) {
                options[1] = true;
                command = strtok(NULL ,  );
                at = atoi(command);
            }
            if (strcmp(command , -byword) == 0)
                options[2] = true;
            if (strcmp(command , -all) == 0)
                options[3] = true;
            if (command == NULL  command[0] == '0'  command[0] == '=')
                break;
            command = strtok(NULL ,  );
            if (command == NULL  command[0] == '0'  command[0] == '=')
                break;
        }
        if (command != NULL && command[0] != '0' && command[0] == '=') {
            command = strtok(NULL , );
            strcpy(save , 0);
            strcpy(save , command);
            is_arman = true;
        } else {
            is_arman = false;
        }
    }
    
    number_of_finds = 0;
    bool successful_replace = false;
    int counter = 0;
    int j = 0, start = 0;
    for (int i = 0; i = strlen(all_text); i++) {
        if (j == strlen(string)) {
            if (string[0] == 127) {
                int checker = strlen(string)-1;
                while (checker != -1 && string[checker] != ' ') {
                    checker--;
                }
                checker++;
                if (string[checker] == 127) {
                    int l = start;
                while (l = 0 && all_text[l] != ' ' && all_text[l] != ' ' && all_text[l] != 'n')
                    l--;
                (first_pos + counter) = l+1;
                int last = i-1;
                bool what = false;
                checker = strlen(string)-1;
                    for (int h = i-1; h != -1 && all_text[h] != ' ' && all_text[h] != 'n'; h--) {
                        if (string[checker] == 127)
                            break;
                        if (string[checker] == all_text[h]) {
                            checker--;
                            if (!what) {
                                what = true;
                                last = h;
                            }        
                        } else {
                            if (what) {
                                what = false;
                                h = last;
                                checker = strlen(string)-1;
                            }
                            checker = strlen(string)-1;
                        }
                    }
                    (second_pos + counter) = last+1;
                    
                i = i + 1;
                j = 0;
                } else {
                int l = start;
                while (l = 0 && all_text[l] != ' ' && all_text[l] != ' ' && all_text[l] != 'n')
                    l--;
                (first_pos + counter) = l+1;
                (second_pos + counter) = i;
                int end = start;
                while (all_text[end] != 'n' && all_text[end] != '0' && all_text[end] != ' ')
                    end++;
                i = end + 1;
                j = 0;
                }
                
            } else if (string[strlen(string) - 1] == 127) {
                (first_pos + counter) = start;
                (second_pos + counter) = i;
                int end = start;
                while (all_text[end] != 'n' && all_text[end] != '0' && all_text[end] != ' ')
                    end++;
                i = end + 1;
                j = 0;
            } else {
            int checker = strlen(string)-1;
                while (checker != -1 && string[checker] != ' ') {
                    checker--;
                }
                checker++;
                if (string[checker] == 127) {
                (first_pos + counter) = start;
                int last = i-1;
                bool what = false;
                checker = strlen(string)-1;
                    for (int h = i-1; h != -1 && all_text[h] != ' ' && all_text[h] != 'n'; h--) {
                        if (string[checker] == 127)
                            break;
                        if (string[checker] == all_text[h]) {
                            checker--;
                            if (!what) {
                                what = true;
                                last = h;
                            }        
                        } else {
                            if (what) {
                                what = false;
                                h = last;
                                checker = strlen(string)-1;
                            }
                            checker = strlen(string)-1;
                        }
                    }
                    (second_pos + counter) = last+1;
                    j = 0;
                i = i + 1;
                    } else {
                (first_pos + counter) = start;
                (second_pos + counter) = i;
                j = 0;
               
                }
           }
            if (is_replace) {
                if (options[1]) {
                    if (at-1 == counter) {
                        successful_replace = true;
                        i = replacing_the_word(all_text , (first_pos + counter) , (second_pos + counter)-1 , rep);
                    }
                } else if (options[3]) {
                    successful_replace = true;
                    i = replacing_the_word(all_text , (first_pos + counter) , (second_pos + counter)-1 , rep);
                } else if (counter == 0){
                    successful_replace = true;
                        i = replacing_the_word(all_text , (first_pos + counter) , (second_pos + counter)-1 , rep);
                }
            }
            counter++;
        }
        if (i = strlen(all_text)) {
            if (string[j] == 127 && j == strlen(string) - 1) {
                (first_pos + counter) = start;
                (second_pos + counter) = i;
                if (is_replace) {
                    if (options[1]) {
                        if (at-1 == counter) {
                            successful_replace = true;
                            i = replacing_the_word(all_text , (first_pos + counter) , (second_pos + counter)-1 , rep);
                        }
                    } else if (options[3]) {
                        successful_replace = true;
                        i = replacing_the_word(all_text , (first_pos + counter) , (second_pos + counter)-1 , rep);
                    } else if (counter == 0) {
                    successful_replace = true;
                        i = replacing_the_word(all_text , (first_pos + counter) , (second_pos + counter)-1 , rep);
                    }
                }
                counter++;
            }
            break;
        }
        if (j == 0)
            start = i;
        if (string[j] == 127) {
            if (j == 0  string[j - 1] == ' ') {
                j++;
                while (string[j] != ' ' && string[j] != '0')
                    j++;
                j--;
                while (all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0') {
                    i++;
                }
                i--;
                bool flag = false;
                int started_j = j , started_i = i;
                while (i != -1 && all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0') {
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
                while (i != 0 && all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0')
                    i--;
                if (string[j] == 127) {
                    i++;
                    while (all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0')
                        i++;
                    i--;
                    while (string[j] != ' ' && string[j] != '0')
                        j++;
                }
                if (string[j] != ' ' && string[j] != '0') {
                    i = start;
                    j = 0;
                }
            } else {
                j++;
                while (all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0') {
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
        if (successful_replace)
            strcpy(command_args , replace successfully done!);
        else
            strcpy(command_args , replace failed);
    if (successful_replace) {
        strcpy(text , all_text);
        create_dollar_text();
    }
    if(is_arman) {
        is_arman = false;
        arman(save);
    }
    one_word = false;
}


void replace2(char command) {
    char newcommand = (char )calloc(maximum_size_of_input , sizeof(char));
    char str1 = (char )calloc(maximum_size_of_input , sizeof(char));
    char str2 = (char )calloc(maximum_size_of_input , sizeof(char));
    int skip;
    command = strtok(NULL , );
        strcpy(newcommand, command);
        find_string(newcommand, str1, &skip);
        command += (7 + skip);
    strcpy(newcommand , command);
    find_string(newcommand , str2 , &skip);
    command += (7 + skip);
    strcpy(newcommand , command);
    create_dot_text();
    bool options[2] = {false , false};
    int at;
    char newcommand2 = (char )calloc(maximum_size_of_input , sizeof(char));
    strcpy(newcommand2 , newcommand);
    strtok(newcommand2 ,  );
    while (newcommand2 != NULL) {
        if (strcmp(newcommand2 , -at) == 0) {
            options[0] = true;
            newcommand2 = strtok(NULL ,  );
            at = atoi(newcommand2);
        }
        if (strcmp(newcommand2 , -all) == 0)
            options[1] = true;
        if (newcommand2[0] == '0'  newcommand2 == NULL)
            break;
        newcommand2 = strtok(NULL ,  );
    }
    if (options[0] && options[1]) {
        strcpy(command_args , invalid mix of options!n);
        return;
    }
    strcpy(newcommand , 0);
    strcat(newcommand , find --str );
    strcat(newcommand , str1);
    strcat(newcommand , );
    if (options[0]) {
        strcat(newcommand ,  -at );
        char num = (char )calloc(maximum_size_of_input , sizeof(char));
        sprintf(num , %d , at);
        strcat(newcommand , num);
    } else if (options[1]){
        strcat(newcommand ,  -all);
    }
    char find_command = (char )calloc(maximum_size_of_input , sizeof(char));
    strcpy(find_command , newcommand);
    strtok(find_command ,  );
    is_replace = true;
    find3(find_command , str2);
    is_saved = '-';
}

void find_string2(char resume , char string)
{
    if (resume[0] == '') {
        resume[strlen(resume)-2] = '0';
        resume++;
    } 
    int j = 0;
    for (int i = 0; i != strlen(resume); i++) {
        if (resume[i] == '') {
            if (resume[i+1] == 'n') {
                string[j] = 'n';
                i++;
            } else {
                i++;
                string[j] = resume[i];
            }
        } else if (resume[i] == '') {
            string[j] = (char)127;
        } else {
            string[j] = resume[i];
        }
        j++;
    }
    string[j] = '0';
}

int find2(char string) {
    char all_text[maximum_size_of_input];
    int counter = 0;
    strcpy(all_text , text);
    int j = 0, start = 0;
    number_of_finds = 0;
    for (int i = 0; i = strlen(all_text); i++) {
        if (j == strlen(string)) {
            if (string[0] == 127) {
                int checker = strlen(string)-1;
                while (checker != -1 && string[checker] != ' ') {
                    checker--;
                }
                checker++;
                if (string[checker] == 127) {
                    int l = start;
                while (l = 0 && all_text[l] != ' ' && all_text[l] != ' ' && all_text[l] != 'n')
                    l--;
                (first_pos + counter) = l+1;
                int last = i-1;
                bool what = false;
                checker = strlen(string)-1;
                    for (int h = i-1; h != -1 && all_text[h] != ' ' && all_text[h] != 'n'; h--) {
                        if (string[checker] == 127)
                            break;
                        if (string[checker] == all_text[h]) {
                            checker--;
                            if (!what) {
                                what = true;
                                last = h;
                            }        
                        } else {
                            if (what) {
                                what = false;
                                h = last;
                                checker = strlen(string)-1;
                            }
                            checker = strlen(string)-1;
                        }
                    }
                    (second_pos + counter) = last+1;
                    
                i = i + 1;
                j = 0;
                } else {
                int l = start;
                while (l = 0 && all_text[l] != ' ' && all_text[l] != ' ' && all_text[l] != 'n')
                    l--;
                (first_pos + counter) = l+1;
                (second_pos + counter) = i;
                int end = start;
                while (all_text[end] != 'n' && all_text[end] != '0' && all_text[end] != ' ')
                    end++;
                i = end + 1;
                j = 0;
                }
                
            } else if (string[strlen(string) - 1] == 127) {
                (first_pos + counter) = start;
                (second_pos + counter) = i;
                int end = start;
                while (all_text[end] != 'n' && all_text[end] != '0' && all_text[end] != ' ')
                    end++;
                i = end + 1;
                j = 0;
            } else {
            int checker = strlen(string)-1;
                while (checker != -1 && string[checker] != ' ') {
                    checker--;
                }
                checker++;
                if (string[checker] == 127) {
                (first_pos + counter) = start;
                int last = i-1;
                bool what = false;
                checker = strlen(string)-1;
                    for (int h = i-1; h != -1 && all_text[h] != ' ' && all_text[h] != 'n'; h--) {
                        if (string[checker] == 127)
                            break;
                        if (string[checker] == all_text[h]) {
                            checker--;
                            if (!what) {
                                what = true;
                                last = h;
                            }        
                        } else {
                            if (what) {
                                what = false;
                                h = last;
                                checker = strlen(string)-1;
                            }
                            checker = strlen(string)-1;
                        }
                    }
                    (second_pos + counter) = last+1;
                    j = 0;
                i = i + 1;
                    } else {
                    
                (first_pos + counter) = start;
                (second_pos + counter) = i;
                j = 0;
                
                
                }
            }
            counter++;
        }
        if (i = strlen(all_text)) {
            if (string[j] == 127 && j == strlen(string) - 1) {
                (first_pos + counter) = start;
                (second_pos + counter) = i;
                counter++;
            }
            break;
        }
        if (j == 0)
            start = i;
        if (string[j] == 127) {
            if (j == 0  string[j - 1] == ' ') {
                j++;
                while (string[j] != ' ' && string[j] != '0')
                    j++;
                j--;
                while (all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0') {
                    i++;
                }
                i--;
                bool flag = false;
                int started_j = j , started_i = i;
                while (i != -1 && all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0') {
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
                while (i != 0 && all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0')
                    i--;
                if (string[j] == 127) {
                    i++;
                    while (all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0')
                        i++;
                    i--;
                    while (string[j] != ' ' && string[j] != '0')
                        j++;
                }
                if (string[j] != ' ' && string[j] != '0') {
                    i = start;
                    j = 0;
                }
            } else {
                j++;
                while (all_text[i] != ' ' && all_text[i] != 'n' && all_text[i] != '0') {
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
    number_of_finds = counter;
    if (counter == 0)
        strcpy(command_args , Match not find!);
    is_find = true;
    which_find = 0;
    int help = go_to_position(y_coor , x_coor);
    while (which_find  counter && first_pos[which_find]  help)
        which_find++;
    if (which_find = counter)
        which_find = 0;
}

void shifting(char text , int ind , int size) {
    for (int i = strlen(text); i = ind; i--) {
        text[i+size] = text[i];
    }
}

void left_shifting(char text , int ind , int size) {
    for (int i = ind; i = strlen(text); i++) {
        text[i-size] = text[i];
    }
}

void create_dot_text() {
    strcpy(dot_text , text);
}

void create_dollar_text() {
    strcpy(dollar_text , dot_text);
    strcpy(dot_text , 0);
}

void undo() {
    strcpy(dot_text , dollar_text);
    strcpy(dollar_text , text);
    strcpy(text , dot_text);
    strcpy(dot_text , 0);
    do_undo = true;
    starting_settings();
}

void auto_indent2() {
    create_dot_text();
    int outer_open_acolad , outer_close_acolad , tabs = 0 , c , o;
    char all_text[maximum_size_of_input];
    strcpy(all_text , text);
    bool flag = false;
    for (int i = 0; i != strlen(all_text); i++) {
        if (all_text[i] == '{') {
            outer_open_acolad = i;
            if (c  outer_open_acolad) {
                tabs = 0;
                flag = false;
            } else if (outer_close_acolad  outer_open_acolad)
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
                strcpy(command_args , invalid text for auto indent!);
                return;
            }
            int starting_of_line = 0;
            for (int j = i; j != 0; j--) {
                if (all_text[j] == 'n') {
                    starting_of_line = j + 1;
                    break;
                }
            }
            int j , now_tabs = 0;
            for (j = starting_of_line; all_text[j] == 't' && now_tabs != tabs; j++) {
                now_tabs++;
            }
            if (now_tabs  tabs) {
                shifting(all_text , j , tabs-now_tabs);
                c += tabs-now_tabs;
                outer_open_acolad += tabs-now_tabs;
                outer_close_acolad += tabs-now_tabs;
                i = outer_open_acolad;
                int l = j;
                for (int k = 0; k != tabs-now_tabs; k++) {
                    all_text[l] = 't';
                    l++;
                }
                j = l;
            }
            int k;
            for (k = j; k != outer_open_acolad && (all_text[k] == ' '  all_text[k] == 't'); k++);
            if (k-j != 0) {
                left_shifting(all_text, k, k - j);
                c -= (k-j);
                outer_open_acolad -= (k-j);
                outer_close_acolad -= (k-j);
            }
            if (j != outer_open_acolad) {
                j = outer_open_acolad-1;
                while (all_text[j] == ' '  all_text[j] == 't')
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
            if (all_text[j] != 'n') {
                while(all_text[j] != 'n' && all_text[j] != '0' && (all_text[j] == ' '  all_text[j] == 't'))
                    j++;
                if (j - outer_open_acolad - 1 != 0) {
                    left_shifting(all_text , j , j - outer_open_acolad - 1);
                    c -= j - outer_open_acolad - 1;
                    outer_close_acolad -= (j - outer_open_acolad - 1);
                    j = outer_open_acolad + 1;
                }
                if (all_text[j] != 'n') {
                    shifting(all_text , j , 1);
                    c++;
                    all_text[j] = 'n';
                    outer_close_acolad += 1;
                    outer_open_acolad += 1;
                }
            }
            j++;
            shifting(all_text , j , tabs+1);
            c += (tabs+1);
            outer_close_acolad += tabs+1;
            for (int k = 0; k != tabs+1; k++) {
                all_text[j] = 't';
                j++;
            }
            if (all_text[j] != 't' && j != outer_close_acolad) {
                j = outer_close_acolad-1;
                while (all_text[j] == ' '  all_text[j] == 't')
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
            all_text[j] = 'n';
            j++;
            shifting(all_text , j , tabs);
            c += tabs;
            for (int k = 0; k != tabs; k++) {
                all_text[j+k] = 't';
            }
            outer_close_acolad += tabs;
            j = outer_close_acolad+1;
            if (all_text[j] != 'n') {
                while (all_text[j] != 'n' && all_text[j] != '0' && (all_text[j] == ' '  all_text[j] == 't'))
                    j++;
                left_shifting(all_text , j , j-outer_close_acolad-1);
                c -= j-outer_close_acolad-1;
            }
            j = outer_close_acolad + 1;
            if (all_text[j] != 'n' && all_text[j] != '0') {
                shifting(all_text , j , 1);
                c++;
                all_text[j] = 'n';
                j++;
                shifting(all_text , j , tabs);
                c += tabs;
                for (int k = 0; k != tabs; k++)
                    all_text[j+k] = 't';
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
    strcpy(text , all_text);
    do_auto_indent = true;
    create_dollar_text();
}


FILE create_file(char path)
{
    if (path[0] == '') {
        if (strlen(path)  7) {
                strcpy(command_args , invalid address);
            return NULL;
        }
        if (path[1] == 'r' && path[2] == 'o' && path[3] == 'o' && path[4] == 't' && path[5] == '') {
            char virual_path = (char )calloc(strlen(path) , sizeof(char));
            virual_path[0] = '0';
            path++;
            if (access(path, F_OK) == 0 && !is_saveas) {
                strcpy(command_args ,This file exists);
                return NULL;
                }
            strtok(path , );
            while (path != NULL)
            {
                strcat(virual_path , path);
                path = strtok(NULL , );
                if (path != NULL) {
                    mkdir(virual_path , 0700);
                    strcat(virual_path, );
                }
            }
            strcpy(address , virual_path);
            return fopen(virual_path , w);
        } else {
            strcpy(command_args , invalid addressn);
            return NULL;
        }
    } else if (path[0] == '' && path[strlen(path)-1] == '') {
        path++;
        path[strlen(path)-1] = '0';
        return create_file(path);
    } else {
            strcpy(command_args , invalid address);
        return NULL;
    }
}

FILE openfile(char path)
{
    if (path[0] == '') {
        if (strlen(path)  7) {
            strcpy(command_args , invalid address);
            return NULL;
        }
        if (path[1] == 'r' && path[2] == 'o' && path[3] == 'o' && path[4] == 't' && path[5] == '') {
            char virual_path = (char )calloc(strlen(path) , sizeof(char));
            virual_path[0] = '0';
            path++;
            bool flag = false;
            if (access(path, F_OK) == -1) {
                flag = true;
                char checker = (char )calloc(maximum_size_of_input , sizeof(char));
                strcpy(checker , path);
                char virtual_checker = (char )calloc(maximum_size_of_input , sizeof(char));
                virtual_checker[0] = '0';
                strtok(checker , );
                while (checker != NULL)
                {
                    strcat(virtual_checker , checker);
                    checker = strtok(NULL , );
                    if (checker != NULL) {
                        if (access(virtual_checker, X_OK) == -1) {
                            flag = false;
                            break;
                        }
                        strcat(virtual_checker, );
                    }
                }
                    if (flag)
                        strcpy(command_args , This file doesn't exist);
                    else
                        strcpy(command_args , This directory doesn't exist);
                return NULL;
            }
            strtok(path , );
            while (path != NULL)
            {
                strcat(virual_path , path);
                path = strtok(NULL , );
                if (path != NULL) {
                    mkdir(virual_path , 0700);
                    strcat(virual_path, );
                }
            }
            strcpy(address , virual_path);
            return fopen(virual_path , r+);
        } else {
            strcpy(command_args , invalid address);
            return NULL;
        }
    } else if (path[0] == '' && path[strlen(path)-1] == '') {
        path++;
        path[strlen(path)-1] = '0';
        return openfile(path);
    } else {
            strcpy(command_args , invalid address);
        return NULL;
    }
}



void saving() {
    for (int i = 0; i != strlen(text); i++) {
        fputc(text[i] , file);
    }
}

void show() {
    char now_char = fgetc(file);
    int i = 0;
    while (now_char != EOF) {
    text[i] = now_char;
    now_char = fgetc(file);
    i++;
    }
    text[i] = '0';
}

void starting_settings() {
    now_mode = 0 ; counter = 1 ; lines = 1; y_coor = 1;  x_coor = 0 ; y_majazi_coor = 1 ; x_majazi_coor = 0 ;first_line = 1 ;first_character = 0 ; last_character = 0 ;
        strcpy(command_args , command args);
         first_visual = false , is_visual = true , do_visual = false , is_cut = false , is_saveas = false;
}

void process2(char input) {
    if (input[0] != '')
        strtok(input ,  );
    char help = (char )calloc(length_of_design , sizeof(char));
    char help2 = (char )calloc(length_of_design , sizeof(char));
    strcpy(help , address);
    strcpy(help2 , file_name);
    if (strcmp(input , save) == 0) {
        if (file == NULL) {
            strcpy(command_args , Path and name of file );
            echo();
            starting_program();
            wgetstr(win4 , address);
            noecho();
            int start = strlen(address)-1;
            while (address[start] != '')
                start--;    
            start++;
            int end;
            if (address[0] == '')
                end = strlen(address)-2;
            else
                end = strlen(address)-1;
            int i = 0;
            while (start = end) {
                file_name[i] = address[start];
                start++;
                i++;
            }
            file_name[i] = '0';
            file = create_file(address);
            if (file == NULL) {
            strcpy(address , help);
            strcpy(file_name , help2);
             return;
             }
             saving();
             fclose(file);
            strcpy(command_args , done!);
            is_saved = '+';
        } else {
            file = fopen(address , w);
            saving();
            strcpy(command_args , done);
            fclose(file);
            is_saved = '+';
        }
    } else if (strcmp(input , saveas) == 0) {
        strcpy(command_args , Path and name of file );
            echo();
            starting_program();
            wgetstr(win4 , address);
            noecho();
            int start = strlen(address)-1;
            while (address[start] != '')
                start--;    
            start++;
            int end;
            if (address[0] == '')
                end = strlen(address)-2;
            else
                end = strlen(address)-1;
            int i = 0;
            while (start = end) {
                file_name[i] = address[start];
                start++;
                i++;
            }
            file_name[i] = '0';
            is_saveas = true;
            file = create_file(address);
            is_saveas = false;
            if (file == NULL) {
            strcpy(address , help);
            strcpy(file_name , help2);
             return;
             }
             saving();
             fclose(file);
            strcpy(command_args , done!);
            is_saved = '+';
    } else if (strcmp(input , open) == 0) {
            FILE check = fopen(address , r+);
            bool is_opened = (check != NULL);
            if (check != NULL)
                fclose(check);
            strcpy(command_args , Path and name of file );
            echo();
            starting_program();
            wgetstr(win4 , address);
            noecho();
            int start = strlen(address)-1;
            while (address[start] != '')
                start--;    
            start++;
            int end;
            if (address[0] == '')
                end = strlen(address)-2;
            else
                end = strlen(address)-1;
            int i = 0;
            while (start = end) {
                file_name[i] = address[start];
                start++;
                i++;
            }
            file_name[i] = '0';
            file = openfile(address);
            if (file == NULL) {
            strcpy(address , help);
            strcpy(file_name , help2);
            return;
            }
            fclose(file);
            if (is_opened) {
            file = fopen(help , w);
            saving();
            fclose(file); 
            }
            file = fopen(address , r+);
            show();
            fclose(file);
            strcpy(command_args , done!);
            is_saved = '+';
            starting_settings();
    } else if (strcmp(input , auto-indent) == 0) {
        input = strtok(NULL ,  );
        if (input == NULL  input[0] == '0') {
            auto_indent2();
            if (do_auto_indent) {
                starting_settings();
                is_saved = '-';
                do_auto_indent = false;
            }
        } else {
         full auto-indent
        }
    } else if (strcmp(input, undo) == 0) {
        input = strtok(NULL ,  );
        if (input == NULL  input[0] == '0') {
            undo();
            if (do_undo) {
                starting_settings();
                is_saved = '-';
                do_undo = false;
            }
        } else {
          full undo
        }
    } else if (input[0] == '') {
    char string[length_of_design];
    input++;
    find_string2(input , string);
    find2(string);
    } else if (strcmp(input , replace) == 0) {
        replace2(input);
    } else {
    strcpy(command_args , invalid command);
    }
}

void left_shifting2 () {
    int size = strlen(text);
    int j = first_real_selected;
    if (is_cut) {
    int counter = 0;
    for (int i = first_real_selected; i = second_real_selected; i++) {
        cb[counter] = text[i];
        counter++;
    }
    cb[counter] = '0';
    }
    for (int i = second_real_selected+1; i = size+1; i++) {
        text[j] = text[i];
        j++;
    }
}

int min (int num1 , int num2) {
if (num1 = num2)
return num1;
return num2;
}

int max (int num1 , int num2) {
if (num1 = num2)
return num1;
return num2;
}

void find_first_character() {
  int i = 0;
  for (int j = 1; j != first_line; j++)
      i += real_length_of_lines[j];
  first_character = i;
}

void find_last_character() {
int i = 0;
for (int j = 1; j = first_line + LINES - 6; j++)
    i += real_length_of_lines[j];
    i += (real_length_of_lines[first_line+LINES-5]-1);
last_character = i;
}

int go_to_position(int line , int col)
{
  int i = 0;
  for (int j = 1; j  line; j++) {
    i += real_length_of_lines[j];
  }
  i += col;
  return i;
}

WINDOW create_newwin(int height, int width, int starty, int startx)
{
        WINDOW local_win;
    local_win = newwin(height, width, starty, startx);
    wrefresh(local_win);
    return local_win;
}

void starting_program() {
  start_color();
  win1 = create_newwin(LINES-3 , 5 , 0 , 0);
  init_pair(1 , COLOR_BLACK , COLOR_CYAN);
  wbkgd(win1 , COLOR_PAIR(1));
  int last = 0 , last2 = 0 , last3 = 0;
  lines = 0;
  for (int i = 0; i = strlen(text); i++) {
  if ((is_find && number_of_finds != 0 && i == first_pos[which_find])  (is_replace && i == first_pos[0])) {
      first_line = lines+1;
      x_majazi_coor = 0;
      y_majazi_coor = 1;
      y_coor = lines+1;
      x_coor = i-last3;
  }
  if (text[i] == 't')
      last2 = (last2 + 8)  8  8;
    else if (text[i] == 'n'  text[i] == '0') {
      length_of_lines[lines+1] = last2-last+1;
      real_length_of_lines[lines+1] = i-last3+1;
      lines++;
      last = 0;
      last2 = 0;
      last3 = i+1;
    } else
        last2++;
  }
  if (is_visual == 0) {
  second_selected = go_to_position(y_coor , x_coor);
  x_majazi_second_selected = x_majazi_coor;
  y_majazi_second_selected = y_majazi_coor;
  x_second_selected = x_coor;
  y_second_selected = y_coor;
  first_real_selected = min(first_selected , second_selected);
  second_real_selected = max(first_selected , second_selected);
  }
  if (do_visual) {
   if (second_selected  first_selected) {
   x_coor = x_first_selected;
   y_coor = y_first_selected;
   x_majazi_coor = x_majazi_first_selected;
   y_majazi_coor = y_majazi_first_selected;
   first_character = first_character_before_visual;
   first_line = first_line_before_visual;
   }
   do_visual = false;
  }
  while (y_majazi_coor = LINES-6)
  {
  first_line++;
  y_majazi_coor--;
  }
  while (y_majazi_coor = 3 && first_line != 1) {
  first_line--;
  y_majazi_coor++;
  }
  find_first_character();
  find_last_character();
  for (int i = 1; i = LINES; i++) {
    mvwprintw(win1 , i , 1 , %3d , i+first_line-1);
  }
  main_win = create_newwin(LINES-3 , COLS-5 , 0 , 5);
  wmove(main_win , 1 , 0);
  int index_of_find = 0;
  init_pair(5 , COLOR_BLACK , COLOR_YELLOW);
  for (int i = first_character; i = min(last_character , strlen(text)-1); i++) {
      if (index_of_find  number_of_finds && i = second_pos[index_of_find])
              index_of_find++;
      if ((is_find && number_of_finds != 0 && i == first_pos[which_find])  (is_replace && i == first_pos[0])) {
          getyx(main_win , y_majazi_coor , x_majazi_coor);
          is_replace = false;
      }
      if ((is_visual == 0 && i = first_real_selected && i= second_real_selected)  (is_find && index_of_find  number_of_finds && i = first_pos[index_of_find] && i  second_pos[index_of_find]))
      wattron(main_win , A_BOLD  A_UNDERLINE  COLOR_PAIR(5));
      wprintw(main_win , %c , text[i]);
      wattroff(main_win , A_BOLD  A_UNDERLINE  COLOR_PAIR(5));
      }
  wbkgd(main_win , COLOR_PAIR(1));
  init_pair(2 , COLOR_RED , COLOR_GREEN);
  win2 = create_newwin(1 , 8 , LINES-3 , 0);
  wbkgd(win2 , COLOR_PAIR(2));
  wprintw(win2 , %s , mode[now_mode]);
  win3 = create_newwin(1 , COLS-8 , LINES-3 , 8);
  init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
  wbkgd(win3 , COLOR_PAIR(3));
  wprintw(win3 ,   %s   %c , file_name, is_saved);
  win4 = create_newwin(2 , COLS , LINES-2 , 0);
  init_pair(4 , COLOR_WHITE , COLOR_BLUE);
  wbkgd(win4 , COLOR_PAIR(4));
  wprintw(win4 , %s , command_args);
  wmove(main_win , y_majazi_coor , x_majazi_coor);
  wrefresh(main_win);
  wrefresh(win4);
  wrefresh(win3);
  wrefresh(win2);
  wrefresh(win1);
  refresh();
}

void process() {
  char help = (char )calloc(length_of_design , sizeof(char));
  wmove(main_win, y_majazi_coor , x_majazi_coor);
  keypad(main_win , TRUE);
  keypad(stdscr , TRUE);
  keypad(win1 , TRUE);
  keypad(win2 , TRUE);
  keypad(win3 , TRUE);
  keypad(win4 , TRUE);
  noecho();
  if (strcmp(mode[now_mode] , INSERT) == 0) {
  is_find = false;
  if (!is_insert) {
  create_dot_text();
  is_insert = true;
  }
  is_visual = true;
    int x = wgetch(main_win);
    if (x == KEY_F(1)) {
      now_mode++;
      now_mode %= 3;
      if (now_mode == 1)
      first_visual = true;
    } else {
      int checker = go_to_position(y_coor , x_coor);
      for (int i = strlen(text); i = checker; i--) {
    text[i+1] = text[i];
      }
      text[checker] = x;
      if (x == 'n') {
      y_majazi_coor++;
      x_majazi_coor = 0;
    y_coor++;
    x_coor = 0;
      } else if (x == 't') {
          x_majazi_coor = (x_majazi_coor+8)88;
          if (x_majazi_coor  COLS-6) {
              y_majazi_coor++;
              x_majazi_coor = 0;
          }
          x_coor++;
      } else {
      x_majazi_coor++;
      x_coor++;
      if (x_majazi_coor  COLS-6) {
              y_majazi_coor++;
              x_majazi_coor = 0;
          }
      }
      is_saved = '-';
      starting_program();
    }
  } else {
  if (is_insert && strcmp(dot_text , text) != 0) {
      is_insert = false;
      create_dollar_text();
  }
  int x = wgetch(main_win);
  if (is_find) {
       if (x == 'n') {
               which_find++;
               if (which_find == number_of_finds)
                   which_find = 0;
               
       } else {
               is_find = false;
       }
  } else {
  if (x == '' && strcmp(mode[now_mode] , NORMAL) == 0) {
    strcpy(command_args , );
    starting_program();
    echo();
    wgetstr(win4 , help);
    strcat(command_args, help);
    process2(help);
  } else if (x == 'u' && strcmp(mode[now_mode] , NORMAL) == 0) {
      undo();
  } else if (x == '=' && strcmp(mode[now_mode] , NORMAL) == 0) {
      auto_indent2();
            if (do_auto_indent) {
                starting_settings();
                is_saved = '-';
                do_auto_indent = false;
            }
  } else if (x == 'p' && strcmp(mode[now_mode] , NORMAL) == 0) {
  create_dot_text();
  is_saved = '-';
      is_visual = true;
      int size_of_cb = strlen(cb) , size_of_text = strlen(text);
      int checker = go_to_position(y_coor , x_coor);
      for (int i = size_of_text; i = checker; i--) {
    text[i + size_of_cb] = text[i];
    
      }
      for (int i = 0; i != size_of_cb; i++) {
          text[checker] = cb[i];
          if (text[checker] == 'n') {
          y_majazi_coor++;
          x_majazi_coor = 0;
          y_coor++;
          x_coor = 0;
          } else if (text[checker] == 't') {
              x_majazi_coor = (x_majazi_coor+8)88;
          if (x_majazi_coor  COLS-6) {
              y_majazi_coor++;
              x_majazi_coor = 0;
          }
          x_coor++;
          } else {
          x_majazi_coor++;
          x_coor++;
          if (x_majazi_coor  COLS-6) {
              y_majazi_coor++;
              x_majazi_coor = 0;
          }
          }
          checker++;
      }
      create_dollar_text();
      starting_program();
    } else if (x == KEY_F(1)) {
    now_mode++;
    now_mode %= 3;
    if (now_mode == 1) {
    first_visual = true;
    }
  } else if ((strcmp(mode[now_mode] , NORMAL) == 0  strcmp(mode[now_mode] , VISUAL) == 0)) {
  is_visual = strcmp(mode[now_mode] , VISUAL);
  if (first_visual) {
  first_selected = go_to_position(y_coor , x_coor);
  x_first_selected = x_coor;
  y_first_selected = y_coor;
  x_majazi_first_selected = x_majazi_coor;
  y_majazi_first_selected = y_majazi_coor;
  first_character_before_visual = first_character;
  first_line_before_visual = first_line;
  first_visual = false;
  }
  int now;
    if (x == KEY_LEFT  x == 'h') {
      if (x_majazi_coor  0) {
      x_coor--;
      x_majazi_coor--;
      }
    }else if (x == KEY_RIGHT  x == 'l') {
      if (x_majazi_coor  length_of_lines[y_coor]-1) {
    x_coor++;
    x_majazi_coor++;
    }
    }else if (x == KEY_DOWN  x == 'j') {
      if (y_majazi_coor  lines && y_coor  lines) {
    y_coor++;
    y_majazi_coor++;
    if (x_majazi_coor = length_of_lines[y_coor]-1) {
      x_coor = real_length_of_lines[y_coor]-1;
      x_majazi_coor = length_of_lines[y_coor]-1;
      }
      }
    }else if (x == KEY_UP  x == 'k') {
      if (y_majazi_coor  1) {
      y_majazi_coor--;
        y_coor--;
    if (x_majazi_coor = length_of_lines[y_coor]-1) {
      x_coor = real_length_of_lines[y_coor]-1;
      x_majazi_coor = length_of_lines[y_coor]-1;
      }
      }
    } else if (x == 'd' && is_visual == 0) {
    create_dot_text();
    is_saved = '-';
     left_shifting2();
     is_visual = true;
     now_mode = 0;
     do_visual = true;
     create_dollar_text();
    }
    else if (x == 'c' && is_visual == 0) {
    create_dot_text();
    is_saved = '-';
    is_cut = true;
    left_shifting2();
    is_cut = false;
    is_visual = true;
    now_mode = 0;
    do_visual = true;
    create_dollar_text();
    } else if (x == 'y' && is_visual == 0){
    first_visual = false;
    int counter = 0;
    for (int i = first_real_selected; i = second_real_selected; i++) {
    cb[counter] = text[i];
    counter++;
    }
    cb[counter] = '0';
    now_mode = 0;
    is_visual = 1;
    create_dollar_text();
    }
  }
  }
  }
  if (strcmp(mode[now_mode] , NORMAL) != 0)
      strcpy(command_args , command args);
  refresh();
  wrefresh(main_win);
}

int main() {
mkdir(root , 0770);
  initscr();
  strcpy(text , 0);
  starting_program();
  while (1) {
    process();
    starting_program();
  }
  getch();
  endwin();
}