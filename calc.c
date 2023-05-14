#ifndef __CALC_C__
#define __CALC_C__

#include "common.h"
#include "parse.c"
#include "int_op.c"


#define MAX_GROUP_COUNT     (MAX_DIGIT_EXPRESSION / 2 + 5)

// NUMERALs (항수)
#define NUMERAL_NOTHING     (-1)    // 항수 오류
#define NUMERAL_NULLINARY   0       // "0항"
#define NUMERAL_UNARY       1       // 단항
#define NUMERAL_BINARY      2       // 이항

#define LEVEL_NOTHING       (-1)    // 연산자 레벨 오류


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
int group_expression(int exp_groups[MAX_GROUP_COUNT][3],
                     int[MAX_TOKEN_COUNT][2],
                     int, int);
/**
 * exp_token에 따라 토큰에 있는 큰 정수값 result로 복사
 */
int copy_from_number_type(int exp_token[2],
                          int[MAX_TOKEN_COUNT][BINT_ARR_LEN],
                          int[VARIABLE_COUNT][BINT_ARR_LEN],
                          int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                          int result[BINT_ARR_LEN]);
/**
 * token_index를 포함하는 그룹 발견 시:
 *  - result에 그룹 결과값 저장
 *  - 해당 그룹의 끝 인덱스 반환.
 * 아니면 INVALID_INDEX 반환
 */
int search_group_result(int token_index, int,
                        int[MAX_GROUP_COUNT][3],
                        int[MAX_GROUP_COUNT][BINT_ARR_LEN],
                        int[BINT_ARR_LEN]);
/**
 * 이항 연산자 값인 operator_type에 따라:
 *  - result = result (연산자) victim
 * 단항이 올 경우 FAIL 리턴
 */
int operator_calculation(int operator_type, int[BINT_ARR_LEN], int[BINT_ARR_LEN]);

/**
 * 그룹화된 토큰 계산
 */
int calculate_groups(int[MAX_TOKEN_COUNT][2],
                     int[MAX_TOKEN_COUNT][BINT_ARR_LEN],
                     int[VARIABLE_COUNT][BINT_ARR_LEN],
                     int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                     int[MAX_GROUP_COUNT][3],
                     int[MAX_GROUP_COUNT][BINT_ARR_LEN]);

/**
 * 수식 처리 및 계산.
 * 커맨드 모드는 따로 핸들링하지 않으므로 커맨드 토큰 발견 시 SKIP 리턴
 */
int eval_expression(int[MAX_TOKEN_COUNT][2],
                    int[MAX_TOKEN_COUNT][BINT_ARR_LEN],
                    int[VARIABLE_COUNT][BINT_ARR_LEN],
                    int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                    int, int,
                    int[BINT_ARR_LEN]);

/**
 * 히스토리 저장 함수
 */
void update_history(int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                    int[BINT_ARR_LEN]);



