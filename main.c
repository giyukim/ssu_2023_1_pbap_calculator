#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Debug
#define DEBUG                   0                                       // 디버그 여부 (1 : 디버그)

// Common constants
#define SKIP                    2                                       //
#define SUCCESS                 1                                       //
#define FAIL                    0                                       //
#define INVALID_INDEX           (-1)                                    //
#define VARIABLE_COUNT          5                                       //
#define HISTORY_VARIABLE_COUNT  3                                       //

#define MAX_DIGIT               30                                      // 최대 정수 자릿 수
#define MAX_DIGIT_EXPRESSION    100                                     // 최대 수식 자릿 수
#define MAX_TOKEN_COUNT         (MAX_DIGIT_EXPRESSION + 1)              // 최대 토큰 수
#define MAX_STRING              (MAX_DIGIT_EXPRESSION + 5)              // 수식 배열 크기
#define BINT_ARR_LEN            (MAX_DIGIT + 1)                         // 정수 배열 크기

#define POSITIVE                1                                       // 정수 : 양수
#define NEGATIVE                (-1)                                    // 정수 : 음수

// Screen IOs
#define MAX_LINE_COUNT          20                                      // 최대 저장 줄 수

#define LINE_TYPE_INPUT         0                                       //
#define LINE_TYPE_OUTPUT        1                                       //
#define LINE_TYPE_NOTHING       2                                       //

// TOKEN types
#define TOKEN_INTEGER           0                                       //
#define TOKEN_OPERATOR          1                                       //
#define TOKEN_COMMAND           2                                       //
#define TOKEN_VARIABLE          3                                       //
#define TOKEN_HISTORY_VARIABLE  4                                       //
#define TOKEN_NOTHING           (-1)                                    //

#define TOKEN_UNKNOWN           (-2)                                    //
#define TOKEN_END               (-3)                                    //

// TOKEN_OPERATOR values
#define OPERATOR_PLUS           0                                       //
#define OPERATOR_UNARY_PLUS     1                                       //
#define OPERATOR_MINUS          2                                       //
#define OPERATOR_UNARY_MINUS    3                                       //
#define OPERATOR_MULTIPLY       4                                       //
#define OPERATOR_DIVDE          5                                       //
#define OPERATOR_MODULO         6                                       //
#define OPERATOR_ASSIGN         7                                       //
#define OPERATOR_NOTHING        (-1)                                    //

// TOKEN_COMMANDs
#define COMMAND_NOTHING         (-1)                                    //
#define COMMAND_HISTORY         0                                       //
#define COMMAND_LOAD            1                                       //
#define COMMAND_REFRESH         2                                       //
#define COMMAND_RESET           3                                       //
#define COMMAND_SAVE            4                                       //
#define COMMAND_QUIT            5                                       //

#define LOOP_CONTINUE           1                                       //
#define LOOP_STOP               0                                       //

#define MAX_GROUP_COUNT         (MAX_DIGIT_EXPRESSION / 2 + 5)          //

// NUMERALs (항수)
#define NUMERAL_NOTHING         (-1)                                    // 항수 오류
#define NUMERAL_NULLINARY       0                                       // "0항"
#define NUMERAL_UNARY           1                                       // 단항
#define NUMERAL_BINARY          2                                       // 이항

#define LEVEL_NOTHING           (-1)                                    // 연산자 레벨 오류

#define MAX_FILENAME_STRING     16                                      // 파일 이름 배열 최대 크기

// main.c
int eval_expression(char exp_raw[MAX_STRING], int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], char line_result[MAX_STRING], int command_result[1]);
int screen_iteration(int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int lines_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING]);

// calc.c
/**
 * 연산자 -> 연산자 레벨 반환 함수
 */
int get_operator_level(int[2]);

/**
 * 연산자 레벨 -> 항수 변환 함수
 */
int level_to_numeral(int);

/**
 * 그룹 길이 반환 함수.
 * [index][2] == LEVEL_NOTHING을 만족하는 가장 작은 index값이 있을 때,
 * index을 리턴함
 */
int get_group_len(int[MAX_GROUP_COUNT][3]);

/**
 * 그룹 초기화 함수.
 * 모든 index에 대해 [index][2] = LEVEL_NOTHING을 실행함
 */
void group_reset(int[MAX_GROUP_COUNT][3]);

/**
 * 토큰 그룹화 함수.
 * 그룹 결과를 exp_groups에 저장.
 * 실패 시 INVALID_INDEX 리턴.
 */
int group_expression(int exp_groups[MAX_GROUP_COUNT][3], int[MAX_TOKEN_COUNT][2], int, int);

/**
 * exp_token에 따라 토큰에 있는 큰 정수값 result로 복사
 */
int copy_from_number_type(int exp_token[2], int[MAX_TOKEN_COUNT][BINT_ARR_LEN], int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int result[BINT_ARR_LEN]);

/**
 * token_index를 포함하는 그룹 발견 시:
 *  - result에 그룹 결과값 저장
 *  - 해당 그룹의 끝 인덱스 반환.
 * 아니면 INVALID_INDEX 반환
 */
int search_group_result(int token_index, int, int[MAX_GROUP_COUNT][3], int[MAX_GROUP_COUNT][BINT_ARR_LEN], int[BINT_ARR_LEN]);
/**
 * 이항 연산자 값인 operator_type에 따라:
 *  - result = result (연산자) victim
 * 단항이 올 경우 FAIL 리턴
 */
int operator_calculation(int operator_type, int[BINT_ARR_LEN], int[BINT_ARR_LEN]);

/**
 * 그룹화된 토큰 계산
 */
int calculate_groups(int[MAX_TOKEN_COUNT][2], int[MAX_TOKEN_COUNT][BINT_ARR_LEN], int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int[MAX_GROUP_COUNT][3], int[MAX_GROUP_COUNT][BINT_ARR_LEN]);

/**
 * 토큰화된 수식 처리 및 계산.
 * 커맨드 모드는 따로 핸들링하지 않으므로 커맨드 토큰 발견 시 SKIP 리턴
 */
int eval_exp_tokens(int[MAX_TOKEN_COUNT][2], int[MAX_TOKEN_COUNT][BINT_ARR_LEN], int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int, int, int[BINT_ARR_LEN]);

/**
 * 히스토리 저장 함수
 */
void update_history(int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int[BINT_ARR_LEN]);

// command.c
void command_history(int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int[MAX_LINE_COUNT], char[MAX_LINE_COUNT][MAX_STRING]);         // History (H/h) Command
int command_load(int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN]);                                 // Load    (L/l) Command
int command_save(int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN]);                                 // Save    (S/s) Command
void command_refresh(int[MAX_LINE_COUNT]);                                                                                      // Refresh (R/r) Command
void command_reset(int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int[MAX_LINE_COUNT]);          // Reset   (T/t) Command
int command_quit();                                                                                                             // Quit    (Q/q) Command

