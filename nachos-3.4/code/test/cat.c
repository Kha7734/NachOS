#include "syscall.h"
#include "copyright.h"

int main()
{
    char buffer[MAX_LEN], fileName[LIMIT_FILE_NAME];
    int fileId, byteRead = 0, byteWrite = 0;

    Write("\nEnter file name (Enter Ctrl + a to exit): ", MAX_LEN, CONSOLE_OUTPUT);
    byteRead = Read(fileName, LIMIT_FILE_NAME, CONSOLE_INPUT);
    if(byteRead == -1)
        Halt();

    fileId = Open(fileName, READ_ONLY);
    if(fileId != -1)
    {
        do
        {
            byteRead = Read(buffer, MAX_LEN, fileId);
            if(byteRead == -2)
            {
                byteRead = Seek(-1, fileId) - byteWrite;
            }
            Write(buffer, byteRead, CONSOLE_OUTPUT);
            byteWrite = byteRead + byteWrite;

        }while(byteRead == MAX_LEN);

        Close(fileId);
    }

    Halt();
}