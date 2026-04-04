#include "types.h"
#include <string.h>

/* Convert SMPL type keywords to internal type */
ValueType smpl_type_to_value_type(const char *type_str) {
    if (!type_str) return TYPE_ERROR;
    
    if (strcmp(type_str, "int") == 0 || strcmp(type_str, "CARGO_INT") == 0)
        return TYPE_INT;
    if (strcmp(type_str, "float") == 0 || strcmp(type_str, "CARGO_FLOAT") == 0)
        return TYPE_FLOAT;
    if (strcmp(type_str, "char") == 0 || strcmp(type_str, "CARGO_CHAR") == 0)
        return TYPE_CHAR;
    if (strcmp(type_str, "double") == 0 || strcmp(type_str, "CARGO_DOUBLE") == 0)
        return TYPE_DOUBLE;
    if (strcmp(type_str, "void") == 0 || strcmp(type_str, "VOID_SPACE") == 0)
        return TYPE_VOID;
    if (strcmp(type_str, "string") == 0)
        return TYPE_STRING;
    
    return TYPE_ERROR;
}

/* Convert internal type to C type string */
const char *value_type_to_c(ValueType type) {
    switch (type) {
        case TYPE_INT:    return "int";
        case TYPE_FLOAT:  return "float";
        case TYPE_CHAR:   return "char";
        case TYPE_DOUBLE: return "double";
        case TYPE_VOID:   return "void";
        case TYPE_STRING: return "char*";
        case TYPE_BOOL:   return "int";
        default:          return "int";
    }
}

/* Convert internal type to string */
const char *value_type_to_string(ValueType type) {
    switch (type) {
        case TYPE_INT:    return "int";
        case TYPE_FLOAT:  return "float";
        case TYPE_CHAR:   return "char";
        case TYPE_DOUBLE: return "double";
        case TYPE_VOID:   return "void";
        case TYPE_STRING: return "string";
        case TYPE_BOOL:   return "bool";
        default:          return "error";
    }
}

/* Check if types are compatible for assignment */
int types_compatible(ValueType target, ValueType source) {
    /* Exact match */
    if (target == source) return 1;
    
    /* Widening conversions (safe) */
    if (target == TYPE_FLOAT && source == TYPE_INT) return 1;
    if (target == TYPE_DOUBLE && source == TYPE_INT) return 1;
    if (target == TYPE_DOUBLE && source == TYPE_FLOAT) return 1;
    
    /* Integer promotions */
    if (target == TYPE_INT && source == TYPE_CHAR) return 1;
    if (target == TYPE_INT && source == TYPE_BOOL) return 1;
    
    /* Narrowing conversions (allowed with warning) */
    if (target == TYPE_INT && (source == TYPE_FLOAT || source == TYPE_DOUBLE)) return 1;
    if (target == TYPE_CHAR && source == TYPE_INT) return 1;
    
    return 0;
}
