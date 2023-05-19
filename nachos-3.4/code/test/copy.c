#include "syscall.h"
#include "copyright.h"

int main()
{
    char src[LIMIT_FILE_NAME + 1], dest[LIMIT_FILE_NAME + 1];
    int fileIdSrc, fileIdDest;
    char buffer[MAX_LEN + 1];
    int byteRead = 0, byteWrite = 0;

    Write("\nEnter source file: ", LIMIT_FILE_NAME, CONSOLE_OUTPUT);
    Read(src, LIMIT_FILE_NAME, CONSOLE_INPUT);

    Write("\nEnter destination file: ", LIMIT_FILE_NAME, CONSOLE_OUTPUT);
    Read(dest, LIMIT_FILE_NAME, CONSOLE_INPUT);

    fileIdSrc = Open(src, READ_ONLY);
    fileIdDest = Open(dest, READ_WRITE);
    if(fileIdSrc == -1 || fileIdDest == -1)
    {
        Write("\nError: File name is invalid.", MAX_LEN, ConsoleOutput);
    }
    else
    {
        do
        {
            byteRead = Read(buffer, MAX_LEN, fileIdSrc);
            if(byteRead == -2)
            {
                byteRead = Seek(-1, fileIdSrc) - Seek(-1, fileIdDest);
            }
            Write(buffer, byteRead, fileIdDest);
        }while(byteRead == MAX_LEN);

        Close(fileIdSrc);
        Close(fileIdDest);
    }
    Halt();
}
