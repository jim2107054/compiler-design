#ifndef AST_H
#define AST_H

#include "types.h"

/* AST Node Types */
typedef enum {
    /* Expressions */
    AST_INTEGER,
    AST_FLOAT,
    AST_CHAR,
    AST_STRING,
    AST_IDENTIFIER,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_FUNCTION_CALL,
    AST_ARRAY_ACCESS,
    
    /* Statements */
    AST_DECLARATION,
    AST_ASSIGNMENT,
    AST_ARRAY_ASSIGN,
    AST_IF_STMT,
    AST_WHILE_LOOP,
    AST_FOR_LOOP,
    AST_DO_WHILE,
    AST_SWITCH_STMT,
    AST_CASE_STMT,
    AST_BREAK,
    AST_CONTINUE,
    AST_RETURN,
    AST_BLOCK,
    AST_EXPR_STMT,
    AST_INPUT,
    AST_OUTPUT,
    
    /* Program structure */
    AST_PROGRAM,
    AST_FUNCTION_DEF,
    AST_PARAM,
    AST_STMT_LIST
} ASTNodeType;

/* Forward declaration */
struct ASTNode;

/* AST Node Structure */
typedef struct ASTNode {
    ASTNodeType type;
    int line_number;
    char *expr_type;  /* Type annotation for expressions */
    
    /* Data specific to node type */
    union {
        /* Literals */
        struct { int value; } int_literal;
        struct { float value; } float_literal;
        struct { char *value; } char_literal;
        struct { char *value; } string_literal;
        struct { char *name; } identifier;
        
        /* Binary operations */
        struct {
            char *op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;
        
        /* Unary operations */
        struct {
            char *op;
            struct ASTNode *operand;
        } unary_op;
        
        /* Function call */
        struct {
            char *func_name;
            struct ASTNode *args;
        } func_call;
        
        /* Array access */
        struct {
            char *array_name;
            struct ASTNode *index;
        } array_access;
        
        /* Declaration */
        struct {
            char *var_type;
            char *var_name;
            int is_array;
            struct ASTNode *array_size;
            struct ASTNode *init_value;
        } declaration;
        
        /* Assignment */
        struct {
            char *var_name;
            struct ASTNode *value;
        } assignment;
        
        /* Array assignment */
        struct {
            char *array_name;
            struct ASTNode *index;
            struct ASTNode *value;
        } array_assign;
        
        /* If statement */
        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;
        
        /* While loop */
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_loop;
        
        /* For loop */
        struct {
            struct ASTNode *init;
            struct ASTNode *condition;
            struct ASTNode *update;
            struct ASTNode *body;
        } for_loop;
        
        /* Do-while loop */
        struct {
            struct ASTNode *body;
            struct ASTNode *condition;
        } do_while;
        
        /* Switch statement */
        struct {
            struct ASTNode *expr;
            struct ASTNode *cases;
        } switch_stmt;
        
        /* Case statement */
        struct {
            struct ASTNode *value;
            struct ASTNode *stmts;
            struct ASTNode *next_case;
            int is_default;
        } case_stmt;
        
        /* Return statement */
        struct {
            struct ASTNode *value;
        } return_stmt;
        
        /* Block */
        struct {
            struct ASTNode *statements;
        } block;
        
        /* I/O */
        struct {
            char *var_name;
        } input;
        
        struct {
            struct ASTNode *expr;
        } output;
        
        /* Function definition */
        struct {
            char *return_type;
            char *func_name;
            struct ASTNode *params;
            struct ASTNode *body;
        } func_def;
        
        /* Parameter */
        struct {
            char *param_type;
            char *param_name;
            struct ASTNode *next;
        } param;
        
        /* Statement list */
        struct {
            struct ASTNode *stmt;
            struct ASTNode *next;
        } stmt_list;
        
        /* Expression statement */
        struct {
            struct ASTNode *expr;
        } expr_stmt;
    } data;
} ASTNode;

/* ═══════════════════════════════════════════════════════════════════════════
 * AST CONSTRUCTOR FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Literals */
ASTNode *ast_int_literal(int value, int line);
ASTNode *ast_float_literal(float value, int line);
ASTNode *ast_char_literal(const char *value, int line);
ASTNode *ast_string_literal(const char *value, int line);
ASTNode *ast_identifier(const char *name, int line);

/* Operations */
ASTNode *ast_binary_op(const char *op, ASTNode *left, ASTNode *right, int line);
ASTNode *ast_unary_op(const char *op, ASTNode *operand, int line);
ASTNode *ast_function_call(const char *func_name, ASTNode *args, int line);
ASTNode *ast_array_access(const char *array_name, ASTNode *index, int line);

/* Declarations and Assignments */
ASTNode *ast_declaration(const char *var_type, const char *var_name, 
                         int is_array, ASTNode *array_size, 
                         ASTNode *init_value, int line);
ASTNode *ast_assignment(const char *var_name, ASTNode *value, int line);
ASTNode *ast_array_assignment(const char *array_name, ASTNode *index, 
                              ASTNode *value, int line);

/* Control Structures */
ASTNode *ast_if_stmt(ASTNode *condition, ASTNode *then_branch, 
                     ASTNode *else_branch, int line);
ASTNode *ast_while_loop(ASTNode *condition, ASTNode *body, int line);
ASTNode *ast_for_loop(ASTNode *init, ASTNode *condition, ASTNode *update, 
                      ASTNode *body, int line);
ASTNode *ast_do_while(ASTNode *body, ASTNode *condition, int line);
ASTNode *ast_switch_stmt(ASTNode *expr, ASTNode *cases, int line);
ASTNode *ast_case_stmt(ASTNode *value, ASTNode *stmts, int is_default, int line);

/* Other Statements */
ASTNode *ast_break(int line);
ASTNode *ast_continue(int line);
ASTNode *ast_return(ASTNode *value, int line);
ASTNode *ast_block(ASTNode *statements, int line);
ASTNode *ast_expr_stmt(ASTNode *expr, int line);

/* I/O */
ASTNode *ast_input(const char *var_name, int line);
ASTNode *ast_output(ASTNode *expr, int line);

/* Functions */
ASTNode *ast_function_def(const char *return_type, const char *func_name, 
                          ASTNode *params, ASTNode *body, int line);
ASTNode *ast_param(const char *param_type, const char *param_name, int line);

/* Lists */
ASTNode *ast_stmt_list(ASTNode *stmt, ASTNode *next);
ASTNode *ast_append_stmt(ASTNode *list, ASTNode *stmt);
ASTNode *ast_append_case(ASTNode *case_list, ASTNode *new_case);

/* Program */
ASTNode *ast_program(ASTNode *statements, int line);

/* ═══════════════════════════════════════════════════════════════════════════
 * AST UTILITY FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Print AST for debugging */
void ast_print(ASTNode *node, int depth);

/* Free AST memory */
void ast_free(ASTNode *node);

/* Type checking pass */
void ast_type_check(ASTNode *node);

/* Global AST root (set by parser) */
extern ASTNode *g_program_ast;

#endif /* AST_H */