// draw.c
void draw_main(int variable[VARIABLE_COUNT][BINT_ARR_LEN], int line_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING]);
void draw_result(char result[MAX_STRING + 5]);
void draw_input(char result[MAX_STRING]);
void draw_lines(int line_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING]);
void append_line(int[MAX_LINE_COUNT], char[MAX_LINE_COUNT][MAX_STRING], int, char[MAX_STRING]);
void clear_screen();
void await_input(char dest[MAX_STRING]);

// file_io.c
void big_int_fprint(int [BINT_ARR_LEN], char [BINT_ARR_LEN]);                 // To String
void big_int_tointeger(char [BINT_ARR_LEN], int [BINT_ARR_LEN]);                // TO Intger

int file_io_write_new(char [MAX_FILENAME_STRING], char [][MAX_STRING], int);    // 새로운 파일로 출력
int file_io_write_add(char [MAX_FILENAME_STRING], char [MAX_STRING]);           // 기존 파일에 추가 출력
int file_io_read(char [MAX_FILENAME_STRING], int, char [][MAX_STRING]);         // 파일 입력
int file_remove(char [MAX_FILENAME_STRING]);                                    // 파일 삭제

int status_save(int [5][BINT_ARR_LEN], int [3][BINT_ARR_LEN]);                  // 상태 저장
int status_load(int [5][BINT_ARR_LEN], int [3][BINT_ARR_LEN]);                  // 상태 로드

// int_op.c
int big_int_lcomb(int, int[BINT_ARR_LEN], int, int[BINT_ARR_LEN], int[BINT_ARR_LEN]);
int big_int_shift(int, int[BINT_ARR_LEN], int[BINT_ARR_LEN]);
int big_int_times(int, int[BINT_ARR_LEN], int[BINT_ARR_LEN]);
int big_int_mul(int[BINT_ARR_LEN], int[BINT_ARR_LEN], int[BINT_ARR_LEN]);
int big_int_divmod(int[BINT_ARR_LEN], int[BINT_ARR_LEN], int[BINT_ARR_LEN], int[BINT_ARR_LEN]);

void big_int_from(long long, int[BINT_ARR_LEN]);
int big_int_len(int[BINT_ARR_LEN]);
void big_int_copy(int[BINT_ARR_LEN], int[BINT_ARR_LEN]);
int big_int_fix(int, int[BINT_ARR_LEN]);
int big_int_parse(char[MAX_STRING], int, int, int[BINT_ARR_LEN]);
void big_int_tostring(int[BINT_ARR_LEN], char[MAX_STRING]);
void big_int_print(int[BINT_ARR_LEN]);

// parse.c
_Bool is_digit_integer(char);
_Bool is_number_type(int[2]);
_Bool is_unary_operator(int[2]);
_Bool is_assign_operator(int[2]);

int assign_token_operator(char, int[2], _Bool);
int assign_token_command(char, int[2]);
int assign_token_variable(char, int[2]);
int assign_token(char, int[MAX_TOKEN_COUNT][2], int);

int parse_to_token(char[MAX_STRING], char, int[MAX_TOKEN_COUNT][2], int[MAX_TOKEN_COUNT][BINT_ARR_LEN]);
_Bool token_validate_command(int[MAX_TOKEN_COUNT][2]);
int get_token_len(int[MAX_TOKEN_COUNT][2]);
void debug_print_tokens(int[MAX_TOKEN_COUNT][2], int[MAX_TOKEN_COUNT][BINT_ARR_LEN]);


int main() {
    int variables[VARIABLE_COUNT][BINT_ARR_LEN];
    int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN];
    int lines_type[MAX_LINE_COUNT];
    char lines[MAX_LINE_COUNT][MAX_STRING];

    while(1) if(screen_iteration(variables, history_variables, lines_type, lines) == LOOP_STOP) break;
}



int eval_expression(char exp_raw[MAX_STRING], int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], char line_result[MAX_STRING], int command_result[1])
{
    int count_exp_raw = strlen(exp_raw);
    int exp_tokens[MAX_TOKEN_COUNT][2];
    int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN] = {0};
    int big_int_result[BINT_ARR_LEN], function_result;

    // 문자열 -> 토큰
    if(parse_to_token(exp_raw, count_exp_raw, exp_tokens, exp_tokens_integer) == FAIL) return FAIL;

    // 토큰 계산 및 실행
    int token_len = get_token_len(exp_tokens);
    if((function_result = eval_exp_tokens(exp_tokens, exp_tokens_integer, variables, history_variables, 0, token_len - 1, big_int_result)) != SKIP) {
        if(function_result == FAIL) return FAIL;
        big_int_tostring(big_int_result, line_result);
        command_result[0] = COMMAND_NOTHING;
        return SUCCESS;
    } else if(token_validate_command(exp_tokens)) { // 커맨드 핸들링
        command_result[0] = exp_tokens[0][1];
        return SUCCESS;
    }
    return FAIL;
}

int screen_iteration(int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int lines_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING])
{
    char exp_raw[MAX_STRING], str_result[MAX_STRING];
    int command_result[1], loop = LOOP_CONTINUE;
    _Bool error = 0;
    static _Bool first = 1;

    if(first) { // 첫 실행 시 화면 출력
        command_reset(variables, history_variables, lines_type);
        clear_screen();
        draw_main(variables, lines_type, lines);
        first = 0;
    }

    // 입력 대기
    await_input(exp_raw);

    // 입력값을 줄에 추가
    append_line(lines_type, lines, LINE_TYPE_INPUT, exp_raw);

    // 입력값 해석+계산 후 결과값 저장
    if(eval_expression(exp_raw, variables, history_variables, str_result, command_result) == FAIL) {
        error = 1;
        goto raise_error;
    }

    // 커맨드인지 확인
    switch(command_result[0]) {
        // 커맨드가 아님. 단순 식
        case COMMAND_NOTHING:
            break;
        case COMMAND_HISTORY:
            command_history(history_variables, lines_type, lines);
            goto print_screen;
        case COMMAND_SAVE:
            if(command_save(variables, history_variables) == FAIL) {
                error = 1;
                goto raise_error;
            }
            strcpy(str_result, "cal.txt에 저장");
            break;
        case COMMAND_LOAD:
            if(command_load(variables, history_variables) == FAIL) {
                error = 1;
                goto raise_error;
            }
            command_refresh(lines_type);
            strcpy(str_result, "cal.txt로부터 복구");
            break;
        case COMMAND_REFRESH:
            command_refresh(lines_type);
            goto print_screen;
        case COMMAND_RESET:
            command_reset(variables, history_variables, lines_type);
            goto print_screen;
        case COMMAND_QUIT:
            strcpy(str_result, "bye");
            loop = LOOP_STOP;
            break;
        default:
            error = 1;
            goto raise_error;
    }

    raise_error:
    if(error) strcpy(str_result, "error");

    // 출력값 줄에 추가
    append_line(lines_type, lines, LINE_TYPE_OUTPUT, str_result);

    print_screen:
    // 화면 출력
    clear_screen();
    draw_main(variables, lines_type, lines);

    return loop;
}

