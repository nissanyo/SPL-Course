using namespace std;
#include <string>
#include <iostream>
#include "../include/Commands.h"
#include "../include/GlobalVariables.h"
#include "../include/Files.h"

//=================================================================
BaseCommand::BaseCommand(string args_val) : args(args_val) {}

string BaseCommand::getArgs() {return args;}

BaseFile* BaseCommand::place(string str, FileSystem &fs)
{
    if(str == "..")
        return fs.getWorkingDirectory().getParent();
    if(str == "")
        return &fs.getWorkingDirectory();
    else {
        int i = 0;
        while (fs.getWorkingDirectory().getChildren()[i]->getName() != str)
            i++;
        return fs.getWorkingDirectory().getChildren()[i];
    }
}

bool BaseCommand::search(string str, FileSystem &fs)
{
    bool found = true;

    if(str.length()!=0 && str.at(0)=='/') {
        fs.setWorkingDirectory(&fs.getRootDirectory());
        str = str.substr(1);
    }

    while(found && str.length()!=0)
    {
        while(str.length()!=0 && str.at(0)=='/')
            str=str.substr(1);

        if(str.length()>=2 && str.substr(0,2)=="..") {
            if(fs.getWorkingDirectory().getParent() != nullptr) {
                fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                str=str.substr(2);
            }
            else found = false;
        }
        else {
            if(str.length()!=0) {
                string s = "";
                while (str.length() != 0 && str.at(0) != '/') {
                    s += str.at(0);
                    str = str.substr(1);
                }
                bool dirOk = false;
                for (int i=0; (!dirOk) && i< signed (fs.getWorkingDirectory().getChildren().size()); i++) {
                    if (s == fs.getWorkingDirectory().getChildren()[i]->getName()) {
                        dirOk = true;
                        if(!fs.getWorkingDirectory().getChildren()[i]->isFile()) {
                            fs.setWorkingDirectory((Directory*) (fs.getWorkingDirectory().getChildren()[i]));
                        }
                        else {
                            while(str.length()!=0 && str.at(0)=='/')
                                str=str.substr(1,str.length()-1);
                            if (str.length() != 0)
                                found = false;
                        }
                    }
                }
                if(!dirOk)
                    found=false;
            }
        }
    }
    return found;
}


vector<string> BaseCommand::getDirs(string str)
{
    vector <string> dir;
    while(str.length()!=0)
    {
        string tempDir = "" ;

        while (str.length()!=0 && ((str.at(0) == '/') | (str.at(0)==' ')) )
            str = str.substr(1);

        while (str.length()!=0 && ((str.at(0)!= '/') & (str.at(0)!=' ')) )
        {
            tempDir += str.at(0);
            str = str.substr(1);
        }
        dir.push_back(tempDir);
    }
    return dir;
}

BaseCommand::~BaseCommand(){}
//=================================================================

PwdCommand::PwdCommand(string args_val) : BaseCommand(args_val) {}

void PwdCommand::execute(FileSystem & fs)
{
    cout << fs.getWorkingDirectory().getAbsolutePath() <<  endl;
}

string PwdCommand::toString()
{
    return "pwd";
}

PwdCommand* PwdCommand::clone(){
    return new PwdCommand(getArgs());
}

//=================================================================
CdCommand::CdCommand(string args) : BaseCommand(args){}

void CdCommand::execute(FileSystem & fs)
{
    string str = getArgs();
    Directory &temp = fs.getWorkingDirectory();

    if(! BaseCommand::search(str,fs)) {
        fs.setWorkingDirectory(&temp);
        cout << "The system cannot find the path specified" << endl;
    }
}

string CdCommand::toString()
{
    return "cd";
}

CdCommand* CdCommand::clone(){
    return new CdCommand(getArgs());
}

//=================================================================
LsCommand::LsCommand(string args) : BaseCommand (args){}

void LsCommand::execute(FileSystem &fs) {

    string str = getArgs();
    vector<string> dirs = BaseCommand::getDirs(str);
    Directory &temp = fs.getWorkingDirectory();
    bool bySize = false;

    while (str.length()!=0 && str.at(0) == ' ')
        str = str.substr(1);

    if(str.length()>=2 && str.substr(0,2)=="-s"){
        bySize = true;
        str=str.substr(2);
    }


    if (!BaseCommand::search(str, fs)) {
        cout << "The system cannot find the path specified" << endl;
    } else {

        if(bySize)
            fs.getWorkingDirectory().sortBySize();
        for (int i = 0; i < signed(fs.getWorkingDirectory().getChildren().size()); i++) {
            if (!fs.getWorkingDirectory().getChildren()[i]->isFile())
                cout << "DIR" <<"\t" << fs.getWorkingDirectory().getChildren()[i]->getName() << "\t"
                     << fs.getWorkingDirectory().getChildren()[i]->getSize() << endl;
            else
                cout << "FILE" <<"\t" << fs.getWorkingDirectory().getChildren()[i]->getName() << "\t"
                     << fs.getWorkingDirectory().getChildren()[i]->getSize() << endl;
        }
    }
    fs.setWorkingDirectory(&temp);
}

