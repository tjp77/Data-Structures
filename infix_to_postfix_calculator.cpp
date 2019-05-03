#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include "stack.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::list;


void StringToList(string _string, list<string> &_list, char _delimiter)
{
    // Clear if there is already a held expression, so new can be entered.
    if (_list.size() != 0)
    { _list.erase(_list.begin(), _list.end()); }

    string tmp = "";

    for (int i = 0; i < _string.length(); ++i)
    {
        if (_string[i] != _delimiter)
        {
            tmp += _string[i];
        }
        else
        {
            _list.push_back(tmp);
            tmp = "";
        }
    }

}//


bool IsLowerPriority(string _newop, string _oldop)
{
    int newp = 2, oldp = 2;

    if (_newop == "+" || _newop == "-")
    { newp = 1; }

    if (_oldop == "+"  || _oldop == "-" || _oldop == "(")
    { oldp = 1; }

    return (newp < oldp ? true : false);

}//

double Operate(string _op1, string _op2, string _operator)
{
    double iop1, iop2;

    std::stringstream convt1(_op1);
    convt1 >> iop1;
    std::stringstream convt2(_op2);
    convt2 >> iop2;

    if (_operator == "+")
    {
        return iop1 + iop2;
    }
    else if (_operator == "-")
    {
        return iop1 - iop2;
    }
    else if (_operator == "*")
    {
        return iop1 * iop2;
    }
    else if (_operator == "/")
    {
        return iop1 / iop2;
    }

    return 0;

}//

void GetValue(string &_operand)
{
    cout << "Enter a value for variable " << _operand << "." << endl;
    getline(cin, _operand);
    cout << endl;
}//


// Takes a string list and an iterator pointing to where in the list to start from.
string InfixToPostfix(list<string> _inputList)
{
    stack opStack;
    string _expstr = "";

    for (list<string>::iterator _itr = _inputList.begin(); _itr != _inputList.end(); ++_itr)
    {
        if (*_itr == "-"
         || *_itr == "+"
         || *_itr == "*"
         || *_itr == "/")
        {
            while (opStack.top != NULL && IsLowerPriority(*_itr, opStack.top->value))
            {
                _expstr += opStack.top->value + " ";
                opStack.Pop();
            }

            opStack.Push(*_itr);
        }
        else if (*_itr == "(")
        {
            opStack.Push(*_itr);
        }
        else if (*_itr == ")")
        {
            while (opStack.top->value != "(")
            {
                _expstr += opStack.top->value + " ";
                opStack.Pop();
            }

            opStack.Pop();
        }
        else
        { _expstr += *_itr + " "; }

    }// for each token

    while (opStack.top != NULL)
    {
        _expstr += opStack.top->value + " ";
        opStack.Pop();
    }

    return _expstr;

}//


void ConvertExpression(string &_expstr, bool _testing = false)
{
    if (!_testing)
    {
        cout << "Enter an infix expression:" << endl << endl;
        getline(cin, _expstr);
    }

    cout << endl << _expstr << " ";

    list<string> inputList;

    // Separate into list of tokens and operators
    // to later be iterated through.
    StringToList(_expstr, inputList, ' ');

    _expstr = InfixToPostfix(inputList);

    cout << "in postfix form is: " << _expstr << endl << endl;

}//

void Evaluate(string &_expstr)
{
    if (_expstr == "")
    {
        cout << "No existing expression." << endl << endl;
        ConvertExpression(_expstr);
    }

    list<string> tokenList;
    stack operandStack;
    string op1, op2;

    StringToList(_expstr, tokenList, ' ');

    for (list<string>::iterator _itr = tokenList.begin(); _itr != tokenList.end(); ++_itr)
    {
        if (*_itr == "-"
         || *_itr == "+"
         || *_itr == "*"
         || *_itr == "/")
        {
            op2 = operandStack.top->value;
            operandStack.Pop();
            op1 = operandStack.top->value;
            operandStack.Pop();

            if (op2.find_first_not_of( "0123456789.-" ) != string::npos)
            { GetValue(op2); }

            if (op1.find_first_not_of( "0123456789.-" ) != string::npos)
            { GetValue(op1); }

            std::stringstream convt;
            convt << Operate(op1, op2, *_itr);

            operandStack.Push(convt.str());
        }
        else
        { operandStack.Push(*_itr); }

    }// for

    cout << "Evaluates to: " << operandStack.top->value << endl << endl;

}//

void Test()
{
    cout << "---TESTING---" << endl << endl;

    string expstr = "a + b * c + d #";
    ConvertExpression(expstr, true);

    expstr = "( a + b ) * c + d #";
    ConvertExpression(expstr, true);

    expstr = "( a - b ) * r + 10 / 2 #";
    ConvertExpression(expstr, true);

    expstr = "( 3 * 3 ) * 3 #";
    ConvertExpression(expstr, true);
    Evaluate(expstr);

    expstr = "4 * -6 #";
    ConvertExpression(expstr, true);
    Evaluate(expstr);

    cout << "---END TESTING---" << endl << endl;

}//


int main()
{
    Test();

    cout << "What would you like to do?" << endl << endl;

    int choice = 0;
    string expstr = "";

    while (true)
    {
        cout << "1) Convert an infix expression to postfix." << endl
             << "2) Evaluate expression." << endl
             << "3) Exit." << endl << endl;

        cin >> choice;
        cin.ignore();

        switch(choice)
        {
            case 1:

                ConvertExpression(expstr);

            break;

            case 2:

                Evaluate(expstr);

            break;

            case 3:

                return 0;

            break;

            default:

                cout << "Invalid choice." << endl << endl;
                choice = 0;
                continue;

            break;
        }

        choice = 0;

    }// End while

    return 0;

}//