void command_history(int history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int lines_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING])
{
    char temp_string[MAX_STRING];
    for(int i = 0; i < HISTORY_VARIABLE_COUNT; i++) {
        char print_string[MAX_STRING + 5] = {'h', (char)i + '0' + 1, ' ', '=', ' '};
        big_int_tostring(history[i], temp_string);
        strcat(print_string, temp_string);
        append_line(lines_type, lines, LINE_TYPE_OUTPUT, print_string);
    }
}

void command_refresh(int lines_type[MAX_LINE_COUNT])
{
    for(int i = 0; i < MAX_LINE_COUNT; i++) {
        lines_type[i] = LINE_TYPE_NOTHING;
    }
}

void command_reset(int dest_variable[VARIABLE_COUNT][BINT_ARR_LEN], int dest_history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int lines_type[MAX_LINE_COUNT])
{
    for(int loop_temp_i = 0; loop_temp_i < VARIABLE_COUNT; loop_temp_i++) {
        big_int_from(0, dest_variable[loop_temp_i]);
        if(loop_temp_i >= 0 && loop_temp_i < HISTORY_VARIABLE_COUNT) big_int_from(0, dest_history[loop_temp_i]);
    }
    command_refresh(lines_type);
}

int command_load(int dest_variable[VARIABLE_COUNT][BINT_ARR_LEN], int dest_history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN])
{
    return status_load(dest_variable, dest_history) ? SUCCESS : FAIL;
}

int command_save(int variable[VARIABLE_COUNT][BINT_ARR_LEN], int history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN])
{
    return status_save(variable, history) ? SUCCESS : FAIL;
}

void draw_main(int variable[VARIABLE_COUNT][BINT_ARR_LEN], int line_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING]){
    printf("************************************************************************\n");
    printf("                               큰정수계산기\n");
    printf("************************************************************************\n");
    printf("* 변 수\n");
    printf("************************************************************************\n");
    printf("a = ");
    big_int_print(variable[0]);
    printf("\nb = ");
    big_int_print(variable[1]);
    printf("\nc = ");
    big_int_print(variable[2]);
    printf("\nd = ");
    big_int_print(variable[3]);
    printf("\ne = ");
    big_int_print(variable[4]);
    printf("\n************************************************************************\n");
    printf("Help : H(istory) L(oad) R(efresh) (rese)T S(ave) Q(uit)\n"); // Reflesh -> Refresh
    printf("************************************************************************\n");
    draw_lines(line_type, lines);
}

void draw_lines(int line_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING])
{
    for(int i = 0; i < MAX_LINE_COUNT && line_type[i] != LINE_TYPE_NOTHING; i++) {
        if(line_type[i] == LINE_TYPE_INPUT) {
            draw_input(lines[i]);
            printf("\n");
        } else if(line_type[i] == LINE_TYPE_OUTPUT) {
            draw_result(lines[i]);
            printf("\n");
        }
    }
}

void draw_result(char result[MAX_STRING + 5])
{
    printf("======> %s", result);
}

void await_input(char dest[MAX_STRING])
{
    printf("(input) ");
    // scanf("%[^\n]s", dest);
    // 버그때문에 scanf를 못 쓰겠음
    int op, i = 0;
    while((op = getchar()) != '\n') {
        dest[i++] = op;
    }
    dest[i] = '\0';
}

void draw_input(char result[MAX_STRING])
{
    printf("(input) %s", result);
}

void clear_screen()
{
    system("@cls||clear");
}

void append_line(int lines_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING], int incoming_type, char incoming_line[MAX_STRING])
{
    int line_count = 0;
    for(; line_count < MAX_LINE_COUNT; line_count++) {
        if(lines_type[line_count] == LINE_TYPE_NOTHING)
            break;
    }

    if(line_count == MAX_LINE_COUNT) {
        for(int i = 0; i < MAX_LINE_COUNT - 1; i++) {
            lines_type[i] = lines_type[i+1];
            strcpy(lines[i], lines[i+1]);
        }
        line_count = MAX_LINE_COUNT - 1;
    }
    lines_type[line_count] = incoming_type;
    strcpy(lines[line_count], incoming_line);
}

int get_operator_level(int exp_token[2])
{
    if(exp_token[0] != TOKEN_OPERATOR) return LEVEL_NOTHING;
    switch(exp_token[1]) {
        case OPERATOR_UNARY_PLUS:
        case OPERATOR_UNARY_MINUS:  return 3;
        case OPERATOR_MULTIPLY:
        case OPERATOR_DIVDE:
        case OPERATOR_MODULO:       return 2;
        case OPERATOR_PLUS:
        case OPERATOR_MINUS:        return 1;
        case OPERATOR_ASSIGN:       return 0;
        default:                    return LEVEL_NOTHING;
    }
}

int level_to_numeral(int level)
{
    switch(level) {
        case 3:  return NUMERAL_UNARY;
        case 2:
        case 1:  return NUMERAL_BINARY;
        case 0:  return NUMERAL_NULLINARY;
        default: return NUMERAL_NOTHING;
    }
}

int get_group_len(int exp_groups[MAX_GROUP_COUNT][3])
{
    for(int i = 0; i < MAX_GROUP_COUNT; i++) if(exp_groups[i][2] == LEVEL_NOTHING) return i;
    return MAX_GROUP_COUNT;
}

void group_reset(int exp_groups[MAX_GROUP_COUNT][3])
{
    for(int i = 0; i < MAX_GROUP_COUNT; i++) exp_groups[i][2] = LEVEL_NOTHING;
}

int group_expression(int exp_groups[MAX_GROUP_COUNT][3], int exp_tokens[MAX_TOKEN_COUNT][2], int level, int start)
{
    int end = start;
    if(level == get_operator_level(exp_tokens[start]) && is_unary_operator(exp_tokens[start])) end = start + 1;
    else {
        for(; exp_tokens[end][0] != TOKEN_END; end++) {
            int operator_level = get_operator_level(exp_tokens[end]);

            if(is_number_type(exp_tokens[end])) continue; // 숫자 타입
            else if(operator_level == LEVEL_NOTHING) return INVALID_INDEX; // 연산자도 아님. 커맨드?
            else if(level == operator_level) continue; // 현재 레벨 == 연산자 레벨
            else if(level > operator_level) break; // 현재 래벨 > 연산자 레벨
            else if(is_unary_operator(exp_tokens[end])) { // 현재 레벨 < 연산자 레벨 && 단항 연산자
                end = group_expression(exp_groups, exp_tokens, level+1, end);
                if(end == INVALID_INDEX) return INVALID_INDEX;
            } else { // 현재 레벨 < 연산자 레벨 && 이항 연산자
                end = group_expression(exp_groups, exp_tokens, level+1, end-1);
                if(end == INVALID_INDEX) return INVALID_INDEX;
            }
        }
        end--;
    }

    int group_length = get_group_len(exp_groups);
    exp_groups[group_length][0] = start;
    exp_groups[group_length][1] = end;
    exp_groups[group_length][2] = level;
    return end;
}

