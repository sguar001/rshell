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
        TokenTest(RShell* node);
        string getString();
        void getChild();
        bool Parse(string c);
        int execute();
        int Tree(vector<string> s, int count);
        void Tree_Builder(vector<string> s, int count, TokenTest* &node, int i);
        void tokenSearch(vector<string> &v, int &count);
        bool tokenSort(vector<string> &v);
        void print();
        void exec(vector<string>& v);
        int connector(int &count);
        string PrecedenceParse(string c);
        bool ParseA(string c);
        bool isTest(vector<string> &v);
        void checkInput(int &flag, string &path, vector<string> &v);
        
    private:
        string tk;
        string tkr;
        bool passed;
};
#endif