#ifndef SCHANDLE_H
#define SCHANDLE_H


#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "string.h"
#include "addrspace.h"

// Handle syscall Exit
int doSC_Exit(int exitStatus);

// Handle syscall Create file
int doSC_CreateFile(char* filename);

// Handle syscall Open
int doSC_Open(char* filename, int type);

// Handle syscall Read
int doSC_Read(int virtAddr, int charcount, int fileId);

// Handle syscall Write
int doSC_Write(int virtAddr, int charcount, int fileId);

// Handle syscall Seek
int doSC_Seek(int pos, int fileId);

// Handle syscall Close
int doSC_Close(int fileId);

// Handle syscall Delete file
int doSC_Delete(char* filename);

// Handle syscall Exec
int doSC_Exec(char*procName);

#endif