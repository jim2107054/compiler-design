#include "semantic.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * RUNTIME EXECUTION CONTEXT
 * ═══════════════════════════════════════════════════════════════════════════ */

typedef struct {
    char name[256];
    RuntimeValue value;
} RuntimeVariable;

typedef struct RuntimeContext {
    RuntimeVariable *variables;
    int var_count;
    int var_capacity;
    SymbolTable *symbol_table;
    int error_count;
    int warning_count;
    int break_flag;
    int continue_flag;
    int return_flag;
    RuntimeValue return_value;
} RuntimeContext;

/* ═══════════════════════════════════════════════════════════════════════════
 * HELPER FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

static void report_error(RuntimeContext *ctx, int line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Semantic error (line %d): ", line);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    ctx->error_count++;
}

static void report_warning(RuntimeContext *ctx, int line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Warning (line %d): ", line);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    ctx->warning_count++;
}

RuntimeValue runtime_value_create(ValueType type, int valid) {
    RuntimeValue val;
    val.type = type;
    val.valid = valid;
    memset(&val.data, 0, sizeof(val.data));
    return val;
}

static RuntimeValue invalid_value(void) {
    return runtime_value_create(TYPE_ERROR, 0);
}

int runtime_value_to_bool(RuntimeValue val) {
    if (!val.valid) return 0;
    
    switch (val.type) {
        case TYPE_INT:    return val.data.int_val != 0;
        case TYPE_FLOAT:  return val.data.float_val != 0.0f;
        case TYPE_DOUBLE: return val.data.double_val != 0.0;
        case TYPE_CHAR:   return val.data.char_val != '\0';
        default:          return 0;
    }
}

static RuntimeValue convert_value(RuntimeValue val, ValueType target_type) {
    if (!val.valid || val.type == target_type) return val;
    
    RuntimeValue result = runtime_value_create(target_type, 1);
    
    switch (target_type) {
        case TYPE_INT:
            if (val.type == TYPE_FLOAT) result.data.int_val = (int)val.data.float_val;
            else if (val.type == TYPE_DOUBLE) result.data.int_val = (int)val.data.double_val;
            else if (val.type == TYPE_CHAR) result.data.int_val = (int)val.data.char_val;
            break;
        case TYPE_FLOAT:
            if (val.type == TYPE_INT) result.data.float_val = (float)val.data.int_val;
            else if (val.type == TYPE_DOUBLE) result.data.float_val = (float)val.data.double_val;
            break;
        case TYPE_DOUBLE:
            if (val.type == TYPE_INT) result.data.double_val = (double)val.data.int_val;
            else if (val.type == TYPE_FLOAT) result.data.double_val = (double)val.data.float_val;
            break;
        default:
            return invalid_value();
    }
    
    return result;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * RUNTIME VARIABLE MANAGEMENT
 * ═══════════════════════════════════════════════════════════════════════════ */

static void runtime_ensure_capacity(RuntimeContext *ctx) {
    if (ctx->var_count >= ctx->var_capacity) {
        int new_cap = ctx->var_capacity == 0 ? 50 : ctx->var_capacity * 2;
        ctx->variables = (RuntimeVariable *)realloc(ctx->variables, 
                                                    new_cap * sizeof(RuntimeVariable));
        ctx->var_capacity = new_cap;
    }
}

static RuntimeVariable *runtime_find_var(RuntimeContext *ctx, const char *name) {
    for (int i = 0; i < ctx->var_count; i++) {
        if (strcmp(ctx->variables[i].name, name) == 0) {
            return &ctx->variables[i];
        }
    }
    return NULL;
}

static void runtime_set_var(RuntimeContext *ctx, const char *name, RuntimeValue value) {
    RuntimeVariable *var = runtime_find_var(ctx, name);
    if (!var) {
        runtime_ensure_capacity(ctx);
        var = &ctx->variables[ctx->var_count++];
        strncpy(var->name, name, sizeof(var->name) - 1);
    }
    var->value = value;
}

