/* Jessica Theofilia 894476 */
/* dsh.c */
/* Estensione 3: set e echo */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 4096
#define MAX_ARGS 256
#define MAX_VAR_VALUE 512
#define MAX_PROMPT 32
#define MAX_VAR 512
#define MAX_VAR_LENGTH 32

char _path[MAX_VAR_VALUE] = "/bin/:/usr/bin";
const char* _variable[MAX_VAR];

// Cancella le variabili allocate (AGGIUNTIVO)
void clear() {
    for(size_t i = 0; i < MAX_VAR; i++)
        if(_variable[i] != NULL) {
            free((char*) _variable[i]);
            _variable[i] = NULL;
        }
}

void panic(const char* msg) {
    if(errno) {
        fprintf(stderr, "PANIC: %s: %s\n\n", msg, strerror(errno));
    } else {
        fprintf(stderr, "PANIC: %s\n\n", msg);
    }
    clear();
    exit(EXIT_FAILURE);
}

int prompt(char* buf, size_t buf_size, const char* prompt_string) {
    printf("%s ", prompt_string);
    if(fgets(buf, buf_size, stdin) == NULL) {
        return EOF;
    }
    size_t cur = -1;
    do {
        cur++;
        if(buf[cur] == '\n') {
            buf[cur] = '\0';
            break;
        }
    } while(buf[cur] != '\0');
    return cur;
}

// Implementazione del set (ESERCIZIO)
void set(const char* new_var, const char* new_value) {
    int cur_pos;
    const char* temp;
    if(new_var != NULL && new_value != NULL) {
        if(new_value == NULL) {
            panic("set: error");
        }
        // Sostituiamo la variabile
        if(strcmp(new_var, "PATH") == 0) {
#if USE_DEBUG_PRINTF
            printf("DEBUG: new_path: %s\n", new_value);
#endif
            cur_pos = 0;
            while(new_value[cur_pos] != '\0') {
                cur_pos++;
                if(cur_pos >= MAX_VAR_VALUE - 1 && new_value[cur_pos] != '\0') {
                    fprintf(stderr, "Error: PATH string too long\n");
                    return;
                }
            }
            if(cur_pos > 0)
                memcpy(_path, new_value, cur_pos + 1);

            printf("%s\n", _path);
        } else {
#if USE_DEBUG_PRINTF
            printf("DEBUG: new_var: %s\n", new_var);
#endif  
            cur_pos = 0;
            while(new_var[cur_pos] != '\0') {
                cur_pos++;
                if(cur_pos >= MAX_VAR_LENGTH && new_var[cur_pos] != '\0') {
                    fprintf(stderr, "Error: VARIABLE NAME string too long\n");
                    return;
                }
            }
            int var_pos = -1;
            for(int i = 0; i < MAX_VAR; i=i+2) {
                temp = _variable[i];
                if(temp != NULL && strcmp(temp, new_var) == 0) {
                    var_pos = i;
                    break;
                }
            }
            if(var_pos != -1) {
                free((char*) _variable[var_pos+1]);
                char* value = (char*) malloc (sizeof(char)*(strlen(new_value) + 1));
                memcpy(value, new_value, sizeof(char)*(strlen(new_value) + 1));
                _variable[var_pos+1] = value;
                printf("%s\n", value);
            } else {
                for(int i = 0; i < MAX_VAR; i=i+2) {
                    if(_variable[i] == NULL) {
                        char* var = (char*) malloc (sizeof(char)*(strlen(new_var) + 1));
                        memcpy(var, new_var, sizeof(char)*(strlen(new_var) + 1));
                        _variable[i] = var;
                        char* value = (char*) malloc (sizeof(char)*(strlen(new_value) + 1));
                        memcpy(value, new_value, sizeof(char)*(strlen(new_value) + 1));
                        _variable[i+1] = value;
                        printf("%s\n", value);
                        break;
                    }
                }
            }
        }       
    } else if (new_var != NULL && new_value == NULL){ 
        cur_pos = 0;
            while(new_var[cur_pos] != '\0') {
                cur_pos++;
                if(cur_pos >= MAX_VAR_LENGTH && new_var[cur_pos] != '\0') {
                    fprintf(stderr, "Error: VARIABLE NAME string too long\n");
                    return;
                }
            }
            int var_pos = -1;
            for(int i = 0; i < MAX_VAR; i=i+2) {
                temp = _variable[i];
                if(temp != NULL && strcmp(temp, new_var) == 0) {
                    var_pos = i;
                    break;
                }
            }
            if(var_pos != -1) {
                free((char*) _variable[var_pos]);
                free((char*) _variable[var_pos+1]);
                _variable[var_pos] = NULL;
                _variable[var_pos+1] = NULL;
            }           
    } else {
        panic("set: variable error");
    }
}

// Implementazione di echo (ESERCIZIO)
void echo(const char* var_name) {
    if(var_name != NULL) {
        const char* var = var_name + 1;
        int var_pos = -1;
        if(var_name[0] == '$') {

            if(strcmp(var, "PATH") == 0) {
                printf("%s\n", _path);
                return;
            }

            for(int i = 0; i < MAX_VAR; i=i+2) {
                if(_variable[i] != NULL && strcmp(_variable[i], var) == 0) {
                    var_pos = i;
                    break;
                }
            }

            if(var_pos != -1)
                printf("%s\n", _variable[var_pos+1]);
            else {
                panic("echo: variable error");
            }
        } else if ((var_name[0] == '$' && var[0] == '\0') || var_name[0] != '$'){
            fprintf(stderr, "Error: VARIABLE NAME is not valid\n");
                return;
        }
    } else {
        panic("echo: command error");
    }
}