string LsCommand::toString() {
    return "ls";
}

LsCommand* LsCommand::clone(){
    return new LsCommand(getArgs());
}

//=================================================================
MkdirCommand::MkdirCommand(string args) : BaseCommand (args){}

void MkdirCommand::execute(FileSystem & fs)
{

    string str = getArgs();
    vector <string> dirs = BaseCommand::getDirs(str);
    bool exists = true;
    bool isLegal = true;
    Directory &temp = fs.getWorkingDirectory();

    if(str.at(0) == '/'){
        fs.setWorkingDirectory(&fs.getRootDirectory());
    }
    while((str.at(0) == ' ')|(str.at(0)=='/'))
        str=str.substr(1);

    for(int i=0; isLegal && i<signed(dirs.size()); i++) {
        if (dirs[i] == "..") {
            if (fs.getWorkingDirectory().getParent() != nullptr) {
                fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
            } else {
                cout << "The system cannot find the path specified" << endl;
                isLegal = false;
            }
        }
        else {
            Directory &tmp = fs.getWorkingDirectory();
            if(BaseCommand::search(dirs[i],fs))
            {
                fs.setWorkingDirectory(&tmp);
                if (place(dirs[i], fs)->isFile()) {
                    cout << "The directory already exists" << endl;
                    fs.setWorkingDirectory(&temp);
                    fs.getWorkingDirectory().sortByName();

                    return;
                } else
                    fs.setWorkingDirectory( (Directory*) place(dirs[i], fs));
            }
            else
            {
                BaseCommand::search(dirs[i],fs);
                Directory *d = new Directory(dirs[i], &fs.getWorkingDirectory());
                fs.getWorkingDirectory().addFile(d);
                fs.setWorkingDirectory(d);
                exists = false;
            }
        }
    }
    fs.setWorkingDirectory(&temp);
    fs.getWorkingDirectory().sortByName();

    if(isLegal & exists)
        cout << "The directory already exists" << endl;
}

string MkdirCommand::toString()
{
    return "mkdir";
}

MkdirCommand* MkdirCommand::clone(){
    return new MkdirCommand(getArgs());
}

//==================================================================

MkfileCommand::MkfileCommand(string args) : BaseCommand (args){}

void MkfileCommand::execute(FileSystem & fs)
{

    string str = getArgs();
    vector <string> dirs = BaseCommand::getDirs(str);
    bool isLegal = true;
    Directory &temp = fs.getWorkingDirectory();

    if(str.at(0) == '/'){
        fs.setWorkingDirectory(&fs.getRootDirectory());
    }

    for(int i=0; isLegal && i<signed(dirs.size()); i++) {
        if (dirs[i] == "..") {
            if (fs.getWorkingDirectory().getParent() != nullptr) {
                fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
            } else {
                cout << "The system cannot find the path specified" << endl;
                isLegal = false;
            }
        }
        else {
            if (dirs.size() - i > 2 && !BaseCommand::search(dirs[i], fs)) {
                cout << "The system cannot find the path specified" << endl;
                isLegal = false;
            }
            else {
                if (signed(dirs.size())- 2 == i) {
                    if (!BaseCommand::search(dirs[i], fs)) {
                        File *f1 = new File(dirs[i], stoi(dirs[i + 1]));
                        fs.getWorkingDirectory().addFile(f1);
                        isLegal = false;
                    } else
                        cout << "File already exists" << endl;
                }
            }
        }

    }
    fs.setWorkingDirectory(&temp);
    fs.getWorkingDirectory().sortByName();
}

string MkfileCommand::toString()
{
    return "mkfile";
}

MkfileCommand* MkfileCommand::clone(){
    return new MkfileCommand(getArgs());
}

//==================================================================

CpCommand::CpCommand (string args) : BaseCommand (args){}

