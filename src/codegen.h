#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symbol_table.h"
#include "ir.h"
#include <stdio.h>

/* Code Generation Context */
typedef struct {
    FILE *output;
    int indent_level;
    SymbolTable *symbol_table;
} CodeGenContext;

/* ═══════════════════════════════════════════════════════════════════════════
 * CODE GENERATION FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Initialize code generation context */
void codegen_init(CodeGenContext *ctx, FILE *output, SymbolTable *table);

/* Generate C code from AST */
int codegen_generate(ASTNode *root, const char *output_filename, SymbolTable *table);

/* Generate code from TAC */
int codegen_from_tac(TACContext *tac_ctx, const char *output_filename, SymbolTable *table);

/* Generate expression code */
void codegen_expr(ASTNode *expr, CodeGenContext *ctx);

/* Generate statement code */
void codegen_stmt(ASTNode *stmt, CodeGenContext *ctx);

/* Generate declaration code */
void codegen_declaration(ASTNode *decl, CodeGenContext *ctx);

/* ═══════════════════════════════════════════════════════════════════════════
 * HELPER FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Print indentation */
void codegen_indent(CodeGenContext *ctx);

/* Increase indentation level */
void codegen_indent_inc(CodeGenContext *ctx);

/* Decrease indentation level */
void codegen_indent_dec(CodeGenContext *ctx);

/* Map SMPL operator to C operator */
const char *codegen_map_operator(const char *smpl_op);

/* Get printf format string for type */
const char *codegen_printf_format(const char *type);

/* Get scanf format string for type */
const char *codegen_scanf_format(const char *type);

#endif /* CODEGEN_H */
