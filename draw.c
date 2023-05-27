#ifndef __DRAW_C__
#define __DRAW_C__

#include "common.h"
#include "int_op.c"


// 매인 화면 출력 함수
void draw_main(int[VARIABLE_COUNT][BINT_ARR_LEN], int[MAX_LINE_COUNT], char[MAX_LINE_COUNT][MAX_STRING]);

// 줄 개수 계산 함수
int get_lines_count(int[MAX_LINE_COUNT]);

// 저장된 모든 줄을 출력하는 함수
void draw_lines(int[MAX_LINE_COUNT], char[MAX_LINE_COUNT][MAX_STRING]);

// "출력 줄" 출력 함수
void draw_result(char[MAX_STRING + 5]);

// "입력 줄" 출력 함수
void draw_input(char[MAX_STRING]);

// 줄 "부착" 함수
// 더 이상 부착할 공간이 없을 시 모든 줄을 -1칸씩 이동
void append_line(int[MAX_LINE_COUNT], char[MAX_LINE_COUNT][MAX_STRING], int, char[MAX_STRING]);


void draw_main(int variable[VARIABLE_COUNT][BINT_ARR_LEN],
               int line_types[MAX_LINE_COUNT],
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
    draw_lines(line_types, lines);
}

int get_lines_count(int line_types[MAX_LINE_COUNT])
{
    int count = 0;
    for(; count < MAX_LINE_COUNT && line_types[count] != LINE_TYPE_NOTHING; count++);
    return count;
}

void draw_lines(int line_types[MAX_LINE_COUNT],
                char lines[MAX_LINE_COUNT][MAX_STRING])
{
    int line_count = get_lines_count(line_types);
    for(int i = 0; i < MAX_LINE_COUNT && line_types[i] != LINE_TYPE_NOTHING; i++)
    {
        if(line_types[i] == LINE_TYPE_INPUT)
        {
            draw_input(lines[i]);
            printf("\n");
        }
        else if(line_types[i] == LINE_TYPE_OUTPUT)
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

void append_line(int line_types[MAX_LINE_COUNT],
                 char lines[MAX_LINE_COUNT][MAX_STRING],
                 int incoming_type,
                 char incoming_line[MAX_STRING])
{
    int line_count = get_lines_count(line_types);

    if(line_count == MAX_LINE_COUNT)
    {
        for(int i = 0; i < MAX_LINE_COUNT - 1; i++)
        {
            line_types[i] = line_types[i+1];
            strcpy(lines[i], lines[i+1]);
        }
        line_count = MAX_LINE_COUNT - 1;
    }
    line_types[line_count] = incoming_type;
    strcpy(lines[line_count], incoming_line);
}

#endif