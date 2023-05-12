#include "test_parse.h"
#include "../calc.c"

int main(void)
{
	int variables[VARIABLE_COUNT][BINT_ARR_LEN];
	int history_variables[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN];

	for(int i = 0; i < VARIABLE_COUNT; i++) big_int_from(0, variables[i]);
	for(int i = 0; i < HISTORY_VARIABLE_COUNT; i++) big_int_from(0, history_variables[i]);

	while(1) {
		_Bool error = 0;
		
		int exp_tokens[MAX_TOKEN_COUNT][2];
		int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN] = {0};
		int result[BINT_ARR_LEN];
		
		// 입력 문자열
		int count_exp_raw = 0, op;
		char exp_raw[MAX_STRING] = {0};
		
		// 계산식 입력
		printf("(input) ");

		// 버그때문에 scanf를 못 쓰겠음
		while((op = getchar()) != '\n')
		{
			exp_raw[count_exp_raw++] = op;
		}
		exp_raw[count_exp_raw] = '\0';
		
		// 문자열 -> 토큰
		if(parse_to_token(exp_raw, count_exp_raw, exp_tokens, exp_tokens_integer) == FAIL)
		{
			error = 1;
			goto raise_error;
		}

		// <디버깅>
		debug_print_tokens(exp_tokens, exp_tokens_integer);
		// </디버깅>

		// 토큰 계산
		int token_len = get_token_len(exp_tokens);
		if(eval_expression(exp_tokens, exp_tokens_integer, variables, history_variables, 0, token_len - 1, result) == FAIL)
		{
			error = 1;
			goto raise_error;
		}
		printf(">>>>> ");
		big_int_print(result);
		printf("\n");
		
raise_error:
		if(error) printf("error\n\n");
	}

	return 0;
}