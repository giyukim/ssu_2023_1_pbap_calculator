#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SKIP					2
#define SUCCESS					1
#define FAIL					0

#define MAX_STRING		        101
#define MAX_DIGIT 		        30
#define MAX_DIGIT_EXPRESSION	100
#define BINT_ARR_LEN			(MAX_DIGIT + 1)

// TOKEN types
#define TOKEN_INTEGER			0
#define TOKEN_OPERATOR			1
#define TOKEN_COMMAND			2
#define TOKEN_VARIABLE			3
#define TOKEN_HISTORY_VARIABLE	4
#define TOKEN_NOTHING			(-1)
#define TOKEN_UNKNOWN			(-2)
#define TOKEN_END				(-3)

// TOKEN_OPERATOR values
#define OPERATOR_PLUS			0
#define OPERATOR_UNARY_PLUS		1
#define OPERATOR_MINUS			2
#define OPERATOR_UNARY_MINUS	3
#define OPERATOR_MULTIPLY		4
#define OPERATOR_DIVDE			5
#define OPERATOR_MODULO			6
#define OPERATOR_ASSIGN			7

// TOKEN_COMMANDs
#define COMMAND_HISTORY			0
#define COMMAND_LOAD			1
#define COMMAND_REFRESH			2
#define COMMAND_RESET			3
#define COMMAND_SAVE			4
#define COMMAND_QUIT			5

#define POSITIVE		        1
#define NEGATIVE	        	(-1)


int big_int_parse(int[MAX_STRING], int, int, int[BINT_ARR_LEN]){ // TODO

}

void big_int_fprint(int[BINT_ARR_LEN]){ // TODO

}



void big_int_from(long long value, int result[BINT_ARR_LEN])
{
    result[MAX_DIGIT] = value >= 0 ? POSITIVE : NEGATIVE;
    value = value < 0 ? -value : value;
    for(int i = 0; i < MAX_DIGIT; i++)
    {
        result[i] = value % 10;
        value /= 10;
    }
}

void big_int_copy(int source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
    for(int i = 0; i < BINT_ARR_LEN; i++)
    {
        dest[i] = source[i];
    }
}

int big_int_fix(int carry_end, int big_int[BINT_ARR_LEN])
{
    // carry: 자리올림
    int carry = 0, digit;
    int temp_result[BINT_ARR_LEN];

    temp_result[MAX_DIGIT] = big_int[MAX_DIGIT];
    // 각 자릿수가 0~9를 넘지 않도록 수정
    for(int i = 0; i < MAX_DIGIT; i++)
    {
        digit = big_int[i] + carry;
        temp_result[i] = digit % 10;
        carry = digit / 10;

        if(temp_result[i] < 0)
        {
            temp_result[i] += 10;
            carry--;
        }
    }
    carry += carry_end;

    // 만약 자리올림의 절댓값이 1이고 원래 부호와 다를 경우
    if(carry * temp_result[MAX_DIGIT] == NEGATIVE)
    {
        temp_result[MAX_DIGIT] = -temp_result[MAX_DIGIT];

        // 0이 아닌 가장 낮은 자릿수 검색
        int start = MAX_DIGIT;
        for(int i = 0; i < MAX_DIGIT; i++)
        {
            if(temp_result[i] == 0) continue;
            start = i;
            break;
        }

        // 0이 아닌 자릿수가 없음
        //  => |result| = 10^MAX_DIGIT
        //  => 오버플로우 / 언더플로우
        if(start == MAX_DIGIT)
        {
            return FAIL;
        }

        // 자리내림
        temp_result[start] = 10 - temp_result[start];
        for(int i = start+1; i < MAX_DIGIT; i++)
        {
            temp_result[i] = 9 - temp_result[i];
        }
    }
    else if(carry != 0) // 오버플로우 / 언더플로우
    {
        return FAIL;
    }

    big_int_copy(temp_result, big_int);
    return SUCCESS;
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
    for(int i = 0; i < MAX_DIGIT; i++)
    {
        if(source[i] == 0) continue;

        int new_digit = i + amount;
        if(new_digit < 0) continue;
        if(new_digit >= MAX_DIGIT)
            return FAIL;

        temp_result[new_digit] = source[i];
    }

    big_int_copy(temp_result, dest);
    return SUCCESS;
}


