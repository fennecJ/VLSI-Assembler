#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sym_table.h"

void sym_insert(char *symbol, int addr);
int  sym_find_or_insert(char *symbol, int addr);
void sym_list();
void sym_free_table();

/* A symbol table implemented with linked list */
typedef struct __symbol_impl sym_table;
struct __symbol_impl {
    char *symbol;
    int addr;
    sym_table *next;
};

static sym_table *_head = NULL;
static sym_table **tail = &_head;


void sym_insert(char *symbol, int addr) {
    sym_table *new_node = malloc(sizeof(sym_table));
    new_node->symbol = malloc(sizeof(char) * (strlen(symbol) + 1));
    strcpy(new_node->symbol, symbol);
    new_node->addr = addr;
    new_node->next = NULL;
    *tail = new_node;
    tail = &(*tail)->next;
}


int sym_find(char *symbol){
    sym_table **head = &_head;
    while (*head) {
        if (strcmp(symbol, (*head)->symbol) == 0) {
            return (*head)->addr;
        }
        head = &(*head)->next;
    }
    return -1;
}

int sym_find_or_insert(char *symbol, int addr) {
    sym_table **head = &_head;
    while (*head) {
        if (strcmp(symbol, (*head)->symbol) == 0) {
            return (*head)->addr;
        }
        head = &(*head)->next;
    }
   sym_insert(symbol, addr);
    return (*head)->addr;
}


void sym_list() { /* Used for debug */
    sym_table **head = &_head;
    while (*head) {
        printf("%s\t%d\n", (*head)->symbol, (*head)->addr);
        head = &(*head)->next;
    }
}


void sym_free_table() {
    sym_table **head = &_head;
    while (*head) {
        sym_table *tmp = *head;
        head = &(*head)->next;
        free(tmp);
    }
    tail = &_head;
}