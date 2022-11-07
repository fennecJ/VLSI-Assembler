#include "ins.h"


#define OP_LEN sizeof(op_table)/sizeof(char *)
char *op_table[] = {
    "NOP" , "BRA",
    "LD"  , "STR",
    "ADD" , "MUL",
    "CMP" , "SHF",
    "ROT" , "HLT",
    "EXT1", 
};
int op_len = OP_LEN;

#define CC_LEN sizeof(cc_table)/sizeof(char *)
char *cc_table[] = {"ALW", "CARRY", "EVEN", "PARITY", "ZERO", "NEG"};
int cc_len = CC_LEN;