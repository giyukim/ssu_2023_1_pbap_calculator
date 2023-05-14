#ifndef __FILE_IO_C__
#define __FILE_IO_C__

#include "common.h"

#define MAX_FILENAME_STRING 16

int fileio_write_new(char [MAX_FILENAME_STRING], char [][MAX_STRING], int);     // 새로운 파일로 출력
int fileio_write_add(char [MAX_FILENAME_STRING], char [MAX_STRING]);            // 기존 파일에 추가 출력
int fileio_read(char [MAX_FILENAME_STRING]);                                    // 파일 입력 TODO
int file_remove(char [MAX_FILENAME_STRING]);                                    // 파일 삭제

int status_store(char [5][BINT_ARR_LEN], char [3][BINT_ARR_LEN]);

int fileio_write_new(char file_name[MAX_FILENAME_STRING], char text[][MAX_STRING], int text_line_count)
{
    FILE *file;
    file = fopen(file_name, "w");
    if(file == NULL) {
        fclose(file);
        return FAIL;
    }
    for (int loop_temp_array_pointer = 0; loop_temp_array_pointer < text_line_count; loop_temp_array_pointer++) {
        fprintf(file, "%s\n", text[loop_temp_array_pointer]);
    }
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

int status_store(char variable[5][BINT_ARR_LEN], char history[3][BINT_ARR_LEN])
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
    int is_success = 0;

    file_remove(file_name);
    is_success += fileio_write_add(file_name, variable[0]);
    is_success += fileio_write_add(file_name, variable[1]);
    is_success += fileio_write_add(file_name, variable[2]);
    is_success += fileio_write_add(file_name, variable[3]);
    is_success += fileio_write_add(file_name, variable[4]);
    is_success += fileio_write_add(file_name, history[0]);
    is_success += fileio_write_add(file_name, history[1]);
    is_success += fileio_write_add(file_name, history[2]);

    return is_success == 8 ? SUCCESS : FAIL;
}

#endif