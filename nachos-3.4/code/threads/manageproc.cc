#include "manageproc.h"

// Manage process with 2 arrays:
//      + m_threadArr: Pointer to processing threads
//      + BitMap: Manage threads, find free slot.
ManageProc::ManageProc() : BitMap(LIMIT_PROC)
{
    m_threadArr = new Thread*[LIMIT_PROC];

    for(int i = 0; i < LIMIT_PROC; i++)
        m_threadArr[i] = NULL;

    this->Mark(0);
    m_threadArr[0] = currentThread; // Main thread
}

ManageProc::~ManageProc()
{
    delete m_threadArr;
}

// Handling threads for processing
int ManageProc::doExec(char* procName)
{
    if (!procName)
    {
        DEBUG(dbgSys, "\nError: process name is NULL.");
        printf("\nError: process name is NULL.");
        return -1;
    }
    if(strcmp(procName, currentThread->getName()) == 0)
    {
        DEBUG(dbgSys, "\nError: Cannot execute itself.");
        printf("\nError: Cannot execute itself.");
        return -1;
    }

    int processId = this->Find();
    if(processId == -1)
    {
        DEBUG(dbgSys, "\nError: Out of free slot.");
        printf("\nError: Out of free slot.");
        return -1;
    }
    
    // If slot is available, initialize new thread with m_processId = processId
    m_threadArr[processId] = new Thread(procName);
    if(!m_threadArr[processId])
    {
    	DEBUG(dbgAddr, "\nError: Not enough memory.");
        printf("\nError: Not enough memory.");
        return -1;
    }
    m_threadArr[processId]->m_processId = processId;
    m_threadArr[processId]->Fork(StartProcess_2, processId);
    
    return processId;
}

// Handling for SC_Exit
int ManageProc::checkExit(int exitStatus)
{
	if(exitStatus != 0)
	{
		return -1;
	}
    int currentProcessId = currentThread->m_processId;
    if(currentProcessId == 0)   // Main thread
    {
    	if (currentThread->space != NULL)
            delete currentThread->space;
        interrupt->Halt();
    }
    if (currentThread->space != NULL)
        delete currentThread->space;
    
    currentThread->Finish();
    delete m_threadArr[currentProcessId];
    this->Clear(currentProcessId);
    
    return currentProcessId;
}
