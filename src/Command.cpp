#include "Command.h"
#include <iostream>
#include <boost/tokenizer.hpp>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;
using namespace boost;


Command::Command()
{
    commandString = "";
    ///pass = false;
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

void Command::execute()
{
    string command = this->getString();
    
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep(" ",";||&&", drop_empty_tokens);
    tokenizer tok(command, sep);
    vector<char*> vec;
    
    for(tokenizer::iterator itr = tok.begin(); itr != tok.end(); itr++)
    {
        string temp = static_cast<string>(*itr);
        vec.push_back((char*)temp.c_str());
    }
    vec.push_back(NULL);
    
    char** commands = &vec[0];
    //cout << "command: " << vec.at(0) << endl;
    this->success = true;
    pid_t pid = fork();
    int status;

    if(pid < 0)
    {
        //cout << "error" << endl;
        //cout << "EXITING" << endl;
        exit(1);
    }
    else if (pid == 0)
    {
        int result = execvp(commands[0], commands);
        if(result == -1)
        {
            //cout << "INPUTTED COMMAND IS FALSE " << endl;
            this->success = false;
        //cout << "after " << endl;
            
        }
        else
            {
                this->success = true;
                //cout << "after << endl";
            }
        //cout << "after 1 " << endl;
        
    }
    else
    {
        //cout << "after 3" << endl;
        while(waitpid(pid, &status, 0) != pid);
        //cout << "after 4" << endl;
    }
     //cout << "after 5" << endl;
//    return;
    //cout <<  "agfter6 " << this->success << endl;
}


void Command::connector()
{
    
}

// void Command::setString()
// {
//     this->commandString = "";
// }

// bool Command::isSuccess()
// {
//     if(this->pass)
//     {
//         return true;
//     }
//     return false;
// }

// void Command::setBool(bool b)
// {
//     this->pass = b;
// }