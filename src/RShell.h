#ifndef RSHELL_H
#define RSHELL_H
#include <string>
#include <unistd.h>
#include <cstring>

using namespace std;

class RShell
{
    public:
        RShell(){};
        
        virtual void execute() = 0;
        virtual void print() = 0;
        // virtual void connector(int count, RShell*) =0;
        virtual void connector() = 0;
        virtual string getString() = 0;
        //virtual void setString() = 0;
        //virtual bool isSuccess() = 0;
        //virtual void setBool(bool) = 0; 
        
    public:
        RShell* left;
        RShell* right;
        RShell* root;
        bool success;
};
#endif




