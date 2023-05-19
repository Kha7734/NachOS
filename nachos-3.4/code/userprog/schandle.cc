#include "schandle.h"

int doSC_Exit(int exitStatus)
{
    int exitcode = manProc->checkExit(exitStatus); 
    return exitcode;
}

int doSC_CreateFile(char* filename)
{
    int checkValid = 0;

    if (!filename)
    {
        printf("\nNot enough memory in system.");
        DEBUG(dbgAddr, "\nNot enough memory in system.");
        checkValid = -1;
    }

    OpenFile* checkExist = fileSystem->Open(filename);
    if(checkExist)
    {
        printf("\nError: File is exist.");
        checkValid = -1;
    }
    else
    {
        bool result = fileSystem->Create(filename, 0); // Initial size = 0
        if (result == false)
        {
            printf("\nError: Cannot create file '%s'", filename);
            checkValid = -1;
        }
    }
 
    delete checkExist;
    return checkValid;
}

int doSC_Open(char* filename, int type)
{   
    
    int fileId = -1;

    if(!manFile->checkType(type))
        DEBUG(dbgFile, "\nError: FileType is not valid");
    else
    {
        fileId = manFile->checkOpening(filename);
        if(fileId != -1)
        {
            DEBUG(dbgFile, "\nFile is already opening.");
            printf("\nFile is already opening.");
            return fileId;
        }

        // Marked in bitmap if success
        fileId = manFile->checkFreeSlot();
        if(fileId == -1)
            DEBUG(dbgFile, "\nError: Out of free slot");
        else if(!filename)
        {
            DEBUG(dbgFile, "\nError: Filename is invalid.");
            printf("\nError: Filename is invalid.");
            manFile->Clear(fileId);
            fileId = -1;
        }
        else if(fileSystem->Open(filename) == NULL)
        {
            DEBUG(dbgFile, "\nError: File name is not available.");
            printf("\nError: File name is not available.");
            manFile->Clear(fileId);
            fileId = -1;
        }
        else 
        {
            // Storage fileType and fileName for Read and Write file.
            manFile->m_fileOpen[fileId] = fileSystem->Open(filename);
            manFile->m_fileType[fileId] = type;
            strcpy(manFile->m_fileName[fileId],filename);
        }
    }
    return fileId;
}

int doSC_Read(int virtAddr, int charcount, int fileId)
{
    int byteRead = -1;

    if(!manFile->checkFileId(fileId))
        DEBUG(dbgFile, "\nError: FileId is invalid.");
    else if(fileId == CONSOLE_OUTPUT)
    {
        DEBUG(dbgFile, "\nError: Cannot read Console output file.");
        printf("\nError: Cannot read Console output.");
    }
    else if(fileId == CONSOLE_INPUT)
        byteRead = manFile->readFromConsole(virtAddr, charcount, fileId);
    else
        byteRead = manFile->readFromFile(virtAddr, charcount, fileId);

    return byteRead;
}

int doSC_Write(int virtAddr, int charcount, int fileId)
{
    int byteWrite = -1;

    if(!manFile->checkFileId(fileId))
        printf("\nError: File Id is invalid.");
    else if((fileId > 1) && (!manFile->m_fileOpen[fileId]))
        printf("\nError: Cannot open file to write.");
    else if(manFile->m_fileType[fileId] == READ_ONLY)
        printf("Error: Cannot write to file in read only mode.");
    else
    {
        if(fileId == CONSOLE_OUTPUT)
            byteWrite = manFile->writeToConsole(virtAddr, charcount);
        else 
            byteWrite = manFile->writeToFile(virtAddr, charcount, fileId);
    }

    return byteWrite;
}

int doSC_Seek(int pos, int fileId)
{
    const int ERROR = -1;

    // Check if id is out of range of the file id (0->9) 
    if(!manFile->checkFileId(fileId))
    {
        DEBUG('f', "\nError: File Id is invalid.");
        printf("\nError: FileId is invalid.");
    }
    // Check if fileId is marked (file is opened)
    else if (!manFile->Test(fileId))
    {
        DEBUG(dbgFile, "\nError: Cannot seek because file is not opened.");
        printf("\nError: Cannot seek because the file is not opened.");
    }
    // Check if the Seek function is called on Console mode
    else if (fileId == 0 || fileId == 1)
    {
        DEBUG(dbgFile, "\nError: Cannot seek on Console.");
        printf("\nError: Can not seek on Console.");
    }
    else
    {
        OpenFile* file = manFile->m_fileOpen[fileId];
        int fileLength = file->Length();

        // if pos == -1 return length of file
        pos = (pos == - 1) ? fileLength : pos;

        if (pos < 0 || pos > fileLength)
        {
            DEBUG(dbgAddr, "\nError: Seek position is invalid.");
            printf("\nError: Seek position is invalid. FileLen: %d", fileLength);
        }
        else
            manFile->m_fileOpen[fileId]->Seek(pos);

        return pos;
    }
    return ERROR;
}

int doSC_Close(int fileId)
{
    int result = -1;

    if(manFile->checkFileId(fileId) && manFile->m_fileOpen[fileId])
        result = (manFile->closeFile(fileId)) ? fileId : -1; 

    return result;
}

int doSC_Delete(char* filename)
{
    if(!filename)
    {
        DEBUG(dbgAddr, "\nError: Not enough memory in system.");
        return -1;
    }

    if(fileSystem->Open(filename) == NULL)
        return -1;

    int fileId = manFile->checkOpening(filename);
    if(fileId != -1)
        manFile->closeFile(fileId);
    fileSystem->Remove(filename);
    
    return 0;
}

int doSC_Exec(char*procName)
{
    int processId = -1;
    if(!procName)
    {
        DEBUG(dbgAddr, "\nError: Not enough memory in system.");
    }
    else if(strlen(procName) <= 0)
    {
        DEBUG(dbgFile, "\nError: Process name is invalid.");
    }
    else
    {
        OpenFile* file = fileSystem->Open(procName);
        if(!file)
        {
            DEBUG(dbgFile, "\nError: Cannot open this file to execute.");
            printf("\nError: Cannot open this file to execute.");
        }
        delete file;

        processId = manProc->doExec(procName);
        if(processId < 0)
        {
            DEBUG(dbgAddr, "\nError: Cannot add new process.");
            printf("\nError: Cannot add new process.");
        }
    }
    return processId;   
} 

