#include "../file_io.c"

int main()
{
    char variable[5][BINT_ARR_LEN] = {
            "93871738",
            "19383819",
            "09813",
            "1",
            "0"
    };
    char history[3][BINT_ARR_LEN] = {
            "108083901811",
            "9838891",
            "0",
    };

    printf("%d", status_store(variable, history));

    return 0;
}