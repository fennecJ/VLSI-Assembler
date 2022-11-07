#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ins.c"
char **parse_line(char *line, int *cnt);
char *read_line(FILE *fi);

int main(){
    FILE *fi = fopen("tst1", "r");
    int len = 0;
    while (fi != NULL)
    {
        char *line = read_line(fi);
        char ** args = parse_line(line, &len);
        for(int i = 0; i < len; i++)
            printf("%s", args[i]);
            printf("\n");
    }
}

#define PARSE_LINE_TOK " ,"
/**
 * @brief Input a string, seperate all args by comma and space. Return 
 * ptr of char ptr with array of args.
 * 
 * @param line 
 * @param cnt 
 * @return char** 
 */
char **parse_line(char *line, int *cnt){
    int args_cnt = 0;
    int arg_index = 0;
    char **args = calloc(4, sizeof(char *)); //No longer than three args
    char *arg = NULL;
    char *remained_args = NULL;
    if (!args) {
        fprintf(stderr, "Mem alloc failed in parse_line\n");
        exit(-1);
    }
    arg = strtok_r(line, PARSE_LINE_TOK, &remained_args);
    while(arg){
        args[arg_index++] = arg;
        arg = strtok_r(NULL, PARSE_LINE_TOK, &remained_args);
    }
    *cnt = arg_index;
    return args;
}

/**
 * @brief Allocate and read in a line from specific file stream fi.
 * 
 * @return char* 
 */
char *read_line(FILE *fi){
    char *line = NULL;
    ssize_t bufsize = 0; 
    if (getline(&line, &bufsize, fi) == -1){
        if (feof(fi)) {
            exit(-1);
        } else  {
        perror("readline");
        exit(-1);
        }
    }
  return line;
}
