#ifndef __COMMAND_C__
#define __COMMAND_C__

#include "common.h"
#include "file_io.c"
#include "draw.c"

void command_history(int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                     int[MAX_LINE_COUNT], char[MAX_LINE_COUNT][MAX_STRING]);                    // History (H/h) Command
int command_load(int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN]); // Load    (L/l) Command
int command_save(int[VARIABLE_COUNT][BINT_ARR_LEN], int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN]); // Save    (S/s) Command
void command_refresh(int[MAX_LINE_COUNT]);                                                      // Refresh (R/r) Command
void command_reset(int[VARIABLE_COUNT][BINT_ARR_LEN],
                   int[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN], int[MAX_LINE_COUNT]);             // Reset   (T/t) Command
int command_quit();                                                                             // Quit    (Q/q) Command

void command_history(int history[HISTORY_VARIABLE_COUNT][BINT_ARR_LEN],
                     int lines_type[MAX_LINE_COUNT],
                     char lines[MAX_LINE_COUNT][MAX_STRING])
{
    char temp_string[MAX_STRING];
    for(int i = 0; i < HISTORY_VARIABLE_COUNT; i++)
    {
        big_int_tostring(history[i], temp_string);
        append_line(lines_type, lines, LINE_TYPE_OUTPUT, temp_string);
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