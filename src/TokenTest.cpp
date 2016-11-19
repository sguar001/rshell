#include "TokenTest.h"
#include <iostream>
#include <boost/tokenizer.hpp>
#include <sys/types.h>
#include <sys/wait.h>


using namespace boost;
using namespace std;

TokenTest::TokenTest(string s)
{
    tk = s;                       
}

TokenTest::TokenTest(RShell* node)
{
    this->tk = this->getString();
}

bool TokenTest::Parse(string c) //here we are parsing the inputted string from 
{                               //the user
    string command = c;    
    string newCommand = command; //set string equal to another to search for #
    for(unsigned i = 0; i < command.size(); i++)
    {
        if(command[i] == '#') // check for # in string
        {
            newCommand = ""; // if found set string to empty
            for(unsigned j = 0; j < i - 1; j++)
            {
                newCommand += command[j]; //append the string till the #
            }
            break;  // leave once fully appended
        }
    }
    
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("",";||&&#", drop_empty_tokens);
    tokenizer tok(newCommand, sep);
    vector<string> commands;
    string newParse;
    // tokenize the string based on || &&     
    for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); beg++)
    {
        if( (*beg)[0] == '(' || (*beg)[1] == '(') 
        {
            for(tokenizer::iterator ending = beg; ending != tok.end(); ending++)
            {
                int size = (*ending).size();
                if( ((*ending)[size - 1] == ')') || ((*ending)[size - 2] == ')')
                || ((*ending)[0] == ')' ))
                {
                    while(*beg != *ending)
                    {
                        newParse += *beg; //(if () append back together 
                        beg++;            // to be entered as a single command
                    }
                    newParse += *ending;
                    break;
                }
            }
            commands.push_back(newParse); // push back the parse onto vector
            newParse = "";
        }
        else
        {
            commands.push_back(*beg); // if no () then just push it back
        }
        
    }
    int count = 0;
    
    tokenSearch(commands, count); // call search to look 

    bool result = tokenSort(commands); //sets the return results to token sort
    return result;
}

int TokenTest::Tree(vector<string> s, int totalToken) //set up the tree
{
    int value = 0;
    int count = totalToken;
    unsigned i = 0;
    TokenTest* T;
    for(i = 0; i < s.size(); i++)
    {

        if((s.at(i) == "&&" || s.at(i) == "||") && count >= 0) // if token
        { // sets up the bottom tree node
            T = new TokenTest(s.at(i));
            T->left = new Command(s.at(i - 1));
            T->right = new Command(s.at(i + 1));
            string test = T->getString();
            if(count == 1)
            {
                T->root = T;
                value = T->connector(count);
                return value;
            }
            Tree_Builder(s, count, T, i);
            return 0;
        }
    }
    return 0;
}  

void TokenTest::Tree_Builder(vector<string> s, int totalToken, TokenTest*& node,
int j)
{
    int count = totalToken;     
    int i = j + 1; // offset so it doesnt use same node
    TokenTest* T;
    for(unsigned k = i; k < s.size(); k++)
    {
        if(s.at(k) == "&&" || s.at(k) == "||")
        {                                      //sets up the rest of the tree
            T = new TokenTest(s.at(k));
            T->left = node; 
            
            T->right = new Command(s.at(k + 1));
           
            Tree_Builder(s, count, T, k);      // calls to keep building tree
            root = T;
            T->connector(count);    // calls to recursively go through tree 
            return;
        }
    }  
}

void TokenTest::tokenSearch(vector<string> &v, int &count)
{
   for(unsigned i = 0; i < v.size(); i++)
    {
        if(i + 1 != v.size())
        {
            if((v.at(i) == "&") && (v.at(i + 1) == "&")) // appends tokens 
            {                                            // and counts them
                v.at(i) = "&&";
                v.erase(v.begin()+(i + 1));
                count++;
            }
            else if((v.at(i) == "|") && (v.at(i + 1) == "|")) // appends tokens
            {                                                 // and counts them
                v.at(i) = "||";
                 v.erase(v.begin()+(i + 1));
                 count++;
            }
            
        }
    }
}

bool TokenTest::tokenSort(vector<string> &v)
{
    unsigned temp = v.size() - 1;
    unsigned count = 0;
    for(unsigned i = 0; i < v.size(); i++)
    {
        if(v.at(i) == "&&") // counts the token
        {
            count++;
        }
        else if (v.at(i) == "||") // counts the token
        {
            count++;
        }
        else if(v.at(i) == ";" && i ==  1) //if size is one and theres semicolom
        {
            vector<string> comman;         //execute a single command
            vector<string>::iterator it1;
            comman.push_back(v[0]);
          
            Command *C = new Command(v[0]);
            C->execute();                 // call execute on the command
            
            v.erase(v.begin());           // erase it
            v.erase(v.begin());           // erase it
          
            tokenSort(v);                 // call function again incase its
                                          // linked commands
        }
        else if(v.at(i) == ";" && count > 0) // semi colon and connectors
        {
          vector<string> command2;
          vector<string>::iterator it;
          int j = 0;
          for( it = v.begin();it < v.end(); it++, j++)  // make new vector
          {                                             // for all up to ;
                command2.push_back(*it);
          }
          vector<string>::iterator it1;
    
          Tree(command2, count);            // call the tree builder on command
          for(unsigned j = 0; j< command2.size() ; j++) //delete that vector
          {                                             
              command2.erase(command2.begin());
          }
          for(unsigned k = 0; k < i+1 ; k++)
          {
              v.erase(v.begin());  // delete actual vector up to the semi colon
          }
          
         count = 0;
       
        tokenSort(v);   // call token sort for the rest of the command
      }                            
      else if ( count > 0 && i == temp) //connectors and we are at size
      {                                 //(so theres no semi colon)
          vector<string> command2;
          vector<string>::iterator it;
          int j = 0;
          for( it = v.begin();it < v.end(); it++, j++) // push back command on
          {                                            // command 2
                command2.push_back(*it);
          }
           vector<string>::iterator it1;
    
          Tree(command2, count);                      // call to build tree
          
          for(unsigned j = 0; j< command2.size() ; j++)
          {
              command2.erase(command2.begin());       //delete command2
          }
          for(unsigned k = 0; k < i+1 ; k++)
          {
              v.erase(v.begin());    //delete v
          }
          
         count = 0;
         
        return true; //return true so keep looping command line dont exit
      }
      else if (v.size() == 1 && count == 0)   //single command no semi colon
      {
            string currentString = v.at(0);
            Command *C = new Command(currentString);
            C->execute();              //execute this command
            //exec(comman);
          
          vector<string> comman;
          vector<string>::iterator it1;
          comman.push_back(v[0]);
 
          v.erase(v.begin());
      }
    }
    
    if (v.size() == 0 && count == 0)  
    {
         return true;
    }
      return false;
}

