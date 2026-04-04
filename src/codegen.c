#include "codegen.h"
#include "types.h"
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * CODE GENERATION CONTEXT
 * ═══════════════════════════════════════════════════════════════════════════ */

void codegen_init(CodeGenContext *ctx, FILE *output, SymbolTable *table) {
    ctx->output = output;
    ctx->indent_level = 0;
    ctx->symbol_table = table;
}

void codegen_indent(CodeGenContext *ctx) {
    for (int i = 0; i < ctx->indent_level; i++) {
        fprintf(ctx->output, "    ");
    }
}

void codegen_indent_inc(CodeGenContext *ctx) {
    ctx->indent_level++;
}

void codegen_indent_dec(CodeGenContext *ctx) {
    if (ctx->indent_level > 0) {
        ctx->indent_level--;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * OPERATOR MAPPING
 * ═══════════════════════════════════════════════════════════════════════════ */

const char *codegen_map_operator(const char *smpl_op) {
    if (!smpl_op) return "";
    
    /* Arithmetic operators */
    if (strcmp(smpl_op, "COMBINE") == 0) return "+";
    if (strcmp(smpl_op, "REDUCE") == 0) return "-";
    if (strcmp(smpl_op, "AMPLIFY") == 0) return "*";
    if (strcmp(smpl_op, "SPLIT") == 0) return "/";
    if (strcmp(smpl_op, "REMAINDER") == 0) return "%";
    if (strcmp(smpl_op, "BOOST") == 0) return "++";
    if (strcmp(smpl_op, "DEGRADE") == 0) return "--";
    
    /* Relational operators */
    if (strcmp(smpl_op, "EXCEEDS") == 0) return ">";
    if (strcmp(smpl_op, "BELOW") == 0) return "<";
    if (strcmp(smpl_op, "EXCEEDS_OR_EQUAL") == 0) return ">=";
    if (strcmp(smpl_op, "BELOW_OR_EQUAL") == 0) return "<=";
    if (strcmp(smpl_op, "MATCHES") == 0) return "==";
    if (strcmp(smpl_op, "DIFFERS") == 0) return "!=";
    
    /* Logical operators */
    if (strcmp(smpl_op, "BOTH") == 0) return "&&";
    if (strcmp(smpl_op, "EITHER") == 0) return "||";
    if (strcmp(smpl_op, "NEGATE") == 0) return "!";
    
    /* Already C operators */
    return smpl_op;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * FORMAT SPECIFIERS
 * ═══════════════════════════════════════════════════════════════════════════ */

const char *codegen_printf_format(const char *type) {
    if (!type) return "%d";
    if (strcmp(type, "int") == 0) return "%d";
    if (strcmp(type, "float") == 0) return "%f";
    if (strcmp(type, "double") == 0) return "%lf";
    if (strcmp(type, "char") == 0) return "%c";
    if (strcmp(type, "string") == 0 || strstr(type, "char*")) return "%s";
    return "%d";
}

const char *codegen_scanf_format(const char *type) {
    if (!type) return "%d";
    if (strcmp(type, "int") == 0) return "%d";
    if (strcmp(type, "float") == 0) return "%f";
    if (strcmp(type, "double") == 0) return "%lf";
    if (strcmp(type, "char") == 0) return " %c";
    if (strcmp(type, "string") == 0 || strstr(type, "char*")) return "%s";
    return "%d";
}

/* ═══════════════════════════════════════════════════════════════════════════
 * EXPRESSION CODE GENERATION
 * ═══════════════════════════════════════════════════════════════════════════ */

void codegen_expr(ASTNode *expr, CodeGenContext *ctx) {
    if (!expr) return;
    
    switch (expr->type) {
        case AST_INTEGER:
            fprintf(ctx->output, "%d", expr->data.int_literal.value);
            break;
            
        case AST_FLOAT:
            fprintf(ctx->output, "%f", expr->data.float_literal.value);
            break;
            
        case AST_CHAR:
            fprintf(ctx->output, "'%s'", expr->data.char_literal.value);
            break;
            
        case AST_STRING:
            fprintf(ctx->output, "\"%s\"", expr->data.string_literal.value);
            break;
            
        case AST_IDENTIFIER:
            fprintf(ctx->output, "%s", expr->data.identifier.name);
            break;
            
        case AST_BINARY_OP:
            fprintf(ctx->output, "(");
            codegen_expr(expr->data.binary_op.left, ctx);
            fprintf(ctx->output, " %s ", codegen_map_operator(expr->data.binary_op.op));
            codegen_expr(expr->data.binary_op.right, ctx);
            fprintf(ctx->output, ")");
            break;
            
        case AST_UNARY_OP:
            fprintf(ctx->output, "%s", codegen_map_operator(expr->data.unary_op.op));
            fprintf(ctx->output, "(");
            codegen_expr(expr->data.unary_op.operand, ctx);
            fprintf(ctx->output, ")");
            break;
            
        case AST_ARRAY_ACCESS:
            fprintf(ctx->output, "%s[", expr->data.array_access.array_name);
            codegen_expr(expr->data.array_access.index, ctx);
            fprintf(ctx->output, "]");
            break;
            
        case AST_FUNCTION_CALL:
            fprintf(ctx->output, "%s(", expr->data.func_call.func_name);
            {
                ASTNode *arg = expr->data.func_call.args;
                int first = 1;
                while (arg) {
                    if (!first) fprintf(ctx->output, ", ");
                    first = 0;
                    
                    if (arg->type == AST_STMT_LIST) {
                        codegen_expr(arg->data.stmt_list.stmt, ctx);
                        arg = arg->data.stmt_list.next;
                    } else {
                        codegen_expr(arg, ctx);
                        break;
                    }
                }
            }
            fprintf(ctx->output, ")");
            break;
            
        default:
            break;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * DECLARATION CODE GENERATION
 * ═══════════════════════════════════════════════════════════════════════════ */

void codegen_declaration(ASTNode *decl, CodeGenContext *ctx) {
    const char *c_type = value_type_to_c(smpl_type_to_value_type(decl->data.declaration.var_type));
    
    codegen_indent(ctx);
    fprintf(ctx->output, "%s %s", c_type, decl->data.declaration.var_name);
    
    if (decl->data.declaration.is_array && decl->data.declaration.array_size) {
        if (decl->data.declaration.array_size->type == AST_INTEGER) {
            fprintf(ctx->output, "[%d]", decl->data.declaration.array_size->data.int_literal.value);
        }
    }
    
    if (decl->data.declaration.init_value) {
        fprintf(ctx->output, " = ");
        codegen_expr(decl->data.declaration.init_value, ctx);
    }
    
    fprintf(ctx->output, ";\n");
}

/* ═══════════════════════════════════════════════════════════════════════════
 * STATEMENT CODE GENERATION
 * ═══════════════════════════════════════════════════════════════════════════ */

void codegen_stmt(ASTNode *stmt, CodeGenContext *ctx) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case AST_DECLARATION:
            codegen_declaration(stmt, ctx);
            break;
            
        case AST_ASSIGNMENT:
            codegen_indent(ctx);
            fprintf(ctx->output, "%s = ", stmt->data.assignment.var_name);
            codegen_expr(stmt->data.assignment.value, ctx);
            fprintf(ctx->output, ";\n");
            break;
            
        case AST_ARRAY_ASSIGN:
            codegen_indent(ctx);
            fprintf(ctx->output, "%s[", stmt->data.array_assign.array_name);
            codegen_expr(stmt->data.array_assign.index, ctx);
            fprintf(ctx->output, "] = ");
            codegen_expr(stmt->data.array_assign.value, ctx);
            fprintf(ctx->output, ";\n");
            break;
            
        case AST_OUTPUT:
            {
                codegen_indent(ctx);
                
                /* Determine type of expression for format specifier */
                const char *format = "%d";  /* default */
                if (stmt->data.output.expr->expr_type) {
                    format = codegen_printf_format(stmt->data.output.expr->expr_type);
                }
                
                fprintf(ctx->output, "printf(\"%s\\n\", ", format);
                codegen_expr(stmt->data.output.expr, ctx);
                fprintf(ctx->output, ");\n");
            }
            break;
            
        case AST_INPUT:
            {
                codegen_indent(ctx);
                
                Symbol *sym = symbol_table_lookup(ctx->symbol_table, stmt->data.input.var_name);
                const char *format = sym ? codegen_scanf_format(sym->type) : "%d";
                
                fprintf(ctx->output, "scanf(\"%s\", &%s);\n", 
                       format, stmt->data.input.var_name);
            }
            break;
            
        case AST_IF_STMT:
            codegen_indent(ctx);
            fprintf(ctx->output, "if (");
            codegen_expr(stmt->data.if_stmt.condition, ctx);
            fprintf(ctx->output, ") {\n");
            codegen_indent_inc(ctx);
            codegen_stmt(stmt->data.if_stmt.then_branch, ctx);
            codegen_indent_dec(ctx);
            codegen_indent(ctx);
            
            if (stmt->data.if_stmt.else_branch) {
                fprintf(ctx->output, "} else {\n");
                codegen_indent_inc(ctx);
                codegen_stmt(stmt->data.if_stmt.else_branch, ctx);
                codegen_indent_dec(ctx);
                codegen_indent(ctx);
            }
            fprintf(ctx->output, "}\n");
            break;
            
        case AST_WHILE_LOOP:
            codegen_indent(ctx);
            fprintf(ctx->output, "while (");
            codegen_expr(stmt->data.while_loop.condition, ctx);
            fprintf(ctx->output, ") {\n");
            codegen_indent_inc(ctx);
            codegen_stmt(stmt->data.while_loop.body, ctx);
            codegen_indent_dec(ctx);
            codegen_indent(ctx);
            fprintf(ctx->output, "}\n");
            break;
            
        case AST_FOR_LOOP:
            codegen_indent(ctx);
            fprintf(ctx->output, "for (");
            
            /* Init - remove indentation and semicolon */
            if (stmt->data.for_loop.init) {
                if (stmt->data.for_loop.init->type == AST_DECLARATION) {
                    const char *c_type = value_type_to_c(
                        smpl_type_to_value_type(stmt->data.for_loop.init->data.declaration.var_type));
                    fprintf(ctx->output, "%s %s = ", c_type, 
                           stmt->data.for_loop.init->data.declaration.var_name);
                    if (stmt->data.for_loop.init->data.declaration.init_value) {
                        codegen_expr(stmt->data.for_loop.init->data.declaration.init_value, ctx);
                    }
                } else if (stmt->data.for_loop.init->type == AST_ASSIGNMENT) {
                    fprintf(ctx->output, "%s = ", stmt->data.for_loop.init->data.assignment.var_name);
                    codegen_expr(stmt->data.for_loop.init->data.assignment.value, ctx);
                }
            }
            fprintf(ctx->output, "; ");
            
            /* Condition */
            if (stmt->data.for_loop.condition) {
                codegen_expr(stmt->data.for_loop.condition, ctx);
            }
            fprintf(ctx->output, "; ");
            
            /* Update */
            if (stmt->data.for_loop.update) {
                if (stmt->data.for_loop.update->type == AST_ASSIGNMENT) {
                    fprintf(ctx->output, "%s = ", stmt->data.for_loop.update->data.assignment.var_name);
                    codegen_expr(stmt->data.for_loop.update->data.assignment.value, ctx);
                } else if (stmt->data.for_loop.update->type == AST_EXPR_STMT) {
                    codegen_expr(stmt->data.for_loop.update->data.expr_stmt.expr, ctx);
                }
            }
            
            fprintf(ctx->output, ") {\n");
            codegen_indent_inc(ctx);
            codegen_stmt(stmt->data.for_loop.body, ctx);
            codegen_indent_dec(ctx);
            codegen_indent(ctx);
            fprintf(ctx->output, "}\n");
            break;
            
        case AST_DO_WHILE:
            codegen_indent(ctx);
            fprintf(ctx->output, "do {\n");
            codegen_indent_inc(ctx);
            codegen_stmt(stmt->data.do_while.body, ctx);
            codegen_indent_dec(ctx);
            codegen_indent(ctx);
            fprintf(ctx->output, "} while (");
            codegen_expr(stmt->data.do_while.condition, ctx);
            fprintf(ctx->output, ");\n");
            break;
            
        case AST_SWITCH_STMT:
            codegen_indent(ctx);
            fprintf(ctx->output, "switch (");
            codegen_expr(stmt->data.switch_stmt.expr, ctx);
            fprintf(ctx->output, ") {\n");
            codegen_indent_inc(ctx);
            
            {
                ASTNode *case_node = stmt->data.switch_stmt.cases;
                while (case_node) {
                    codegen_indent(ctx);
                    if (case_node->data.case_stmt.is_default) {
                        fprintf(ctx->output, "default:\n");
                    } else {
                        fprintf(ctx->output, "case ");
                        codegen_expr(case_node->data.case_stmt.value, ctx);
                        fprintf(ctx->output, ":\n");
                    }
                    
                    codegen_indent_inc(ctx);
                    codegen_stmt(case_node->data.case_stmt.stmts, ctx);
                    codegen_indent_dec(ctx);
                    
                    case_node = case_node->data.case_stmt.next_case;
                }
            }
            
            codegen_indent_dec(ctx);
            codegen_indent(ctx);
            fprintf(ctx->output, "}\n");
            break;
            
        case AST_BREAK:
            codegen_indent(ctx);
            fprintf(ctx->output, "break;\n");
            break;
            
        case AST_CONTINUE:
            codegen_indent(ctx);
            fprintf(ctx->output, "continue;\n");
            break;
            
        case AST_RETURN:
            codegen_indent(ctx);
            fprintf(ctx->output, "return");
            if (stmt->data.return_stmt.value) {
                fprintf(ctx->output, " ");
                codegen_expr(stmt->data.return_stmt.value, ctx);
            }
            fprintf(ctx->output, ";\n");
            break;
            
        case AST_BLOCK:
            {
                ASTNode *stmts = stmt->data.block.statements;
                while (stmts) {
                    if (stmts->type == AST_STMT_LIST) {
                        codegen_stmt(stmts->data.stmt_list.stmt, ctx);
                        stmts = stmts->data.stmt_list.next;
                    } else {
                        codegen_stmt(stmts, ctx);
                        break;
                    }
                }
            }
            break;
            
        case AST_STMT_LIST:
            {
                ASTNode *current = stmt;
                while (current) {
                    codegen_stmt(current->data.stmt_list.stmt, ctx);
                    current = current->data.stmt_list.next;
                }
            }
            break;
            
        case AST_EXPR_STMT:
            codegen_indent(ctx);
            codegen_expr(stmt->data.expr_stmt.expr, ctx);
            fprintf(ctx->output, ";\n");
            break;
            
        case AST_FUNCTION_DEF:
            {
                const char *ret_type = value_type_to_c(
                    smpl_type_to_value_type(stmt->data.func_def.return_type));
                fprintf(ctx->output, "%s %s(", ret_type, stmt->data.func_def.func_name);
                
                /* Parameters */
                ASTNode *param = stmt->data.func_def.params;
                int first = 1;
                while (param) {
                    if (!first) fprintf(ctx->output, ", ");
                    first = 0;
                    
                    const char *param_type = value_type_to_c(
                        smpl_type_to_value_type(param->data.param.param_type));
                    fprintf(ctx->output, "%s %s", param_type, param->data.param.param_name);
                    
                    param = param->data.param.next;
                }
                
                fprintf(ctx->output, ") {\n");
                codegen_indent_inc(ctx);
                codegen_stmt(stmt->data.func_def.body, ctx);
                codegen_indent_dec(ctx);
                fprintf(ctx->output, "}\n\n");
            }
            break;
            
        case AST_PROGRAM:
            fprintf(ctx->output, "#include <stdio.h>\n");
            fprintf(ctx->output, "#include <stdlib.h>\n\n");
            fprintf(ctx->output, "int main() {\n");
            codegen_indent_inc(ctx);
            {
                ASTNode *stmts = stmt->data.block.statements;
                while (stmts) {
                    if (stmts->type == AST_STMT_LIST) {
                        codegen_stmt(stmts->data.stmt_list.stmt, ctx);
                        stmts = stmts->data.stmt_list.next;
                    } else {
                        codegen_stmt(stmts, ctx);
                        break;
                    }
                }
            }
            codegen_indent(ctx);
            fprintf(ctx->output, "return 0;\n");
            codegen_indent_dec(ctx);
            fprintf(ctx->output, "}\n");
            break;
            
        default:
            break;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PUBLIC API
 * ═══════════════════════════════════════════════════════════════════════════ */

int codegen_generate(ASTNode *root, const char *output_filename, SymbolTable *table) {
    FILE *f = fopen(output_filename, "w");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file '%s' for writing\n", output_filename);
        return 0;
    }
    
    CodeGenContext ctx;
    codegen_init(&ctx, f, table);
    
    codegen_stmt(root, &ctx);
    
    fclose(f);
    return 1;
}

int codegen_from_tac(TACContext *tac_ctx, const char *output_filename, SymbolTable *table) {
    /* Future: Generate C code from TAC instead of AST */
    /* For now, this is a stub */
    (void)tac_ctx;
    (void)output_filename;
    (void)table;
    return 0;
}
