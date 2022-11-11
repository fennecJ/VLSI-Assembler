#include "ins.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define OP_LEN sizeof(op_table)/sizeof(char *)
char *op_table[] = {
    "NOP" , "BRA",
    "LD"  , "STR",
    "ADD" , "MUL",
    "CMP" , "SHF",
    "ROT" , "HLT",
    "EXT1", 
};

#define CC_LEN sizeof(cc_table)/sizeof(char *)
char *cc_table[] = {"ALW", "CARRY", "EVEN", "PARITY", "ZERO", "NEG"};
char *alt_cc_table[] = {"CCA", "CCC", "CCE", "CCP", "CCZ", "CCN"};

int op_find(char *str){
    if(!str)
        perror("op_find");
    for(int i = 0; i < OP_LEN; i++)
        if(strcmp(str, op_table[i]) == 0)
            return i;
    return -1;
}

int cc_find(char *str){
    if(!str)
        perror("cc_find");
    for(int i = 0; i < CC_LEN; i++)
        if(strcmp(str, cc_table[i]) == 0 || strcmp(str, alt_cc_table[i]) == 0)
            return i;
    fprintf(stderr ,"Condition code not found: %s\n", str);
    exit(-1);
}