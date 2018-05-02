#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include "FileSystem.h"



class BaseCommand {
private:
    string args;

public:
    BaseCommand(string args);

    string getArgs();

    virtual void execute(FileSystem &fs) = 0;
    virtual string toString() = 0;

    vector<string> getDirs(string str);
    bool search(string str, FileSystem &fs);
    BaseFile* place(string str,FileSystem &fs);
    virtual BaseCommand* clone()=0;
    virtual ~BaseCommand();

};

class PwdCommand : public BaseCommand {
private:
public:
    PwdCommand(string args);
    void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
    virtual string toString();
    virtual PwdCommand* clone();
};

class CdCommand : public BaseCommand {
private:
public:
    CdCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual CdCommand* clone();
};

class LsCommand : public BaseCommand {
private:
public:
    LsCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual LsCommand* clone();
};

class MkdirCommand : public BaseCommand {
private:
public:
    MkdirCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual MkdirCommand* clone();
};

class MkfileCommand : public BaseCommand {
private:
public:
    MkfileCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual MkfileCommand* clone();
};

class CpCommand : public BaseCommand {
private:
public:
    CpCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual CpCommand* clone();

};

class MvCommand : public BaseCommand {
private:
public:
    MvCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual MvCommand* clone();
};

class RenameCommand : public BaseCommand {
private:
public:
    RenameCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual RenameCommand* clone();
};


class RmCommand : public BaseCommand {
private:
public:
    RmCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual RmCommand* clone();

};

class HistoryCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    HistoryCommand(string args, const vector<BaseCommand *> & history);
    void execute(FileSystem & fs);
    string toString();
    virtual HistoryCommand* clone();

};


class VerboseCommand : public BaseCommand {
private:
public:
    VerboseCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual VerboseCommand* clone();

};


class ErrorCommand : public BaseCommand {
private:
public:
    ErrorCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    ErrorCommand* clone();

};


class ExecCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    ExecCommand(string args, const vector<BaseCommand *> & history);
    void execute(FileSystem & fs);
    string toString();
    virtual ExecCommand* clone();

};




#endif