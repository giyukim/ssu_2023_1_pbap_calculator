#ifndef __INT_OP_C__
#define __INT_OP_C__


#include "common.h"


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


void big_int_copy(int source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
	for(int i = 0; i < BINT_ARR_LEN; i++)
	{
		dest[i] = source[i];
	}
}


int big_int_lcomb(int a, int x[BINT_ARR_LEN],
		          int b, int y[BINT_ARR_LEN],
				  int result[BINT_ARR_LEN])
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
	_Bool negative_factor = factor < 0;
	if(negative_factor) factor = -factor;
	if(factor > 9)
		return FAIL;
	
	int temp_result[BINT_ARR_LEN];
	temp_result[MAX_DIGIT] = source[MAX_DIGIT] * (negative_factor ? -1 : 1);
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

int big_int_divmod(int x[BINT_ARR_LEN], int y[BINT_ARR_LEN],
                   int div_result[BINT_ARR_LEN], int mod_result[BINT_ARR_LEN])
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
	for(int i = MAX_DIGIT - 1; i >= 0; i--)
	{
		if(big_int[i] != 0) return i + 1;
	}
	return 0;
}

/**
 * Made by 안현석, 김민재
 * @param big_int 큰 정수
 */
void big_int_tostring(int big_int[BINT_ARR_LEN],
					  char result[MAX_STRING])
{
	int len = big_int_len(big_int);
	if(len == 0)
	{
		strcpy(result, "0");
		return;
	}

	int str_len = 0;
	if (big_int[MAX_DIGIT] == NEGATIVE)
	{
		result[str_len++] = '-';
	}
	for (int i = len - 1; i >= 0; i--) {
		result[str_len++] = big_int[i] + '0';
		if(i != 0 && i % 3 == 0)
		{
			result[str_len++] = ',';	
		}
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
	if (start < 0 || end >= MAX_STRING || start > end)
	{
        return FAIL;
    }

	int temp_result[BINT_ARR_LEN], i;
	big_int_from(0, temp_result);

	if(str[start] == '-')
	{
		temp_result[MAX_DIGIT] = NEGATIVE;
		start++;
	}

	for(int j = start; j <= end; j++)
	{
		char digit = str[j];
		if(digit < '0' || digit > '9') return FAIL;

		int i = end - j;
		if(i >= MAX_DIGIT) return FAIL;
		temp_result[i] = str[j] - '0';
	}

	big_int_copy(temp_result, result);
	return SUCCESS;
}

#endif
