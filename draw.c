#ifndef __DRAW_C__
#define __DRAW_C__

#include "common.h"
#include "int_op.c"

void draw_main(int variable[VARIABLE_COUNT][BINT_ARR_LEN], int line_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING]);
void draw_result(char result[MAX_STRING + 5]);
void draw_input(char expresion_raw[MAX_STRING]);
void draw_lines(int line_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING]);

void draw_main(int variable[VARIABLE_COUNT][BINT_ARR_LEN],
               int line_type[MAX_LINE_COUNT],
               char lines[MAX_LINE_COUNT][MAX_STRING]){
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
    draw_lines(line_type, lines);
}

void draw_lines(int line_type[MAX_LINE_COUNT],
                char lines[MAX_LINE_COUNT][MAX_STRING])
{
    for(int i = 0; i < MAX_LINE_COUNT && line_type[i] != LINE_TYPE_NOTHING; i++)
    {
        if(line_type[i] == LINE_TYPE_INPUT)
        {
            draw_input(lines[i]);
            printf("\n");
        }
        else if(line_type[i] == LINE_TYPE_OUTPUT)
        {
            draw_result(lines[i]);
            printf("\n");
        }
    }
}

void draw_result(char result[MAX_STRING + 5])
{
    printf("======> %s", result);
}

void await_input(char dest[MAX_STRING])
{
    printf("(input) ");
    // scanf("%[^\n]s", dest);
    // 버그때문에 scanf를 못 쓰겠음
    int op, i = 0;
    while((op = getchar()) != '\n')
    {
        dest[i++] = op;
    }
    dest[i] = '\0';
}

void draw_input(char result[MAX_STRING])
{
    printf("(input) %s", result);
}

void clear_screen()
{
    system("@cls||clear");
}

#endif