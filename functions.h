#include <iostream>
#include <string>
#include <stack>
using namespace std;

int dec(char c)
{
    switch (c)
    {
        case 'A':
        return 10;
        case 'B':
        return 11;
        case 'C':
        return 12;
        case 'D':
        return 13;
        case 'E':
        return 14;
        case 'F':
        return 15;
        default:
        return stoi(to_string(c))-48;   //ascii value to magnitude
    }
}

int hextodec(string hex)
{
    int len = hex.length();
    int base = 1;
    int dec_num = 0;
    for (int i = len-1; i>=0; i--)
    {
        dec_num += dec(hex[i])*base;
        base = base*16;
    }
    return dec_num;
}
/*
int main()
{
    cout << hextodec("4D2") << endl;
    return 0;
}
*/
string hex(int d)
{
    switch (d)
    {
        case 10:
        return "A";
        case 11:
        return "B";
        case 12:
        return "C";
        case 13:
        return "D";
        case 14:
        return "E";
        case 15:
        return "F";
        default:
        return to_string(d);
    }
}

string dectohex(int dec_num)
{
    string hex_str = "";
    stack<int> stack;

    int div, rem;
    div = dec_num;
    rem = div % 16;
    stack.push(rem);
    div = (div)/16;
    while(div > 15)
    {
        rem = div % 16;
        stack.push(rem);
        div = (div)/16;
    }
    stack.push(div);
    while(!stack.empty())
    {
        
        hex_str = hex_str + hex(stack.top());
        stack.pop();
    }
    return hex_str;

}

vector<string> split(string str, char del)
{
      vector<string> v;
      string temp = "";
   
      for(int i=0; i<str.size(); i++)
        {
            if(str[i] != del)
            {
                temp += str[i];
            }
            else
            {
                v.push_back(temp);
                temp = "";
            }
        }
       
      v.push_back(temp);
      return v;
}


