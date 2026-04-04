#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global AST root */
ASTNode *g_program_ast = NULL;

/* ═══════════════════════════════════════════════════════════════════════════
 * INTERNAL HELPER FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

static ASTNode *new_ast_node(ASTNodeType type, int line) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Fatal: Memory allocation failed for AST node\n");
        exit(1);
    }
    node->type = type;
    node->line_number = line;
    node->expr_type = NULL;
    return node;
}

static char *safe_strdup(const char *s) {
    if (!s) return NULL;
    char *copy = strdup(s);
    if (!copy) {
        fprintf(stderr, "Fatal: Memory allocation failed for string\n");
        exit(1);
    }
    return copy;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * LITERAL CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_int_literal(int value, int line) {
    ASTNode *node = new_ast_node(AST_INTEGER, line);
    node->data.int_literal.value = value;
    node->expr_type = safe_strdup("int");
    return node;
}

ASTNode *ast_float_literal(float value, int line) {
    ASTNode *node = new_ast_node(AST_FLOAT, line);
    node->data.float_literal.value = value;
    node->expr_type = safe_strdup("float");
    return node;
}

ASTNode *ast_char_literal(const char *value, int line) {
    ASTNode *node = new_ast_node(AST_CHAR, line);
    node->data.char_literal.value = safe_strdup(value);
    node->expr_type = safe_strdup("char");
    return node;
}

ASTNode *ast_string_literal(const char *value, int line) {
    ASTNode *node = new_ast_node(AST_STRING, line);
    node->data.string_literal.value = safe_strdup(value);
    node->expr_type = safe_strdup("string");
    return node;
}

ASTNode *ast_identifier(const char *name, int line) {
    ASTNode *node = new_ast_node(AST_IDENTIFIER, line);
    node->data.identifier.name = safe_strdup(name);
    /* Type filled in by type checking pass */
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * OPERATION CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_binary_op(const char *op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *node = new_ast_node(AST_BINARY_OP, line);
    node->data.binary_op.op = safe_strdup(op);
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

ASTNode *ast_unary_op(const char *op, ASTNode *operand, int line) {
    ASTNode *node = new_ast_node(AST_UNARY_OP, line);
    node->data.unary_op.op = safe_strdup(op);
    node->data.unary_op.operand = operand;
    return node;
}

ASTNode *ast_function_call(const char *func_name, ASTNode *args, int line) {
    ASTNode *node = new_ast_node(AST_FUNCTION_CALL, line);
    node->data.func_call.func_name = safe_strdup(func_name);
    node->data.func_call.args = args;
    return node;
}

ASTNode *ast_array_access(const char *array_name, ASTNode *index, int line) {
    ASTNode *node = new_ast_node(AST_ARRAY_ACCESS, line);
    node->data.array_access.array_name = safe_strdup(array_name);
    node->data.array_access.index = index;
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * DECLARATION AND ASSIGNMENT CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_declaration(const char *var_type, const char *var_name,
                         int is_array, ASTNode *array_size,
                         ASTNode *init_value, int line) {
    ASTNode *node = new_ast_node(AST_DECLARATION, line);
    node->data.declaration.var_type = safe_strdup(var_type);
    node->data.declaration.var_name = safe_strdup(var_name);
    node->data.declaration.is_array = is_array;
    node->data.declaration.array_size = array_size;
    node->data.declaration.init_value = init_value;
    return node;
}

ASTNode *ast_assignment(const char *var_name, ASTNode *value, int line) {
    ASTNode *node = new_ast_node(AST_ASSIGNMENT, line);
    node->data.assignment.var_name = safe_strdup(var_name);
    node->data.assignment.value = value;
    return node;
}

ASTNode *ast_array_assignment(const char *array_name, ASTNode *index,
                              ASTNode *value, int line) {
    ASTNode *node = new_ast_node(AST_ARRAY_ASSIGN, line);
    node->data.array_assign.array_name = safe_strdup(array_name);
    node->data.array_assign.index = index;
    node->data.array_assign.value = value;
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * CONTROL STRUCTURE CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_if_stmt(ASTNode *condition, ASTNode *then_branch,
                     ASTNode *else_branch, int line) {
    ASTNode *node = new_ast_node(AST_IF_STMT, line);
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

ASTNode *ast_while_loop(ASTNode *condition, ASTNode *body, int line) {
    ASTNode *node = new_ast_node(AST_WHILE_LOOP, line);
    node->data.while_loop.condition = condition;
    node->data.while_loop.body = body;
    return node;
}

ASTNode *ast_for_loop(ASTNode *init, ASTNode *condition, ASTNode *update,
                      ASTNode *body, int line) {
    ASTNode *node = new_ast_node(AST_FOR_LOOP, line);
    node->data.for_loop.init = init;
    node->data.for_loop.condition = condition;
    node->data.for_loop.update = update;
    node->data.for_loop.body = body;
    return node;
}

ASTNode *ast_do_while(ASTNode *body, ASTNode *condition, int line) {
    ASTNode *node = new_ast_node(AST_DO_WHILE, line);
    node->data.do_while.body = body;
    node->data.do_while.condition = condition;
    return node;
}

ASTNode *ast_switch_stmt(ASTNode *expr, ASTNode *cases, int line) {
    ASTNode *node = new_ast_node(AST_SWITCH_STMT, line);
    node->data.switch_stmt.expr = expr;
    node->data.switch_stmt.cases = cases;
    return node;
}

ASTNode *ast_case_stmt(ASTNode *value, ASTNode *stmts, int is_default, int line) {
    ASTNode *node = new_ast_node(AST_CASE_STMT, line);
    node->data.case_stmt.value = value;
    node->data.case_stmt.stmts = stmts;
    node->data.case_stmt.next_case = NULL;
    node->data.case_stmt.is_default = is_default;
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * OTHER STATEMENT CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_break(int line) {
    return new_ast_node(AST_BREAK, line);
}

ASTNode *ast_continue(int line) {
    return new_ast_node(AST_CONTINUE, line);
}

ASTNode *ast_return(ASTNode *value, int line) {
    ASTNode *node = new_ast_node(AST_RETURN, line);
    node->data.return_stmt.value = value;
    return node;
}

ASTNode *ast_block(ASTNode *statements, int line) {
    ASTNode *node = new_ast_node(AST_BLOCK, line);
    node->data.block.statements = statements;
    return node;
}

ASTNode *ast_expr_stmt(ASTNode *expr, int line) {
    ASTNode *node = new_ast_node(AST_EXPR_STMT, line);
    node->data.expr_stmt.expr = expr;
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * I/O CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_input(const char *var_name, int line) {
    ASTNode *node = new_ast_node(AST_INPUT, line);
    node->data.input.var_name = safe_strdup(var_name);
    return node;
}

ASTNode *ast_output(ASTNode *expr, int line) {
    ASTNode *node = new_ast_node(AST_OUTPUT, line);
    node->data.output.expr = expr;
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * FUNCTION CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_function_def(const char *return_type, const char *func_name,
                          ASTNode *params, ASTNode *body, int line) {
    ASTNode *node = new_ast_node(AST_FUNCTION_DEF, line);
    node->data.func_def.return_type = safe_strdup(return_type);
    node->data.func_def.func_name = safe_strdup(func_name);
    node->data.func_def.params = params;
    node->data.func_def.body = body;
    return node;
}

ASTNode *ast_param(const char *param_type, const char *param_name, int line) {
    ASTNode *node = new_ast_node(AST_PARAM, line);
    node->data.param.param_type = safe_strdup(param_type);
    node->data.param.param_name = safe_strdup(param_name);
    node->data.param.next = NULL;
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * LIST CONSTRUCTORS
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_stmt_list(ASTNode *stmt, ASTNode *next) {
    ASTNode *node = new_ast_node(AST_STMT_LIST, stmt ? stmt->line_number : 0);
    node->data.stmt_list.stmt = stmt;
    node->data.stmt_list.next = next;
    return node;
}

ASTNode *ast_append_stmt(ASTNode *list, ASTNode *stmt) {
    if (!list) {
        return ast_stmt_list(stmt, NULL);
    }
    
    ASTNode *curr = list;
    while (curr->data.stmt_list.next != NULL) {
        curr = curr->data.stmt_list.next;
    }
    curr->data.stmt_list.next = ast_stmt_list(stmt, NULL);
    return list;
}

ASTNode *ast_append_case(ASTNode *case_list, ASTNode *new_case) {
    if (!case_list) {
        return new_case;
    }
    
    ASTNode *curr = case_list;
    while (curr->data.case_stmt.next_case != NULL) {
        curr = curr->data.case_stmt.next_case;
    }
    curr->data.case_stmt.next_case = new_case;
    return case_list;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PROGRAM CONSTRUCTOR
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode *ast_program(ASTNode *statements, int line) {
    ASTNode *node = new_ast_node(AST_PROGRAM, line);
    node->data.block.statements = statements;
    g_program_ast = node;
    return node;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * AST PRINTING (for debugging)
 * ═══════════════════════════════════════════════════════════════════════════ */

static void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

void ast_print(ASTNode *node, int depth) {
    if (!node) return;
    
    print_indent(depth);
    
    switch (node->type) {
        case AST_INTEGER:
            printf("INT: %d\n", node->data.int_literal.value);
            break;
        case AST_FLOAT:
            printf("FLOAT: %f\n", node->data.float_literal.value);
            break;
        case AST_CHAR:
            printf("CHAR: %s\n", node->data.char_literal.value);
            break;
        case AST_STRING:
            printf("STRING: %s\n", node->data.string_literal.value);
            break;
        case AST_IDENTIFIER:
            printf("ID: %s\n", node->data.identifier.name);
            break;
        case AST_BINARY_OP:
            printf("BINOP: %s\n", node->data.binary_op.op);
            ast_print(node->data.binary_op.left, depth + 1);
            ast_print(node->data.binary_op.right, depth + 1);
            break;
        case AST_UNARY_OP:
            printf("UNOP: %s\n", node->data.unary_op.op);
            ast_print(node->data.unary_op.operand, depth + 1);
            break;
        case AST_ASSIGNMENT:
            printf("ASSIGN: %s\n", node->data.assignment.var_name);
            ast_print(node->data.assignment.value, depth + 1);
            break;
        case AST_DECLARATION:
            printf("DECL: %s %s%s\n", 
                   node->data.declaration.var_type,
                   node->data.declaration.var_name,
                   node->data.declaration.is_array ? "[]" : "");
            if (node->data.declaration.init_value) {
                ast_print(node->data.declaration.init_value, depth + 1);
            }
            break;
        case AST_IF_STMT:
            printf("IF\n");
            print_indent(depth + 1);
            printf("Condition:\n");
            ast_print(node->data.if_stmt.condition, depth + 2);
            print_indent(depth + 1);
            printf("Then:\n");
            ast_print(node->data.if_stmt.then_branch, depth + 2);
            if (node->data.if_stmt.else_branch) {
                print_indent(depth + 1);
                printf("Else:\n");
                ast_print(node->data.if_stmt.else_branch, depth + 2);
            }
            break;
        case AST_WHILE_LOOP:
            printf("WHILE\n");
            ast_print(node->data.while_loop.condition, depth + 1);
            ast_print(node->data.while_loop.body, depth + 1);
            break;
        case AST_FOR_LOOP:
            printf("FOR\n");
            ast_print(node->data.for_loop.init, depth + 1);
            ast_print(node->data.for_loop.condition, depth + 1);
            ast_print(node->data.for_loop.update, depth + 1);
            ast_print(node->data.for_loop.body, depth + 1);
            break;
        case AST_DO_WHILE:
            printf("DO-WHILE\n");
            ast_print(node->data.do_while.body, depth + 1);
            ast_print(node->data.do_while.condition, depth + 1);
            break;
        case AST_SWITCH_STMT:
            printf("SWITCH\n");
            ast_print(node->data.switch_stmt.expr, depth + 1);
            ast_print(node->data.switch_stmt.cases, depth + 1);
            break;
        case AST_CASE_STMT:
            if (node->data.case_stmt.is_default) {
                printf("DEFAULT:\n");
            } else {
                printf("CASE:\n");
                ast_print(node->data.case_stmt.value, depth + 1);
            }
            ast_print(node->data.case_stmt.stmts, depth + 1);
            ast_print(node->data.case_stmt.next_case, depth);
            break;
        case AST_BREAK:
            printf("BREAK\n");
            break;
        case AST_CONTINUE:
            printf("CONTINUE\n");
            break;
        case AST_RETURN:
            printf("RETURN\n");
            if (node->data.return_stmt.value) {
                ast_print(node->data.return_stmt.value, depth + 1);
            }
            break;
        case AST_BLOCK:
            printf("BLOCK\n");
            ast_print(node->data.block.statements, depth + 1);
            break;
        case AST_STMT_LIST:
            ast_print(node->data.stmt_list.stmt, depth);
            ast_print(node->data.stmt_list.next, depth);
            break;
        case AST_OUTPUT:
            printf("OUTPUT\n");
            ast_print(node->data.output.expr, depth + 1);
            break;
        case AST_INPUT:
            printf("INPUT: %s\n", node->data.input.var_name);
            break;
        case AST_FUNCTION_DEF:
            printf("FUNCTION: %s %s\n", 
                   node->data.func_def.return_type,
                   node->data.func_def.func_name);
            ast_print(node->data.func_def.params, depth + 1);
            ast_print(node->data.func_def.body, depth + 1);
            break;
        case AST_FUNCTION_CALL:
            printf("CALL: %s\n", node->data.func_call.func_name);
            ast_print(node->data.func_call.args, depth + 1);
            break;
        case AST_ARRAY_ACCESS:
            printf("ARRAY_ACCESS: %s\n", node->data.array_access.array_name);
            ast_print(node->data.array_access.index, depth + 1);
            break;
        case AST_PROGRAM:
            printf("PROGRAM\n");
            ast_print(node->data.block.statements, depth + 1);
            break;
        default:
            printf("Unknown node type\n");
            break;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * AST MEMORY MANAGEMENT
 * ═══════════════════════════════════════════════════════════════════════════ */

void ast_free(ASTNode *node) {
    if (!node) return;
    
    /* Free type annotation */
    if (node->expr_type) {
        free(node->expr_type);
    }
    
    /* Free node-specific data */
    switch (node->type) {
        case AST_STRING:
            free(node->data.string_literal.value);
            break;
        case AST_CHAR:
            free(node->data.char_literal.value);
            break;
        case AST_IDENTIFIER:
            free(node->data.identifier.name);
            break;
        case AST_BINARY_OP:
            free(node->data.binary_op.op);
            ast_free(node->data.binary_op.left);
            ast_free(node->data.binary_op.right);
            break;
        case AST_UNARY_OP:
            free(node->data.unary_op.op);
            ast_free(node->data.unary_op.operand);
            break;
        case AST_FUNCTION_CALL:
            free(node->data.func_call.func_name);
            ast_free(node->data.func_call.args);
            break;
        case AST_ARRAY_ACCESS:
            free(node->data.array_access.array_name);
            ast_free(node->data.array_access.index);
            break;
        case AST_DECLARATION:
            free(node->data.declaration.var_type);
            free(node->data.declaration.var_name);
            ast_free(node->data.declaration.array_size);
            ast_free(node->data.declaration.init_value);
            break;
        case AST_ASSIGNMENT:
            free(node->data.assignment.var_name);
            ast_free(node->data.assignment.value);
            break;
        case AST_ARRAY_ASSIGN:
            free(node->data.array_assign.array_name);
            ast_free(node->data.array_assign.index);
            ast_free(node->data.array_assign.value);
            break;
        case AST_IF_STMT:
            ast_free(node->data.if_stmt.condition);
            ast_free(node->data.if_stmt.then_branch);
            ast_free(node->data.if_stmt.else_branch);
            break;
        case AST_WHILE_LOOP:
            ast_free(node->data.while_loop.condition);
            ast_free(node->data.while_loop.body);
            break;
        case AST_FOR_LOOP:
            ast_free(node->data.for_loop.init);
            ast_free(node->data.for_loop.condition);
            ast_free(node->data.for_loop.update);
            ast_free(node->data.for_loop.body);
            break;
        case AST_DO_WHILE:
            ast_free(node->data.do_while.body);
            ast_free(node->data.do_while.condition);
            break;
        case AST_SWITCH_STMT:
            ast_free(node->data.switch_stmt.expr);
            ast_free(node->data.switch_stmt.cases);
            break;
        case AST_CASE_STMT:
            ast_free(node->data.case_stmt.value);
            ast_free(node->data.case_stmt.stmts);
            ast_free(node->data.case_stmt.next_case);
            break;
        case AST_RETURN:
            ast_free(node->data.return_stmt.value);
            break;
        case AST_BLOCK:
        case AST_PROGRAM:
            ast_free(node->data.block.statements);
            break;
        case AST_EXPR_STMT:
            ast_free(node->data.expr_stmt.expr);
            break;
        case AST_INPUT:
            free(node->data.input.var_name);
            break;
        case AST_OUTPUT:
            ast_free(node->data.output.expr);
            break;
        case AST_FUNCTION_DEF:
            free(node->data.func_def.return_type);
            free(node->data.func_def.func_name);
            ast_free(node->data.func_def.params);
            ast_free(node->data.func_def.body);
            break;
        case AST_PARAM:
            free(node->data.param.param_type);
            free(node->data.param.param_name);
            ast_free(node->data.param.next);
            break;
        case AST_STMT_LIST:
            ast_free(node->data.stmt_list.stmt);
            ast_free(node->data.stmt_list.next);
            break;
        default:
            break;
    }
    
    free(node);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TYPE CHECKING (Will be expanded in semantic.c)
 * ═══════════════════════════════════════════════════════════════════════════ */

void ast_type_check(ASTNode *node) {
    /* This function is implemented in semantic.c */
    /* Declared here to satisfy the header contract */
}
