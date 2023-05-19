#include "syscall.h"
#include "copyright.h"

int main()
{   
    int pingId, pongId;
    Write("\nPing-Pong test starting...\n", MAX_LEN, CONSOLE_OUTPUT);
    pingId = Exec("./test/ping");
    pongId = Exec("./test/pong");
    while(1){}
}
