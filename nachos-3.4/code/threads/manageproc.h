#ifndef MAN_EXEC_H
#define MAN_EXEC_H

#include "bitmap.h"
#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "addrspace.h"  // StartProcess_2

const char dbgSys = 's';

class ManageProc : public BitMap
{
public: 
    Thread** m_threadArr;
    ManageProc();
    ~ManageProc();

public:
    int doExec(char* procName);
    int checkExit(int exitStatus);
};

#endif
