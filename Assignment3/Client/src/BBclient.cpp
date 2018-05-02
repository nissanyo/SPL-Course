#include <stdlib.h>
#include <iostream>
#include <boost/thread.hpp>
#include "../include/ConnectionHandler.h"
#include <boost/algorithm/string.hpp>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
using namespace boost;
using namespace std;
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

bool toSignOut;

class ReadFromServer{
private: ConnectionHandler *connectionHandler;

public: ReadFromServer(ConnectionHandler *ch): connectionHandler(ch){};
    void run(){
        while(1){
            std::string answer;
            if (!connectionHandler->getLine(answer)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
            int len=answer.length();
            answer.resize(len-1);
            std::cout << answer << std::endl ;
            if (answer == "ACK signout succeeded") {
                toSignOut = true;
                std::cout<<"Ready to exit. Press enter\n";
                break;
            }
        }
    };
};

class ReadFromKeyboard {

private: ConnectionHandler *connectionHandler;


public: ReadFromKeyboard(ConnectionHandler *ch): connectionHandler(ch){};
    void run() {
        while (1) {
            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            if (!connectionHandler->sendLine(line)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
            if (toSignOut)
                break;
        }
    };
};



int main(int argc, char *argv[]) {



        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
            return -1;
        }
        std::string host = argv[1];
        short port = atoi(argv[2]);



        ConnectionHandler connectionHandler(host, port);
        if (!connectionHandler.connect()) {
            std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
            return 1;
        }

	    toSignOut = false;
        ReadFromServer server(&connectionHandler);
        boost::thread serverThread(&ReadFromServer::run, &server);

        ReadFromKeyboard keyboard(&connectionHandler);
        boost::thread keyboardThread(&ReadFromKeyboard::run, &keyboard);

        keyboardThread.join();
        serverThread.join();
        return 0;
}
