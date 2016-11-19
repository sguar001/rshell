#include "RShell.h"
#include "TokenTest.h"
#include "Command.h"
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    int counter = 0;
    bool result = true;
    while(result == true)
    {
    
        
        string command = "";
        cout << "$ ";                         //Start of Command prompt
        getline(cin,command);                 //Takes in users command line
        TokenTest* A = new TokenTest();       // creates Token for parsing
        result = A->Parse(command);           //Passes in command to be parsed
        
        counter++;
        if(result == false && counter > 1)
          {
              exit(0);
          }
    }
   
  return 0;  
}