void debug_print_groups(int exp_groups[MAX_GROUP_COUNT][3], int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN])
{
    if(!DEBUG) return;
    printf("Groups : \n");
    for(int i = 0; exp_groups[i][2] != NUMERAL_NOTHING; i++) {
        printf("    Start: %3d, End: %3d, Level: %d ", exp_groups[i][0], exp_groups[i][1], exp_groups[i][2]);
        switch(level_to_numeral(exp_groups[i][2])) {
            case NUMERAL_BINARY:    printf("(BINARY)   "); break;
            case NUMERAL_UNARY:     printf("(UNARY)    "); break;
            case NUMERAL_NULLINARY: printf("(NULLINARY)"); break;
            case NUMERAL_NOTHING:   printf("(NOTHING)  "); break;
        }
        printf(" -> ");
        big_int_print(exp_groups_result[i]);
        printf("\n");
    }
}

int copy_from_number_type(int exp_token[2], int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN], int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int result[BINT_ARR_LEN])
{
    switch(exp_token[0]) {
        case TOKEN_INTEGER:
            big_int_copy(exp_tokens_integer[exp_token[1]], result);
            return SUCCESS;
        case TOKEN_VARIABLE:
            big_int_copy(variables[exp_token[1]], result);
            return SUCCESS;
        case TOKEN_HISTORY_VARIABLE:
            big_int_copy(history_variables[exp_token[1]], result);
            return SUCCESS;
    }
    return FAIL;
}

int search_group_result(int token_index, int group_index, int exp_groups[MAX_GROUP_COUNT][3], int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN], int result[BINT_ARR_LEN])
{
    // 만약 token_index를 포함하는 그룹이 현재 그룹보다 전에 있었을 경우
    // 가장 최근 그룹 순으로 검색
    for(int g_index2 = group_index - 1; g_index2 >= 0; g_index2--) {
        int start = exp_groups[g_index2][0], end = exp_groups[g_index2][1];
        if(token_index >= start && token_index <= end) {
            big_int_copy(exp_groups_result[g_index2], result);
            return end;
        }
    }
    return INVALID_INDEX;
}

int operator_calculation(int operator_type, int victim[BINT_ARR_LEN], int result[BINT_ARR_LEN])
{
    int dummy[BINT_ARR_LEN];
    switch(operator_type) {
        case OPERATOR_PLUS: // 덧셈
            return big_int_lcomb(1, result, 1, victim, result);
        case OPERATOR_MINUS: // 뺄셈
            return big_int_lcomb(1, result, -1, victim, result);
        case OPERATOR_MULTIPLY: // 곱셈
            return big_int_mul(result, victim, result);
        case OPERATOR_DIVDE: // 나눗셈
            return big_int_divmod(result, victim, result, dummy);
        case OPERATOR_MODULO: // 나머지
            return big_int_divmod(result, victim, dummy, result);
        case OPERATOR_NOTHING: // 단순 복사
            big_int_copy(victim, result);
            return SUCCESS;
        default:
            return FAIL;
    }
}

int calculate_groups(int exp_tokens[MAX_TOKEN_COUNT][2], int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN], int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int exp_groups[MAX_GROUP_COUNT][3], int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN])
{
    int group_len = get_group_len(exp_groups);
    // 전체 그룹 계산. [0]부터 차례대로
    for(int group_index = 0; group_index < group_len; group_index++) {
        int start = exp_groups[group_index][0], end = exp_groups[group_index][1], level = exp_groups[group_index][2];
        int group_numeral = level_to_numeral(level);
        int result_big_int[BINT_ARR_LEN];

        // 그룹 타입이 단항일 경우
        if(group_numeral == NUMERAL_UNARY) {
            int unary_operator_type = exp_tokens[start][1];

            // 숫자 토큰의 큰 정수값 복사
            if(copy_from_number_type(exp_tokens[start+1], exp_tokens_integer, variables, history_variables, result_big_int) == FAIL) return FAIL;

            // 만약 부호가 "-"일 경우
            if(unary_operator_type == OPERATOR_UNARY_MINUS) {
                // result_big_int = -result_big_int
                if(big_int_times(-1, result_big_int, result_big_int) == FAIL) return FAIL;
            }
        } else if(group_numeral == NUMERAL_BINARY || group_numeral == NUMERAL_NULLINARY) { // 그룹 타입이 이항이거나 0항일 경우
            int operator_type, temp_big_int[BINT_ARR_LEN], dummy_big_int[BINT_ARR_LEN];
            for(int token_index = start; token_index <= end; token_index++) {
                // 만약 타입이 이항인데 i가 처음이 아닐 경우 -> i 앞에 부호가 있음
                if(group_numeral == NUMERAL_BINARY && token_index != start) {
                    // i 앞에 있는게 부호가 아닐 경우
                    if(exp_tokens[token_index][0] != TOKEN_OPERATOR) return FAIL;
                    operator_type = exp_tokens[token_index][1];
                    token_index++;
                } else {
                    operator_type = OPERATOR_NOTHING;
                }

                // 만약 i가 포함된 그룹이 현재 그룹보다 전에 없었을 경우
                int new_index = search_group_result(token_index, group_index, exp_groups, exp_groups_result, temp_big_int);
                if(new_index == INVALID_INDEX) {
                    // 숫자 토큰의 큰 정수값 복사
                    if(copy_from_number_type(exp_tokens[token_index], exp_tokens_integer, variables, history_variables, temp_big_int) == FAIL) return FAIL;
                } else {
                    token_index = new_index;
                }

                // 실제 계산
                if(operator_calculation(operator_type, temp_big_int, result_big_int) == FAIL) return FAIL;
            }
        }

        // 결과값을 그룹 결과값으로 복사
        big_int_copy(result_big_int, exp_groups_result[group_index]);
    }

    return SUCCESS;
}