static RuntimeValue runtime_get_var(RuntimeContext *ctx, const char *name) {
    RuntimeVariable *var = runtime_find_var(ctx, name);
    if (var) {
        return var->value;
    }
    return invalid_value();
}

/* ═══════════════════════════════════════════════════════════════════════════
 * EXPRESSION EVALUATION
 * ═══════════════════════════════════════════════════════════════════════════ */

static RuntimeValue eval_expr(ASTNode *expr, RuntimeContext *ctx);

static RuntimeValue eval_binary_op(ASTNode *node, RuntimeContext *ctx) {
    RuntimeValue left = eval_expr(node->data.binary_op.left, ctx);
    RuntimeValue right = eval_expr(node->data.binary_op.right, ctx);
    
    if (!left.valid || !right.valid) {
        return invalid_value();
    }
    
    const char *op = node->data.binary_op.op;
    RuntimeValue result;
    
    /* Promote to common type */
    ValueType result_type = TYPE_INT;
    if (left.type == TYPE_DOUBLE || right.type == TYPE_DOUBLE) {
        result_type = TYPE_DOUBLE;
    } else if (left.type == TYPE_FLOAT || right.type == TYPE_FLOAT) {
        result_type = TYPE_FLOAT;
    }
    
    left = convert_value(left, result_type);
    right = convert_value(right, result_type);
    result = runtime_value_create(result_type, 1);
    
    /* Arithmetic operators */
    if (strcmp(op, "+") == 0 || strcmp(op, "COMBINE") == 0) {
        if (result_type == TYPE_INT) result.data.int_val = left.data.int_val + right.data.int_val;
        else if (result_type == TYPE_FLOAT) result.data.float_val = left.data.float_val + right.data.float_val;
        else result.data.double_val = left.data.double_val + right.data.double_val;
    } else if (strcmp(op, "-") == 0 || strcmp(op, "REDUCE") == 0) {
        if (result_type == TYPE_INT) result.data.int_val = left.data.int_val - right.data.int_val;
        else if (result_type == TYPE_FLOAT) result.data.float_val = left.data.float_val - right.data.float_val;
        else result.data.double_val = left.data.double_val - right.data.double_val;
    } else if (strcmp(op, "*") == 0 || strcmp(op, "AMPLIFY") == 0) {
        if (result_type == TYPE_INT) result.data.int_val = left.data.int_val * right.data.int_val;
        else if (result_type == TYPE_FLOAT) result.data.float_val = left.data.float_val * right.data.float_val;
        else result.data.double_val = left.data.double_val * right.data.double_val;
    } else if (strcmp(op, "/") == 0 || strcmp(op, "SPLIT") == 0) {
        if (result_type == TYPE_INT) {
            if (right.data.int_val == 0) {
                report_error(ctx, node->line_number, "Division by zero");
                return invalid_value();
            }
            result.data.int_val = left.data.int_val / right.data.int_val;
        } else if (result_type == TYPE_FLOAT) {
            result.data.float_val = left.data.float_val / right.data.float_val;
        } else {
            result.data.double_val = left.data.double_val / right.data.double_val;
        }
    } else if (strcmp(op, "%") == 0 || strcmp(op, "REMAINDER") == 0) {
        if (result_type != TYPE_INT) {
            report_error(ctx, node->line_number, "Modulus requires integer operands");
            return invalid_value();
        }
        if (right.data.int_val == 0) {
            report_error(ctx, node->line_number, "Division by zero");
            return invalid_value();
        }
        result.data.int_val = left.data.int_val % right.data.int_val;
    }
    /* Relational operators - return int (0 or 1) */
    else {
        result = runtime_value_create(TYPE_INT, 1);
        int cmp_result = 0;
        
        if (strcmp(op, ">") == 0 || strcmp(op, "EXCEEDS") == 0) {
            if (result_type == TYPE_INT) cmp_result = left.data.int_val > right.data.int_val;
            else if (result_type == TYPE_FLOAT) cmp_result = left.data.float_val > right.data.float_val;
            else cmp_result = left.data.double_val > right.data.double_val;
        } else if (strcmp(op, "<") == 0 || strcmp(op, "BELOW") == 0) {
            if (result_type == TYPE_INT) cmp_result = left.data.int_val < right.data.int_val;
            else if (result_type == TYPE_FLOAT) cmp_result = left.data.float_val < right.data.float_val;
            else cmp_result = left.data.double_val < right.data.double_val;
        } else if (strcmp(op, ">=") == 0 || strcmp(op, "EXCEEDS_OR_EQUAL") == 0) {
            if (result_type == TYPE_INT) cmp_result = left.data.int_val >= right.data.int_val;
            else if (result_type == TYPE_FLOAT) cmp_result = left.data.float_val >= right.data.float_val;
            else cmp_result = left.data.double_val >= right.data.double_val;
        } else if (strcmp(op, "<=") == 0 || strcmp(op, "BELOW_OR_EQUAL") == 0) {
            if (result_type == TYPE_INT) cmp_result = left.data.int_val <= right.data.int_val;
            else if (result_type == TYPE_FLOAT) cmp_result = left.data.float_val <= right.data.float_val;
            else cmp_result = left.data.double_val <= right.data.double_val;
        } else if (strcmp(op, "==") == 0 || strcmp(op, "MATCHES") == 0) {
            if (result_type == TYPE_INT) cmp_result = left.data.int_val == right.data.int_val;
            else if (result_type == TYPE_FLOAT) cmp_result = fabs(left.data.float_val - right.data.float_val) < 0.0001f;
            else cmp_result = fabs(left.data.double_val - right.data.double_val) < 0.0001;
        } else if (strcmp(op, "!=") == 0 || strcmp(op, "DIFFERS") == 0) {
            if (result_type == TYPE_INT) cmp_result = left.data.int_val != right.data.int_val;
            else if (result_type == TYPE_FLOAT) cmp_result = fabs(left.data.float_val - right.data.float_val) >= 0.0001f;
            else cmp_result = fabs(left.data.double_val - right.data.double_val) >= 0.0001;
        } else if (strcmp(op, "&&") == 0 || strcmp(op, "BOTH") == 0) {
            cmp_result = runtime_value_to_bool(left) && runtime_value_to_bool(right);
        } else if (strcmp(op, "||") == 0 || strcmp(op, "EITHER") == 0) {
            cmp_result = runtime_value_to_bool(left) || runtime_value_to_bool(right);
        }
        
        result.data.int_val = cmp_result ? 1 : 0;
    }
    
    return result;
}

