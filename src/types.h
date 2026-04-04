#ifndef TYPES_H
#define TYPES_H

/* Common type definitions used across the SMPL compiler */

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_DOUBLE,
    TYPE_VOID,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_ERROR
} ValueType;

/* Convert SMPL type keywords to internal type */
ValueType smpl_type_to_value_type(const char *type_str);

/* Convert internal type to C type string */
const char *value_type_to_c(ValueType type);

/* Convert internal type to string */
const char *value_type_to_string(ValueType type);

/* Check if types are compatible for assignment */
int types_compatible(ValueType target, ValueType source);

#endif /* TYPES_H */
