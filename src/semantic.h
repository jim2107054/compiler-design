#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

/* Execution/Analysis Result */
typedef struct {
    int success;
    int error_count;
    int warning_count;
} AnalysisResult;

/* Runtime Value (for interpretation) */
typedef struct {
    ValueType type;
    union {
        int int_val;
        float float_val;
        double double_val;
        char char_val;
        char *string_val;
    } data;
    int valid;
} RuntimeValue;

/* ═══════════════════════════════════════════════════════════════════════════
 * SEMANTIC ANALYSIS FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Perform semantic analysis on the AST */
AnalysisResult semantic_analyze(ASTNode *root);

/* Type check the entire AST */
void semantic_type_check(ASTNode *node, SymbolTable *table);

/* Check variable declarations and usage */
void semantic_check_variables(ASTNode *node, SymbolTable *table);

/* Check function declarations and calls */
void semantic_check_functions(ASTNode *node, SymbolTable *table);

/* ═══════════════════════════════════════════════════════════════════════════
 * RUNTIME INTERPRETATION (Execute SMPL programs)
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Execute/interpret the AST (runtime interpreter) */
AnalysisResult semantic_execute(ASTNode *root);

/* Create runtime value */
RuntimeValue runtime_value_create(ValueType type, int valid);

/* Convert runtime value to boolean */
int runtime_value_to_bool(RuntimeValue val);

#endif /* SEMANTIC_H */