int eval_exp_tokens(int exp_tokens[MAX_TOKEN_COUNT][2], int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN], int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int start, int end, int result[BINT_ARR_LEN])
{
    // 만약 수식이 대입 모드인 경우
    if(exp_tokens[start][0] == TOKEN_VARIABLE && is_assign_operator(exp_tokens[start + 1])) {
        int temp_result[BINT_ARR_LEN];
        // 우변을
        if(eval_exp_tokens(exp_tokens, exp_tokens_integer, variables, history_variables, start + 2, end, temp_result) == FAIL) return FAIL;

        // 좌변으로 복사
        big_int_copy(temp_result, variables[exp_tokens[start][1]]);
        big_int_copy(temp_result, result);
        return SUCCESS;
    }

    // 아니면 일반 수식 모드
    for(int i = start; i <= end; i++) {
        // 만약 대입 연산자가 있을 경우
        if(is_assign_operator(exp_tokens[i])) return FAIL;

        // 만약 커맨드 토큰이 있을 경우
        if(exp_tokens[i][0] == TOKEN_COMMAND) return SKIP;
    }

    // { {start1, end1, level1}, {start2, end2, level2}, ..., {startn, endn, LEVEL_NOTHING} }
    int exp_groups[MAX_GROUP_COUNT][3];
    int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN];
    group_reset(exp_groups);

    // 토큰 그룹화
    if(group_expression(exp_groups, exp_tokens, 0, start) == INVALID_INDEX) return FAIL;

    // 그룹 계산
    int exp_groups_len = get_group_len(exp_groups);
    if(calculate_groups(exp_tokens, exp_tokens_integer, variables, history_variables, exp_groups, exp_groups_result) == FAIL) return FAIL;

    // 마지막 그룹 계산 결과 -> 결과값으로 복사
    big_int_copy(exp_groups_result[exp_groups_len - 1], result);

    // 히스토리 업데이트
    update_history(history_variables, result);

    // <디버그>
    debug_print_groups(exp_groups, exp_groups_result);
    // </디버그>

    return SUCCESS;
}

void update_history(int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int eval_result[BINT_ARR_LEN])
{
    for(int i = HISTORY_VARIABLE_COUNT - 2; i >= 0; i--) big_int_copy(history_variables[i], history_variables[i+1]);
    big_int_copy(eval_result, history_variables[0]);
}

void big_int_fprint(int source[BINT_ARR_LEN], char dest[BINT_ARR_LEN])
{
    for(int temp_loop_original = 0; temp_loop_original < big_int_len(source); temp_loop_original++) dest[temp_loop_original] = (char)(source[temp_loop_original] + '0');
    if(source[MAX_DIGIT] == +1) dest[big_int_len(source)] = '+';
    else if(source[MAX_DIGIT] == -1) dest[big_int_len(source)] = '-';
}

void big_int_tointeger(char source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
    big_int_from(0, dest);
    for(int temp_loop_original = 0; temp_loop_original < strlen(source) - 1; temp_loop_original++) dest[temp_loop_original] = (int)(source[temp_loop_original]) - '0';
    if(source[strlen(source) - 1] == '+') dest[MAX_DIGIT] = +1;
    else if(source[strlen(source) - 1] == '-') dest[MAX_DIGIT] = -1;
}

int file_io_write_new(char file_name[MAX_FILENAME_STRING], char text[][MAX_STRING], int text_line_count)
{
    FILE *file = fopen(file_name, "w");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    for (int loop_temp_array_pointer = 0; loop_temp_array_pointer < text_line_count; loop_temp_array_pointer++) fprintf(file, "%s\n", text[loop_temp_array_pointer]);
    fclose(file);
    return SUCCESS;
}

int file_io_write_add(char file_name[MAX_FILENAME_STRING], char text[MAX_STRING])
{
    FILE *file = fopen(file_name, "a");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    fprintf(file, "%s\n", text);
    fclose(file);
    return SUCCESS;
}

int file_io_read(char file_name[MAX_FILENAME_STRING], int text_line_count, char dest[][MAX_STRING])
{
    FILE *file = fopen(file_name, "r");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    for(int loop_temp_line = 0; loop_temp_line < text_line_count; loop_temp_line++) fscanf(file, "%s", dest[loop_temp_line]);
    fclose(file);
    return SUCCESS;
}

int file_remove(char file_name[MAX_FILENAME_STRING])
{
    FILE *file;
    if (!(file = fopen(file_name, "r"))) {
        fclose(file);
        return SKIP;
    }
    fclose(file);
    return remove(file_name) ? FAIL : SUCCESS;
}

int status_save(int variable[5][BINT_ARR_LEN], int history[3][BINT_ARR_LEN])
{
    /*
     * VAR A
     * VAR B
     * VAR C
     * VAR D
     * VAR E
     * VAR H1
     * VAR H2
     * VAR H3
     */
    char file_name[MAX_FILENAME_STRING] = "cal.txt";
    char text[8][MAX_STRING] = {0};

    big_int_fprint(variable[0], text[0]);
    big_int_fprint(variable[1], text[1]);
    big_int_fprint(variable[2], text[2]);
    big_int_fprint(variable[3], text[3]);
    big_int_fprint(variable[4], text[4]);
    big_int_fprint(history[0], text[5]);
    big_int_fprint(history[1], text[6]);
    big_int_fprint(history[2], text[7]);

    return file_io_write_new(file_name, text, 8) ? SUCCESS : FAIL;
}

int status_load(int dest_variable[5][BINT_ARR_LEN], int dest_history[3][BINT_ARR_LEN])
{
    char file_name[MAX_FILENAME_STRING] = "cal.txt";
    char data[8][MAX_STRING] = {0};
    if(!file_io_read(file_name, 8, data)) return FAIL;
    big_int_tointeger(data[0], dest_variable[0]);
    big_int_tointeger(data[1], dest_variable[1]);
    big_int_tointeger(data[2], dest_variable[2]);
    big_int_tointeger(data[3], dest_variable[3]);
    big_int_tointeger(data[4], dest_variable[4]);
    big_int_tointeger(data[5], dest_history[0]);
    big_int_tointeger(data[6], dest_history[1]);
    big_int_tointeger(data[7], dest_history[2]);
    return SUCCESS;
}

void big_int_from(long long value, int result[BINT_ARR_LEN])
{
    result[MAX_DIGIT] = value >= 0 ? POSITIVE : NEGATIVE;
    value = value < 0 ? -value : value;
    for(int i = 0; i < MAX_DIGIT; i++) {
        result[i] = value % 10;
        value /= 10;
    }
}


int big_int_fix(int carry_end, int big_int[BINT_ARR_LEN])
{
    // carry: 자리올림
    int carry = 0, digit;
    int temp_result[BINT_ARR_LEN];

    temp_result[MAX_DIGIT] = big_int[MAX_DIGIT];
    // 각 자릿수가 0~9를 넘지 않도록 수정
    for(int i = 0; i < MAX_DIGIT; i++) {
        digit = big_int[i] + carry;
        temp_result[i] = digit % 10;
        carry = digit / 10;
        if(temp_result[i] < 0) {
            temp_result[i] += 10;
            carry--;
        }
    }
    carry += carry_end;

    // 만약 자리올림의 절댓값이 1이고 원래 부호와 다를 경우
    if(carry * temp_result[MAX_DIGIT] == NEGATIVE) {
        temp_result[MAX_DIGIT] = -temp_result[MAX_DIGIT];

        // 0이 아닌 가장 낮은 자릿수 검색
        int start = MAX_DIGIT;
        for(int i = 0; i < MAX_DIGIT; i++) {
            if(temp_result[i] == 0) continue;
            start = i;
            break;
        }

        // 0이 아닌 자릿수가 없음
        //  => |result| = 10^MAX_DIGIT
        //  => 오버플로우 / 언더플로우
        if(start == MAX_DIGIT) return FAIL;

        // 자리내림
        temp_result[start] = 10 - temp_result[start];
        for(int i = start+1; i < MAX_DIGIT; i++) temp_result[i] = 9 - temp_result[i];
    } else if(carry != 0) return FAIL; // 오버플로우 / 언더플로우

    big_int_copy(temp_result, big_int);
    return SUCCESS;
}


