#ifndef __COMMAND_C__
#define __COMMAND_C__

#include "common.h"
#include "file_io.c"
#include "draw.c"


// History (H/h) 커맨드
void command_history(int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                     int[MAX_LINE_COUNT], char[MAX_LINE_COUNT][MAX_STRING]);

// Load    (L/l) 커맨드
int command_load(int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN]);

// Save    (S/s) 커맨드
int command_save(int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN]);

// Refresh (R/r) 커맨드
void command_refresh(int[MAX_LINE_COUNT]);                                                     

// Reset   (T/t) 커맨드
void command_reset(int[VARIABLE_COUNT][BINT_ARR_LEN],
                   int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int[MAX_LINE_COUNT]);


void command_history(int history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int lines_type[MAX_LINE_COUNT], char lines[MAX_LINE_COUNT][MAX_STRING])
{
    char temp_string[MAX_STRING];
    for(int i = 0; i < HISTORY_VARIABLE_COUNT; i++)
    {
        char print_string[MAX_STRING + 5] = {'h', (char)i + '0' + 1, ' ', '=', ' '};
        big_int_tostring(history[i], temp_string);
        strcat(print_string, temp_string);
        append_line(lines_type, lines, LINE_TYPE_OUTPUT, print_string);
    }
}

void command_refresh(int lines_type[MAX_LINE_COUNT])
{
    for(int i = 0; i < MAX_LINE_COUNT; i++)
    {
        lines_type[i] = LINE_TYPE_NOTHING;
    }
}

void command_reset(int dest_variable[VARIABLE_COUNT][BINT_ARR_LEN],
                   int dest_history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                   int lines_type[MAX_LINE_COUNT])
{
    for(int loop_temp_i = 0; loop_temp_i < VARIABLE_COUNT; loop_temp_i++) {
        big_int_from(0, dest_variable[loop_temp_i]);
        if(loop_temp_i >= 0 && loop_temp_i < HISTORY_VARIABLE_COUNT) big_int_from(0, dest_history[loop_temp_i]);
    }
    command_refresh(lines_type);
}

int command_load(int dest_variable[VARIABLE_COUNT][BINT_ARR_LEN], int dest_history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN])
{
    return status_load(dest_variable, dest_history) ? SUCCESS : FAIL;
}

int command_save(int variable[VARIABLE_COUNT][BINT_ARR_LEN], int history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN])
{
    return status_save(variable, history) ? SUCCESS : FAIL;
}

#endif