int get_operator_level(int exp_token[2])
{
    if(exp_token[0] != TOKEN_OPERATOR)
    {
        return LEVEL_NOTHING;
    }
    switch(exp_token[1])
    {
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
    switch(level)
    {
        case 3:  return NUMERAL_UNARY;
        case 2:
        case 1:  return NUMERAL_BINARY;
        case 0:  return NUMERAL_NULLINARY;
        default: return NUMERAL_NOTHING;
    }
}

int get_group_len(int exp_groups[MAX_GROUP_COUNT][3])
{
    for(int i = 0; i < MAX_GROUP_COUNT; i++)
    {
        if(exp_groups[i][2] == LEVEL_NOTHING) return i;
    }
    return MAX_GROUP_COUNT;
}

void group_reset(int exp_groups[MAX_GROUP_COUNT][3])
{
    for(int i = 0; i < MAX_GROUP_COUNT; i++)
    {
        exp_groups[i][2] = LEVEL_NOTHING;
    }
}

int group_expression(int exp_groups[MAX_GROUP_COUNT][3],
                     int exp_tokens[MAX_TOKEN_COUNT][2],
                     int level, int start)
{
    int end = start;
    if(level == get_operator_level(exp_tokens[start]) && is_unary_operator(exp_tokens[start]))
    {
        end = start + 1;
    }
    else
    {
        for(; exp_tokens[end][0] != TOKEN_END; end++)
        {
            int operator_level = get_operator_level(exp_tokens[end]);

            // 숫자 타입
            if(is_number_type(exp_tokens[end])) continue;

            // 연산자도 아님. 커맨드?
            else if(operator_level == LEVEL_NOTHING) return INVALID_INDEX;

            // 현재 레벨 == 연산자 레벨
            else if(level == operator_level) continue;

            // 현재 래벨 > 연산자 레벨
            else if(level > operator_level) break;

            // 현재 레벨 < 연산자 레벨 && 단항 연산자
            else if(is_unary_operator(exp_tokens[end]))
            {
                end = group_expression(exp_groups, exp_tokens, level+1, end);
                if(end == INVALID_INDEX) return INVALID_INDEX;
            }

            // 현재 레벨 < 연산자 레벨 && 이항 연산자
            else
            {
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

void debug_print_groups(int exp_groups[MAX_GROUP_COUNT][3],
                        int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN])
{
	printf("Groups : \n");
    for(int i = 0; exp_groups[i][2] != NUMERAL_NOTHING; i++)
    {
        printf("    Start: %3d, End: %3d, Level: %d ", exp_groups[i][0], exp_groups[i][1], exp_groups[i][2]);
        switch(level_to_numeral(exp_groups[i][2]))
        {
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

int copy_from_number_type(int exp_token[2],
                          int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN],
                          int variables[VARIABLE_COUNT][BINT_ARR_LEN],
                          int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                          int result[BINT_ARR_LEN])
{
    switch(exp_token[0])
    {
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

int search_group_result(int token_index, int group_index,
                        int exp_groups[MAX_GROUP_COUNT][3],
                        int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN],
                        int result[BINT_ARR_LEN])
{
    // 만약 token_index를 포함하는 그룹이 현재 그룹보다 전에 있었을 경우
    // 가장 최근 그룹 순으로 검색
    for(int g_index2 = group_index - 1; g_index2 >= 0; g_index2--)
    {
        int start = exp_groups[g_index2][0], end = exp_groups[g_index2][1];
        if(token_index >= start && token_index <= end)
        {
            big_int_copy(exp_groups_result[g_index2], result);
            return end;
        }
    }
    return INVALID_INDEX;
}

int operator_calculation(int operator_type, int victim[BINT_ARR_LEN], int result[BINT_ARR_LEN])
{
    int dummy[BINT_ARR_LEN];
    switch(operator_type)
    {
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

int calculate_groups(int exp_tokens[MAX_TOKEN_COUNT][2],
                     int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN],
                     int variables[VARIABLE_COUNT][BINT_ARR_LEN],
                     int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                     int exp_groups[MAX_GROUP_COUNT][3],
                     int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN])
{
    int group_len = get_group_len(exp_groups);
    // 전체 그룹 계산. [0]부터 차례대로
    for(int group_index = 0; group_index < group_len; group_index++)
    {
        int start = exp_groups[group_index][0], end = exp_groups[group_index][1], level = exp_groups[group_index][2];
        int group_numeral = level_to_numeral(level);
        int result_big_int[BINT_ARR_LEN];

        // 그룹 타입이 단항일 경우
        if(group_numeral == NUMERAL_UNARY)
        {
            int unary_operator_type = exp_tokens[start][1];
            
            // 숫자 토큰의 큰 정수값 복사
            if(copy_from_number_type(exp_tokens[start+1], exp_tokens_integer, variables, history_variables, result_big_int) == FAIL)
                return FAIL;
            
            // 만약 부호가 "-"일 경우
            if(unary_operator_type == OPERATOR_UNARY_MINUS)
            {
                // result_big_int = -result_big_int
                if(big_int_times(-1, result_big_int, result_big_int) == FAIL)
                    return FAIL;
            }
        }

        // 그룹 타입이 이항이거나 0항일 경우
        else if(group_numeral == NUMERAL_BINARY || group_numeral == NUMERAL_NULLINARY)
        {
            int operator_type, temp_big_int[BINT_ARR_LEN], dummy_big_int[BINT_ARR_LEN];
            for(int token_index = start; token_index <= end; token_index++)
            {
                // 만약 타입이 이항인데 i가 처음이 아닐 경우 -> i 앞에 부호가 있음
                if(group_numeral == NUMERAL_BINARY && token_index != start)
                {
                    // i 앞에 있는게 부호가 아닐 경우
                    if(exp_tokens[token_index][0] != TOKEN_OPERATOR) return FAIL;
                    operator_type = exp_tokens[token_index][1];
                    token_index++;
                }
                else
                {
                    operator_type = OPERATOR_NOTHING;
                }

                // 만약 i가 포함된 그룹이 현재 그룹보다 전에 없었을 경우
                int new_index = search_group_result(token_index, group_index, exp_groups, exp_groups_result, temp_big_int);
                if(new_index == INVALID_INDEX)
                {
                    // 숫자 토큰의 큰 정수값 복사
                    if(copy_from_number_type(exp_tokens[token_index], exp_tokens_integer, variables, history_variables, temp_big_int) == FAIL)
                        return FAIL;
                }
                else
                {
                    token_index = new_index;
                }

                // 실제 계산
                if(operator_calculation(operator_type, temp_big_int, result_big_int) == FAIL)
                    return FAIL;
            }
        }

        // 결과값을 그룹 결과값으로 복사
        big_int_copy(result_big_int, exp_groups_result[group_index]);
    }
}

int eval_expression(int exp_tokens[MAX_TOKEN_COUNT][2],
                    int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN],
                    int variables[VARIABLE_COUNT][BINT_ARR_LEN],
                    int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                    int start, int end,
                    int result[BINT_ARR_LEN])
{
    // 만약 수식이 대입 모드인 경우
    if(exp_tokens[start][0] == TOKEN_VARIABLE && is_assign_operator(exp_tokens[start + 1]))
    {
        int temp_result[BINT_ARR_LEN];
        // 우변을
        if(eval_expression(exp_tokens, exp_tokens_integer, variables, history_variables, start + 2, end, temp_result) == FAIL)
            return FAIL;
        
        // 좌변으로 복사
        big_int_copy(temp_result, variables[exp_tokens[start][1]]);
        big_int_copy(temp_result, result);
        return SUCCESS;
    }

    // 아니면 일반 수식 모드
    for(int i = start; i <= end; i++)
    {
        // 만약 대입 연산자가 있을 경우
        if(is_assign_operator(exp_tokens[i]))
            return FAIL;
        
        // 만약 커맨드 토큰이 있을 경우
        if(exp_tokens[i][0] == TOKEN_COMMAND)
            return SKIP;
    }

    // { {start1, end1, level1}, {start2, end2, level2}, ..., {startn, endn, LEVEL_NOTHING} }
    int exp_groups[MAX_GROUP_COUNT][3];
    int exp_groups_result[MAX_GROUP_COUNT][BINT_ARR_LEN];
    group_reset(exp_groups);

    // 토큰 그룹화
    if(group_expression(exp_groups, exp_tokens, 0, start) == INVALID_INDEX)
        return FAIL;
    
    // 그룹 계산
    int exp_groups_len = get_group_len(exp_groups);
    if(calculate_groups(exp_tokens, exp_tokens_integer, variables, history_variables, exp_groups, exp_groups_result) == FAIL)
        return FAIL;

    // 마지막 그룹 계산 결과 -> 결과값으로 복사
    big_int_copy(exp_groups_result[exp_groups_len - 1], result);

    // 히스토리 업데이트
    update_history(history_variables, result);

    // <디버그>
    debug_print_groups(exp_groups, exp_groups_result);
    // </디버그>
    return SUCCESS;
}

void update_history(int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                    int eval_result[BINT_ARR_LEN])
{
    for(int i = HISTORY_VARIABLE_COUNT - 2; i >= 0; i--)
    {
        big_int_copy(history_variables[i], history_variables[i+1]);
    }
    big_int_copy(eval_result, history_variables[0]);
}

#endif