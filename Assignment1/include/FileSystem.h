#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
    Directory* rootDirectory;
    Directory* workingDirectory;
public:
    FileSystem();
    FileSystem(FileSystem &fs); // copy constructor
    ~FileSystem(); // destructor
    FileSystem& operator=(const FileSystem &fs);  //copy assignment operator
    FileSystem(FileSystem &&fs);    // move constructor
    FileSystem& operator=(FileSystem &&fs);    // move assignment operator


    Directory& getRootDirectory() const; // Return reference to the root directory
    Directory& getWorkingDirectory() const; // Return reference to the working directory
    void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system
    void clear();

};


#endif
