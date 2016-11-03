#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <iostream>
#include "RShell.h"
//#include "Token.h"

class Token;
class TokenTest;
using namespace std;

class Command : public RShell
{
    public:
        Command();
        Command(string c);
        void print();
        void execute();
        vector <Command*> commandVec;
        string getString();
        //void setString();
        // void connector(int count, RShell*);
        void connector();

    private:
        string commandString;
        bool pass;
};
#endif