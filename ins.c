#define PRINTF_BIN_INT4 "%c%c%c%c"
#define BIN_INT4(i)   \
    (((i) & 0x08) ? '1' : '0'),       \
    (((i) & 0x04) ? '1' : '0'),       \
    (((i) & 0x02) ? '1' : '0'),       \
    (((i) & 0x01) ? '1' : '0')

#define PRINTF_BIN_INT8 \
    PRINTF_BIN_INT4 "_" PRINTF_BIN_INT4
#define BIN_INT8(i) \
    BIN_INT4((i) >> 4),   BIN_INT4(i)

#define PRINTF_BIN_INT12 \
    PRINTF_BIN_INT8 "_" PRINTF_BIN_INT4
#define BIN_INT12(i) \
    BIN_INT8((i) >> 4),   BIN_INT4(i)


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