int big_int_times(int factor, int source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
    if(factor < 0 || factor > 9)
        return FAIL;

    int temp_result[BINT_ARR_LEN];
    temp_result[MAX_DIGIT] = source[MAX_DIGIT];
    for(int i = 0; i < MAX_DIGIT; i++)
    {
        temp_result[i] = factor * source[i];
    }

    if(big_int_fix(0, temp_result) == FAIL)
        return FAIL;

    big_int_copy(temp_result, dest);
    return SUCCESS;
}


int big_int_mul(int x[BINT_ARR_LEN], int y[BINT_ARR_LEN], int result[BINT_ARR_LEN])
{
    int temp_big_int[BINT_ARR_LEN], temp_result[BINT_ARR_LEN];
    big_int_from(0, temp_result);

    for(int yd = 0; yd < MAX_DIGIT; yd++)
    {
        char y_digit = y[yd];
        if(y_digit == 0) continue;

        // temp_big_int = x
        big_int_copy(x, temp_big_int);
        temp_big_int[MAX_DIGIT] = POSITIVE;

        // temp_big_int *= y_digit
        if(big_int_times(y_digit, temp_big_int, temp_big_int) == FAIL)
            return FAIL;

        // temp_big_int *= 10^yd
        if(big_int_shift(yd, temp_big_int, temp_big_int) == FAIL)
            return FAIL;

        // temp_result += temp_big_int
        if(big_int_lcomb(1, temp_big_int, 1, temp_result, temp_result) == FAIL)
            return FAIL;
    }

    temp_result[MAX_DIGIT] = x[MAX_DIGIT] * y[MAX_DIGIT];
    big_int_copy(temp_result, result);
    return SUCCESS;
}

int big_int_len(int big_int[BINT_ARR_LEN])
{
    for(int i = MAX_DIGIT - 1; i >= 0; i--)
    {
        if(big_int[i] != 0) return i + 1;
    }
    return 0;
}

int big_int_divmod(int x[BINT_ARR_LEN], int y[BINT_ARR_LEN], int div_result[BINT_ARR_LEN], int mod_result[BINT_ARR_LEN])
{
    int temp_div_result[BINT_ARR_LEN], temp_big_int[BINT_ARR_LEN], x_copy[BINT_ARR_LEN];
    big_int_from(0, temp_div_result);

    // x_copy = x
    big_int_copy(x, x_copy);
    x_copy[MAX_DIGIT] = POSITIVE;

    // 흔히 쓰는 나눗셈 방법: 자리 옮겨가면서 나누는 수보다 작아질 때까지 빼기
    int x_len = big_int_len(x), y_len = big_int_len(y);
    for(int i = x_len - y_len; i >= 0; i--)
    {
        // temp_big_int = y
        big_int_copy(y, temp_big_int);
        temp_big_int[MAX_DIGIT] = POSITIVE;

        // temp_big_int *= 10^i
        if(big_int_shift(i, temp_big_int, temp_big_int) == FAIL)
            return FAIL;

        // x_copy가 0보다 작아질 때까지 빼기
        int divisor_digit = 0;
        while(x_copy[MAX_DIGIT] == POSITIVE) // x_copy >= 0
        {
            // x_copy -= temp_big_int
            if(big_int_lcomb(1, x_copy, -1, temp_big_int, x_copy) == FAIL)
                return FAIL;

            divisor_digit++;
        }

        // x_copy += temp_big_int
        if(big_int_lcomb(1, x_copy, 1, temp_big_int, x_copy) == FAIL)
            return FAIL;

        temp_div_result[i] = divisor_digit - 1;
    }

    big_int_copy(temp_div_result, div_result);
    big_int_copy(x_copy, mod_result); // 이 때의 x_copy는 나머지 값이므로 mod_result로 복사

    // 연산 도중 실패할 수 있으므로 부호는 나중에 저장
    div_result[MAX_DIGIT] = x[MAX_DIGIT] * y[MAX_DIGIT];
    mod_result[MAX_DIGIT] = x[MAX_DIGIT];
    return SUCCESS;
}

void big_int_print(int big_int[BINT_ARR_LEN])
{
    // TODO: 임시 코드 수정하기
    printf("%+d, ", big_int[MAX_DIGIT]);
    for(int i = MAX_DIGIT - 1; i >= 0; i--)
    {
        printf("%d ", big_int[i]);
    }
    printf("\n");
}