void TokenTest:: exec(vector<string> &v)
{
    
    string command  = v[0];
    
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
    this->success = true;
    pid_t pid = fork();
    int status;

    if(pid < 0)
    {
        exit(1);
    }
    else if (pid == 0)
    {
        int result = execvp(commands[0], commands);
        if(result == -1)
        {
            this->success = false;
        }
        else
            {
                this->success = true;
            }
    }
    else
    {
        while(waitpid(pid, &status, 0) != pid);
    }
}

void TokenTest::print()
{
    if(this == 0)
    {
        return;
    }   
}

string TokenTest::getString()
{
    return tk;  // returns string
}
void TokenTest::getChild()
{
   this->left->print();
   this->right->print();
   
}

int TokenTest::execute()   //conditions based on connector for executing
{
   if(this->getString() == "||")  // if ||
   {
        if(this->left->getString() == "||" || this->left->getString() == "&&" )
        {                           // if the child is a token such as || or &&
            if(this->left->success == false)                  // if child false
            {
                this->right->execute();               // execute the right child
                if(this->right->success == true)         // if right child true
                {
                    this->success = true;    // the token node is evaluated true
                    return 1;    //return true
                }
                else
                {
                    this->success = false;          //token is evaluated false
                    return 0;                       // return false
                }
            }
            else
            {
                this->success = true;              // token evaluated true
                return 1;                         // return true
            }
        }
        else
        {
            this->left->execute();             // if the child isnt a token
            if(this->left->success == false) // go through same condition checks
            {
                this->right->execute();
                if(this->right->success == true)
                {
                    this->success = true;
                    return 1;
                    
                }
                else
                {
                    this->success = false;
                    return 0;
                }    
            }
            else
            {
                this->success = true;
                return 1;
            }
        }    
   }
   else if(this->getString() == "&&" )  // if tokens &&
   {
       if(this->left->getString() == "||" || this->left->getString() == "&&" )
        {             // if child is a node such as || or &&
            if(this->left->success == true)
            {
                this->right->execute();
                if(this->right->success == true)   // checks conditions
                {
                    this->success = true;
                    return 1;
                }
                else
                {
                    this->success = false;
                    return 0;
                }
            }
            else
            {
                this->success = false;
                return 0;
            }
        }
       else                             // checks conditions if child isnt 
       {                                // a token
            this->left->execute();
           if(this->left->success == false)
            {
                this->success = false;
                return 0;
                
            }
            else if (this->left->success == true)
            {
                this->right->execute();
                if(this->right->success == true)
                {
                    this->success = true;  
                    return 1;
                }
                else
                {
                    this->success = false;
                    return 0;
                }
            }
       }    
       
   }
   
    return 0;
}
int TokenTest::connector(int &count)  //recursively calls the tree
{
    int value = 0;
    if(count > 0)
    {
        count --;
        this->left->connector(count); // call connector till count is zero
    }                                 // means there are no more connectors left
    if(count == 0)
    {
        value = this->execute();    // now call execute
    }
    return value;
}








bool TokenTest::ParseA(string c)  // same as parse but catches precedence 
{                                 // when its already a command in the tree
    string command = c;
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("",";||&&", drop_empty_tokens);
    tokenizer tok(command, sep);
    vector<string> commands;
    string newParse;
    
    for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); beg++)
    {
        if( (*beg)[0] == '(' )           // checks for open parenthesis
        {
            for(tokenizer::iterator ending = beg; ending != tok.end(); ending++)
            {
                int size = (*ending).size();
                if( ((*ending)[size - 1] == ')') || ((*ending)[size - 2] == ')')
                || ((*ending)[0] == ')' ))  // checks for closed parathesis
                {
                    while(*beg != *ending)
                    {
                        newParse += *beg; // parses together without parenthesis
                        beg++;
                    }
                    newParse += *ending;
                }
            }
            commands.push_back(newParse);
        }
        else
        {
            commands.push_back(*beg);
        }
    }
    int count = 0;
    
    tokenSearch(commands, count); // calls search for connectors
    int value = Tree(commands, count); // calls to assemble tree
    bool result;
    if(value == 1)
    {
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}