void CpCommand::execute(FileSystem & fs) {

    string str = getArgs();
    Directory &original = fs.getWorkingDirectory();

    int i=0;
    while(str.at(i)!=' ')
        i++;

    string sPath=str.substr(0,i);
    string sDest=str.substr(i+1);
    vector <string> path = getDirs(sPath);
    vector <string> dest = getDirs(sDest);

    if(BaseCommand::search(sDest,fs)) {

        Directory &d = fs.getWorkingDirectory();
        fs.setWorkingDirectory(&original);

        if (!BaseCommand::search(sPath, fs))
            cout << "No such file or directory" << endl;
        else {
            fs.setWorkingDirectory(&original);

            for (int i=0; i<signed(path.size()-1); i++)
            {
                BaseCommand::search(path[i],fs);
            }

            BaseFile *f = place(path[path.size()-1], fs);
            fs.setWorkingDirectory(&original);

            BaseCommand::search(sDest,fs);

            if(!BaseCommand::search(f->getName(),fs)){
                if (f->isFile())
                    d.addFile(new File(f->getName(),f->getSize()));
                else {
                    d.addFile(new Directory(*(Directory*)f)); 
                }
                fs.setWorkingDirectory(&original);
                fs.getWorkingDirectory().sortByName();
            }
            //else
            //  cout << "File/Director named '" << f->getName() << "' is already exist in" <<d.getName()<< endl;
        }
    }
    else
        cout << "No such file or directory" << endl;

    fs.setWorkingDirectory(&original);
}

string CpCommand::toString(){
    return "cp";
}

CpCommand* CpCommand::clone(){
    return new CpCommand(getArgs());
}

//==================================================================

MvCommand::MvCommand(string args) : BaseCommand(args) {}

void MvCommand::execute(FileSystem & fs) {

    string str = getArgs();
    Directory &original = fs.getWorkingDirectory();

    int j=0;
    while(str.length()!=0 && str.at(j)!=' ')
        j++;

    string sPath=str.substr(0,j);
    string sDest=str.substr(j+1);
    vector <string> path = getDirs(sPath);
    vector <string> dest = getDirs(sDest);

    if(BaseCommand::search(sDest,fs)) {

        Directory &d = fs.getWorkingDirectory();
        fs.setWorkingDirectory(&original);

        if (!BaseCommand::search(sPath, fs))
            cout << "No such file or directory" << endl;
        else {
            fs.setWorkingDirectory(&original);

            if(sPath.at(0) == '/')
                fs.setWorkingDirectory(&fs.getRootDirectory());

            for (int i = 0; i <signed( (path.size() - 1)) ; i++) {
                BaseCommand::search(path[i], fs);
            }

            BaseFile *f = place(path[path.size() - 1], fs);
            Directory &fParent = fs.getWorkingDirectory();
            fs.setWorkingDirectory(&original);

            bool legal= (f!=&fs.getRootDirectory());  
            while (legal && &fs.getWorkingDirectory()!= &fs.getRootDirectory())
            {
                if(f == &fs.getWorkingDirectory()) 
                    legal = false;
                else
                    fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
            }

            if(legal) {
                fs.setWorkingDirectory(&original);
                BaseCommand::search(sDest, fs);

                if (!BaseCommand::search(f->getName(), fs)) {
                    if (f->isFile()) {
                        File *toMove = new File(f->getName(), f->getSize());
                        d.addFile(toMove);
                    }
                    else {
                        Directory *toMove = new Directory(*((Directory*)f));
                        toMove->setParent(&d);
                        d.addFile(toMove);
                    }

                    fParent.removeFile(f);

                } //else
                //cout << "File/Director named '" << f->getName() << "' is already exist in directory " << d.getName() << endl;
            } else
                cout << "Can't move directory" << endl;
        }
    }
    else
        cout << "No such file or directory" << endl;

    fs.setWorkingDirectory(&original);
    fs.getWorkingDirectory().sortByName();
}

string MvCommand::toString()
{
    return "mv";
}

MvCommand* MvCommand::clone(){
    return new MvCommand(getArgs());
}

//==================================================================

RenameCommand::RenameCommand(string args_val) : BaseCommand(args_val) {}

void RenameCommand::execute(FileSystem & fs)
{

    Directory *original = &fs.getWorkingDirectory();
    string str = getArgs();

    int space=0;
    while(str.length()!=0 && str.at(space)!=' ')
        space++;

    string path=str.substr(0, space);
    string name=str.substr(space+1);

    if(!BaseCommand::search(path, fs))
        cout << "No such file or directory" << endl;
    else{
        fs.setWorkingDirectory(original);
        vector<string> dirs = getDirs(path);

        for (int i = 0; i < signed( dirs.size()-1); i++) {
            BaseCommand::search(dirs[i], fs);
        }
        BaseFile *f = place(dirs[dirs.size() - 1], fs);

        if(original == f)
            cout << "Can't rename the working directory" << endl;
        else{
           if(!BaseCommand::search(name, fs))
                f->setName(name);
        }
    }
    fs.setWorkingDirectory(original);
    fs.getWorkingDirectory().sortByName();
}

