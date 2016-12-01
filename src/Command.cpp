#include "Command.h"
#include <iostream>
#include <boost/tokenizer.hpp>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace boost;


Command::Command()
{
    commandString = "";
}

Command::Command(string c)
{
    commandString = c;
}

string Command::getString()
{
    return this->commandString;
}

void Command::print()
{
    //cout << this->commandString;
}

int Command::execute()
{
    string command = this->getString();
    string test;
    
    for(string::iterator beg = command.begin(); beg != command.end(); beg++)
    {                          // searches for precendence as a child then 
        if( (*beg) == '(' )   
        {
            for(string::iterator ending = beg + 1; ending != command.end(); ending++)
            {
                if((*ending) == ')')
                {
                    while(*beg != *ending)
                    {
                        test += *beg;
                        beg++;
                    }
                    test += *ending;
                    TokenTest* A = new TokenTest();  // parses without semicolon
                    test = A->PrecedenceParse(test); // and executes
                    bool result = A->ParseA(test); 
                    this->success = result; //SET PROPER RETURN VALUE
                    return 0;
                }
            }
            
        }
        
    }
    
    if(command == "exit" || command == " exit" || command == "exit ") //exits prompt
    {
        exit(0);
    }
    
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep(" ",";||&&", drop_empty_tokens);
    tokenizer tok(command, sep);
    vector<char*> vec;
    vector<string> testVec;
    for(tokenizer::iterator itr = tok.begin(); itr != tok.end(); itr++)
    {
        string temp = static_cast<string>(*itr);
        testVec.push_back(temp);
        vec.push_back((char*)temp.c_str());
    }
    vec.push_back(NULL);
    
    char** commands = &vec[0];
    
    if(testVec.at(0) == "test")    // checks for test commands
    {
        this->success = isTest(testVec);
        return 0;
    }
    if(testVec.at(0) == "[" && testVec.at(testVec.size() - 1) == "]")
    {                                   // checks for [] 
        this->success = isTest(testVec);
        return 0;
    }
    if(testVec.at(0) == "cd")    // checks for test commands
    {
        // cout << "this is a cd command" << endl;
        this->success = isCd(testVec);
        return 0;
    }
    pid_t pid = -1;
    int child_status;      
    this->success = true;
    pid = fork();       // run fork and execute the child and parent proccesses
    
    if(pid == 0)
    {

        int result = execvp(commands[0], commands); // call execvp on command

        if( result == -1)
        {
            this->success = false;
            exit(1);
        }    
         return 0;
    }
    else 
    {    
        pid_t newPid;
        int exstat;
        do
        {
            newPid = wait(&child_status);
            exstat = WEXITSTATUS(child_status);
            if(newPid != pid) kill(newPid, SIGTERM);
        }while(newPid != pid);
        if(exstat == 1)
        {
            this->success = false;
        }
        if(exstat == 2)
        {
            this->success = false;
        }
    }
   
    return 1;
}


int Command::connector(int &count)
{
    return 0;
}

bool Command::isTest(vector<string> &v) // executes text commands
{

    struct stat status;
    int flag = 0; //sets -e initially incase no flag is found
    string path = "";
    
    checkInput(flag, path, v); // checks vector for the flag and path name
    char * cpath = (char*)path.c_str();
    
    
    if(stat(cpath, &status) == 0)             // if it euals zero file/path exists 
    {                                         
        //file/directory exists                
        if(flag == 0)                         // -e or no flag enter
        {
            cout << "(TRUE)" << endl;
            return true;
        }
        if(flag == 1) // -d                   // check for -d
        {
            // cout << " Its -d" << endl;
            if(S_ISDIR(status.st_mode))
            {
                cout << "(TRUE)" << endl;
                return true;
            }
            else
            {
                cout << "(FALSE)" << endl;
                return false;
            }
        }
        else if(flag == 2) // -f              //check for -f
        {
            // cout << " Its -f" << endl;
            if(S_ISREG(status.st_mode))
            {
                cout << "(TRUE)" << endl;
                return true;
            }
            else
            {
                cout << "(FALSE)" << endl;
                return false;
            }
        }
    }    
    else
    {
        //file/directory doesn't exist
        cout << "(FALSE)" << endl;
        return false;
    }
    return false;
}

