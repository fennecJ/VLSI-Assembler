#ifndef _POSIX_C_SOURCE
	#define  _POSIX_C_SOURCE 200809L
#endif
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ins.h"

typedef struct _instruct{
   int op   :  4;
   int type :  4;
   int src  : 12;
   int dest : 12;
} instruct;

char **parse_line(char *line, int *cnt);
char *read_line(FILE *fi);
instruct get_all_val(char **args, int argsLen);

int main(){
    instruct ins = {0, -1, 1 ,5};
    FILE *fi = fopen("tst1", "r");
    FILE *fo = fopen("machineCode", "w");
    int len = 0;
    //printf("%d %d %d %d\n", ins.op, ins.type, ins.src, ins.dest);
    while (fi != NULL)
    {
        char *line = read_line(fi);
        char ** args = parse_line(line, &len);
        ins = get_all_val(args, len);
        fprintf(fo, PRINTF_BIN_INT4"_"PRINTF_BIN_INT4"_"PRINTF_BIN_INT12"_"PRINTF_BIN_INT12"\n",
              BIN_INT4(ins.op), BIN_INT4(ins.type), BIN_INT12(ins.src), BIN_INT12(ins.dest));
    }
    fclose(fi);
    fclose(fo);
    return 0;
}

#define PARSE_LINE_TOK " ,\n\t\a"
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
    if (!args) {
        fprintf(stderr, "Mem alloc failed in parse_line\n");
        exit(-1);
    }
    arg = strtok(line, PARSE_LINE_TOK);
    while(arg){
        args[arg_index++] = arg;
        arg = strtok(NULL, PARSE_LINE_TOK);
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
        } else {
        perror("readline");
        exit(-1);
        }
    }
  return line;
}

int extract_int(char *str){
    char *p = str;
    while (*p) {
        if (isdigit(*p) || *p == '-' || *p == '+') {
            return (int)strtol(p, &p, 10);
        } else {
            p++;
        }
    }
    fprintf(stderr, "Err when parsing %s\nNo int to be found.", str);
    exit(-1);
}

instruct get_all_val(char **args, int argsLen){
    instruct ins = {0};
    // Deal with op
    for(int i = 0; i < op_len; i++){
        if(strcmp(args[0], op_table[i]) == 0){
            ins.op = i;
            break;
        }
    }

    if(argsLen == 1){   //Must be HLT currently
        ins.type = -1;
        ins.src = -1;
        ins.dest = -1;
        return ins;
    }

    // Deal with dest
    ins.dest = extract_int(args[1]);

    // Deal with src
    if(strcmp(args[0], "BRA") == 0){
        for(int i = 0; i < cc_len; i++){
            if(strcmp(args[2], cc_table[i]) == 0){
                ins.type = i;
                break;
            }
        }
        ins.src = 0;
    }else{
        ins.type = (args[2][0] == '#') ? 8 : 0;
        ins.src = extract_int(args[2]);
    }
}
