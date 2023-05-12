#ifndef __PARSE_C__
#define __PARSE_C__


#include "common.h"
#include "int_op.c"


_Bool is_digit_integer(char);
_Bool is_number_type(int[2]);
_Bool is_unary_operator(int[2]);
_Bool is_assign_operator(int[2]);

int assign_token_operator(char, int[2], _Bool);
int assign_token_command(char, int[2]);
int assign_token_variable(char, int[2]);
int assign_token(char, int[MAX_TOKEN_COUNT][2], int);

int parse_to_token(char[MAX_STRING], char,
                   int[MAX_TOKEN_COUNT][2],
                   int[MAX_TOKEN_COUNT][BINT_ARR_LEN]);
int get_token_len(int[MAX_TOKEN_COUNT][2]);


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
	return exp_token[0] == TOKEN_OPERATOR &&
			(exp_token[1] == OPERATOR_UNARY_PLUS || exp_token[1] == OPERATOR_UNARY_MINUS);
}

_Bool is_assign_operator(int exp_token[2])
{
	return exp_token[0] == TOKEN_OPERATOR && exp_token[1] == OPERATOR_ASSIGN;
}

int assign_token_operator(char target, int exp_token[2], _Bool is_unary)
{
	exp_token[0] = TOKEN_OPERATOR;
	switch(target)
	{
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
	switch(target_upper)
	{
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
	switch(target_upper)
	{
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
	if(target == ' ')
	{
		exp_tokens[index][0] = TOKEN_NOTHING;
		return SUCCESS;
	}

	// 정수
	if(is_digit_integer(target))
	{
		exp_tokens[index][0] = TOKEN_INTEGER;
		
		// 정수 앞에 있는게 연산자가 아닐 경우
		if(index > 0 && exp_tokens[index-1][0] != TOKEN_OPERATOR) return FAIL;
		
		return SUCCESS;
	}
	
	// 연산자
	_Bool is_unary = index == 0 || exp_tokens[index-1][0] == TOKEN_OPERATOR;
	if((result = assign_token_operator(target, exp_tokens[index], is_unary)) != SKIP)
	{
		if(result == FAIL) return FAIL;

		// 단항도 아닌데 맨 앞에 있을 경우
		if(index == 0 && !is_unary_operator(exp_tokens[index]))
			return FAIL;

		// 연산자 앞에 단항 연산자가 있을 경우
		if(index > 0 && is_unary_operator(exp_tokens[index-1]))
			return FAIL;
		
		return SUCCESS;
	}
	
	// 커맨드
	if((result = assign_token_command(target, exp_tokens[index])) != SKIP)
	{
		if(result == FAIL) return FAIL;
		
		// 만약 진짜 커맨드일 경우
		if(exp_tokens[index][0] == TOKEN_COMMAND)
		{
			// 수식의 맨 앞에 있지 않을 경우
			if(index != 0) return FAIL;
		}
		// 히스토리 변수일 경우
		else if(exp_tokens[index][0] == TOKEN_HISTORY_VARIABLE)
		{
			// 변수 앞에 있는게 연산자가 아닐 경우
			if(index > 0 && exp_tokens[index-1][0] != TOKEN_OPERATOR)
				return FAIL;
		}

		return SUCCESS;
	}
	
	// 변수
	if((result = assign_token_variable(target, exp_tokens[index])) != SKIP)
	{
		if(result == FAIL) return FAIL;

		// 변수 앞에 있는게 연산자가 아닐 경우
		if(index > 0 && exp_tokens[index-1][0] != TOKEN_OPERATOR)
			return FAIL;
		
		return SUCCESS;
	}
	
	return FAIL;
}


int parse_to_token(char exp_raw[MAX_STRING], char count_exp_raw,
                   int exp_tokens[MAX_TOKEN_COUNT][2],
                   int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN])
{
	// 수식 길이 초과 시 에러 
	if(count_exp_raw >= 100 || count_exp_raw == 0)
		return FAIL;

	int count_exp_tokens = 0;
	int count_exp_tokens_integer = 0;
	
	// 토큰화
	for(int raw_pointer = 0; raw_pointer < count_exp_raw; raw_pointer++)
	{
		// 현재 무슨 토큰인지 확인
		char current_char = exp_raw[raw_pointer];
		if(assign_token(current_char, exp_tokens, count_exp_tokens) == FAIL)
			return FAIL;
		
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
					if(count_exp_tokens != 0)
						return FAIL;
					break;
				}

				exp_tokens[count_exp_tokens][1] = history_number - 1;
				raw_pointer++;
				break;
		}
		
		count_exp_tokens++;
	}

	// 토큰 개수가 0일 경우
	if(count_exp_tokens == 0)
		return FAIL;

	// 커맨드 모드인데 토큰 개수가 1보다 많을 경우
	if(count_exp_tokens > 1 && exp_tokens[0][0] == TOKEN_COMMAND)
		return FAIL;

	for(int i = 0; i < count_exp_tokens; i++)
	{
		// 연산자
		if(exp_tokens[i][0] == TOKEN_OPERATOR)
		{
			// 문자 끝에 있을 경우
			if(i == count_exp_tokens - 1)
				return FAIL;
			
			// 다음 토큰이 숫자나 단항 연산자가 아닐 경우
			if(!is_number_type(exp_tokens[i+1]) && !is_unary_operator(exp_tokens[i+1]))
				return FAIL;
		}
	}

	// 마무리
	exp_tokens[count_exp_tokens][0] = TOKEN_END;

	return SUCCESS;
}

int get_token_len(int exp_tokens[MAX_TOKEN_COUNT][2])
{
	for(int i = 0; i < MAX_TOKEN_COUNT; i++)
	{
		if(exp_tokens[i][0] == TOKEN_END) return i;
	}
	return MAX_TOKEN_COUNT;
}

#endif