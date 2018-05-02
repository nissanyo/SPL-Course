using namespace std;
#include <iostream>
#include "../include/FileSystem.h"
#include "../include/GlobalVariables.h"
#include "../include/Commands.h"



FileSystem::FileSystem() : rootDirectory(new Directory("/", nullptr)), workingDirectory(rootDirectory) {}


//  Copy constructor
FileSystem::FileSystem(FileSystem &fs) : rootDirectory(new Directory(*fs.rootDirectory)), workingDirectory(rootDirectory) 
{
    if((verbose==1)|(verbose==3))
        cout << "FileSystem::FileSystem(FileSystem &fs)" << endl;

    string path=fs.workingDirectory->getAbsolutePath();
    CdCommand *cd = new CdCommand (path);
    cd->execute(*this);
}


//  Destructor
FileSystem::~FileSystem(){

    if((verbose==1)|(verbose==3))
        cout << "FileSystem::~FileSystem()" << endl;

    delete(rootDirectory); 
    rootDirectory=nullptr;
    workingDirectory=nullptr;
}


//  Copy assignment operator
FileSystem &FileSystem::operator=(const FileSystem &fs){

    if((verbose==1)|(verbose==3))
        cout << "FileSystem& FileSystem::operator=(const FileSystem &fs)" << endl;

    if(this == &fs)
        return *this;

    clear();

    rootDirectory = new Directory(*fs.rootDirectory);
    workingDirectory = rootDirectory;

    string path = (fs.workingDirectory)->getAbsolutePath();
    CdCommand *cd = new CdCommand (path);
    cd->execute(*this);

    return *this;
}

void FileSystem::clear(){

    delete(rootDirectory);
    rootDirectory= nullptr;
    workingDirectory=nullptr;
}

//  Move constructor
FileSystem::FileSystem(FileSystem &&fs) : rootDirectory(fs.rootDirectory), workingDirectory(fs.workingDirectory)
{
    if((verbose==1)|(verbose==3))
        cout << "FileSystem::FileSystem(FileSystem &&fs)" << endl;

    fs.rootDirectory=nullptr;     
    fs.workingDirectory=nullptr;   
}

//  Move assignment operator
FileSystem& FileSystem::operator=(FileSystem &&fs) {

    if((verbose==1)|(verbose==3))
        cout << "FileSystem& FileSystem::operator=(FileSystem &&fs)" << endl;

    if(this==&fs)
        return *this;

    clear();

    rootDirectory = fs.rootDirectory;
    workingDirectory = fs.workingDirectory;

    fs.rootDirectory=nullptr;       
    fs.workingDirectory=nullptr; 

    return *this;
}

Directory& FileSystem::getRootDirectory() const
{
    return *rootDirectory;
}


Directory& FileSystem::getWorkingDirectory() const
{
    return *workingDirectory;
}


void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory)
{
    workingDirectory = newWorkingDirectory;
}