#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "semantic.h"
#include "ir.h"
#include "codegen.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * SMPL COMPILER - Main Entry Point
 * ═══════════════════════════════════════════════════════════════════════════
 *  Author : MD. Jahid Hasan Jim
 *  Roll   : 2107054
 *  Course : CSE 3212 — Compiler Design Lab
 *  Inst   : Khulna University of Engineering & Technology (KUET)
 *  
 *  Space Mission Programming Language (SMPL) Compiler
 *  Complete modular architecture with:
 *    - Lexical Analysis (Flex)
 *    - Syntax Analysis (Bison)
 *    - Abstract Syntax Tree (AST)
 *    - Symbol Table
 *    - Semantic Analysis
 *    - Runtime Interpretation (NEW!)
 *    - Three-Address Code Generation
 *    - C Code Generation
 * ═══════════════════════════════════════════════════════════════════════════ */

/* External declarations from parser */
extern FILE *yyin;
extern int yyparse(void);
extern int yylineno;

/* Compilation options */
typedef struct {
    int execute_mode;      /* Run interpreter */
    int tac_only;          /* Only generate TAC */
    int print_ast;         /* Print AST tree */
    int print_symbols;     /* Print symbol table */
    int verbose;           /* Verbose output */
    const char *input_file;
    const char *output_file;
    const char *tac_file;
} CompilerOptions;

/* ═══════════════════════════════════════════════════════════════════════════
 * USAGE AND HELP
 * ═══════════════════════════════════════════════════════════════════════════ */

static void print_usage(const char *prog_name) {
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║  SMPL Compiler - Space Mission Programming Language             ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");
    printf("Usage: %s [options] <input_file> [output_file]\n\n", prog_name);
    printf("Options:\n");
    printf("  --execute, -e         Execute the program (runtime interpreter)\n");
    printf("  --tac-only, -t        Only generate TAC (no C code)\n");
    printf("  --tac-file <file>     Specify TAC output file (default: output.tac)\n");
    printf("  --print-ast           Print AST tree structure\n");
    printf("  --print-symbols       Print symbol table\n");
    printf("  --verbose, -v         Verbose output\n");
    printf("  --help, -h            Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s program.smpl program.c           # Compile to C\n", prog_name);
    printf("  %s -e program.smpl                  # Execute (interpret)\n", prog_name);
    printf("  %s -t program.smpl program.tac      # Generate TAC only\n", prog_name);
    printf("  %s --print-ast program.smpl         # Show AST structure\n", prog_name);
    printf("\n");
}

