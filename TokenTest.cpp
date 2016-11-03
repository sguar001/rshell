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

void TokenTest::Parse(string c)
{
    string command = c;
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("",";||&&", drop_empty_tokens);
    tokenizer tok(command, sep);
    vector<string> commands;
    
    for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); beg++)
    {
        commands.push_back(*beg);
    }
    // cout << "size: " << commands.size() << endl;
    int count = 0;
    
    tokenSearch(commands, count);
    tokenSort(commands);
    return;
    //Tree(commands, count);
}

void TokenTest::Tree(vector<string> s, int totalToken)
{
    int count = totalToken;
    unsigned i = 0;
    TokenTest* T;
    for(i = 0; i < s.size(); i++)
    {

        if((s.at(i) == "&&" || s.at(i) == "||") && count >= 0)
        {
            //cout << "Node_" << i << endl;
            T = new TokenTest(s.at(i));
            T->left = new Command(s.at(i - 1));
            T->right = new Command(s.at(i + 1));
            string test = T->getString();
            //cout << "Node:" << test << endl;
            //cout << "leftChild, rightChild" << endl;
            //T->getChild();
            //cout << endl;
            if(count == 1)
            {
                // T->connector(count, T);
                T->root = T;
                T->connector();
            }
            // count--;
            Tree_Builder(s, count, T, i);
            return;
        }
    }
}  

void TokenTest::Tree_Builder(vector<string> s, int totalToken, TokenTest*& node, int j)
{
    // cout << "a" << endl;
    int count = totalToken;
    int i = j + 1; // offset so it doesnt use same node
    //cout << i;
    TokenTest* T;
    for(unsigned k = i; k < s.size(); k++)
    {
        //cout << "k's value" << k <<  endl;
        if(s.at(k) == "&&" || s.at(k) == "||")
        {
            T = new TokenTest(s.at(k));
            T->left = node; // need to update node pointer somehow??
            
            T->right = new Command(s.at(k + 1));
            
            //string test = T->getString();
            //cout << "Node: " << test << endl;
            //cout << "leftChild, rightChild" << endl;
            //T->getChild();
            //cout << endl;
            //count--;
            Tree_Builder(s, count, T, k);
            // T->connector(count, T);
            root = T;
            T->connector();
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
            if((v.at(i) == "&") && (v.at(i + 1) == "&"))
            {
                v.at(i) = "&&";
                v.erase(v.begin()+(i + 1));
                count++;
            }
            else if((v.at(i) == "|") && (v.at(i + 1) == "|"))
            {
                v.at(i) = "||";
                 v.erase(v.begin()+(i + 1));
                 count++;
            }
            else if (v.at(i) == "#")
            {
                vector<string>::iterator it;
                int j = 0;
                for( it = v.begin() + i;it < v.end(); it++, j++)
                {
                    v.erase(v.begin() + i);
                    
                }
                return;
            }
            
            
        }
    }
}

void TokenTest::tokenSort(vector<string> &v)
{
    
    
    int count = 0;
    for(unsigned i = 0; i < v.size(); i++)
    {
      //cout << "here" << endl;
      if(v.at(i) == "exit")
      {
          //cout << "here1" << endl;
          while(true)
          {
            exit(1);
          }
          
      }
      else if(v.at(i) == " exit")
      {
          //cout << "here2" << endl;
          while(true)
          {
            exit(1);
          }
      }
      else if(v.at(i) == "&&")
      {
          count ++;
      }
      else if(v.at(i) == " ")
      {
          return;
      }
      else if (v.at(i) == "||")
      {
          count++;
      }
      if(v.at(i) == ";" && i ==  1)
      {
          vector<string> comman;
          vector<string>::iterator it1;
          comman.push_back(v[0]);
          //cout << "I will execute here for single command" << endl;
          exec(comman);
          //cout << "here" << endl;
          //comman.erase(v.begin());
          //cout << "I have executed a single command" << endl;
          v.erase(v.begin());
          v.erase(v.begin());
          //cout << v.size() << endl;
          tokenSort(v);
          
        }
      else if(v.at(i) == ";" && count > 0)
      {
          vector<string> command2;
          vector<string>::iterator it;
          int j = 0;
          for( it = v.begin();it < v.end(); it++, j++)
          {
                //cout << "in vector building loop" << endl;
                command2.push_back(*it);
          }
           vector<string>::iterator it1;
    
          Tree(command2, count);
           //cout << "after tree" << endl;
         // cout <<"i's value" << i << endl;
          for(int j = 0; i< command2.size() ; j++)
          {
              //cout << "in vector destruction loop" << endl;
              command2.erase(command2.begin());
          }
          //cout <<"i's value2 " << i << endl;
          for(unsigned k = 0; k < i+1 ; k++)
          {
              //cout << "in  actual vector destruct loop" << endl;
              v.erase(v.begin());
          }
          
         count = 0;
        //cout <<"size " << v.size() << endl;   
        tokenSort(v);  
      }
    }
      if (v.size() == 0)
       {
           //cout << "exiting" << endl;
           return;
       }
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
    //cout << "command: " << vec.at(0) << endl;
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
        // cout << "zero" << endl;
        return;
    }   
    // cout << "token: " << this->getString() << endl; 

    //this->getChild();
}