static RuntimeValue eval_expr(ASTNode *expr, RuntimeContext *ctx) {
    if (!expr) return invalid_value();
    
    switch (expr->type) {
        case AST_INTEGER:
            {
                RuntimeValue val = runtime_value_create(TYPE_INT, 1);
                val.data.int_val = expr->data.int_literal.value;
                return val;
            }
        case AST_FLOAT:
            {
                RuntimeValue val = runtime_value_create(TYPE_FLOAT, 1);
                val.data.float_val = expr->data.float_literal.value;
                return val;
            }
        case AST_CHAR:
            {
                RuntimeValue val = runtime_value_create(TYPE_CHAR, 1);
                val.data.char_val = expr->data.char_literal.value[0];
                return val;
            }
        case AST_IDENTIFIER:
            return runtime_get_var(ctx, expr->data.identifier.name);
            
        case AST_BINARY_OP:
            return eval_binary_op(expr, ctx);
            
        case AST_UNARY_OP:
            {
                RuntimeValue operand = eval_expr(expr->data.unary_op.operand, ctx);
                if (!operand.valid) return invalid_value();
                
                const char *op = expr->data.unary_op.op;
                if (strcmp(op, "-") == 0) {
                    if (operand.type == TYPE_INT) operand.data.int_val = -operand.data.int_val;
                    else if (operand.type == TYPE_FLOAT) operand.data.float_val = -operand.data.float_val;
                    else if (operand.type == TYPE_DOUBLE) operand.data.double_val = -operand.data.double_val;
                } else if (strcmp(op, "!") == 0 || strcmp(op, "NEGATE") == 0) {
                    RuntimeValue result = runtime_value_create(TYPE_INT, 1);
                    result.data.int_val = !runtime_value_to_bool(operand);
                    return result;
                } else if (strcmp(op, "++") == 0 || strcmp(op, "BOOST") == 0) {
                    if (operand.type == TYPE_INT) operand.data.int_val++;
                    else if (operand.type == TYPE_FLOAT) operand.data.float_val++;
                } else if (strcmp(op, "--") == 0 || strcmp(op, "DEGRADE") == 0) {
                    if (operand.type == TYPE_INT) operand.data.int_val--;
                    else if (operand.type == TYPE_FLOAT) operand.data.float_val--;
                }
                return operand;
            }
            
        default:
            return invalid_value();
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * STATEMENT EXECUTION
 * ═══════════════════════════════════════════════════════════════════════════ */

static void exec_stmt(ASTNode *stmt, RuntimeContext *ctx);

static void exec_stmt_list(ASTNode *list, RuntimeContext *ctx) {
    while (list && !ctx->break_flag && !ctx->continue_flag && !ctx->return_flag) {
        if (list->type == AST_STMT_LIST) {
            exec_stmt(list->data.stmt_list.stmt, ctx);
            list = list->data.stmt_list.next;
        } else {
            exec_stmt(list, ctx);
            break;
        }
    }
}

static void exec_stmt(ASTNode *stmt, RuntimeContext *ctx) {
    if (!stmt || ctx->break_flag || ctx->continue_flag || ctx->return_flag) return;
    
    switch (stmt->type) {
        case AST_DECLARATION:
            {
                RuntimeValue init_val;
                if (stmt->data.declaration.init_value) {
                    init_val = eval_expr(stmt->data.declaration.init_value, ctx);
                } else {
                    ValueType vtype = smpl_type_to_value_type(stmt->data.declaration.var_type);
                    init_val = runtime_value_create(vtype, 1);
                }
                runtime_set_var(ctx, stmt->data.declaration.var_name, init_val);
            }
            break;
            
        case AST_ASSIGNMENT:
            {
                RuntimeValue val = eval_expr(stmt->data.assignment.value, ctx);
                if (val.valid) {
                    runtime_set_var(ctx, stmt->data.assignment.var_name, val);
                }
            }
            break;
            
        case AST_OUTPUT:
            {
                RuntimeValue val = eval_expr(stmt->data.output.expr, ctx);
                if (val.valid) {
                    switch (val.type) {
                        case TYPE_INT:    printf("%d\n", val.data.int_val); break;
                        case TYPE_FLOAT:  printf("%f\n", val.data.float_val); break;
                        case TYPE_DOUBLE: printf("%lf\n", val.data.double_val); break;
                        case TYPE_CHAR:   printf("%c\n", val.data.char_val); break;
                        default: break;
                    }
                }
            }
            break;
            
        case AST_INPUT:
            {
                Symbol *sym = symbol_table_lookup(ctx->symbol_table, stmt->data.input.var_name);
                if (sym) {
                    RuntimeValue val;
                    if (strcmp(sym->type, "int") == 0) {
                        val = runtime_value_create(TYPE_INT, 1);
                        scanf("%d", &val.data.int_val);
                    } else if (strcmp(sym->type, "float") == 0) {
                        val = runtime_value_create(TYPE_FLOAT, 1);
                        scanf("%f", &val.data.float_val);
                    } else if (strcmp(sym->type, "char") == 0) {
                        val = runtime_value_create(TYPE_CHAR, 1);
                        scanf(" %c", &val.data.char_val);
                    } else {
                        val = runtime_value_create(TYPE_INT, 1);
                        scanf("%d", &val.data.int_val);
                    }
                    runtime_set_var(ctx, stmt->data.input.var_name, val);
                }
            }
            break;
            
        case AST_IF_STMT:
            {
                RuntimeValue cond = eval_expr(stmt->data.if_stmt.condition, ctx);
                if (runtime_value_to_bool(cond)) {
                    exec_stmt(stmt->data.if_stmt.then_branch, ctx);
                } else if (stmt->data.if_stmt.else_branch) {
                    exec_stmt(stmt->data.if_stmt.else_branch, ctx);
                }
            }
            break;
            
        case AST_WHILE_LOOP:
            while (1) {
                RuntimeValue cond = eval_expr(stmt->data.while_loop.condition, ctx);
                if (!runtime_value_to_bool(cond)) break;
                
                ctx->continue_flag = 0;
                exec_stmt(stmt->data.while_loop.body, ctx);
                
                if (ctx->break_flag) {
                    ctx->break_flag = 0;
                    break;
                }
                if (ctx->return_flag) break;
            }
            break;
            
        case AST_FOR_LOOP:
            exec_stmt(stmt->data.for_loop.init, ctx);
            while (1) {
                RuntimeValue cond = eval_expr(stmt->data.for_loop.condition, ctx);
                if (!runtime_value_to_bool(cond)) break;
                
                ctx->continue_flag = 0;
                exec_stmt(stmt->data.for_loop.body, ctx);
                
                if (ctx->break_flag) {
                    ctx->break_flag = 0;
                    break;
                }
                if (ctx->return_flag) break;
                
                exec_stmt(stmt->data.for_loop.update, ctx);
            }
            break;
            
        case AST_DO_WHILE:
            do {
                ctx->continue_flag = 0;
                exec_stmt(stmt->data.do_while.body, ctx);
                
                if (ctx->break_flag) {
                    ctx->break_flag = 0;
                    break;
                }
                if (ctx->return_flag) break;
                
                RuntimeValue cond = eval_expr(stmt->data.do_while.condition, ctx);
                if (!runtime_value_to_bool(cond)) break;
            } while (1);
            break;
            
        case AST_BREAK:
            ctx->break_flag = 1;
            break;
            
        case AST_CONTINUE:
            ctx->continue_flag = 1;
            break;
            
        case AST_RETURN:
            ctx->return_flag = 1;
            if (stmt->data.return_stmt.value) {
                ctx->return_value = eval_expr(stmt->data.return_stmt.value, ctx);
            }
            break;
            
        case AST_BLOCK:
        case AST_PROGRAM:
            exec_stmt_list(stmt->data.block.statements, ctx);
            break;
            
        case AST_STMT_LIST:
            exec_stmt_list(stmt, ctx);
            break;
            
        case AST_EXPR_STMT:
            eval_expr(stmt->data.expr_stmt.expr, ctx);
            break;
            
        default:
            break;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PUBLIC API
 * ═══════════════════════════════════════════════════════════════════════════ */

AnalysisResult semantic_analyze(ASTNode *root) {
    AnalysisResult result = {1, 0, 0};
    
    if (!root) {
        fprintf(stderr, "Error: NULL AST root\n");
        result.success = 0;
        result.error_count = 1;
        return result;
    }
    
    /* Future: Add type checking, variable usage checking, etc. */
    
    return result;
}

AnalysisResult semantic_execute(ASTNode *root) {
    AnalysisResult result = {1, 0, 0};
    
    if (!root) {
        fprintf(stderr, "Error: NULL AST root\n");
        result.success = 0;
        result.error_count = 1;
        return result;
    }
    
    RuntimeContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    /* Initialize runtime context */
    SymbolTable table;
    symbol_table_init(&table);
    ctx.symbol_table = &table;
    
    /* Execute program */
    exec_stmt(root, &ctx);
    
    /* Cleanup */
    if (ctx.variables) free(ctx.variables);
    symbol_table_free(&table);
    
    result.error_count = ctx.error_count;
    result.warning_count = ctx.warning_count;
    result.success = (ctx.error_count == 0);
    
    return result;
}

void semantic_type_check(ASTNode *node, SymbolTable *table) {
    /* Type checking implementation */
    /* This can be expanded based on requirements */
}

void semantic_check_variables(ASTNode *node, SymbolTable *table) {
    /* Variable usage checking */
}

void semantic_check_functions(ASTNode *node, SymbolTable *table) {
    /* Function checking */
}
