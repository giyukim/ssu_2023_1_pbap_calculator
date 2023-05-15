#ifndef __FILE_IO_C__
#define __FILE_IO_C__

#include "common.h"
#include "int_op.c"

#define MAX_FILENAME_STRING 16

void big_int_tostring(int [BINT_ARR_LEN], char [BINT_ARR_LEN]);               // To String

int fileio_write_new(char [MAX_FILENAME_STRING], char [][MAX_STRING], int);     // 새로운 파일로 출력
int fileio_write_add(char [MAX_FILENAME_STRING], char [MAX_STRING]);            // 기존 파일에 추가 출력
int fileio_read(char [MAX_FILENAME_STRING]);                                    // 파일 입력 TODO
int file_remove(char [MAX_FILENAME_STRING]);                                    // 파일 삭제

void big_int_tostring(int source[BINT_ARR_LEN], char dest[BINT_ARR_LEN])
{
    for(int temp_loop_original = 0; temp_loop_original <= big_int_len(source); temp_loop_original++) dest[temp_loop_original] = (char)(source[temp_loop_original] + '0');
}

int status_store(int [5][BINT_ARR_LEN], int [3][BINT_ARR_LEN]);

int fileio_write_new(char file_name[MAX_FILENAME_STRING], char text[][MAX_STRING], int text_line_count)
{
    FILE *file;
    file = fopen(file_name, "w");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    for (int loop_temp_array_pointer = 0; loop_temp_array_pointer < text_line_count; loop_temp_array_pointer++) fprintf(file, "%s\n", text[loop_temp_array_pointer]);
    fclose(file);
    return SUCCESS;
}

int fileio_write_add(char file_name[MAX_FILENAME_STRING], char text[MAX_STRING])
{
    FILE *file;
    file = fopen(file_name, "a");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    fprintf(file, "%s\n", text);
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

int status_store(int variable[5][BINT_ARR_LEN], int history[3][BINT_ARR_LEN])
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

    big_int_tostring(variable[0], text[0]);
    big_int_tostring(variable[1], text[1]);
    big_int_tostring(variable[2], text[2]);
    big_int_tostring(variable[3], text[3]);
    big_int_tostring(variable[4], text[4]);
    big_int_tostring(history[0], text[5]);
    big_int_tostring(history[1], text[6]);
    big_int_tostring(history[2], text[7]);

    return fileio_write_new(file_name, text, 8) ? SUCCESS : FAIL;
}

#endif