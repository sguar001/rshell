#include "RShell.h"
// #include "Token.h"
#include "TokenTest.h"
#include "Command.h"
#include <string>
#include <iostream>

using namespace std;

int main()
{
    while(true)
    {
        string command = "";
        cout << "$ ";                         //Start of Command prompt
        getline(cin,command);                //Takes in users command line
        TokenTest* A = new TokenTest();             // creates Token for parsing
        A->Parse(command);                  //Passes in command to be parsed
        
    }    
  return 0;  
}