#ifndef __COMMAND_C__
#define __COMMAND_C__

#include "common.h"
#include "file_io.c"
#include "draw.c"

void command_history(int history[3][BINT_ARR_LEN]);                                             // History (H/h) Command
int command_load(int dest_variable[5][BINT_ARR_LEN], int dest_history[3][BINT_ARR_LEN]);        // Load    (L/l) Command
void command_refresh(int variable[5][BINT_ARR_LEN]);                                            // Refresh (R/r) Command
void command_reset(int dest_variable[5][BINT_ARR_LEN], int dest_history[3][BINT_ARR_LEN]);      // Reset   (T/t) Command
int command_save(int variable[5][BINT_ARR_LEN], int history[3][BINT_ARR_LEN]);                  // Save    (S/s) Command
int command_quit();                                                                             // Quit    (Q/q) Command

void command_history(int history[3][BINT_ARR_LEN])
{
    draw_result("h1 = ");
    big_int_print(history[0]);
    printf("\n");
    draw_result("h2 = ");
    big_int_print(history[1]);
    printf("\n");
    draw_result("h3 = ");
    big_int_print(history[2]);
    printf("\n");
}

void command_refresh(int variable[5][BINT_ARR_LEN])
{
    system("@cls||clear");
    draw_main(variable);
}

void command_reset(int dest_variable[5][BINT_ARR_LEN], int dest_history[3][BINT_ARR_LEN])
{
    int bint_reset[BINT_ARR_LEN] = {0};
    int variable[5][BINT_ARR_LEN];
    for(int loop_temp_i = 0; loop_temp_i < 5; loop_temp_i++) {
        big_int_copy(dest_variable[loop_temp_i], variable[loop_temp_i]);
        big_int_copy(bint_reset, dest_variable[loop_temp_i]);
        if(loop_temp_i >= 0 && loop_temp_i < 3) big_int_copy(bint_reset, dest_history[loop_temp_i]);
    }
    command_refresh(variable);
}

int command_load(int dest_variable[5][BINT_ARR_LEN], int dest_history[3][BINT_ARR_LEN])
{
    return status_load(dest_variable, dest_history) ? SUCCESS : FAIL;
}

int command_save(int variable[5][BINT_ARR_LEN], int history[3][BINT_ARR_LEN])
{
    return status_save(variable, history) ? SUCCESS : FAIL;
}

#endif