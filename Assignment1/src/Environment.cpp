#include <iostream>
#include "../include/Environment.h"
#include "../include/GlobalVariables.h"



Environment::Environment() : commandsHistory(), fs() {}


//  Copy constructor
Environment::Environment(Environment &env) : commandsHistory(), fs(FileSystem(env.fs))
{
    if((verbose==1)|(verbose==3))
        cout << "Environment::Environment(Environment &env)" << endl;

    for (int i=0; i< signed(env.getHistory().size()); i++)
    {
        BaseCommand *cmd = env.getHistory()[i]->clone();
        commandsHistory.push_back(cmd);
    }
}

//  Destructor
Environment::~Environment(){

    if((verbose==1)|(verbose==3))
        cout << "Environment::~Environment()" << endl;

    for (int i=0; i< signed(commandsHistory.size()); i++){
        delete(commandsHistory[i]);
    }
}

//  Copy assignment operator
Environment& Environment::operator=(const Environment &env) {
    if((verbose==1)|(verbose==3))
        cout << "Environment& Environment::operator=(const Environment &env)" << endl;

    if(this == &env)
        return *this;

    clear();

    for (int i=0; i< signed(env.getHistory().size()); i++)
    {
        BaseCommand *cmd = env.getHistory()[i]->clone();
        commandsHistory.push_back(cmd);
    }

    fs=env.fs;

    return *this;
}

void Environment::clear(){

    for (int i=0; i< signed(commandsHistory.size()); i++){
        delete(commandsHistory[i]);
        commandsHistory[i] = nullptr;
    }
}


//  Move constructor
Environment::Environment(Environment &&env) : commandsHistory(std::move(env.commandsHistory)), fs(env.fs){

    if((verbose==1)|(verbose==3))
        cout << "Environment Environment(Environment &&env)" << endl;
}


//  Move assignment operator
Environment& Environment::operator=(Environment &&env){

    if((verbose==1)|(verbose==3))
        cout << "Environment &Environment::operator=(Environment &&env)" << endl;

    if (this==&env)
        return *this;

    clear();

    commandsHistory = env.commandsHistory;
    fs = env.fs;

    return *this;
}



void Environment::start()
{
    bool exit = false;

    while(!exit) {

        string usr_string;
        string usr_cmd ="";
        string usr_arg;
        int space = (-1);
        BaseCommand *cmd;

        cout << fs.getWorkingDirectory().getAbsolutePath() + ">";
        getline(cin, usr_string);

        for (int i = 0; ((space == (-1)) && (i <  signed(usr_string.length()))) ; i++)
            if (usr_string.at(i) == ' ')
                space = i;

        if (space == (-1))
            usr_cmd = usr_string;
        else {
            usr_cmd = usr_string.substr(0, space);    
            usr_arg = usr_string.substr(space + 1);  
        }

        if (usr_cmd == "exit")
            exit = true;
        else {
            if (usr_cmd == "pwd")
                cmd = new PwdCommand(usr_arg);
            else if (usr_cmd == "cd")
                cmd = new CdCommand(usr_arg);
            else if (usr_cmd == "ls")
                cmd = new LsCommand(usr_arg);
            else if (usr_cmd == "mkdir")
                cmd = new MkdirCommand(usr_arg);
            else if (usr_cmd == "mkfile")
                cmd = new MkfileCommand(usr_arg);
            else if (usr_cmd == "cp")
                cmd = new CpCommand(usr_arg);
            else if (usr_cmd == "mv")
                cmd = new MvCommand(usr_arg);
            else if (usr_cmd == "rename")
                cmd = new RenameCommand(usr_arg);
            else if (usr_cmd == "rm")
                cmd = new RmCommand(usr_arg);
            else if (usr_cmd == "history")
                cmd = new HistoryCommand(usr_arg, getHistory());
            else if (usr_cmd == "verbose")
                cmd = new VerboseCommand(usr_arg);
            else if (usr_cmd == "exec")
                cmd = new ExecCommand(usr_arg, getHistory());
            else
                cmd = new ErrorCommand(usr_string);
            
            if ((verbose==2)|(verbose==3)){
                if((usr_arg == "") | (usr_arg==" "))
                    cout << usr_cmd << endl;
                else
                    cout << usr_cmd << " " << usr_arg << endl;
            }
            
            cmd->execute(fs);
            addToHistory(cmd);
        }
    }
}


FileSystem& Environment::getFileSystem()
{
    return fs;
}


void Environment::addToHistory(BaseCommand *command)
{
    commandsHistory.push_back(command);
}


const vector<BaseCommand*>& Environment::getHistory() const
{
    return commandsHistory;
}