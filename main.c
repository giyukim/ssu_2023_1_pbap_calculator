#include "common.h"
#include "int_op.c"
#include "parse.c"
#include "calc.c"
#include "command.c"



int eval_expression(char exp_raw[MAX_STRING],
                    int variables[VARIABLE_COUNT][BINT_ARR_LEN],
                    int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                    char line_result[MAX_STRING],
                    int command_result[1])
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


void append_line(int lines_type[MAX_LINE_COUNT],
                 char lines[MAX_LINE_COUNT][MAX_STRING],
                 int incoming_type,
                 char incoming_line[MAX_STRING])
{
    int line_count = 0;
    for(; line_count < MAX_LINE_COUNT; line_count++)
    {
        if(lines_type[line_count] == LINE_TYPE_NOTHING)
            break;
    }

    if(line_count == MAX_LINE_COUNT)
    {
        for(int i = 0; i < MAX_LINE_COUNT - 1; i++)
        {
            lines_type[i] = lines_type[i+1];
            strcpy(lines[i], lines[i+1]);
        }
        line_count = MAX_LINE_COUNT - 1;
    }
    lines_type[line_count] = incoming_type;
    strcpy(lines[line_count], incoming_line);
}


int main() {
    int variables[VARIABLE_COUNT][BINT_ARR_LEN];
    int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN];
    int lines_type[MAX_LINE_COUNT], command_result[1];
    char lines[MAX_LINE_COUNT][MAX_STRING], exp_raw[MAX_STRING], str_result[MAX_STRING];
    _Bool loop = 1;

    command_reset(variables, history_variables, lines_type);
    lines_type[0] = LINE_TYPE_NOTHING;

    while(loop)
    {
        _Bool error = 0;

        clear_screen();
        draw_main(variables, lines_type, lines);
        await_input(exp_raw);
        append_line(lines_type, lines, LINE_TYPE_INPUT, exp_raw);

        if(eval_expression(exp_raw, variables, history_variables, str_result, command_result) == FAIL)
        {
            error = 1;
            goto raise_error;
        }

        switch(command_result[0])
        {
            case COMMAND_HISTORY:
                command_history(history_variables, str_result, 0);
                append_line(lines_type, lines, LINE_TYPE_OUTPUT, str_result);
                command_history(history_variables, str_result, 1);
                append_line(lines_type, lines, LINE_TYPE_OUTPUT, str_result);
                command_history(history_variables, str_result, 2);
                append_line(lines_type, lines, LINE_TYPE_OUTPUT, str_result);
                continue;
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
                continue;
            case COMMAND_RESET:
                command_reset(variables, history_variables, lines_type);
                continue;
            case COMMAND_NOTHING:
                break;
            case COMMAND_QUIT:
                printf("======> bye\n");
                loop = 0;
                break;
            default:
                error = 1;
                goto raise_error;
        }

raise_error:
        if(error) strcpy(str_result, "error");

        append_line(lines_type, lines, LINE_TYPE_OUTPUT, str_result);
    }
}
