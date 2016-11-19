#ifndef RSHELL_H
#define RSHELL_H
#include <string>
#include <unistd.h>
#include <cstring>

using namespace std;

class RShell // abstracts base class
{
    public:
        RShell(){};
        
        virtual int execute() = 0;
        virtual void print() = 0;
        virtual int connector(int &count) = 0;
        virtual string getString() = 0;

    public:
        RShell* left;
        RShell* right;
        RShell* root;
        bool success;
};
#endif




