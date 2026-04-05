#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif
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
    int compile_and_run;   /* Compile to C and execute */
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
    printf("  --run, -r             Compile to C and run immediately\n");
    printf("  --tac-only, -t        Only generate TAC (no C code)\n");
    printf("  --tac-file <file>     Specify TAC output file (default: output.tac)\n");
    printf("  --print-ast           Print AST tree structure\n");
    printf("  --print-symbols       Print symbol table\n");
    printf("  --verbose, -v         Verbose output\n");
    printf("  --help, -h            Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s program.smpl program.c           # Compile to C\n", prog_name);
    printf("  %s -r program.smpl                  # Compile and run\n", prog_name);
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
 * HELPER: Derive output filename from input filename
 * e.g. "samples/sample2_conditionals.smpl" → "output/sample2_conditionals.c"
 * ═══════════════════════════════════════════════════════════════════════════ */
static void derive_output_filename(const char *input_file, char *out_buf, 
                                    size_t buf_size, const char *extension) {
    /* Find the base filename (strip directory) */
    const char *base = input_file;
    const char *p;
    for (p = input_file; *p; p++) {
        if (*p == '/' || *p == '\\') base = p + 1;
    }
    
    /* Find extension in base */
    const char *dot = NULL;
    for (p = base; *p; p++) {
        if (*p == '.') dot = p;
    }
    
    size_t name_len = dot ? (size_t)(dot - base) : strlen(base);
    
    /* Build output/basename.ext */
    snprintf(out_buf, buf_size, "output/%.*s%s", (int)name_len, base, extension);
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
        } else if (strcmp(argv[i], "--run") == 0 || strcmp(argv[i], "-r") == 0) {
            opts->compile_and_run = 1;
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
    
    /* Set default output file if not specified - derive from input filename */
    if (!opts->output_file && !opts->execute_mode) {
        static char derived_output[512];
        derive_output_filename(opts->input_file, derived_output, sizeof(derived_output), ".c");
        opts->output_file = derived_output;
    }
    
    /* Derive TAC filename from input file */
    if (strcmp(opts->tac_file, "output.tac") == 0) {
        static char derived_tac[512];
        derive_output_filename(opts->input_file, derived_tac, sizeof(derived_tac), ".tac");
        opts->tac_file = derived_tac;
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
    
    /* Ensure output directory exists */
    #ifdef _WIN32
    _mkdir("output");
    #else
    mkdir("output", 0755);
    #endif
    
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
     * COMPILE AND RUN MODE (Optional)
     * ═══════════════════════════════════════════════════════════════════════ */
    
    if (opts.compile_and_run) {
        if (opts.verbose) {
            printf("\nPhase 5: Compiling and running C code...\n");
        }
        
        /* Compile C code to executable */
        char gcc_cmd[512];
        char exe_name[256];
        snprintf(exe_name, sizeof(exe_name), "%s.exe", opts.input_file);
        
        /* Try to find GCC */
        snprintf(gcc_cmd, sizeof(gcc_cmd), "gcc \"%s\" -o \"%s\" 2>nul", 
                opts.output_file, exe_name);
        
        if (opts.verbose) {
            printf("    Running: %s\n", gcc_cmd);
        }
        
        int gcc_result = system(gcc_cmd);
        
        if (gcc_result != 0) {
            /* Try MSYS2 GCC */
            snprintf(gcc_cmd, sizeof(gcc_cmd), 
                    "C:\\msys64\\ucrt64\\bin\\gcc.exe \"%s\" -o \"%s\" 2>nul", 
                    opts.output_file, exe_name);
            
            if (opts.verbose) {
                printf("    Trying: %s\n", gcc_cmd);
            }
            
            gcc_result = system(gcc_cmd);
        }
        
        if (gcc_result != 0) {
            fprintf(stderr, "Error: Failed to compile C code with GCC\n");
            fprintf(stderr, "Make sure GCC is installed and in PATH\n");
            tac_free(&tac_ctx);
            symbol_table_free(&symbol_table);
            ast_free(g_program_ast);
            return 1;
        }
        
        if (opts.verbose) {
            printf("    ✓ C compilation successful\n");
            printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
            printf("║  PROGRAM OUTPUT                                               ║\n");
            printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
        } else {
            printf("\n--- Program Output ---\n");
        }
        
        /* Run the executable */
        char run_cmd[256];
        snprintf(run_cmd, sizeof(run_cmd), "\"%s\"", exe_name);
        system(run_cmd);
        
        printf("\n");
        
        if (opts.verbose) {
            printf("✓ Execution completed\n");
        }
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
