// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.
#pragma once
#include "schandle.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------


void IncreasePC()
{
    int counter = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, counter);
    counter = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, counter);
    machine->WriteRegister(NextPCReg, counter + 4);
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch (which)
    {
    case NoException:
    {
        return;
    }
    case PageFaultException:
    {
        DEBUG('a', "\n No valid translation found");
		printf("\n\n No valid translation found");
		interrupt->Halt();
		break;
    }
    case ReadOnlyException:
    {
        DEBUG('a', "\n Write attempted to page marked read-only");
		printf("\n\n Write attempted to page marked read-only");
		interrupt->Halt();
		break;
    }
    case BusErrorException:
    {
        DEBUG('a', "\n Translation resulted invalid physical address");
		printf("\n\n Translation resulted invalid physical address");
		interrupt->Halt();
		break;
    }
    case AddressErrorException:
    {
        DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
		printf("\n\n Unaligned reference or one that was beyond the end of the address space");
		interrupt->Halt();
		break;
    }
    case OverflowException:
    {
		DEBUG('a', "\nInteger overflow in add or sub.");
		printf("\n\n Integer overflow in add or sub.");
		interrupt->Halt();
		break;
    }
	case IllegalInstrException:
    {
		DEBUG('a', "\n Unimplemented or reserved instr.");
		printf("\n\n Unimplemented or reserved instr.");
		interrupt->Halt();
		break;
    }
	case NumExceptionTypes:
    {
		DEBUG('a', "\n Number exception types");
		printf("\n\n Number exception types");
		interrupt->Halt();
		break;
    }

    case SyscallException:
    {
        switch (type)
        {
            case SC_Halt:
            {
                DEBUG('a', "Shutdown, initiated by user program.\n");
                printf("\n\nShutdown, initiated by user program.");
                interrupt->Halt();
                break;
            }
            case SC_Exit:
            {
                int exitStatus = machine->ReadRegister(4);
                int result = doSC_Exit(exitStatus);

                machine->WriteRegister(2, result);
                break;
            }
            case SC_CreateFile:
            {
                int virtAddr;
                DEBUG(dbgAddr, "\nSC_Create call...");
                DEBUG(dbgAddr, "\nReading virtual address of filename.");
            
                // Get file name argument in register r4
                virtAddr = machine->ReadRegister(4);
                DEBUG(dbgAddr, "\nReading filename.");

                char* filename = machine->User2System(virtAddr, LIMIT_FILE_NAME);
                int result = doSC_CreateFile(filename);
                
                machine->WriteRegister(2, result);
                delete[] filename;
                break;
            }
            case SC_Open:
            {
                DEBUG(dbgAddr, "\nSC_Open call...");
                DEBUG(dbgAddr, "\nReading virtual address of filename.");
                int virtAddr = machine->ReadRegister(4);

                DEBUG(dbgAddr, "\nReading file type of filename.");
                int type = machine->ReadRegister(5);
                
                char* filename = machine->User2System(virtAddr, LIMIT_FILE_NAME );
                int result = doSC_Open(filename, type);

                machine->WriteRegister(2, result);
                delete[] filename;
                break;
            }
            case SC_Close:
            {
                DEBUG(dbgAddr, "\nSC_Close call...");
                DEBUG(dbgAddr, "\nReding file id.");
                int fileId = machine->ReadRegister(4);

                int result = doSC_Close(fileId);
                
                machine->WriteRegister(2, result);
                break;
            }
            case SC_Read:
            {
                DEBUG(dbgAddr, "\nSC_Read call...");
                DEBUG(dbgAddr, "\nReading virtual address of filename.");
                int virtAddr = machine->ReadRegister(4);

                DEBUG(dbgAddr, "\nReading char count.");
                int charcount = machine->ReadRegister(5);

                DEBUG(dbgAddr, "\nReading fileId.");
                OpenFileId fileId = machine->ReadRegister(6);

                int result = doSC_Read(virtAddr, charcount, fileId);

                machine->WriteRegister(2, result);
                break;
            }
            case SC_Write:
            {
                DEBUG(dbgAddr, "\nSC_Write call...");
                DEBUG(dbgAddr, "\nReading virtual address of filename.");
                int virtAddr = machine->ReadRegister(4);

                DEBUG(dbgAddr, "\nReading char count.");
                int charcount = machine->ReadRegister(5);

                DEBUG(dbgAddr, "\nReading fileId.");
                OpenFileId fileId = machine->ReadRegister(6);

                int result = doSC_Write(virtAddr, charcount, fileId);

                machine->WriteRegister(2, result);
                break;
            }
            case SC_Seek:
            {
                DEBUG(dbgAddr, "\nSC_Seek call...");
                DEBUG(dbgAddr, "\nReading virtual address of filename.");
                int pos = machine->ReadRegister(4);

                DEBUG(dbgAddr, "\nReading fileId.");
                OpenFileId fileId = machine->ReadRegister(5); 

                int result = doSC_Seek(pos, fileId);

                machine->WriteRegister(2, result);
                break;
            }
            case SC_Delete:
            {
                DEBUG(dbgAddr, "\nSC_Delete call...");
                DEBUG(dbgAddr, "\nReading virtual address of filename.");
                int virtAddr = machine->ReadRegister(4);
                char* filename = machine->User2System(virtAddr,LIMIT_FILE_NAME);
                
                int result = doSC_Delete(filename);

                machine->WriteRegister(2, result);
                delete[] filename;
                break;
            }
            case SC_Exec:
            {
                DEBUG(dbgAddr, "\nSC_Exec call...");
                DEBUG(dbgAddr, "\nReading virtual address of filename.");
                int virtAddr = machine->ReadRegister(4);
                char* procName = machine->User2System(virtAddr, LIMIT_FILE_NAME);
            
                int result = doSC_Exec(procName);

                machine->WriteRegister(2, result);
                break;
            }
            default:
            {
                printf("\nUnexpected system call %d.", type);
                break;
            }
        }
        IncreasePC();
        break;
    }
    default:
        printf("\nUnexpected user mode exception (%d, %d)", which, type);
        interrupt->Halt();
        break;
    }
}

