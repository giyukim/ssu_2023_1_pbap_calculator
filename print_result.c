#include <stdio.h>

void print_result(char message[])
{
    printf("======> %s\n", message);
}

void print_error()
{
    return print_result("error");
}

int main()
{
    print_error();

    return 0;
}