void path_lookup(char* abs_path, const char* rel_path) {
    char* prefix;
    char buf[MAX_VAR_VALUE];
    if(abs_path == NULL || rel_path == NULL)
        panic("get_abs_path: parameter error");
    prefix = strtok(_path, ":");
    while(prefix != NULL) {
        strcpy(buf, prefix);
        strcat(buf, rel_path);
        if(access(buf, X_OK) == 0) {
            strcpy(abs_path, buf);
            return;
        }
        prefix = strtok(NULL, ":");
    }
    strcpy(abs_path, rel_path);
}

void exec_rel2abs(char** arg_list) {
    if(arg_list[0][0] == '/') {
        execv(arg_list[0], arg_list);
    } else {
        char abs_path[MAX_VAR_VALUE];
        path_lookup(abs_path, arg_list[0]);
        execv(abs_path, arg_list);
    }
}

void do_redir(const char* out_path, char** arg_list, const char* mode) {
    if(out_path == NULL)
        panic("do_redir: no path");
    int pid = fork();
    if(pid > 0) {
        int wpid = wait(NULL);
        if (wpid < 0) panic("do_redir: wait");
    } else if(pid == 0) {
        FILE* out = fopen(out_path, mode); 
        if(out == NULL) {
            perror(out_path);
            exit(EXIT_FAILURE);
        }
        dup2(fileno(out), 1);
        exec_rel2abs(arg_list);
        perror(arg_list[0]);
        exit(EXIT_FAILURE);
    } else {
        panic("do_redir: fork");
    }
}

void do_pipe(size_t pipe_pos, char** arg_list) {
    int pipefd[2];
    int pid;
    if(pipe(pipefd) < 0) panic("do_pipe: pipe");
    pid = fork();
    if(pid > 0) {
        int wpid = wait(NULL);
        if (wpid < 0) panic("do_pipe: wait");
    } else if(pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        exec_rel2abs(arg_list);
        perror(arg_list[0]);
        exit(EXIT_FAILURE);
    } else {
        panic("do_pipe: fork");
    }
    pid = fork();
    if(pid > 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        int wpid = wait(NULL);
        if (wpid < 0) panic("do_pipe: wait");
    } else if(pid == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        exec_rel2abs(arg_list + pipe_pos + 1);
        perror(arg_list[pipe_pos + 1]);
        exit(EXIT_FAILURE);
    } else {
        panic("do_pipe: fork");
    }
}
 
void do_exec(char** arg_list) {
    int pid = fork();
    if(pid > 0) {
        int wpid = wait(NULL);
        if (wpid < 0) panic("do_exec: wait");
    } else if(pid == 0) {
        exec_rel2abs(arg_list);
        perror(arg_list[0]);
        exit(EXIT_FAILURE);
    } else {
        panic("do_exec: fork");
    }
}

int main(void) {
    char input_buffer[MAX_LINE];
    size_t arg_count;
    char* arg_list[MAX_ARGS];
    char prompt_string[MAX_PROMPT] = "\0";
    for(int i = 0; i < MAX_VAR; i++) 
        _variable[i] = NULL;
    if(isatty(0)) {
        strcpy(prompt_string, "dsh$ \0"); 
    }
    while(prompt(input_buffer, MAX_LINE, prompt_string) >= 0) {
        arg_count = 0;
        arg_list[arg_count] = strtok(input_buffer, " ");
        if(arg_list[arg_count] == NULL) {
            continue;
        } else {
            do {
                arg_count++;
                if(arg_count > MAX_ARGS) break;
                arg_list[arg_count] = strtok(NULL, " ");
            } while(arg_list[arg_count] != NULL);
        } 
#if USE_DEBUG_PRINTF
        // [DEBUG] print tokens
        printf("DEBUG: tokens:");
        for(size_t i = 0; i < arg_count; i++) {
            printf(" %s", arg_list[i]);
        }
        puts("");
#endif
        if(strcmp(arg_list[0], "exit") == 0) {
            break;
        }
        if(strcmp(arg_list[0], "set") == 0) {
            set(arg_list[1], arg_list[2]);
            continue;
        }
        if(strcmp(arg_list[0], "echo") == 0) {
            echo(arg_list[1]);
            continue;
        }
        {   
            size_t redir_pos = 0;
            size_t append_pos = 0;
            size_t pipe_pos = 0;
            for(size_t i = 0; i < arg_count; i++) {
                if(strcmp(arg_list[i], ">") == 0) {
                    redir_pos = i;
                    break;
                }
                if(strcmp(arg_list[i], ">>") == 0) {
                    append_pos = i;
                    break;
                }
                if(strcmp(arg_list[i], "|") == 0) {
                    pipe_pos = i;
                    break;
                }
            }
            if(redir_pos != 0) {
                arg_list[redir_pos] = NULL;
                do_redir(arg_list[redir_pos+1], arg_list, "w+");
            } else if(append_pos != 0) {
                arg_list[append_pos] = NULL;
                do_redir(arg_list[append_pos+1], arg_list, "a+");
            } else if(pipe_pos != 0) {
                arg_list[pipe_pos] = NULL;
                do_pipe(pipe_pos, arg_list);
            } else {
                do_exec(arg_list);
            }
        }
    }
    clear();
    exit(EXIT_SUCCESS);
 }