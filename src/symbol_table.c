#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 100

/* ═══════════════════════════════════════════════════════════════════════════
 * SYMBOL TABLE IMPLEMENTATION
 * ═══════════════════════════════════════════════════════════════════════════ */

void symbol_table_init(SymbolTable *table) {
    table->symbols = (Symbol *)calloc(INITIAL_CAPACITY, sizeof(Symbol));
    if (!table->symbols) {
        fprintf(stderr, "Fatal: Memory allocation failed for symbol table\n");
        exit(1);
    }
    table->count = 0;
    table->capacity = INITIAL_CAPACITY;
    table->current_scope = 0;
}

void symbol_table_free(SymbolTable *table) {
    if (table->symbols) {
        free(table->symbols);
        table->symbols = NULL;
    }
    table->count = 0;
    table->capacity = 0;
}

static void ensure_capacity(SymbolTable *table) {
    if (table->count >= table->capacity) {
        int new_capacity = table->capacity * 2;
        Symbol *new_symbols = (Symbol *)realloc(table->symbols, 
                                                new_capacity * sizeof(Symbol));
        if (!new_symbols) {
            fprintf(stderr, "Fatal: Memory reallocation failed for symbol table\n");
            exit(1);
        }
        table->symbols = new_symbols;
        table->capacity = new_capacity;
        memset(&table->symbols[table->count], 0, 
               (new_capacity - table->count) * sizeof(Symbol));
    }
}

Symbol *symbol_table_lookup(SymbolTable *table, const char *name) {
    if (!name) return NULL;
    
    /* Search from most recent to oldest (respecting scope) */
    for (int i = table->count - 1; i >= 0; i--) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return &table->symbols[i];
        }
    }
    return NULL;
}

Symbol *symbol_table_lookup_current_scope(SymbolTable *table, const char *name) {
    if (!name) return NULL;
    
    /* Search only in current scope */
    for (int i = table->count - 1; i >= 0; i--) {
        if (table->symbols[i].scope < table->current_scope) {
            break;  /* Reached outer scope */
        }
        if (strcmp(table->symbols[i].name, name) == 0) {
            return &table->symbols[i];
        }
    }
    return NULL;
}

int symbol_table_insert(SymbolTable *table, const char *name, const char *type,
                        int is_function, int is_array, int array_size, int line) {
    if (!name || !type) return 0;
    
    /* Check for redeclaration in current scope */
    Symbol *existing = symbol_table_lookup_current_scope(table, name);
    if (existing) {
        fprintf(stderr, "Error at line %d: Redeclaration of '%s' (first declared at line %d)\n",
                line, name, existing->line_declared);
        return 0;
    }
    
    ensure_capacity(table);
    
    Symbol *sym = &table->symbols[table->count];
    strncpy(sym->name, name, sizeof(sym->name) - 1);
    sym->name[sizeof(sym->name) - 1] = '\0';
    
    strncpy(sym->type, type, sizeof(sym->type) - 1);
    sym->type[sizeof(sym->type) - 1] = '\0';
    
    sym->scope = table->current_scope;
    sym->is_function = is_function;
    sym->is_array = is_array;
    sym->array_size = array_size;
    sym->is_initialized = 0;
    sym->line_declared = line;
    
    table->count++;
    return 1;
}

void symbol_table_enter_scope(SymbolTable *table) {
    table->current_scope++;
}

void symbol_table_exit_scope(SymbolTable *table) {
    if (table->current_scope == 0) {
        fprintf(stderr, "Warning: Attempting to exit global scope\n");
        return;
    }
    
    /* Remove all symbols in current scope */
    int new_count = 0;
    for (int i = 0; i < table->count; i++) {
        if (table->symbols[i].scope < table->current_scope) {
            if (i != new_count) {
                table->symbols[new_count] = table->symbols[i];
            }
            new_count++;
        }
    }
    table->count = new_count;
    table->current_scope--;
}

void symbol_table_print(SymbolTable *table) {
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("  SYMBOL TABLE  (Current Scope: %d)\n", table->current_scope);
    printf("═══════════════════════════════════════════════════════════\n");
    printf("%-20s %-10s %-8s %-8s %-8s %-6s\n", 
           "Name", "Type", "Scope", "Function", "Array", "Line");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < table->count; i++) {
        Symbol *sym = &table->symbols[i];
        printf("%-20s %-10s %-8d %-8s %-8s %-6d\n",
               sym->name,
               sym->type,
               sym->scope,
               sym->is_function ? "Yes" : "No",
               sym->is_array ? "Yes" : "No",
               sym->line_declared);
    }
    printf("═══════════════════════════════════════════════════════════\n\n");
}

int symbol_table_get_scope(SymbolTable *table) {
    return table->current_scope;
}
