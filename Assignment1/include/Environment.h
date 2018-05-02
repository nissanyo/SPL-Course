#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

using namespace std;
#include "Files.h"
#include "Commands.h"
#include <string>
#include <vector>


class Environment {
private:
    vector<BaseCommand *> commandsHistory;
    FileSystem fs;

public:
    Environment();
    Environment(Environment &env); // copy constructor
    ~Environment(); // destructor
    Environment& operator=(const Environment &env); // copy assignment operator
    Environment(Environment &&env);  // move constructor
    Environment&operator=(Environment &&env);  //move assignment operator

    void start();
    FileSystem& getFileSystem(); // Get a reference to the file system
    void addToHistory(BaseCommand *command); // Add a new command to the history
    const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
    void clear();
};


#endif