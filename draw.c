#ifndef __DRAW_C__
#define __DRAW_C__

#include "common.h"
#include "int_op.c"

void draw_main(int variable[5][BINT_ARR_LEN]);
void draw_result(char result[MAX_STRING + 5]);
void draw_input(char expresion_raw[MAX_STRING]);

void draw_main(int variable[5][BINT_ARR_LEN]){
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
}

void draw_result(char result[MAX_STRING + 5])
{
    printf("======> %s", result);
}

void draw_input(char dest[MAX_STRING])
{
    printf("(input) ");
    scanf("%[^\n]s", dest);
}

#endif