void debug_print_tokens(int exp_tokens[1000][2])
{
	printf("Tokens : \n");
	for(int i = 0; exp_tokens[i][0] != TOKEN_END; i++)
	{
		switch(exp_tokens[i][0])
		{
			case TOKEN_INTEGER:
				printf("  TOKEN_INTEGER : [%d]", exp_tokens[i][1]);
				break;
			case TOKEN_OPERATOR:
				printf("  TOKEN_OPERATOR: ");
				switch(exp_tokens[i][1])
				{
					case OPERATOR_PLUS:        printf("+");         break;
					case OPERATOR_UNARY_PLUS:  printf("+ (unary)"); break;
					case OPERATOR_MINUS:       printf("-");         break;
					case OPERATOR_UNARY_MINUS: printf("- (unary)"); break;
					case OPERATOR_ASSIGN:      printf("=");         break;
					case OPERATOR_MULTIPLY:    printf("*");         break;
					case OPERATOR_DIVDE:       printf("/");         break;
					case OPERATOR_MODULO:      printf("%%");         break;
				}
				break;
			case TOKEN_COMMAND:
				printf("  TOKEN_COMMAND : ");
				switch(exp_tokens[i][1])
				{
					case COMMAND_HISTORY:	printf("HISTORY");	break;
					case COMMAND_LOAD:		printf("LOAD");		break;
					case COMMAND_REFRESH:	printf("REFRESH");	break;
					case COMMAND_RESET:		printf("RESET");	break;
					case COMMAND_SAVE:		printf("SAVE");		break;
					case COMMAND_QUIT:		printf("QUIT");		break;
				}
				break;
			case TOKEN_VARIABLE:
				printf("  TOKEN_VARIABLE: "); // 귀찮 ㅎㅎ
				switch(exp_tokens[i][1])
				{
					case 0:	printf("a"); break;
					case 1:	printf("b"); break;
					case 2:	printf("c"); break;
					case 3:	printf("d"); break;
					case 4:	printf("e"); break;
				}
				break;
			case TOKEN_HISTORY_VARIABLE:
				printf("  TOKEN_HISTORY_VARIABLE: "); // 귀찮 ㅎㅎ
				switch(exp_tokens[i][1])
				{
					case 0:	printf("h1"); break;
					case 1:	printf("h2"); break;
					case 2:	printf("h3"); break;
				}
				break;
		}
		printf("\n");
	}
	printf("\n");
}

_Bool is_digit_integer(char target)
{
	return target >= '0' && target <= '9';	
}

_Bool is_number_type(int exp_token[2])
{
	return exp_token[0] == TOKEN_INTEGER || exp_token[0] == TOKEN_VARIABLE || exp_token[0] == TOKEN_HISTORY_VARIABLE;
}

_Bool is_unary_operator(int exp_token[2])
{
	return exp_token[0] == TOKEN_OPERATOR &&
	        (exp_token[1] == OPERATOR_UNARY_PLUS || exp_token[1] == OPERATOR_UNARY_MINUS);
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
	// 변수는 5개밖에 없으니까 간단하게 switch로 생각
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

int assign_token(char target, int exp_tokens[MAX_DIGIT_EXPRESSION + 1][2], int index)
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

	//
	
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


int parse_to_token(char exp_raw[MAX_DIGIT_EXPRESSION + 5], char count_exp_raw,
                   int exp_tokens[1000][2],
				   int exp_tokens_integer[1000][BINT_ARR_LEN])
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
				if(history_number < 1 || history_number > 3)
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


int main(void)
{
	while(1) {
		_Bool error = 0;
		
		int exp_tokens[1000][2];
		int exp_tokens_integer[1000][BINT_ARR_LEN] = {0};
		
		// 입력 문자열
		int count_exp_raw = 0, op;
		char exp_raw[MAX_DIGIT_EXPRESSION + 5] = {0};
		
		// 계산식 입력
		printf("(input) ");

		// 버그때문에 scanf를 못 쓰겠음
		while((op = getchar()) != '\n')
		{
			exp_raw[count_exp_raw++] = op;
		}
		exp_raw[count_exp_raw] = '\0';
		
		if(parse_to_token(exp_raw, count_exp_raw, exp_tokens, exp_tokens_integer) == FAIL)
		{
			error = 1;
			goto raise_error;
		}
		
		// <디버깅>
		debug_print_tokens(exp_tokens);
		// </디버깅>
		
raise_error:
		if(error) printf("error\n\n");
	}

	return 0;
}