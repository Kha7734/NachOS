#include "syscall.h"
#include "copyright.h"

int main()
{
    int byteRead = 0, result = 0;
    char fileName[LIMIT_FILE_NAME + 1];

    Write("Enter filename to create: ", MAX_LEN, CONSOLE_OUTPUT);
    byteRead = Read(fileName, LIMIT_FILE_NAME , CONSOLE_INPUT);
    
    result = CreateFile(fileName);
    if(result == -1)
        Write("\nFail to create file.", MAX_LEN, CONSOLE_OUTPUT);
    else 
        Write("\nCreate file successfully.", MAX_LEN, CONSOLE_OUTPUT);
    
    Halt();
}
