#include "syscall.h"
#include "copyright.h"

int main()
{
    int result = 0, byteRead = 0, fileID = 0;
    char fileName[LIMIT_FILE_NAME + 1];

    Write("Enter filename to delete: ", MAX_LEN, CONSOLE_OUTPUT);
    byteRead = Read(fileName, LIMIT_FILE_NAME, CONSOLE_INPUT);

    result = Delete(fileName);

    if(result != -1)
        Write("\nDelete file successfully.", MAX_LEN, CONSOLE_OUTPUT);
    else 
        Write("\nDelete file unsuccessfully.", MAX_LEN, CONSOLE_OUTPUT);
        
    Halt();
}
