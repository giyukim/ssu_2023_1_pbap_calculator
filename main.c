#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define true 1 //True == 1
#define false 0 //False == 0

#define MAX_DIGIT_EXPRESSION 100 // max digit of expression
#define MAX_DIGIT_INTEGER 30     // max digit of one integer

void draw_map(char a[], char b[], char c[], char d[], char e[]){
    int count_a = strlen(a), count_b = strlen(b), count_c = strlen(c), count_d = strlen(d), count_e = strlen(e);
    printf("************************************************************************\n");
    printf("                               큰정수계산기\n");
    printf("************************************************************************\n");
    printf("* 변 수\n");
    printf("************************************************************************\n");
    if(count_a == 0) printf("a = 0\n");
    else printf("a = %s\n", a);
    if(count_b == 0) printf("b = 0\n");
    else printf("b = %s\n", b);
    if(count_c == 0) printf("c = 0\n");
    else printf("c = %s\n", c);
    if(count_d == 0) printf("d = 0\n");
    else printf("d = %s\n", d);
    if(count_e == 0) printf("e = 0\n");
    else printf("e = %s\n", e);
    printf("************************************************************************\n");
    printf("Help : H(istory) L(oad) R(efresh) (rese)T S(ave) Q(uit)\n"); // Reflesh? -> Refresh
    printf("************************************************************************\n");
}

_Bool is_blank(char target)
{
    if(target == ' ') return true;
    return false;
}

_Bool is_integer(char target) // Judge character target is integer(0~9)
{
    if(target >= 48 && target <= 57) return true;
    return false;
}

_Bool is_operator(char target) // Judge character target is operator(+-*/%)
{
    if(target == '*' || target == '/' || target == '%' || target == '+' || target == '-' || target == '=') return true;
    return false;
}

_Bool is_command(char target)  // Judge character target is command(lrtsqh)
{
    char target_upper = (char)toupper(target);
    if(target_upper == 'L' || target_upper == 'R' || target_upper == 'T' || target_upper == 'S' || target_upper == 'Q' || target_upper == 'H') return true;
    return false;
}

_Bool is_variable(char target)
{
    char target_upper = (char)toupper(target);
    if(target_upper >= 65 && target_upper <= 69) return true;
    return false;
}

void print_result(char message[])
{
    printf("======> %s", message);
}

int main() {
    char variable_a[MAX_DIGIT_INTEGER + 2] = {0}, variable_b[MAX_DIGIT_INTEGER + 2] = {0}, variable_c[MAX_DIGIT_INTEGER + 2] = {0}, variable_d[MAX_DIGIT_INTEGER + 2] = {0}, variable_e[MAX_DIGIT_INTEGER + 2] = {0}; // Variable
    char exp_raw[MAX_DIGIT_EXPRESSION + 5] = {0};
    int count_exp_raw = 0;
    int exp_tokens[MAX_DIGIT_EXPRESSION + 1][2]; /*
     * Format : {{Type, Index}, }
     * Type :
     *  - 0 : Integer
     *  - 1 : Operator
     *  - 2 : Command
     *  - 3 : Variable
     */
    int count_exp_tokens = 0;
    char exp_tokens_integer[MAX_DIGIT_EXPRESSION + 1][MAX_DIGIT_EXPRESSION + 2], exp_tokens_operator[MAX_DIGIT_EXPRESSION + 1], exp_tokens_command[MAX_DIGIT_EXPRESSION + 1], exp_tokens_variable[MAX_DIGIT_EXPRESSION + 1];
    int count_exp_tokens_integer = 0, count_exp_tokens_operator = 0, count_exp_tokens_command = 0, count_exp_tokens_variable = 0;
    char exp_tokens_integer_inverted[MAX_DIGIT_EXPRESSION + 1][MAX_DIGIT_EXPRESSION + 2];
    _Bool error = false;

    draw_map(variable_a, variable_b, variable_c, variable_d, variable_e);


raise_error:
    if(error == true) print_result("error");

    return 0;
}
