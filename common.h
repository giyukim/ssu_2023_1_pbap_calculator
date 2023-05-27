#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 디버그 시 1로 수정
#define DEBUG                   0

// 공통 상수
#define SKIP                    2       // 스킵
#define SUCCESS                 1       // 성공
#define FAIL                    0       // 실패

#define INVALID_INDEX           (-1)

// 큰 정수 + 갖가지 크기
#define MAX_DIGIT               30                              // 정수 최대 자릿수
#define MAX_DIGIT_EXPRESSION    100                             // 수식 최대 길이
#define MAX_TOKEN_COUNT         (MAX_DIGIT_EXPRESSION + 1)      // 최대 토큰 수
#define MAX_STRING              (MAX_DIGIT_EXPRESSION + 5)      // 문자열 최대 길이
#define BINT_ARR_LEN            (MAX_DIGIT + 1)                 // 정수 배열 크기: (정수 최대 자릿수) + 1
#define VARIABLE_COUNT          5                               // 변수 개수
#define HISTORY_VARIABLE_COUNT  3                               // 히스토리 변수 개수

#define POSITIVE                1       // 양수
#define NEGATIVE                (-1)    // 음수

// 스크린 입출력
#define MAX_LINE_COUNT          20      // 화면에 보이는 최대 줄 수

#define LINE_TYPE_INPUT         0       // "입력 줄"
#define LINE_TYPE_OUTPUT        1       // "출력 줄"
#define LINE_TYPE_NOTHING       2       // 아무것도 아닌 줄. 줄 길이 측정 시 필요

// 토큰 타입
#define TOKEN_INTEGER           0       // [숫자 타입] 정수 토큰
#define TOKEN_OPERATOR          1       // 연산자 토큰
#define TOKEN_COMMAND           2       // 커맨드 토큰
#define TOKEN_VARIABLE          3       // [숫자 타입] 변수 토큰
#define TOKEN_HISTORY_VARIABLE  4       // [숫자 타입] 히스토리 변수 토큰
#define TOKEN_NOTHING           (-1)    // 아무 토큰도 아님
#define TOKEN_UNKNOWN           (-2)    // 발견 시 오류 리턴
#define TOKEN_END               (-3)    // 끝 토큰. 토큰 길이 측정 시 필요

// TOKEN_OPERATOR 값 목록
#define OPERATOR_PLUS           0       // +
#define OPERATOR_UNARY_PLUS     1       // + (단항)
#define OPERATOR_MINUS          2       // -
#define OPERATOR_UNARY_MINUS    3       // - (이항)
#define OPERATOR_MULTIPLY       4       // *
#define OPERATOR_DIVDE          5       // /
#define OPERATOR_MODULO         6       // %
#define OPERATOR_ASSIGN         7       // =
#define OPERATOR_NOTHING        (-1)    // 아무 연산자도 아님

// TOKEN_COMMAND 값 목록
#define COMMAND_NOTHING         (-1)    // 아무 커맨드도 아님
#define COMMAND_HISTORY         0       // H(istory)
#define COMMAND_LOAD            1       // L(oad)
#define COMMAND_REFRESH         2       // R(efresh)
#define COMMAND_RESET           3       // (rese)T
#define COMMAND_SAVE            4       // S(ave)
#define COMMAND_QUIT            5       // Q(uit)

#endif