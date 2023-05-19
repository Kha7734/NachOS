#include "managefile.h"

// Manage files by 4 arrays: 
//      + BitMap: Manage closing and opening files, find next slot.
//      + m_fileOpen: Pointer to opening file.
//      + m_fileName: Name of file opening.
//      + m_fileType: Opening type of file.
ManageFile::ManageFile() : BitMap(LIMIT_FILE)
{
    this->Mark(0);
    this->Mark(1);
    m_fileName[0] = strdup("ConsoleInput");
    m_fileName[1] = strdup("ConsoleOutput");
    m_fileType[0] = READ_WRITE;
    m_fileType[1] = READ_WRITE;

    // Initialize fileOpen and fileName arrays
    for(int i = 0; i < LIMIT_FILE; i++)
    {   
        // Avoid Console IO files
        if(i > 1)
        {
            m_fileOpen[i] = NULL;
            m_fileName[i] = new char[LIMIT_FILE_NAME + 1];
            m_fileName[i][0] = '\0';
        }
    }
}

ManageFile::~ManageFile()
{    
    for(int i = 0; i < LIMIT_FILE; i++)
    {
        if(m_fileOpen[i])
        {
            delete m_fileOpen[i];
            m_fileOpen[i] = NULL;
        }
        delete m_fileName[i];
        m_fileName[i] = NULL;
    }
    delete[] m_fileName;
    delete[] m_fileOpen;
    delete[] m_fileType;
}

// Check for validity of file type
bool ManageFile::checkType(int type)
{
    if(type < 0 || type >= 2)
    {
        printf("\nError: Unexpected file type: %d.", type);
        return false;
    }
    return true;
}

// Find empty slot for open new file
int ManageFile::checkFreeSlot()
{
    int freeSlot = this->Find();    // Find and set the empty slot
    if(freeSlot == -1)
    {
        printf("\nError: Out of free slot");
        return -1;
    }
    return freeSlot;
}

// Check for validity of fileId
bool ManageFile::checkFileId(int fileId)
{
    if(fileId < 0 || fileId > 9)
    {
        printf("\nError: FileId is out of range.");
        return false;
    }
    return true;
}

// If file is opening, return the index
int ManageFile::checkOpening(char* filename)
{
    for(int i = 0; i < LIMIT_FILE; i++)
    {
        if(m_fileName[i])
        {
            if(strcmp(filename, m_fileName[i]) == 0)
                return i;
        }
    }
    return -1;
}

// Close file if index is valid
bool ManageFile::closeFile(int index)
{
    if(!checkFileId(index))
        return false;
    else
    {
        printf("\nClosing FileID: %d.", index);
        this->Clear(index);
        // Including Close(fileId) in delete
        delete m_fileOpen[index];
        m_fileOpen[index] = NULL; 
        delete m_fileName[index];
        m_fileName[index] = NULL;
    }
    // machine->WriteRegister(2, 0);
    return true;
}

// Read data from console and return the number of bytes read
int ManageFile::readFromConsole(int virtAddr,int charcount, int fileId)
{
    char *buffer = new char[charcount + 1];
    int byteRead = 0;

    byteRead = gSynchConsole->Read(buffer, charcount);
    
    if(byteRead != -1)
    {
        buffer[byteRead] = '\0';
        machine->System2User(virtAddr, byteRead, buffer);
    }
    // machine->WriteRegister(2, byteRead);
    delete[] buffer;
    return byteRead;
}

// Read data from file and return the number of bytes read
int ManageFile::readFromFile(int virtAddr,int charcount, int fileId)
{
    char *buffer;
    buffer = machine->User2System(virtAddr, charcount);
    OpenFile* file = m_fileOpen[fileId];
    
    if(!file)
    {
        DEBUG(dbgFile, "\nError: Cannot open file to read. DEBUG");
        // machine->WriteRegister(2, -1);
        printf("\nError: cannot open file to read.");

        delete[] buffer;
        return -1;
    }

    int byteRead = file->Read(buffer, charcount);

    buffer[byteRead] = '\0';
    machine->System2User(virtAddr, byteRead, buffer);
    
    // Check whether read to the end of file
    if(file->GetCurrentPos() == file->Length())
        byteRead = -2;
    
    delete[] buffer;
    return byteRead;
}

// Write data to console, return the number of bytes written
int ManageFile::writeToConsole(int virtAddr, int charcount)
{
    char *buffer;
    int i = 0, byteWrite = -1;

    buffer = machine->User2System(virtAddr, charcount);
    if(!buffer)
    {
        DEBUG(dbgFile, "\nError: Cannot write to console.");
        printf("\nError: Cannot write to console.");
    }
    else
    {
        DEBUG(dbgFile, "\nWriting to file.");
        byteWrite = gSynchConsole->Write(buffer, charcount);

    }

    delete[] buffer;
    return byteWrite;
}

// Write data to file and return the number of bytes written
int ManageFile::writeToFile(int virtAddr, int charcount, int fileId)
{
    char* buffer;
    OpenFile* file;
    int byteWrite = -1;

    // Check if file type is read only, return error.
    if(m_fileType[fileId] == READ_ONLY)
    {
        DEBUG(dbgFile, "\nError: Cannot write to file read only.");
        printf("\nError: Cannot write to file read only.");
    }
    else
    {
        file = m_fileOpen[fileId];
        buffer = machine->User2System(virtAddr, charcount);

        byteWrite = file->Write(buffer, charcount);
        if(byteWrite > 0)
            DEBUG(dbgFile, "\nWriting to file.");
        else
        {
            DEBUG(dbgFile, "\nError: Cannot write to file.");
            printf("\nError: Cannot write to file.");
        }
    }

    delete[] buffer;
    return byteWrite;
}
