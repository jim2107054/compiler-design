#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "types.h"

/* Symbol Table Entry */
typedef struct Symbol {
    char name[256];
    char type[32];          /* "int", "float", "char", "double", "void" */
    int scope;
    int is_function;
    int is_array;
    int array_size;
    int is_initialized;
    int line_declared;
} Symbol;

/* Symbol Table Structure */
typedef struct SymbolTable {
    Symbol *symbols;
    int count;
    int capacity;
    int current_scope;
} SymbolTable;

/* ═══════════════════════════════════════════════════════════════════════════
 * SYMBOL TABLE FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Initialize symbol table */
void symbol_table_init(SymbolTable *table);

/* Free symbol table memory */
void symbol_table_free(SymbolTable *table);

/* Lookup symbol by name in current or outer scopes */
Symbol *symbol_table_lookup(SymbolTable *table, const char *name);

/* Lookup symbol only in current scope */
Symbol *symbol_table_lookup_current_scope(SymbolTable *table, const char *name);

/* Insert a new symbol */
int symbol_table_insert(SymbolTable *table, const char *name, const char *type,
                        int is_function, int is_array, int array_size, int line);

/* Enter a new scope */
void symbol_table_enter_scope(SymbolTable *table);

/* Exit current scope */
void symbol_table_exit_scope(SymbolTable *table);

/* Print symbol table for debugging */
void symbol_table_print(SymbolTable *table);

/* Get current scope level */
int symbol_table_get_scope(SymbolTable *table);

#endif /* SYMBOL_TABLE_H */
