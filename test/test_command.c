#include "../common.h"
#include "../command.c"

int main()
{
    char desc[MAX_STRING];
    draw_input(desc);
    printf("%s\n", desc);

    return 0;
}