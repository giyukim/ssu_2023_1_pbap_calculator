#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SKIP                    2
#define SUCCESS                 1
#define FAIL                    0

#define MAX_DIGIT               30
#define MAX_DIGIT_EXPRESSION    100
#define MAX_TOKEN_COUNT         (MAX_DIGIT_EXPRESSION + 1)
#define MAX_STRING              (MAX_DIGIT_EXPRESSION + 5)
#define BINT_ARR_LEN            (MAX_DIGIT + 1)

#define POSITIVE                1
#define NEGATIVE                (-1)

// TOKEN types
#define TOKEN_INTEGER           0
#define TOKEN_OPERATOR          1
#define TOKEN_COMMAND           2
#define TOKEN_VARIABLE          3
#define TOKEN_HISTORY_VARIABLE  4
#define TOKEN_NOTHING           (-1)

#define TOKEN_UNKNOWN           (-2)
#define TOKEN_END               (-3)

// TOKEN_OPERATOR values
#define OPERATOR_PLUS           0
#define OPERATOR_UNARY_PLUS     1
#define OPERATOR_MINUS          2
#define OPERATOR_UNARY_MINUS    3
#define OPERATOR_MULTIPLY       4
#define OPERATOR_DIVDE          5
#define OPERATOR_MODULO         6
#define OPERATOR_ASSIGN         7
#define OPERATOR_NOTHING        (-1)

// TOKEN_COMMANDs
#define COMMAND_HISTORY         0
#define COMMAND_LOAD            1
#define COMMAND_REFRESH         2
#define COMMAND_RESET           3
#define COMMAND_SAVE            4
#define COMMAND_QUIT            5

#define VARIABLE_COUNT          5
#define HISTORY_VARIABLE_COUNT  3

#endif