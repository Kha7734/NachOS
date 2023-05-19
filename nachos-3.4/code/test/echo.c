#include "syscall.h"
#include "copyright.h"


int main()
{
    char buffer[MAX_LEN];
    int byteWrite = 0, byteRead = 0, i = 0;
	do
	{
		byteWrite = Write("\nInput string <= 255 chars (Press Ctrl + a to exit): ", MAX_LEN, CONSOLE_OUTPUT);
        i = 0;
		do
		{
			byteRead = Read(buffer, MAX_LEN, CONSOLE_INPUT);
			if(byteRead == -1)
				Halt();
			if (i == 0)
				Write("String: ", MAX_LEN, CONSOLE_OUTPUT);    
			buffer[byteRead] = '\0';
			if (i == 0)
				i = 1;
			byteWrite = Write(buffer, byteRead, CONSOLE_OUTPUT);
		}while(byteRead >= MAX_LEN);
	}while(1);
    Halt();
}
