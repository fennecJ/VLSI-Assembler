#ifndef _POSIX_C_SOURCE
	#define  _POSIX_C_SOURCE 200809L
#endif  //For readline
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "sym_table.h"
#include "ins.h"

/**
 * @brief A simple 2-parse assembler for cpu used in course VLSI ckt design.
 *  The program process the file 2 times. Support label and one-line comment
 *  token "//"
 * 
 *  1-st time: Store all labels into sym_table and save asm without label and 
 *  comments into tmp file __TMP_ASM_PARSE_FILE
 * 
 *  2-nd time: Replace labels from tmp file and convert asm to machine code.
 *  This prog have little err detection/handler mechnism, user must assure the asm
 *  file is correct.
 * 
 * @author fennecJ
 * @date 2022/11/10
 */

/**
 * @brief Use bitfield to split the machine code into 4 parts
 */
typedef struct _instruct{
   int op   :  4;
   int type :  4;
   int src  : 12;
   int dest : 12;
} instruct;

char **parse_line(char *line, int *cnt);
instruct set_all_field(char **args, int argsLen);
bool isLabel(char *arg);
char **splitColon(char* arg);

int main(int argc, char* argv[]){
    FILE *fi = fopen(argv[1], "r");
    FILE *fo = fopen(argv[2], "w");
    FILE *fo1 = fopen("__TMP_ASM_PARSE_FILE", "w");

    size_t bufsize = 0;
    char *line = NULL;
    instruct ins = {0};
    int len = 0;

    if(argc != 3){
        printf("Usage: %s asm_file_path machineCode_save_path\n", argv[0]);
        exit(-1);
    }

    if(fi == NULL){
            printf("Failed to open file: %s\n", argv[1]);
        }

    /* 1st parse: store symbol to symbol table */
    int addr = 0;
    while (getline(&line, &bufsize, fi) != -1){
        char **args = parse_line(line, &len);
        //args[0] will be NULL if parse_line meet blank line, skip blank line by continue.
        if(!args[0]) continue;
        if(strncmp(args[0], "//", 2) == 0) continue; // If detect comment in begining, skip

        int i = 0;
        if(isLabel(args[0])){
            char **labAndOp = splitColon(args[0]);
            sym_insert(labAndOp[0], addr);
            fprintf(fo1, "%s ", labAndOp[1]);
            i = 1; //Skip label so it won't be printed into tmp parse file
        }

        while (args[i] && i < 3){   // args[3] Must be comment
            if(strncmp(args[i], "//", 2) == 0) break;
            fprintf(fo1, "%s ", args[i++]);
        }
        free(args);
        fprintf(fo1, "\n");
        addr++;
    }
    //sym_list(); //List the symbol table
    fclose(fi);
    fclose(fo1);
    
    /* 2nd parse: replace symbol from symbol table */
    fi = fopen("__TMP_ASM_PARSE_FILE", "r");
    while (getline(&line, &bufsize, fi) != -1){
        char **args = parse_line(line, &len);
        ins = set_all_field(args, len);
        fprintf(fo, PRINTF_BIN_INT4"_"PRINTF_BIN_INT4"_"PRINTF_BIN_INT12"_"PRINTF_BIN_INT12"\n",
              BIN_INT4(ins.op), BIN_INT4(ins.type), BIN_INT12(ins.src), BIN_INT12(ins.dest));
        free(args);
    }

    free(line);
    sym_free_table();
    fclose(fi);
    fclose(fo);
    return 0;
}

#define PARSE_LINE_TOK " ,\n\t\a\r"
/**
 * @brief Input a string, seperate all args by comma and space. Return 
 * ptr of char ptr with array of args.
 * 
 * @param line 
 * @param cnt 
 * @return char** 
 */
char **parse_line(char *line, int *cnt){
    int arg_index = 0;
    char **args = calloc(4, sizeof(char *)); //No longer than four args (INS DEST, SRC COMMENT)
    char *arg = NULL;
    if (!args) {
        fprintf(stderr, "Mem alloc failed in parse_line\n");
        exit(-1);
    }
    arg = strtok(line, PARSE_LINE_TOK);
    while(arg && arg_index < 4){
        args[arg_index++] = arg;
        arg = strtok(NULL, PARSE_LINE_TOK);
    }
    *cnt = arg_index;
    return args;
}


/**
 * @brief Extract value of label or string
 * If is label: return lable addr
 * Else: return int val finded in given string
 * 
 * @param str 
 * @return int 
 */
int extract_int(char *str){
    int lab_val = sym_find(str);
    if(lab_val != -1)
        return lab_val;

    /* Extract int only from given string
       e.g. aa12bb -> 12
            R12 -> 12
            #8 -> 8
    */
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


/**
 * @brief Set up all bit-fields of instruction by given args(asm str array)
 * 
 * @param args 
 * @param argsLen 
 * @return instruct 
 */
instruct set_all_field(char **args, int argsLen){
    instruct ins = {0};
    // Deal with op
    ins.op = op_find(args[0]);

    if(ins.op == 0)     //NOP
        return ins;
    if(argsLen == 1){
        int tmp = 0;
        if(strlen(args[0]) == 8){   // Set value via label
            sscanf(args[0], "%x", &tmp);
            ins.op   = (tmp >> 28) & 0xf;
            ins.type = (tmp >> 24) & 0xf;
            ins.src  = (tmp >> 12) & 0xfff;
            ins.dest = (tmp) & 0xfff;
        }else{                      // HLT
            ins.type = -1;
            ins.src = -1;
            ins.dest = -1;
        }
        return ins;
    }

    // Deal with dest
    ins.dest = extract_int(args[1]);

    // Deal with src
    if(strcmp(args[0], "BRA") == 0){
        ins.type = cc_find(args[2]);
        ins.src = 0;
    }else{
        ins.type = (args[2][0] == '#') ? 8 : 0;
        ins.src = extract_int(args[2]);
    }
    return ins;
}


/**
 * @brief Test if given arg contains label by colon
 * 
 * @param arg 
 * @return true 
 * @return false 
 */
bool isLabel(char *arg){
    int i = 0;
    while (arg[i]){
        if(arg[i++] == ':')
            return true;
    }
    return false;
}


/**
 * @brief Split given string by colon
 * E.g arg = "AB:CD" -> return = {"AB", "CD"} (split by colon)
 * 
 * @param arg 
 * @return char** 
 */
char **splitColon(char *arg){
    char **res = calloc(2, sizeof(char*));
    if (!res) {
        fprintf(stderr, "Mem alloc failed in splitColon\n");
        exit(-1);
    }
    int ind = 0;
    char *tmp = NULL;

    tmp = strtok(arg, ":");
    while(tmp){
        res[ind++] = tmp;
        tmp = strtok(NULL, ":");
    }

    /* There must be exactly 1 colon in label, thus the 
       result will be cut into exactly 2 sub str.
    */
    if(ind != 2){
        fprintf(stderr, "splitColon found err with label: %s\nPlease make sure " \
        "the label is followed by colon with instruction WITHOUT space\n", res[0]);
        exit(-1);
    }
    return res;
}