string TokenTest::getString()
{
    //cout << "Node: " << tk << endl;
    return tk;
}
void TokenTest::getChild()
{
   this->left->print();
   //cout << ", ";
   this->right->print();
   
}

void TokenTest::execute()
{
   
    if(this->getString() == "||")
    {
        //cout << "A) " << endl;
        if(this->left->getString() != "&&" && this->left->getString() != "||")
        {
            this->left->execute();
             //cout <<" Status of: " << this->left->getString() << " " << this->left->success << endl;
                //cout <<" Status of: " << this->right->getString() << " " << this->right->success << endl;
            if(this->left->success == false)
            {
                //cout << "a)LEFT PIPE COMMAND FAILED" << endl;
                
                this->right->execute();
                //cout <<" Status of: " << this->left->getString() << " " << this->left->success << endl;
                //cout <<" Status of: " << this->right->getString() << " " << this->right->success << endl;
                if(this->right->success == true)
                {
                    //cout << "a)RIGHT PIPE COMMAND SUCCEDED" << endl;
                   
                    this->success = true;
    
                }
                else
                {   
                    //cout << "b)RIGHT PIPE COMMAND FAILED" << endl;
                    // cout << "+++++" << endl;
                    // this->right->setString();
                    this->success = false;
                    // return;
                }
            }
            else
            {
                //cout << "b)LEFT PIPE COMMAND SUCCEDED" << endl; 
                this->success = true;
                // cout << "this: " << this->getString() << endl;
                // cout << "this->success: " << this->success << endl;
                //return;
                // cout << "zzzzzzzz" << endl;
                // this->left->setString();
                // this->right->setString();
                // return;
            }
        }
        else
        {
            if(this->left->success == false)
            {
                //cout << "$$$$$$$$$$$$$$$" << endl;
                // this->left->setString();
                this->right->execute();
                 //cout <<" Status of: " << this->left->getString() << " " << this->left->success << endl;
                //cout <<" Status of: " << this->right->getString() << " " << this->right->success << endl;
                // cout << "VALUE: " << this->right->success << endl;
                // cout << "--------------------------------" << endl;
                if(this->right->success == true)
                {
                    // cout << "*************" << endl;
                    // this->right->setString();
                    this->success = true;
                    // return;
                }
                else
                {
                    // cout << "+++++" << endl;
                    // this->right->setString();
                    this->success = false;
                    // return;
                }
            }
            else
            {
                this->success = true;
                //cout << "this: " << this->getString() << endl;
                // //cout << "this->success: " << this->success << endl;
                //return;
                //cout << "zzzzzzzz" << endl;
                // this->left->setString();
                // this->right->setString();
                // return;
            }   
        }
    }
    else if (this->getString() == "&&")
    {
        //cout << "b) Entered the && string" << endl;
        if(this->left->getString() != "&&" && this->left->getString() != "||")
        {
            //cout << "b) enter the this->left->getString() != && && this->left->getString() != ||) string" << endl;
            this->left->execute();
             //cout <<" Status of: " << this->left->getString() << " " << this->left->success << endl;
                //cout <<" Status of: " << this->right->getString() << " " << this->right->success << endl;
            //cout << "b) this->left: " << this->left->getString() << endl;
            //cout << this->left->success << endl;
            if(this->left->success == true)
            {
                //this->left->setString();
                this->right->execute();
                 //cout <<" Status of: " << this->left->getString() << " " << this->left->success << endl;
                //cout <<" Status of: " << this->right->getString() << " " << this->right->success << endl;
                if(this->right->success == true)
                {
                    //this->right->setString();
                    // cout << "b) it was true set parent to true" << endl;
                    this->success = true;
                    // return;
                    
                }
                else
                {
                    //this->right->setString();
                    //cout << "b) it was false set parent to false" << endl;
                    this->success = false;
                    // return;
                }
            }
            else
            {
                //this->left->setString();
                //this->right->setString();
                //cout << "2b) it was false set parent to false" << endl;
                this->success = false;
                // return;
            }
        }
        else
        {
                // this->left->setString();
                this->right->execute();
                if(this->right->success == true && this->left->success == true)
                {
                    // this->right->setString();
                   //  cout << "2b) it was true set parent to true" << endl;
                     //cout << "its set to true" << endl;
                    this->success = true;
                    // return;
                }
                else
                {
                    // this->right->setString();
                    this->success = false;
                    // return;
                    //cout << "3b) it was false set parent to false" << endl;
                    //return;
                }
        
           
                // return; 
            }
         //cout <<" Status of: " << this->left->getString() << " " << this->left->success << endl;
         //cout <<" Status of: " << this->right->getString() << " " << this->right->success << endl;
    }
}


void TokenTest::connector()
{
    // cout << "BEGINNING" << endl;
    if(this->getString() == "&&" || this->getString() == "||")
    {
        // cout << "CCCCCCCCCCCCCCC" << endl;
        this->left->connector();
        // this->right->connector();
        // this->execute();
    }
    
    this->execute();
    
    //cout << "RETURNING FROM EXECUTE CALL" << endl;
    return;
}



// void TokenTest::setString()
// {
//     this->tk = "";
// }

// bool TokenTest::isSuccess()
// {
//     if(this->passed)
//     {
//         return true;
//     }
//     return false;    
// }

// void TokenTest::setBool(bool b)
// {
//     this->passed = b;
// }