static void print_banner(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════╗\n");
    printf("║            SMPL COMPILER - v2.0 (Modular Architecture)               ║\n");
    printf("║         Space Mission Programming Language Compiler                  ║\n");
    printf("║                                                                       ║\n");
    printf("║  Author: MD. Jahid Hasan Jim (Roll: 2107054)                        ║\n");
    printf("║  Course: CSE 3212 - Compiler Design Lab                             ║\n");
    printf("║  Institution: KUET                                                   ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/* ═══════════════════════════════════════════════════════════════════════════
 * COMMAND LINE PARSING
 * ═══════════════════════════════════════════════════════════════════════════ */

static void parse_arguments(int argc, char **argv, CompilerOptions *opts) {
    memset(opts, 0, sizeof(CompilerOptions));
    opts->tac_file = "output.tac";
    
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "--execute") == 0 || strcmp(argv[i], "-e") == 0) {
            opts->execute_mode = 1;
            i++;
        } else if (strcmp(argv[i], "--tac-only") == 0 || strcmp(argv[i], "-t") == 0) {
            opts->tac_only = 1;
            i++;
        } else if (strcmp(argv[i], "--tac-file") == 0) {
            if (i + 1 < argc) {
                opts->tac_file = argv[++i];
                i++;
            } else {
                fprintf(stderr, "Error: --tac-file requires an argument\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "--print-ast") == 0) {
            opts->print_ast = 1;
            i++;
        } else if (strcmp(argv[i], "--print-symbols") == 0) {
            opts->print_symbols = 1;
            i++;
        } else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            opts->verbose = 1;
            i++;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            fprintf(stderr, "Use --help for usage information\n");
            exit(1);
        } else {
            /* Positional arguments */
            if (!opts->input_file) {
                opts->input_file = argv[i];
            } else if (!opts->output_file) {
                opts->output_file = argv[i];
            } else {
                fprintf(stderr, "Error: Too many arguments\n");
                exit(1);
            }
            i++;
        }
    }
    
    /* Validate */
    if (!opts->input_file) {
        fprintf(stderr, "Error: No input file specified\n");
        print_usage(argv[0]);
        exit(1);
    }
    
    /* Set default output file if not specified */
    if (!opts->output_file && !opts->execute_mode) {
        opts->output_file = "output.c";
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * MAIN COMPILER DRIVER
 * ═══════════════════════════════════════════════════════════════════════════ */

int main(int argc, char **argv) {
    CompilerOptions opts;
    SymbolTable symbol_table;
    TACContext tac_ctx;
    AnalysisResult result;
    
    /* Parse command line arguments */
    parse_arguments(argc, argv, &opts);
    
    /* Print banner if verbose */
    if (opts.verbose) {
        print_banner();
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * PHASE 1: LEXICAL & SYNTAX ANALYSIS (Parsing)
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.verbose) {
        printf("Phase 1: Parsing '%s'...\n", opts.input_file);
    }
    
    FILE *input = fopen(opts.input_file, "r");
    if (!input) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", opts.input_file);
        return 1;
    }
    
    yyin = input;
    
    if (yyparse() != 0 || !g_program_ast) {
        fprintf(stderr, "Error: Parsing failed\n");
        fclose(input);
        return 1;
    }
    
    fclose(input);
    
    if (opts.verbose) {
        printf("    ✓ Parsing completed successfully\n");
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * PHASE 2: AST VISUALIZATION (Optional)
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.print_ast) {
        printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
        printf("║  ABSTRACT SYNTAX TREE (AST)                                   ║\n");
        printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
        ast_print(g_program_ast, 0);
        printf("\n");
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * PHASE 3: SEMANTIC ANALYSIS
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.verbose) {
        printf("\nPhase 2: Semantic Analysis...\n");
    }
    
    result = semantic_analyze(g_program_ast);
    
    if (!result.success) {
        fprintf(stderr, "Error: Semantic analysis failed with %d error(s)\n", 
                result.error_count);
        ast_free(g_program_ast);
        return 1;
    }
    
    if (opts.verbose) {
        printf("    ✓ Semantic analysis completed\n");
        if (result.warning_count > 0) {
            printf("    ⚠ %d warning(s) reported\n", result.warning_count);
        }
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * MODE 1: EXECUTE (Runtime Interpretation)
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.execute_mode) {
        if (opts.verbose) {
            printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
            printf("║  RUNTIME EXECUTION (Interpreter Mode)                        ║\n");
            printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
        } else {
            printf("\n--- Program Output ---\n");
        }
        
        result = semantic_execute(g_program_ast);
        
        printf("\n");
        
        if (!result.success) {
            fprintf(stderr, "Execution completed with %d error(s)\n", result.error_count);
            ast_free(g_program_ast);
            return 1;
        }
        
        if (opts.verbose) {
            printf("✓ Execution completed successfully\n");
        }
        
        ast_free(g_program_ast);
        return 0;
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * PHASE 4: THREE-ADDRESS CODE (TAC) GENERATION
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.verbose) {
        printf("\nPhase 3: Generating Three-Address Code (TAC)...\n");
    }
    
    symbol_table_init(&symbol_table);
    tac_init(&tac_ctx);
    
    if (!tac_generate(g_program_ast, &tac_ctx, &symbol_table)) {
        fprintf(stderr, "Error: TAC generation failed\n");
        tac_free(&tac_ctx);
        symbol_table_free(&symbol_table);
        ast_free(g_program_ast);
        return 1;
    }
    
    if (opts.verbose) {
        printf("    ✓ TAC generation completed\n");
    }
    
    /* Write TAC to file */
    if (!tac_write_to_file(&tac_ctx, opts.tac_file)) {
        fprintf(stderr, "Error: Failed to write TAC file '%s'\n", opts.tac_file);
        tac_free(&tac_ctx);
        symbol_table_free(&symbol_table);
        ast_free(g_program_ast);
        return 1;
    }
    
    if (opts.verbose) {
        printf("    ✓ TAC written to '%s'\n", opts.tac_file);
    }
    
    /* If TAC-only mode, stop here */
    if (opts.tac_only) {
        printf("\n✓ TAC generation completed successfully\n");
        printf("  Output: %s\n\n", opts.tac_file);
        
        tac_free(&tac_ctx);
        symbol_table_free(&symbol_table);
        ast_free(g_program_ast);
        return 0;
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * PHASE 5: C CODE GENERATION
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.verbose) {
        printf("\nPhase 4: Generating C code...\n");
    }
    
    if (!codegen_generate(g_program_ast, opts.output_file, &symbol_table)) {
        fprintf(stderr, "Error: C code generation failed\n");
        tac_free(&tac_ctx);
        symbol_table_free(&symbol_table);
        ast_free(g_program_ast);
        return 1;
    }
    
    if (opts.verbose) {
        printf("    ✓ C code written to '%s'\n", opts.output_file);
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * SYMBOL TABLE DISPLAY (Optional)
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.print_symbols) {
        symbol_table_print(&symbol_table);
    }
    
    /* ═══════════════════════════════════════════════════════════════════════
     * CLEANUP & SUCCESS
     * ═══════════════════════════════════════════════════════════════════════ */
    
    tac_free(&tac_ctx);
    symbol_table_free(&symbol_table);
    ast_free(g_program_ast);
    
    if (opts.verbose) {
        printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
        printf("║  COMPILATION SUCCESSFUL                                       ║\n");
        printf("╚═══════════════════════════════════════════════════════════════╝\n");
        printf("\nOutput files:\n");
        printf("  C code: %s\n", opts.output_file);
        printf("  TAC:    %s\n", opts.tac_file);
        printf("\nYou can now compile the C code:\n");
        printf("  gcc %s -o program\n", opts.output_file);
        printf("  ./program\n\n");
    } else {
        printf("\n✓ Compilation completed successfully\n");
        printf("  Output: %s\n", opts.output_file);
        printf("  TAC:    %s\n\n", opts.tac_file);
    }
    
    return 0;
}
