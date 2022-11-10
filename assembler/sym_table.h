#ifndef _SYM_TABLE_H
#define _SYM_TABLE_H
/**
 * @brief Insert symbol into sym table
 * 
 * @param symbol 
 * @param addr 
 */
void sym_insert(char *symbol, int addr);


/**
 * @brief Find symbol in sym table, return addr of 
 * symbol if found. Else return -1.
 * 
 * @param symbol 
 * @return int 
 */
int sym_find(char *symbol);


/**
 * @brief Find symbol in sym table, return addr of 
 * symbol if found. Else insert symbol to sym table and
 * return symbol addr
 * 
 * @param symbol 
 * @param addr 
 * @return int 
 */
int sym_find_or_insert(char *symbol, int addr);


/**
 * @brief List all elements in table
 * 
 */
void sym_list();


/**
 * @brief Free allocated mem of sym table
 * 
 */
void sym_free_table();
#endif // _SYM_TABLE_H