void big_int_copy(int source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
    for(int i = 0; i < BINT_ARR_LEN; i++) dest[i] = source[i];
}


int big_int_lcomb(int a, int x[BINT_ARR_LEN], int b, int y[BINT_ARR_LEN], int result[BINT_ARR_LEN])
{
    int carry = 0;
    int temp_result[BINT_ARR_LEN];

    for(int i = 0; i < MAX_DIGIT; i++)
    {
        int digit_comb = a * x[i] * x[MAX_DIGIT] + b * y[i] * y[MAX_DIGIT] + carry;
        temp_result[i] = digit_comb % 10;
        carry = digit_comb / 10;
    }
    temp_result[MAX_DIGIT] = POSITIVE;

    if(big_int_fix(carry, temp_result) == FAIL)
        return FAIL;

    big_int_copy(temp_result, result);
    return SUCCESS;
}


int big_int_shift(int amount, int source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
    int temp_result[BINT_ARR_LEN] = {0};

    temp_result[MAX_DIGIT] = source[MAX_DIGIT];
    for(int i = 0; i < MAX_DIGIT; i++) {
        if(source[i] == 0) continue;
        int new_digit = i + amount;
        if(new_digit < 0) continue;
        if(new_digit >= MAX_DIGIT) return FAIL;
        temp_result[new_digit] = source[i];
    }

    big_int_copy(temp_result, dest);
    return SUCCESS;
}


int big_int_times(int factor, int source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
    _Bool negative_factor = factor < 0;
    if(negative_factor) factor = -factor;
    if(factor > 9)
        return FAIL;

    int temp_result[BINT_ARR_LEN];
    temp_result[MAX_DIGIT] = source[MAX_DIGIT] * (negative_factor ? -1 : 1);
    for(int i = 0; i < MAX_DIGIT; i++) temp_result[i] = factor * source[i];

    if(big_int_fix(0, temp_result) == FAIL) return FAIL;

    big_int_copy(temp_result, dest);
    return SUCCESS;
}


int big_int_mul(int x[BINT_ARR_LEN], int y[BINT_ARR_LEN], int result[BINT_ARR_LEN])
{
    int temp_big_int[BINT_ARR_LEN], temp_result[BINT_ARR_LEN];
    big_int_from(0, temp_result);

    for(int yd = 0; yd < MAX_DIGIT; yd++) {
        char y_digit = y[yd];
        if(y_digit == 0) continue;

        // temp_big_int = x
        big_int_copy(x, temp_big_int);
        temp_big_int[MAX_DIGIT] = POSITIVE;

        // temp_big_int *= y_digit
        if(big_int_times(y_digit, temp_big_int, temp_big_int) == FAIL) return FAIL;

        // temp_big_int *= 10^yd
        if(big_int_shift(yd, temp_big_int, temp_big_int) == FAIL) return FAIL;

        // temp_result += temp_big_int
        if(big_int_lcomb(1, temp_big_int, 1, temp_result, temp_result) == FAIL) return FAIL;
    }

    temp_result[MAX_DIGIT] = x[MAX_DIGIT] * y[MAX_DIGIT];
    big_int_copy(temp_result, result);
    return SUCCESS;
}

int big_int_divmod(int x[BINT_ARR_LEN], int y[BINT_ARR_LEN], int div_result[BINT_ARR_LEN], int mod_result[BINT_ARR_LEN])
{
    int temp_div_result[BINT_ARR_LEN], temp_big_int[BINT_ARR_LEN], x_copy[BINT_ARR_LEN];
    big_int_from(0, temp_div_result);

    // x_copy = x
    big_int_copy(x, x_copy);
    x_copy[MAX_DIGIT] = POSITIVE;

    int x_len = big_int_len(x), y_len = big_int_len(y);
    // y == 0 -> 에러
    if(y_len == 0) return FAIL;

    // 흔히 쓰는 나눗셈 방법: 자리 옮겨가면서 나누는 수보다 작아질 때까지 빼기
    for(int i = x_len - y_len; i >= 0; i--) {
        // temp_big_int = y
        big_int_copy(y, temp_big_int);
        temp_big_int[MAX_DIGIT] = POSITIVE;

        // temp_big_int *= 10^i
        if(big_int_shift(i, temp_big_int, temp_big_int) == FAIL) return FAIL;

        // x_copy가 0보다 작아질 때까지 빼기
        int divisor_digit = 0;
        while(x_copy[MAX_DIGIT] == POSITIVE) { // x_copy >= 0
            // x_copy -= temp_big_int
            if(big_int_lcomb(1, x_copy, -1, temp_big_int, x_copy) == FAIL) return FAIL;
            divisor_digit++;
        }

        // x_copy += temp_big_int
        if(big_int_lcomb(1, x_copy, 1, temp_big_int, x_copy) == FAIL)
            return FAIL;

        temp_div_result[i] = divisor_digit - 1;
    }

    // 연산 도중 실패할 수 있으므로 부호는 나중에 저장
    temp_div_result[MAX_DIGIT] = x[MAX_DIGIT] * y[MAX_DIGIT];
    x_copy[MAX_DIGIT] = x[MAX_DIGIT];

    // !경고!
    // 이 부분 좀 불안정함. 여기 이후로 x의 부호 값이 이상하게 바뀌는데,
    // 이러면 결과적으로 이상한 값이 출력될 수 있음.
    // TODO: 왜 불안정한지 조사하기
    big_int_copy(temp_div_result, div_result);
    big_int_copy(x_copy, mod_result); // 이 때의 x_copy는 나머지 값이므로 mod_result로 복사
    return SUCCESS;
}

int big_int_len(int big_int[BINT_ARR_LEN])
{
    for(int i = MAX_DIGIT - 1; i >= 0; i--) if(big_int[i] != 0) return i + 1;
    return 0;
}

/**
 * Made by 안현석, 김민재
 * @param big_int 큰 정수
 */
