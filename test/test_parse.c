#include "../parse.c"

void debug_print_tokens(int exp_tokens[MAX_TOKEN_COUNT][2],
                        int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN])
{
	printf("Tokens : \n");
	for(int i = 0; exp_tokens[i][0] != TOKEN_END; i++)
	{
		switch(exp_tokens[i][0])
		{
			case TOKEN_INTEGER:
				printf("  TOKEN_INTEGER : ");
				big_int_print(exp_tokens_integer[exp_tokens[i][1]]);
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
					case OPERATOR_MODULO:      printf("%%");        break;
				}
				break;
			case TOKEN_COMMAND:
				printf("  TOKEN_COMMAND : ");
				switch(exp_tokens[i][1])
				{
					case COMMAND_HISTORY:      printf("HISTORY");   break;
					case COMMAND_LOAD:         printf("LOAD");      break;
					case COMMAND_REFRESH:      printf("REFRESH");   break;
					case COMMAND_RESET:        printf("RESET");     break;
					case COMMAND_SAVE:         printf("SAVE");      break;
					case COMMAND_QUIT:         printf("QUIT");      break;
				}
				break;
			case TOKEN_VARIABLE:
				printf("  TOKEN_VARIABLE: ");
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
				printf("  TOKEN_HISTORY_VARIABLE: ");
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

int main(void)
{
	while(1) {
		_Bool error = 0;
		
		int exp_tokens[MAX_TOKEN_COUNT][2];
		int exp_tokens_integer[MAX_TOKEN_COUNT][BINT_ARR_LEN] = {0};
		
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
		
		if(parse_to_token(exp_raw, count_exp_raw, exp_tokens, exp_tokens_integer) == FAIL)
		{
			error = 1;
			goto raise_error;
		}
		
		// <디버깅>
		debug_print_tokens(exp_tokens, exp_tokens_integer);
		// </디버깅>
		
raise_error:
		if(error) printf("error\n\n");
	}

	return 0;
}