string RenameCommand::toString()
{
    return "rename";
}

RenameCommand* RenameCommand::clone(){
    return new RenameCommand(getArgs());
}

//==================================================================
RmCommand::RmCommand(string args) : BaseCommand(args){}

void RmCommand::execute(FileSystem & fs) {

    string str = getArgs();
    Directory *original = &fs.getWorkingDirectory();
    vector <string> path = getDirs(str);

    if (!BaseCommand::search(str, fs))
        cout << "No such file or directory" << endl;
    else {
        
        fs.setWorkingDirectory(original);
        
        if(str.length()!=0 && str.at(0)=='/')
            fs.setWorkingDirectory(&fs.getRootDirectory());

        
        for (int i = 0; i < signed(path.size()-1); i++) {
            BaseCommand::search(path[i], fs);
        }

        BaseFile *f = place(path[path.size() - 1], fs);
        if((f == original)|(f==&fs.getRootDirectory()))
        {
            cout << "Can't remove directory" << endl;
            fs.setWorkingDirectory(original);
            fs.getWorkingDirectory().sortByName();
            return;
        }
        Directory &fParent = fs.getWorkingDirectory();
        fs.setWorkingDirectory(original);

        if (f->isFile()) {
            fParent.removeFile(f);
        } else
        {
            bool legal = (f != &fs.getRootDirectory());  
            while (legal && fs.getWorkingDirectory().getParent() != nullptr) {
                if (f == &fs.getWorkingDirectory()) 
                    legal = false;
                else
                    fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
            }
            if (legal) {
                fParent.removeFile(f);
            } else
                cout << "Can't remove directory" << endl;
        }
    }

    fs.setWorkingDirectory(original);
    fs.getWorkingDirectory().sortByName();
}

string RmCommand::toString() {
    return "rm";
}

RmCommand* RmCommand::clone(){
    return new RmCommand(getArgs());
}

//==================================================================

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history) : BaseCommand(args), history(history){}

void HistoryCommand::execute(FileSystem &fs) {

    if(history.size() == 0)
        cout << "" << endl;
    else
        for (int i = 0; i < signed(history.size()); i++){
            {
            if(history[i]->toString() == "error")
                cout << i << "\t"  << history[i]->getArgs() << endl;
            else
                cout << i << "\t" << history[i]->toString()<< " " << history[i]->getArgs() << endl;
            }
            
        }
        
        
}

string HistoryCommand::toString() {
    return "history";
}

HistoryCommand* HistoryCommand::clone(){
    return new HistoryCommand(getArgs(), history);
}

//==================================================================

VerboseCommand::VerboseCommand(string args) : BaseCommand (args){}

void VerboseCommand::execute(FileSystem &fs) {
    
    verbose = stoi(getArgs());

    if((verbose!=0)&(verbose!=1)&(verbose!=2)&(verbose!=3))
        cout << "Wrong verbose input" << endl;


}

string VerboseCommand::toString() {
    return "verbose";
}

VerboseCommand* VerboseCommand::clone(){
    return new VerboseCommand(getArgs());
}


//==================================================================

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history){}

void ExecCommand::execute(FileSystem &fs) {

    string str = getArgs();
    int num = (stoi(str));
    if ((num > signed(history.size())) | (num <0))
        cout << "Command not found" << endl;
    else
        history[num]->execute(fs);
}
string ExecCommand::toString() {
    return "exec";
}

ExecCommand* ExecCommand::clone(){
    return new ExecCommand(getArgs(), history);
}


//==================================================================

ErrorCommand::ErrorCommand(string args) : BaseCommand(args){}

void ErrorCommand::execute(FileSystem &fs) {

    int space = (-1);
    string usr_cmd;
    string str = getArgs();

    for (int i = 0; ((space == (-1)) && (i < signed(str.length()))); i++)
        if (str.at(i) == ' ')
            space = i;

    if (space == (-1))
        usr_cmd = str;
    else {
        usr_cmd = str.substr(0, space); 


        cout << usr_cmd << ": Unknown command" << endl;
    }
}

string ErrorCommand::toString() {
    return "error";
}

ErrorCommand* ErrorCommand::clone(){
    return new ErrorCommand(getArgs());
}