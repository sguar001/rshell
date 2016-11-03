#ifndef TOKENTEST_H
#define TOKENTEST_H

#include <string>
#include <vector>
#include <queue>
#include "RShell.h"
#include "Command.h"

class Command;

class TokenTest : public RShell
{
    public:
        TokenTest() {};
        TokenTest(string s);
        //TokenTest(bool passed);
        TokenTest(RShell* node);
        string getString();
        void getChild();
        void Parse(string c);
        void execute();
        void Tree(vector<string> s, int count);
        void Tree_Builder(vector<string> s, int count, TokenTest* &node, int i);
        void tokenSearch(vector<string> &v, int &count);
        void tokenSort(vector<string> &v);
        void print();
        void exec(vector<string>& v);
        void connector();
        //void setString();
        //void setBool();
        //bool isSuccess();
        
    private:
        string tk;
        string tkr;
        bool passed;
};
#endif