void big_int_tostring(int big_int[BINT_ARR_LEN], char result[MAX_STRING])
{
    int len = big_int_len(big_int);
    if(len == 0) {
        strcpy(result, "0");
        return;
    }

    int str_len = 0;
    if (big_int[MAX_DIGIT] == NEGATIVE) result[str_len++] = '-';
    for (int i = len - 1; i >= 0; i--) {
        result[str_len++] = big_int[i] + '0';
        if(i != 0 && i % 3 == 0) result[str_len++] = ',';
    }
    result[str_len] = '\0';
}

void big_int_print(int big_int[BINT_ARR_LEN])
{
    char string[MAX_STRING];
    big_int_tostring(big_int, string);
    printf("%s", string);
}

/**
 * Made by 안현석
 * @param string 전체 수식 문자열
 * @param start 문자열 시작
 * @param end 문자열 끝
 * @param result 큰 정수 결과
 */
int big_int_parse(char str[MAX_STRING], int start, int end, int result[BINT_ARR_LEN])
{
    if (start < 0 || end >= MAX_STRING || start > end) return FAIL;

    int temp_result[BINT_ARR_LEN], i;
    big_int_from(0, temp_result);

    if(str[start] == '-') {
        temp_result[MAX_DIGIT] = NEGATIVE;
        start++;
    }

    for(int j = start; j <= end; j++) {
        char digit = str[j];
        if(digit < '0' || digit > '9') return FAIL;

        i = end - j;
        if(i >= MAX_DIGIT) return FAIL;
        temp_result[i] = str[j] - '0';
    }

    big_int_copy(temp_result, result);
    return SUCCESS;
}

_Bool is_digit_integer(char target)
{
    return target >= '0' && target <= '9';
}

_Bool is_number_type(int exp_token[2])
{
    int type = exp_token[0];
    return type == TOKEN_INTEGER || type == TOKEN_VARIABLE || type == TOKEN_HISTORY_VARIABLE;
}

_Bool is_unary_operator(int exp_token[2])
{
    return exp_token[0] == TOKEN_OPERATOR && (exp_token[1] == OPERATOR_UNARY_PLUS || exp_token[1] == OPERATOR_UNARY_MINUS);
}

_Bool is_assign_operator(int exp_token[2])
{
    return exp_token[0] == TOKEN_OPERATOR && exp_token[1] == OPERATOR_ASSIGN;
}

int assign_token_operator(char target, int exp_token[2], _Bool is_unary)
{
    exp_token[0] = TOKEN_OPERATOR;
    switch(target) {
        case '+': exp_token[1] = is_unary ? OPERATOR_UNARY_PLUS  : OPERATOR_PLUS;  return SUCCESS;
        case '-': exp_token[1] = is_unary ? OPERATOR_UNARY_MINUS : OPERATOR_MINUS; return SUCCESS;

        case '*': exp_token[1] = OPERATOR_MULTIPLY; return is_unary ? FAIL : SUCCESS;
        case '/': exp_token[1] = OPERATOR_DIVDE;    return is_unary ? FAIL : SUCCESS;
        case '%': exp_token[1] = OPERATOR_MODULO;   return is_unary ? FAIL : SUCCESS;
        case '=': exp_token[1] = OPERATOR_ASSIGN;   return is_unary ? FAIL : SUCCESS;
    }
    return SKIP;
}

int assign_token_command(char target, int exp_token[2])
{
    exp_token[0] = TOKEN_COMMAND;
    char target_upper = (char) toupper(target);
    switch(target_upper) {
        case 'L': exp_token[1] = COMMAND_LOAD;           return SUCCESS;
        case 'R': exp_token[1] = COMMAND_REFRESH;        return SUCCESS;
        case 'T': exp_token[1] = COMMAND_RESET;          return SUCCESS;
        case 'S': exp_token[1] = COMMAND_SAVE;           return SUCCESS;
        case 'Q': exp_token[1] = COMMAND_QUIT;           return SUCCESS;

            // 특별한 경우; TOKEN_HISTORY_VARIABLE이 있기 때문
            // COMMAND_HISTORY인지는 assign_token() 함수가 끝나고 체크될 예정
        case 'H': exp_token[0] = TOKEN_HISTORY_VARIABLE; return SUCCESS;
    }
    return SKIP;
}

int assign_token_variable(char target, int exp_token[2])
{
    exp_token[0] = TOKEN_VARIABLE;
    char target_upper = (char) toupper(target);
    // 변수는 VARIABLE_COUNT개밖에 없으니까 간단하게 switch로 생각
    switch(target_upper) {
        case 'A': exp_token[1] = 0; return SUCCESS;
        case 'B': exp_token[1] = 1; return SUCCESS;
        case 'C': exp_token[1] = 2; return SUCCESS;
        case 'D': exp_token[1] = 3; return SUCCESS;
        case 'E': exp_token[1] = 4; return SUCCESS;
    }
    return SKIP;
}

int assign_token(char target, int exp_tokens[MAX_TOKEN_COUNT][2], int index)
{
    int result;

    // 공백
    if(target == ' ') {
        exp_tokens[index][0] = TOKEN_NOTHING;
        return SUCCESS;
    }

    // 정수
    if(is_digit_integer(target)) {
        exp_tokens[index][0] = TOKEN_INTEGER;

        // 정수 앞에 있는게 연산자가 아닐 경우
        if(index > 0 && exp_tokens[index-1][0] != TOKEN_OPERATOR) return FAIL;

        return SUCCESS;
    }

    // 연산자
    _Bool is_unary = index == 0 || exp_tokens[index-1][0] == TOKEN_OPERATOR;
    if((result = assign_token_operator(target, exp_tokens[index], is_unary)) != SKIP) {
        if(result == FAIL) return FAIL;

        // 단항도 아닌데 맨 앞에 있을 경우
        if(index == 0 && !is_unary_operator(exp_tokens[index])) return FAIL;

        // 연산자 앞에 단항 연산자가 있을 경우
        if(index > 0 && is_unary_operator(exp_tokens[index-1])) return FAIL;

        return SUCCESS;
    }

    // 커맨드
    if((result = assign_token_command(target, exp_tokens[index])) != SKIP) {
        if(result == FAIL) return FAIL;

        // 만약 진짜 커맨드일 경우
        if(exp_tokens[index][0] == TOKEN_COMMAND) {
            // 수식의 맨 앞에 있지 않을 경우
            if(index != 0) return FAIL;
        } else if(exp_tokens[index][0] == TOKEN_HISTORY_VARIABLE) { // 히스토리 변수일 경우
            // 변수 앞에 있는게 연산자가 아닐 경우
            if(index > 0 && exp_tokens[index-1][0] != TOKEN_OPERATOR) return FAIL;
        }

        return SUCCESS;
    }

    // 변수
    if((result = assign_token_variable(target, exp_tokens[index])) != SKIP) {
        if(result == FAIL) return FAIL;

        // 변수 앞에 있는게 연산자가 아닐 경우
        if(index > 0 && exp_tokens[index-1][0] != TOKEN_OPERATOR) return FAIL;

        return SUCCESS;
    }

    return FAIL;
}


