#include "../file_io.c"

int main()
{
    int variable[5][BINT_ARR_LEN] = {
            {9, 3, 8, 7, 1, 7, 3, 8, 0},
            {1, 9, 3, 8, 3, 8, 1, 9, 1},
            {0, 9, 8, 1, 3, 0},
            {1, 1},
            {0}
    };
    int history[3][BINT_ARR_LEN] = {
            {1, 0, 8, 0, 8, 3, 9, 0, 1, 8, 1, 1},
            {9, 8, 3, 8, 8, 9, 1},
            {0},
    };

    printf("%d", status_store(variable, history));

    return 0;
}