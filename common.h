#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 디버그 시 1로 수정
#define DEBUG                   0

// Common constants
#define SKIP                    2
#define SUCCESS                 1
#define FAIL                    0

#define INVALID_INDEX           (-1)

#define VARIABLE_COUNT          5
#define HISTORY_VARIABLE_COUNT  3

// Big Integer
#define MAX_DIGIT               30
#define MAX_DIGIT_EXPRESSION    100
#define MAX_TOKEN_COUNT         (MAX_DIGIT_EXPRESSION + 1)
#define MAX_STRING              (MAX_DIGIT_EXPRESSION + 5)
#define BINT_ARR_LEN            (MAX_DIGIT + 1)

#define POSITIVE                1
#define NEGATIVE                (-1)

// Screen IOs
#define MAX_LINE_COUNT          20

#define LINE_TYPE_INPUT         0
#define LINE_TYPE_OUTPUT        1
#define LINE_TYPE_NOTHING       2

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
#define COMMAND_NOTHING         (-1)
#define COMMAND_HISTORY         0
#define COMMAND_LOAD            1
#define COMMAND_REFRESH         2
#define COMMAND_RESET           3
#define COMMAND_SAVE            4
#define COMMAND_QUIT            5

#endif