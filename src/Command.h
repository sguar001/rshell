#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <iostream>
#include "RShell.h"
#include "TokenTest.h"

using namespace std;

class TokenTest;

class Command : public RShell
{
    public:
        Command();
        Command(string c);
        void print();
        int execute();
        vector <Command*> commandVec;
        string getString();
        bool isTest(vector<string> &v);
        void checkInput(int &flag, string &path, vector<string> &v);
        int connector(int &count);

    private:
        string commandString;
        bool pass;
};
#endif