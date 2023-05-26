#ifndef __FILE_IO_C__
#define __FILE_IO_C__

#include "common.h"
#include "int_op.c"


// 최대 파일 이름 길이
#define MAX_FILENAME_STRING 16


// 큰 정수를 파일 전용 문자열로 변환하는 함수
void big_int_tofstr(int [BINT_ARR_LEN], char [BINT_ARR_LEN]);

// 파일 전용 정수 문자열을 큰 정수로 변환하는 함수
void big_int_readfstr(char [BINT_ARR_LEN], int [BINT_ARR_LEN]);


// 새로운 파일로 출력하는 함수
int file_io_write_new(char [MAX_FILENAME_STRING], char [][MAX_STRING], int);    // 새로운 파일로 출력

// 기존 파일에 출력하는 함수
int file_io_write_add(char [MAX_FILENAME_STRING], char [MAX_STRING]);

// 파일 내용을 읽어들이는 함수 
int file_io_read(char [MAX_FILENAME_STRING], int, char [][MAX_STRING]);

// 파일을 삭제하는 함수
// 근데 쓰이긴 할까...
int file_remove(char [MAX_FILENAME_STRING]);


// 변수 상태를 cal.txt에 저장하는 함수
int status_save(int [5][BINT_ARR_LEN], int [3][BINT_ARR_LEN]);

// 변수 상태를 cal.txt로부터 로드하는 함수
int status_load(int [5][BINT_ARR_LEN], int [3][BINT_ARR_LEN]);


void big_int_tofstr(int source[BINT_ARR_LEN], char dest[BINT_ARR_LEN])
{
    for(int temp_loop_original = 0; temp_loop_original < big_int_len(source); temp_loop_original++) dest[temp_loop_original] = (char)(source[temp_loop_original] + '0');
    if(source[MAX_DIGIT] == +1) dest[big_int_len(source)] = '+';
    else if(source[MAX_DIGIT] == -1) dest[big_int_len(source)] = '-';
}

void big_int_readfstr(char source[BINT_ARR_LEN], int dest[BINT_ARR_LEN])
{
    big_int_from(0, dest);
    for(int temp_loop_original = 0; temp_loop_original < strlen(source) - 1; temp_loop_original++) dest[temp_loop_original] = (int)(source[temp_loop_original]) - '0';
    if(source[strlen(source) - 1] == '+') dest[MAX_DIGIT] = +1;
    else if(source[strlen(source) - 1] == '-') dest[MAX_DIGIT] = -1;
}

int file_io_write_new(char file_name[MAX_FILENAME_STRING], char text[][MAX_STRING], int text_line_count)
{
    FILE *file = fopen(file_name, "w");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    for (int loop_temp_array_pointer = 0; loop_temp_array_pointer < text_line_count; loop_temp_array_pointer++) fprintf(file, "%s\n", text[loop_temp_array_pointer]);
    fclose(file);
    return SUCCESS;
}

int file_io_write_add(char file_name[MAX_FILENAME_STRING], char text[MAX_STRING])
{
    FILE *file = fopen(file_name, "a");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    fprintf(file, "%s\n", text);
    fclose(file);
    return SUCCESS;
}

int file_io_read(char file_name[MAX_FILENAME_STRING], int text_line_count, char dest[][MAX_STRING])
{
    FILE *file = fopen(file_name, "r");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    for(int loop_temp_line = 0; loop_temp_line < text_line_count; loop_temp_line++) fscanf(file, "%s", dest[loop_temp_line]);
    fclose(file);
    return SUCCESS;
}

int file_remove(char file_name[MAX_FILENAME_STRING])
{
    FILE *file;
    if (!(file = fopen(file_name, "r"))) {
        fclose(file);
        return SKIP;
    }
    fclose(file);
    return remove(file_name) ? FAIL : SUCCESS;
}

int status_save(int variable[5][BINT_ARR_LEN], int history[3][BINT_ARR_LEN])
{
    /*
     * VAR A
     * VAR B
     * VAR C
     * VAR D
     * VAR E
     * VAR H1
     * VAR H2
     * VAR H3
     */
    char file_name[MAX_FILENAME_STRING] = "cal.txt";
    char text[8][MAX_STRING] = {0};

    big_int_tofstr(variable[0], text[0]);
    big_int_tofstr(variable[1], text[1]);
    big_int_tofstr(variable[2], text[2]);
    big_int_tofstr(variable[3], text[3]);
    big_int_tofstr(variable[4], text[4]);
    big_int_tofstr(history[0], text[5]);
    big_int_tofstr(history[1], text[6]);
    big_int_tofstr(history[2], text[7]);

    return file_io_write_new(file_name, text, 8) ? SUCCESS : FAIL;
}

int status_load(int dest_variable[5][BINT_ARR_LEN], int dest_history[3][BINT_ARR_LEN])
{
    char file_name[MAX_FILENAME_STRING] = "cal.txt";
    char data[8][MAX_STRING] = {0};
    if(!file_io_read(file_name, 8, data)) return FAIL;
    big_int_readfstr(data[0], dest_variable[0]);
    big_int_readfstr(data[1], dest_variable[1]);
    big_int_readfstr(data[2], dest_variable[2]);
    big_int_readfstr(data[3], dest_variable[3]);
    big_int_readfstr(data[4], dest_variable[4]);
    big_int_readfstr(data[5], dest_history[0]);
    big_int_readfstr(data[6], dest_history[1]);
    big_int_readfstr(data[7], dest_history[2]);
    return SUCCESS;
}

#endif