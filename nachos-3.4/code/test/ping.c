#include "syscall.h"

int main()
{
    int i;
    for(i = 0; i < 1000; i++)
        Write("A", MAX_LEN, CONSOLE_OUTPUT);
}