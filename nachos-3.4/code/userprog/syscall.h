/* syscalls.h 
 * 	Nachos system call interface. These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel. 
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation 
 * of liability and disclaimer of warranty provisions.
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"

#define MAX_LEN 255
#define LIMIT_FILE_NAME 32
#define LIMIT_FILE 10
#define READ_ONLY 1
#define READ_WRITE 0
#define CONSOLE_INPUT 0
#define CONSOLE_OUTPUT 1

// Process
#define LIMIT_PROC 10

// Define System call
#define SC_Halt			0
// System call for operation
#define SC_Exit			1
#define SC_Exec			2
#define SC_Join			3
// Syscall for file operation
#define SC_CreateFile	4
#define SC_Open			5
#define SC_Read			6
#define SC_Write		7
#define SC_Close		8
#define SC_Delete       9
// Syscall for multithreading
#define SC_Fork			10
#define SC_Yield		11
// Syscall fundamental
#define SC_Seek			12
#define SC_Tell         13
#define SC_Sum			14
#ifndef IN_ASM


/* The system call interface.  These are the operations the Nachos
 * kernel needs to support, to be able to run user programs.
 *
 * Each of these is invoked by a user program by simply calling the 
 * procedure; an assembly language stub stuffs the system call code
 * into a register, and traps to the kernel.  The kernel procedures
 * are then invoked in the Nachos kernel, after appropriate error checking, 
 * from the system call entry point in exception.cc.
 */
 
int Sum(int a, int b);

// Address space control operations: Exit, Exec, and Join
// UserProgram finished (status = 0: valid exit)
void Exit(int status);	

// A unique identifier for an executing user program (address space)
typedef int SpaceId;	

// Run the executable file, save in NachOS file, name: "name" and return address space indentifier
SpaceId Exec(char *name);

// Only return once when userprogram and "id" has been finished
// Return when status is Exit
int Join(SpaceId id); 	

/* Some operation on (Thread) at user level: 
 * Fork and Yield. Allow running multiple thread in user program
 */

/* Fork a thread to run a procedure ("func") in the *same* address space 
 * as the current thread.
 */
void Fork(void(*func)());
/* Yield the CPU to another runnable thread, whether in this address space 
 * or not. 
 */
void Yield();

//--------------------1---------------------
// Using NachOs to print system information
void Halt();

//--------------------2---------------------
// Operation with file: CreateFile, Open, Read, Write, Close

// Identification only for open file in NachOS
typedef int OpenFileId;

/* When an address space is starting, it has 2 open files corresponding with
 * input from keyboard and output to screen (according to UNIX, it means stdin and stdout).
 * Read, Write can be used directly without open console beforeL.
 */

#define ConsoleInput	0  
#define ConsoleOutput	1  

// Create one file in NachOS, with name: "name"
// Return value: {0: success}, {-1: fail}
int CreateFile(char *name);

// Open NachOS file named "name", and return OpenFileId for reading and writing file.
// OpenFileId Open(char *name);
OpenFileId Open (char* name, int type);

/* Close the file, we're done reading and writing to it. */
int Close(OpenFileId Fileid);

//Doc
/* Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int Read(char *buffer, int charcount, OpenFileId id);

// Write "size" bytes from "buffer" to the open file.
int Write(char *buffer, int charcount, OpenFileId id);

// Seek to position in fileId beforehand
int Seek(int pos, OpenFileId id);

// Delete a file with name 
int Delete(char* fileName);

#endif /* IN_ASM */

#endif /* SYSCALL_H */
