#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sym_table.h"

void insert(char *symbol, int addr);
int find_or_insert(char *symbol, int addr);
void list();
void free_table();

/* A symbol table implemented with linked list */
typedef struct __symbol_impl sym_table;
struct __symbol_impl {
    char *symbol;
    int addr;
    sym_table *next;
};

sym_table *_head = NULL;
sym_table **tail = &_head;


void insert(char *symbol, int addr) {
    sym_table *new_node = malloc(sizeof(sym_table));
    new_node->symbol = malloc(sizeof(char) * (strlen(symbol) + 1));
    strcpy(new_node->symbol, symbol);
    new_node->addr = addr;
    new_node->next = NULL;
    *tail = new_node;
    tail = &(*tail)->next;
}


int find(char *symbol){
    sym_table **head = &_head;
    while (*head) {
        if (strcmp(symbol, (*head)->symbol) == 0) {
            return (*head)->addr;
        }
        head = &(*head)->next;
    }
    return -1;
}

int find_or_insert(char *symbol, int addr) {
    sym_table **head = &_head;
    while (*head) {
        if (strcmp(symbol, (*head)->symbol) == 0) {
            return (*head)->addr;
        }
        head = &(*head)->next;
    }
    insert(symbol, addr);
    return (*head)->addr;
}


void list() { /* Used for debug */
    sym_table **head = &_head;
    while (*head) {
        printf("%s\t%d\n", (*head)->symbol, (*head)->addr);
        head = &(*head)->next;
    }
}


void free_table() {
    sym_table **head = &_head;
    while (*head) {
        sym_table *tmp = *head;
        head = &(*head)->next;
        free(tmp);
    }
    tail = &_head;
}