void Command::checkInput(int &flag, string &path, vector<string> &v)
{
    flag = 0;
    for(unsigned i = 1; i < v.size(); i++)
    {
        if(v.at(i) == "-e")
        {
            flag = 0;
        }
        else if(v.at(i) == "-d")
        {
            flag = 1;
        }
        else if(v.at(i) == "-f")
        {
            flag = 2;
        }
        else//set path if no argument found
        {
            path = v.at(i);
            return;
        }
    }
}

bool Command::isCd(vector<string> &v)
{
    // cout << "I am now in cd" << endl;
    // cout << "current vector size: " << v.size() << endl;
    
    char* thePath;
    thePath = getenv("PWD");
    
    if(thePath == NULL)
    {
        return false; //path doesn't exist
    }
    
    if(v.size() > 1 && v.at(1) == "-") //handle cd - command
    {
        char* oldPath = getenv("PWD");
        char* oldEnv = (char*)"OLDPWD";
        
        char* currPath = getenv("OLDPWD");
        if(currPath == NULL)
        {
            // cout << "IT'S NULL" << endl; 
            return false;
        }
        char* envName = (char*)"PWD";
        setenv(envName, currPath, -1); //change PWD environemnt variable to value in OLDPWD, overwrite if already exists
        // cout << "current PWD: " << getenv("PWD") << endl;

        setenv(oldEnv, oldPath, -1);// change OLDPWD environment variable to value in PWD, overwrite if already exists
        // cout << "after setenv OLDPWD: " << getenv("OLDPWD") << endl;

        
        int cdResult = chdir(currPath); //change directory
        if(cdResult == -1)
        {
            return false;
        }
        return true;
    }
    if(v.size() == 1) // handle cd command
    {
        // cout << "current OLDPWD: " << getenv("OLDPWD") << endl;
        // cout << "current PWD: " << getenv("PWD") << endl;
        
        char* homePath;
        
        char* oldPath = getenv("PWD");
        char* oldEnv = (char*)"OLDPWD";
        setenv(oldEnv, oldPath, -1); //change OLDPWD enviroment variable to value in PWD, overwrite if already exists
        // cout << "current OLDPWD: " << getenv("OLDPWD") << endl;
        
        homePath = getenv("HOME");
        // cout << "home env: " << homePath << endl;
        char* envName = (char*)"PWD";
        setenv(envName, homePath, -1); //change PWD enviroment variable to homePath, overwrite if already exists
        // cout << "after setenv PWD: " << getenv("PWD") << endl;
        // cout << "after setenv OLDPWD: " << getenv("OLDPWD") << endl;


        int cdResult = chdir(homePath); //change directory 
        
        // char* arr2 = get_current_dir_name(); //allocates memory, must delete afterwards - for testing 
        
        // cout << "prev path: " << getenv("OLDPWD") << endl;
        // cout << "new path: " << arr2 << endl;
        if(cdResult == -1)
        {
            return false;
        }
        return true;
    }
    if(v.size() > 1 && v.at(1) != "-") // handle cd <PATH> command 
    {
        string tempPath = v.at(1);
        char* newPath = (char*)tempPath.c_str(); //variable for <PATH> passed in 
        
        int cdResult = chdir(newPath); //change directory 
        // cout << "cdResult: " << cdResult << endl;
        if(cdResult == -1)
        {
            return false;
        }
        
        char* oldPath = getenv("PWD");
        char* oldEnv = (char*)"OLDPWD";
        setenv(oldEnv, oldPath, -1); //change OLDPWD enviroment variable to value in PWD, overwrite if already exists
        // cout << "result1: " << result1 << endl;
        
        char* envName = (char*)"PWD";
        setenv(envName, newPath, -1); //change PWD enviroment variable to newPath, overwrite if already exists
        // int cdResult2 = chdir(newcwd);
        // cout << "cdResult2: " << cdResult2 << endl;
        
        // char* arrl = get_current_dir_name(); //allocates memory, must delete afterwards - for testing 
        
        // cout << "prev path: " << getenv("OLDPWD") << endl;
        // cout << "new path: " << newcwd << endl;
        
        return true;
    }
    return true;
}

string TokenTest::PrecedenceParse(string c)
{
    // parses the string for parenthesis 
    string command = c;
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("()",";||&&", drop_empty_tokens);
    tokenizer tok(command, sep);
    string newParse = "";
    
    for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); beg++)
    {
        newParse += *beg;
    }
    return newParse;
}
