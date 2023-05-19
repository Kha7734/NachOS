#ifndef MANAGEFILE_H
#define MANAGEFILE_H

#include "bitmap.h"
#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "string.h"

const char dbgFile = 'f';
const char dbgAddr = 'a';
const char dbgMach = 'm';


class ManageFile : public BitMap
{
public:
    int *m_fileType = new int[LIMIT_FILE]{0};
    char**m_fileName = new char*[LIMIT_FILE];
    OpenFile** m_fileOpen = new OpenFile*[LIMIT_FILE];
    ManageFile();
    ~ManageFile();

public:
    // Check open and close file
    int checkFreeSlot();
    bool checkType(int type);
    int checkOpening(char*filename);
    bool checkFileId(int fileId);
    bool closeFile(int index);

    // Read from file, console
    int readFromConsole(int virtAddr,int charcount, int fileId);
    int readFromFile(int virtAddr,int charcount, int fileId);
    
    // Write to file, console
    int writeToConsole(int virtAddr, int charcount);
    int writeToFile(int virtAddr, int charcount, int fileId);
};

#endif