int parse_to_token(char exp_raw[MAX_STRING], char count_exp_raw, int exp_tokens[MAX_TOKEN_COUNT][2], int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN])
{
    // 수식 길이 초과 시 에러
    if(count_exp_raw >= 100 || count_exp_raw == 0) return FAIL;

    int count_exp_tokens = 0;
    int count_exp_tokens_integer = 0;

    // 토큰화
    for(int raw_pointer = 0; raw_pointer < count_exp_raw; raw_pointer++) {
        // 현재 무슨 토큰인지 확인
        char current_char = exp_raw[raw_pointer];
        if(assign_token(current_char, exp_tokens, count_exp_tokens) == FAIL) return FAIL;

        // 특이 케이스 핸들링
        int current_token_type = exp_tokens[count_exp_tokens][0];
        int integer_start;
        switch(current_token_type) {
            case TOKEN_NOTHING:
                continue;
            case TOKEN_UNKNOWN:
                return FAIL;
            case TOKEN_INTEGER:
                // 어디까지 정수인지 확인
                integer_start = raw_pointer;
                for(; raw_pointer < count_exp_raw && is_digit_integer(exp_raw[raw_pointer]); raw_pointer++) {}
                int integer_end = --raw_pointer;

                if(big_int_parse(exp_raw, integer_start, integer_end, exp_tokens_integer[count_exp_tokens_integer]) == FAIL)
                    return FAIL;
                exp_tokens[count_exp_tokens][1] = count_exp_tokens_integer;
                count_exp_tokens_integer++;
                break;
            case TOKEN_HISTORY_VARIABLE:
                // 뒤에 더이상 문자가 없을 경우
                if(raw_pointer == count_exp_raw - 1)
                {
                    exp_tokens[count_exp_tokens][0] = TOKEN_COMMAND;
                    exp_tokens[count_exp_tokens][1] = COMMAND_HISTORY;
                }

                // h1, h2, h3 아니면 COMMAND_HISTORY
                int history_number = exp_raw[raw_pointer + 1] - '0';
                if(history_number < 1 || history_number > HISTORY_VARIABLE_COUNT)
                {
                    exp_tokens[count_exp_tokens][0] = TOKEN_COMMAND;
                    exp_tokens[count_exp_tokens][1] = COMMAND_HISTORY;
                }

                // 알고 보니 COMMAND_HISTORY였을 경우
                if(exp_tokens[count_exp_tokens][0] == TOKEN_COMMAND)
                {
                    // 수식의 맨 앞에 있지 않을 경우
                    if(count_exp_tokens != 0) return FAIL;
                    break;
                }

                exp_tokens[count_exp_tokens][1] = history_number - 1;
                raw_pointer++;
                break;
        }

        count_exp_tokens++;
    }

    // 토큰 개수가 0일 경우
    if(count_exp_tokens == 0) return FAIL;

    // 커맨드 모드인데 토큰 개수가 1보다 많을 경우
    if(count_exp_tokens > 1 && exp_tokens[0][0] == TOKEN_COMMAND) return FAIL;

    for(int i = 0; i < count_exp_tokens; i++) {
        // 연산자
        if(exp_tokens[i][0] == TOKEN_OPERATOR) {
            // 문자 끝에 있을 경우
            if(i == count_exp_tokens - 1) return FAIL;

            // 다음 토큰이 숫자나 단항 연산자가 아닐 경우
            if(!is_number_type(exp_tokens[i+1]) && !is_unary_operator(exp_tokens[i+1])) return FAIL;
        }
    }

    // 마무리
    exp_tokens[count_exp_tokens][0] = TOKEN_END;

    // <디버깅>
    debug_print_tokens(exp_tokens, exp_tokens_integer);
    // </디버깅>

    return SUCCESS;
}


_Bool token_validate_command(int exp_tokens[MAX_TOKEN_COUNT][2])
{
    int token_len = get_token_len(exp_tokens);
    return token_len == 1 && exp_tokens[0][0] == TOKEN_COMMAND;
}


int get_token_len(int exp_tokens[MAX_TOKEN_COUNT][2])
{
    for(int i = 0; i < MAX_TOKEN_COUNT; i++) if(exp_tokens[i][0] == TOKEN_END) return i;
    return MAX_TOKEN_COUNT;
}


void debug_print_tokens(int exp_tokens[MAX_TOKEN_COUNT][2], int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN])
{
    if(!DEBUG) return;

    printf("Tokens : \n");
    for(int i = 0; exp_tokens[i][0] != TOKEN_END; i++) {
        printf("  [%3d] ", i);
        switch(exp_tokens[i][0]) {
            case TOKEN_INTEGER:
                printf("TOKEN_INTEGER : ");
                big_int_print(exp_tokens_integer[exp_tokens[i][1]]);
                break;
            case TOKEN_OPERATOR:
                printf("TOKEN_OPERATOR: ");
                switch(exp_tokens[i][1]) {
                    case OPERATOR_PLUS:        printf("+");         break;
                    case OPERATOR_UNARY_PLUS:  printf("+ (unary)"); break;
                    case OPERATOR_MINUS:       printf("-");         break;
                    case OPERATOR_UNARY_MINUS: printf("- (unary)"); break;
                    case OPERATOR_ASSIGN:      printf("=");         break;
                    case OPERATOR_MULTIPLY:    printf("*");         break;
                    case OPERATOR_DIVDE:       printf("/");         break;
                    case OPERATOR_MODULO:      printf("%%");        break;
                }
                break;
            case TOKEN_COMMAND:
                printf("TOKEN_COMMAND : ");
                switch(exp_tokens[i][1]) {
                    case COMMAND_HISTORY:      printf("HISTORY");   break;
                    case COMMAND_LOAD:         printf("LOAD");      break;
                    case COMMAND_REFRESH:      printf("REFRESH");   break;
                    case COMMAND_RESET:        printf("RESET");     break;
                    case COMMAND_SAVE:         printf("SAVE");      break;
                    case COMMAND_QUIT:         printf("QUIT");      break;
                }
                break;
            case TOKEN_VARIABLE:
                printf("TOKEN_VARIABLE: ");
                switch(exp_tokens[i][1]) {
                    case 0:	printf("a"); break;
                    case 1:	printf("b"); break;
                    case 2:	printf("c"); break;
                    case 3:	printf("d"); break;
                    case 4:	printf("e"); break;
                }
                break;
            case TOKEN_HISTORY_VARIABLE:
                printf("TOKEN_HISTORY_VARIABLE: ");
                switch(exp_tokens[i][1]) {
                    case 0:	printf("h1"); break;
                    case 1:	printf("h2"); break;
                    case 2:	printf("h3"); break;
                }
                break;
        }
        printf("\n");
    }
}