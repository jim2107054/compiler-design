#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * TAC CONTEXT MANAGEMENT
 * ═══════════════════════════════════════════════════════════════════════════ */

void tac_init(TACContext *ctx) {
    memset(ctx, 0, sizeof(TACContext));
    ctx->temp_count = 0;
    ctx->label_count = 0;
    ctx->head = NULL;
    ctx->tail = NULL;
}

void tac_free(TACContext *ctx) {
    TACInstruction *current = ctx->head;
    while (current) {
        TACInstruction *next = current->next;
        if (current->result) free(current->result);
        if (current->arg1) free(current->arg1);
        if (current->arg2) free(current->arg2);
        if (current->op) free(current->op);
        free(current);
        current = next;
    }
    ctx->head = NULL;
    ctx->tail = NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TAC INSTRUCTION CREATION
 * ═══════════════════════════════════════════════════════════════════════════ */

static TACInstruction *tac_create_instruction(TACType type) {
    TACInstruction *instr = (TACInstruction *)calloc(1, sizeof(TACInstruction));
    if (!instr) {
        fprintf(stderr, "Fatal: Memory allocation failed for TAC instruction\n");
        exit(1);
    }
    instr->type = type;
    instr->next = NULL;
    return instr;
}

void tac_append(TACContext *ctx, TACInstruction *instr) {
    if (!ctx->head) {
        ctx->head = ctx->tail = instr;
    } else {
        ctx->tail->next = instr;
        ctx->tail = instr;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TEMPORARY AND LABEL GENERATION
 * ═══════════════════════════════════════════════════════════════════════════ */

char *tac_new_temp(TACContext *ctx) {
    char *temp = (char *)malloc(32);
    snprintf(temp, 32, "t%d", ctx->temp_count++);
    return temp;
}

char *tac_new_label(TACContext *ctx) {
    char *label = (char *)malloc(32);
    snprintf(label, 32, "L%d", ctx->label_count++);
    return label;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TAC GENERATION FROM AST
 * ═══════════════════════════════════════════════════════════════════════════ */

static char *safe_strdup(const char *s) {
    return s ? strdup(s) : NULL;
}

char *tac_gen_expr(ASTNode *expr, TACContext *ctx, SymbolTable *table) {
    if (!expr) return NULL;
    
    switch (expr->type) {
        case AST_INTEGER: {
            char *result = (char *)malloc(32);
            snprintf(result, 32, "%d", expr->data.int_literal.value);
            return result;
        }
        
        case AST_FLOAT: {
            char *result = (char *)malloc(32);
            snprintf(result, 32, "%f", expr->data.float_literal.value);
            return result;
        }
        
        case AST_CHAR: {
            char *result = (char *)malloc(32);
            snprintf(result, 32, "'%s'", expr->data.char_literal.value);
            return result;
        }
        
        case AST_STRING: {
            return safe_strdup(expr->data.string_literal.value);
        }
        
        case AST_IDENTIFIER: {
            return safe_strdup(expr->data.identifier.name);
        }
        
        case AST_BINARY_OP: {
            char *left = tac_gen_expr(expr->data.binary_op.left, ctx, table);
            char *right = tac_gen_expr(expr->data.binary_op.right, ctx, table);
            char *result = tac_new_temp(ctx);
            
            TACInstruction *instr = tac_create_instruction(TAC_BINARY_OP);
            instr->result = safe_strdup(result);
            instr->arg1 = left;
            instr->arg2 = right;
            instr->op = safe_strdup(expr->data.binary_op.op);
            tac_append(ctx, instr);
            
            return result;
        }
        
        case AST_UNARY_OP: {
            char *operand = tac_gen_expr(expr->data.unary_op.operand, ctx, table);
            char *result = tac_new_temp(ctx);
            
            TACInstruction *instr = tac_create_instruction(TAC_UNARY_OP);
            instr->result = safe_strdup(result);
            instr->arg1 = operand;
            instr->op = safe_strdup(expr->data.unary_op.op);
            tac_append(ctx, instr);
            
            return result;
        }
        
        case AST_ARRAY_ACCESS: {
            char *index = tac_gen_expr(expr->data.array_access.index, ctx, table);
            char *result = tac_new_temp(ctx);
            
            TACInstruction *instr = tac_create_instruction(TAC_ARRAY_READ);
            instr->result = safe_strdup(result);
            instr->arg1 = safe_strdup(expr->data.array_access.array_name);
            instr->arg2 = index;
            tac_append(ctx, instr);
            
            return result;
        }
        
        case AST_FUNCTION_CALL: {
            /* Generate code for arguments */
            ASTNode *arg = expr->data.func_call.args;
            while (arg) {
                if (arg->type == AST_STMT_LIST) {
                    char *arg_val = tac_gen_expr(arg->data.stmt_list.stmt, ctx, table);
                    
                    TACInstruction *param_instr = tac_create_instruction(TAC_PARAM);
                    param_instr->arg1 = arg_val;
                    tac_append(ctx, param_instr);
                    
                    arg = arg->data.stmt_list.next;
                } else {
                    char *arg_val = tac_gen_expr(arg, ctx, table);
                    
                    TACInstruction *param_instr = tac_create_instruction(TAC_PARAM);
                    param_instr->arg1 = arg_val;
                    tac_append(ctx, param_instr);
                    break;
                }
            }
            
            char *result = tac_new_temp(ctx);
            TACInstruction *call_instr = tac_create_instruction(TAC_CALL);
            call_instr->result = safe_strdup(result);
            call_instr->arg1 = safe_strdup(expr->data.func_call.func_name);
            tac_append(ctx, call_instr);
            
            return result;
        }
        
        default:
            return NULL;
    }
}

void tac_gen_stmt(ASTNode *stmt, TACContext *ctx, SymbolTable *table) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case AST_DECLARATION: {
            /* TAC doesn't explicitly handle declarations - they're implicit */
            if (stmt->data.declaration.init_value) {
                char *value = tac_gen_expr(stmt->data.declaration.init_value, ctx, table);
                
                TACInstruction *instr = tac_create_instruction(TAC_ASSIGN);
                instr->result = safe_strdup(stmt->data.declaration.var_name);
                instr->arg1 = value;
                tac_append(ctx, instr);
            }
            break;
        }
        
        case AST_ASSIGNMENT: {
            char *value = tac_gen_expr(stmt->data.assignment.value, ctx, table);
            
            TACInstruction *instr = tac_create_instruction(TAC_ASSIGN);
            instr->result = safe_strdup(stmt->data.assignment.var_name);
            instr->arg1 = value;
            tac_append(ctx, instr);
            break;
        }
        
        case AST_ARRAY_ASSIGN: {
            char *index = tac_gen_expr(stmt->data.array_assign.index, ctx, table);
            char *value = tac_gen_expr(stmt->data.array_assign.value, ctx, table);
            
            TACInstruction *instr = tac_create_instruction(TAC_ARRAY_WRITE);
            instr->result = safe_strdup(stmt->data.array_assign.array_name);
            instr->arg1 = index;
            instr->arg2 = value;
            tac_append(ctx, instr);
            break;
        }
        
        case AST_OUTPUT: {
            char *value = tac_gen_expr(stmt->data.output.expr, ctx, table);
            
            TACInstruction *instr = tac_create_instruction(TAC_OUTPUT);
            instr->arg1 = value;
            tac_append(ctx, instr);
            break;
        }
        
        case AST_INPUT: {
            TACInstruction *instr = tac_create_instruction(TAC_INPUT);
            instr->result = safe_strdup(stmt->data.input.var_name);
            tac_append(ctx, instr);
            break;
        }
        
        case AST_IF_STMT: {
            char *cond = tac_gen_expr(stmt->data.if_stmt.condition, ctx, table);
            char *else_label = tac_new_label(ctx);
            char *end_label = tac_new_label(ctx);
            
            /* if_false cond goto else_label */
            TACInstruction *if_instr = tac_create_instruction(TAC_IF_GOTO);
            if_instr->arg1 = cond;
            if_instr->arg2 = safe_strdup(else_label);
            if_instr->op = safe_strdup("if_false");
            tac_append(ctx, if_instr);
            
            /* then branch */
            tac_gen_stmt(stmt->data.if_stmt.then_branch, ctx, table);
            
            if (stmt->data.if_stmt.else_branch) {
                /* goto end_label */
                TACInstruction *goto_instr = tac_create_instruction(TAC_GOTO);
                goto_instr->arg1 = safe_strdup(end_label);
                tac_append(ctx, goto_instr);
                
                /* else_label: */
                TACInstruction *else_lbl = tac_create_instruction(TAC_LABEL);
                else_lbl->arg1 = else_label;
                tac_append(ctx, else_lbl);
                
                /* else branch */
                tac_gen_stmt(stmt->data.if_stmt.else_branch, ctx, table);
                
                /* end_label: */
                TACInstruction *end_lbl = tac_create_instruction(TAC_LABEL);
                end_lbl->arg1 = end_label;
                tac_append(ctx, end_lbl);
            } else {
                /* else_label: */
                TACInstruction *else_lbl = tac_create_instruction(TAC_LABEL);
                else_lbl->arg1 = else_label;
                tac_append(ctx, else_lbl);
                
                free(end_label);
            }
            break;
        }
        
        case AST_WHILE_LOOP: {
            char *start_label = tac_new_label(ctx);
            char *end_label = tac_new_label(ctx);
            
            /* start_label: */
            TACInstruction *start_lbl = tac_create_instruction(TAC_LABEL);
            start_lbl->arg1 = start_label;
            tac_append(ctx, start_lbl);
            
            /* condition */
            char *cond = tac_gen_expr(stmt->data.while_loop.condition, ctx, table);
            
            /* if_false cond goto end_label */
            TACInstruction *if_instr = tac_create_instruction(TAC_IF_GOTO);
            if_instr->arg1 = cond;
            if_instr->arg2 = safe_strdup(end_label);
            if_instr->op = safe_strdup("if_false");
            tac_append(ctx, if_instr);
            
            /* body */
            tac_gen_stmt(stmt->data.while_loop.body, ctx, table);
            
            /* goto start_label */
            TACInstruction *goto_instr = tac_create_instruction(TAC_GOTO);
            goto_instr->arg1 = safe_strdup(start_label);
            tac_append(ctx, goto_instr);
            
            /* end_label: */
            TACInstruction *end_lbl = tac_create_instruction(TAC_LABEL);
            end_lbl->arg1 = end_label;
            tac_append(ctx, end_lbl);
            break;
        }
        
        case AST_FOR_LOOP: {
            /* init */
            tac_gen_stmt(stmt->data.for_loop.init, ctx, table);
            
            char *start_label = tac_new_label(ctx);
            char *end_label = tac_new_label(ctx);
            
            /* start_label: */
            TACInstruction *start_lbl = tac_create_instruction(TAC_LABEL);
            start_lbl->arg1 = start_label;
            tac_append(ctx, start_lbl);
            
            /* condition */
            char *cond = tac_gen_expr(stmt->data.for_loop.condition, ctx, table);
            
            /* if_false cond goto end_label */
            TACInstruction *if_instr = tac_create_instruction(TAC_IF_GOTO);
            if_instr->arg1 = cond;
            if_instr->arg2 = safe_strdup(end_label);
            if_instr->op = safe_strdup("if_false");
            tac_append(ctx, if_instr);
            
            /* body */
            tac_gen_stmt(stmt->data.for_loop.body, ctx, table);
            
            /* update */
            tac_gen_stmt(stmt->data.for_loop.update, ctx, table);
            
            /* goto start_label */
            TACInstruction *goto_instr = tac_create_instruction(TAC_GOTO);
            goto_instr->arg1 = safe_strdup(start_label);
            tac_append(ctx, goto_instr);
            
            /* end_label: */
            TACInstruction *end_lbl = tac_create_instruction(TAC_LABEL);
            end_lbl->arg1 = end_label;
            tac_append(ctx, end_lbl);
            break;
        }
        
        case AST_DO_WHILE: {
            char *start_label = tac_new_label(ctx);
            char *end_label = tac_new_label(ctx);
            
            /* start_label: */
            TACInstruction *start_lbl = tac_create_instruction(TAC_LABEL);
            start_lbl->arg1 = start_label;
            tac_append(ctx, start_lbl);
            
            /* body */
            tac_gen_stmt(stmt->data.do_while.body, ctx, table);
            
            /* condition */
            char *cond = tac_gen_expr(stmt->data.do_while.condition, ctx, table);
            
            /* if cond goto start_label */
            TACInstruction *if_instr = tac_create_instruction(TAC_IF_GOTO);
            if_instr->arg1 = cond;
            if_instr->arg2 = safe_strdup(start_label);
            if_instr->op = safe_strdup("if_true");
            tac_append(ctx, if_instr);
            
            free(end_label);
            break;
        }
        
        case AST_RETURN: {
            TACInstruction *instr = tac_create_instruction(TAC_RETURN);
            if (stmt->data.return_stmt.value) {
                instr->arg1 = tac_gen_expr(stmt->data.return_stmt.value, ctx, table);
            }
            tac_append(ctx, instr);
            break;
        }
        
        case AST_BLOCK:
        case AST_PROGRAM:
            {
                ASTNode *stmts = stmt->data.block.statements;
                while (stmts) {
                    if (stmts->type == AST_STMT_LIST) {
                        tac_gen_stmt(stmts->data.stmt_list.stmt, ctx, table);
                        stmts = stmts->data.stmt_list.next;
                    } else {
                        tac_gen_stmt(stmts, ctx, table);
                        break;
                    }
                }
            }
            break;
            
        case AST_STMT_LIST:
            {
                ASTNode *current = stmt;
                while (current) {
                    tac_gen_stmt(current->data.stmt_list.stmt, ctx, table);
                    current = current->data.stmt_list.next;
                }
            }
            break;
            
        case AST_EXPR_STMT:
            tac_gen_expr(stmt->data.expr_stmt.expr, ctx, table);
            break;
            
        default:
            break;
    }
}

int tac_generate(ASTNode *root, TACContext *ctx, SymbolTable *table) {
    if (!root || !ctx) return 0;
    
    tac_gen_stmt(root, ctx, table);
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TAC OUTPUT
 * ═══════════════════════════════════════════════════════════════════════════ */

void tac_print(TACContext *ctx) {
    TACInstruction *instr = ctx->head;
    
    printf("\n; ==========================================\n");
    printf("; Three-Address Code (TAC)\n");
    printf("; ==========================================\n\n");
    
    while (instr) {
        switch (instr->type) {
            case TAC_ASSIGN:
                printf("%s = %s\n", instr->result, instr->arg1);
                break;
            case TAC_BINARY_OP:
                printf("%s = %s %s %s\n", instr->result, instr->arg1, instr->op, instr->arg2);
                break;
            case TAC_UNARY_OP:
                printf("%s = %s%s\n", instr->result, instr->op, instr->arg1);
                break;
            case TAC_LABEL:
                printf("%s:\n", instr->arg1);
                break;
            case TAC_GOTO:
                printf("goto %s\n", instr->arg1);
                break;
            case TAC_IF_GOTO:
                printf("%s %s goto %s\n", instr->op, instr->arg1, instr->arg2);
                break;
            case TAC_PARAM:
                printf("param %s\n", instr->arg1);
                break;
            case TAC_CALL:
                printf("%s = call %s\n", instr->result, instr->arg1);
                break;
            case TAC_RETURN:
                if (instr->arg1) {
                    printf("return %s\n", instr->arg1);
                } else {
                    printf("return\n");
                }
                break;
            case TAC_ARRAY_READ:
                printf("%s = %s[%s]\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_ARRAY_WRITE:
                printf("%s[%s] = %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_INPUT:
                printf("input %s\n", instr->result);
                break;
            case TAC_OUTPUT:
                printf("output %s\n", instr->arg1);
                break;
            default:
                break;
        }
        instr = instr->next;
    }
    
    printf("\n; ==========================================\n\n");
}

int tac_write_to_file(TACContext *ctx, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file '%s' for writing\n", filename);
        return 0;
    }
    
    TACInstruction *instr = ctx->head;
    
    fprintf(f, "; ==========================================\n");
    fprintf(f, "; Three-Address Code (TAC)\n");
    fprintf(f, "; Generated by SMPL Compiler\n");
    fprintf(f, "; ==========================================\n\n");
    
    while (instr) {
        switch (instr->type) {
            case TAC_ASSIGN:
                fprintf(f, "%s = %s\n", instr->result, instr->arg1);
                break;
            case TAC_BINARY_OP:
                fprintf(f, "%s = %s %s %s\n", instr->result, instr->arg1, instr->op, instr->arg2);
                break;
            case TAC_UNARY_OP:
                fprintf(f, "%s = %s%s\n", instr->result, instr->op, instr->arg1);
                break;
            case TAC_LABEL:
                fprintf(f, "%s:\n", instr->arg1);
                break;
            case TAC_GOTO:
                fprintf(f, "goto %s\n", instr->arg1);
                break;
            case TAC_IF_GOTO:
                fprintf(f, "%s %s goto %s\n", instr->op, instr->arg1, instr->arg2);
                break;
            case TAC_PARAM:
                fprintf(f, "param %s\n", instr->arg1);
                break;
            case TAC_CALL:
                fprintf(f, "%s = call %s\n", instr->result, instr->arg1);
                break;
            case TAC_RETURN:
                if (instr->arg1) {
                    fprintf(f, "return %s\n", instr->arg1);
                } else {
                    fprintf(f, "return\n");
                }
                break;
            case TAC_ARRAY_READ:
                fprintf(f, "%s = %s[%s]\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_ARRAY_WRITE:
                fprintf(f, "%s[%s] = %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_INPUT:
                fprintf(f, "input %s\n", instr->result);
                break;
            case TAC_OUTPUT:
                fprintf(f, "output %s\n", instr->arg1);
                break;
            default:
                break;
        }
        instr = instr->next;
    }
    
    fprintf(f, "\n; ==========================================\n");
    fclose(f);
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TAC OPTIMIZATION STUBS (Can be expanded)
 * ═══════════════════════════════════════════════════════════════════════════ */

void tac_optimize_constant_folding(TACContext *ctx) {
    /* TODO: Implement constant folding */
}

void tac_optimize_strength_reduction(TACContext *ctx) {
    /* TODO: Implement strength reduction */
}

void tac_optimize_algebraic(TACContext *ctx) {
    /* TODO: Implement algebraic simplification */
}

void tac_optimize(TACContext *ctx) {
    tac_optimize_constant_folding(ctx);
    tac_optimize_strength_reduction(ctx);
    tac_optimize_algebraic(ctx);
}
