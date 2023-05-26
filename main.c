#include "common.h"
#include "int_op.c"
#include "parse.c"
#include "calc.c"
#include "command.c"


#define LOOP_CONTINUE   1
#define LOOP_STOP       0


int eval_expression(char exp_raw[MAX_STRING], int variables[VARIABLE_COUNT][BINT_ARR_LEN], int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], char line_result[MAX_STRING], int command_result[1])
{
    int count_exp_raw = strlen(exp_raw);
    int exp_tokens[MAX_TOKEN_COUNT][2];
    int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN] = {0};
    int big_int_result[BINT_ARR_LEN], function_result;

    // 문자열 -> 토큰
    if(parse_to_token(exp_raw, count_exp_raw, exp_tokens, exp_tokens_integer) == FAIL)
        return FAIL;

    // 토큰 계산 및 실행
    int token_len = get_token_len(exp_tokens);
    if((function_result = eval_exp_tokens(exp_tokens, exp_tokens_integer, variables, history_variables, 0, token_len - 1, big_int_result)) != SKIP)
    {
        if(function_result == FAIL)
            return FAIL;
        big_int_tostring(big_int_result, line_result);
        command_result[0] = COMMAND_NOTHING;
        return SUCCESS;
    }
    // 커맨드 핸들링
    else if(token_validate_command(exp_tokens))
    {
        command_result[0] = exp_tokens[0][1];
        return SUCCESS;
    }
    return FAIL;
}


int screen_iteration(int variables[VARIABLE_COUNT][BINT_ARR_LEN],
                     int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                     int lines_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING])
{
    char exp_raw[MAX_STRING], str_result[MAX_STRING];
    int command_result[1], loop = LOOP_CONTINUE;
    _Bool error = 0;
    static _Bool first = 1;

    if(first)
    {
        // 첫 실행 시 화면 출력
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
    if(eval_expression(exp_raw, variables, history_variables, str_result, command_result) == FAIL)
    {
        error = 1;
        goto raise_error;
    }

    // 커맨드인지 확인
    switch(command_result[0])
    {
        // 커맨드가 아님. 단순 식
        case COMMAND_NOTHING:
            break;
        case COMMAND_HISTORY:
            command_history(history_variables, lines_type, lines);
            goto print_screen;
        case COMMAND_SAVE:
            if(command_save(variables, history_variables) == FAIL)
            {
                error = 1;
                goto raise_error;
            }
            strcpy(str_result, "cal.txt에 저장");
            break;
        case COMMAND_LOAD:
            if(command_load(variables, history_variables) == FAIL)
            {
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


int main() {
    int variables[VARIABLE_COUNT][BINT_ARR_LEN];
    int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN];
    int lines_type[MAX_LINE_COUNT];
    char lines[MAX_LINE_COUNT][MAX_STRING];

    while(1)
    {
        if(screen_iteration(variables, history_variables, lines_type, lines) == LOOP_STOP)
            break;
    }
}
