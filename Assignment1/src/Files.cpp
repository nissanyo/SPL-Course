using namespace std;
#include <string>
#include "../include/Files.h"
#include <iostream>
#include <algorithm>
#include "../include/GlobalVariables.h"


BaseFile::BaseFile(string name_val) : name(name_val)  {}

string BaseFile::getName() const
{
    return name;
}

void BaseFile::setName(string newName)
{
    name=newName;
}

BaseFile::~BaseFile(){}


//================================================================================================
File::File(string name_val, int size_val) :  BaseFile(name_val), size(size_val) {}



int File::getSize()
{
    return size;
}

bool File::isFile() {
    return true;
}

File::~File(){}


//================================================================================================

Directory::Directory(string name_val, Directory *parent_val)  : BaseFile(name_val), children(), parent(parent_val) {
}


//  copy constructor
Directory::Directory(Directory &d) : BaseFile(d.getName()), children(), parent(d.parent)
{
    if((verbose==1)|(verbose==3))
        cout << "Directory::Directory(Directory &d)" << endl;

    for(int i=0; i< signed (d.children.size()); i++){
        if(d.children[i]->isFile())
            children.push_back(new File((d.children[i])->getName(),(d.children[i])->getSize()));
        else {
            children.push_back(new Directory(* ((Directory*)d.children[i])));
        }
    }
}


//  destructor
Directory::~Directory(){

    if((verbose==1)|(verbose==3))
        cout << "Directory::~Directory()" << endl;

    this->clear();

}

//  copy assignment operator
Directory& Directory::operator=(const Directory &d)
{
    if((verbose==1)|(verbose==3))
        cout << "Directory& Directory::operator=(const Directory &d)" << endl;

    if(this==&d)
        return *this;

    clear();

    for (int i=0; i< signed (d.children.size()); i++){

        if(d.children[i]->isFile())
            children.push_back(new File(d.children[i]->getName(),d.children[i]->getSize()));
        else
            children.push_back(new Directory (*(Directory*)d.children[i]));
    }

    parent = d.getParent();
    return *this;
}

void Directory::clear(){

    for (int i=0; i< signed (children.size()); i++)
        delete(children[i]);
    parent = nullptr;
    children.clear();
}


// move constructor
Directory::Directory(Directory &&d) : BaseFile(d.getName()), children(), parent(){

    if((verbose==1)|(verbose==3))
        cout << "Directory::Directory(Directory &&d)" << endl;

    children = d.children;
    parent = d.parent;

    d.clear();
    this->parent->addFile(this);

    //d.children = nullptr;

}


//  move assignment operator
Directory& Directory::operator=(Directory &&d){
    if((verbose==1)|(verbose==3))
        cout << "Directory& Directory::operator=(Directory &&d)" << endl;

    if(this==&d)
        return *this;

    clear();

    children=d.children;
    parent=d.parent;

    d.clear();
    this->parent->addFile(this);

    return *this;
}


Directory* Directory::getParent() const
{
    return parent;
}


void Directory::setParent(Directory *newParent)
{
    parent=newParent;
}

bool Directory::isFile()
{
    return false;
}

void Directory::addFile(BaseFile* file) {
    children.push_back(file);
    sortByName();
}

void Directory::removeFile(string name_val)
{
    for (size_t i = 0; i < children.size(); ++i)
        if (children[i]->getName()==name_val)
        {
            if(!(children[i]->isFile()))
                ((Directory*)children[i])->setParent(nullptr);
            delete(children[i]);
            children.erase(children.begin()+i);
            return;

        }
}

void Directory::removeFile(BaseFile* file)
{
    for (size_t i = 0; i < children.size(); ++i)
        if (children[i]==file)
        {
            if(!(file->isFile()))
                ((Directory*)children[i])->setParent(nullptr);
            delete(children[i]);
            children.erase(children.begin()+i);
            return;
        }

}


void Directory::sortByName()
{
    sort(children.begin(), children.end(), compareName);
}

void Directory::sortBySize() {
    sort(children.begin(), children.end(), compareSize);
}

bool Directory::compareName(BaseFile *b1, BaseFile *b2){
    return (b1->getName())<(b2->getName());
}

bool Directory::compareSize(BaseFile *b1, BaseFile *b2)
{
    if(b1->getSize()>b2->getSize())
        return false;
    if(b2->getSize()>b1->getSize())
        return true;
    else
        return (b1->getName())<(b2->getName());
}



vector<BaseFile*> Directory::getChildren()
{
    return children;
}


int Directory::getSize()
{
    int dir_size=0;
    for (int i=0; i< signed (children.size()); i++)
        dir_size = dir_size + (children[i]->getSize());
    return dir_size;
}



string Directory::getAbsolutePath()
{
    string str=getName();

    if(parent != nullptr)
    {
        if(parent->parent!= nullptr)
            str = parent->getAbsolutePath() + "/" + str;
        else
            str = parent->getAbsolutePath() + str;
    }
    
    return str;
}