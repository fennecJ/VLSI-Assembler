struct __covert_table {
    char *ins;
    char *bin;
};

struct __covert_table op_table[12] = {
    {.ins = "NOP",  .bin = "0000"}, {.ins = "BRA",  .bin = "0001"},
    {.ins = "LD",   .bin = "0010"}, {.ins = "STR",  .bin = "0011"},
    {.ins = "ADD",  .bin = "0100"}, {.ins = "MUL",  .bin = "0101"},
    {.ins = "CMP",  .bin = "0110"}, {.ins = "SHF",  .bin = "0111"},
    {.ins = "ROT",  .bin = "1000"}, {.ins = "HLT",  .bin = "1001"},
    {.ins = "EXT1", .bin = "1010"}, {.ins = "EXT2", .bin = "1011"},
};