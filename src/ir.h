#ifndef IR_H
#define IR_H

#include "ast.h"
#include "symbol_table.h"

/* Three-Address Code (TAC) Types */
typedef enum {
    TAC_ASSIGN,
    TAC_BINARY_OP,
    TAC_UNARY_OP,
    TAC_COPY,
    TAC_LABEL,
    TAC_GOTO,
    TAC_IF_GOTO,
    TAC_PARAM,
    TAC_CALL,
    TAC_RETURN,
    TAC_ARRAY_READ,
    TAC_ARRAY_WRITE,
    TAC_INPUT,
    TAC_OUTPUT
} TACType;

/* TAC Instruction */
typedef struct TACInstruction {
    TACType type;
    char *result;
    char *arg1;
    char *arg2;
    char *op;
    struct TACInstruction *next;
} TACInstruction;

/* TAC Generator Context */
typedef struct {
    TACInstruction *head;
    TACInstruction *tail;
    int temp_count;
    int label_count;
} TACContext;

/* ═══════════════════════════════════════════════════════════════════════════
 * TAC GENERATION FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Initialize TAC context */
void tac_init(TACContext *ctx);

/* Free TAC context */
void tac_free(TACContext *ctx);

/* Generate TAC from AST */
int tac_generate(ASTNode *root, TACContext *ctx, SymbolTable *table);

/* Generate TAC for expression (returns temporary variable name) */
char *tac_gen_expr(ASTNode *expr, TACContext *ctx, SymbolTable *table);

/* Generate TAC for statement */
void tac_gen_stmt(ASTNode *stmt, TACContext *ctx, SymbolTable *table);

/* Write TAC to file */
int tac_write_to_file(TACContext *ctx, const char *filename);

/* Print TAC to stdout */
void tac_print(TACContext *ctx);

/* ═══════════════════════════════════════════════════════════════════════════
 * TAC OPTIMIZATION
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Perform constant folding */
void tac_optimize_constant_folding(TACContext *ctx);

/* Perform strength reduction */
void tac_optimize_strength_reduction(TACContext *ctx);

/* Perform algebraic simplification */
void tac_optimize_algebraic(TACContext *ctx);

/* Run all optimizations */
void tac_optimize(TACContext *ctx);

/* ═══════════════════════════════════════════════════════════════════════════
 * HELPER FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Generate new temporary variable */
char *tac_new_temp(TACContext *ctx);

/* Generate new label */
char *tac_new_label(TACContext *ctx);

/* Append instruction to TAC list */
void tac_append(TACContext *ctx, TACInstruction *instr);

#endif /* IR_H */
