//
//  eval.cpp
//  Homework2
//
//  Created by Vivianne Dinh on 2/2/20.
//  Copyright © 2020 Vivianne Dinh. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool correctSyntax(string infix);
bool valuesInMap(const Map& values, string infix);
void inToPost(string infix, string& postfix);
bool precedence(char f, char s);
bool isOperator(char c);
bool isOperand(char c);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    // delete spaces in infix
    string noSpace;
    for (int i = 0; i < infix.length(); i++)
        if (infix[i] != ' ')
            noSpace += infix[i];
    infix = noSpace;
    
    if (!correctSyntax(infix))
        return 1;
    
    postfix = "";
    inToPost(infix, postfix);
    if (!valuesInMap(values, infix))
        return 2;
    
    // evaluate postfix
    stack<int> numStack;
    for (int i = 0; i < postfix.length(); i++) {
        if (isOperand(postfix[i])) {
            int val;
            values.get(postfix[i], val);
            numStack.push(val);
        }
        else {
            int v2 = numStack.top();
            numStack.pop();
            int v1 = numStack.top();
            numStack.pop();
            switch (postfix[i]) {
                case '-':
                    numStack.push(v1-v2);
                    break;
                case '+': {
                    numStack.push(v1+v2);
                    break;
                }
                case '*':
                    numStack.push(v1*v2);
                    break;
                case '/': {
                    if (v2 == 0)
                        return 3;
                    else
                        numStack.push(v1/v2);
                    break;
                }
            }
        }
    }
    result = numStack.top();
    return 0;
}

int main()
{
   char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
   int  vals[] = {  3,  -9,   6,   2,   4,   1  };
   Map m;
   for (int k = 0; vars[k] != '#'; k++)
       m.insert(vars[k], vals[k]);
   string pf;
   int answer;
   assert(evaluate("a+ e", m, pf, answer) == 0  && pf == "ae+"  &&  answer == -6);
   answer = 999;
   assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
   assert(evaluate("a*b", m, pf, answer) == 2  && pf == "ab*"  &&  answer == 999);
   assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  && pf == "yoau-*+"  &&  answer == -1);
   answer = 999;
   assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                           pf == "oyy-/"  &&  answer == 999);
   assert(evaluate(" a  ", m, pf, answer) == 0  &&
                           pf == "a"  &&  answer == 3);
   assert(evaluate("((a))", m, pf, answer) == 0  &&
                           pf == "a"  &&  answer == 3);
   cout << "Passed all tests" << endl;
}

bool isOperand(char c)
{
    return (isalpha(c) && islower(c));
}

bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool isParenthesis(char c)
{
    return (c == '(' || c == ')');
}

bool correctSyntax(const string infix)
{
    // check if infix is empty
    if (infix == "" || isOperator(infix[0]))
        return false;
    
    int numOperator = 0;
    int numOpenPar = 0;
    int numClosePar = 0;
    int numOperand = 0;
    string parenthesis;
    
    // count operators, operands, and parenthesis (closed and opened)
    for (int i = 0; i < infix.length(); i++)
        if (isOperator(infix[i]))
            numOperator++;
        else if (isOperand(infix[i]))
            numOperand++;
        else if (infix[i] == '(') {
            numOpenPar++;
            parenthesis += '(';
        }
        else if (infix[i] == ')') {
            numClosePar++;
            parenthesis += ')';
        }
        else
            return false;
    
    // check for incorrect order of elements in infix
    for (int i = 0; i < infix.size()-1; i++) {
        if (isOperand(infix[i]) && isOperand(infix[i+1]))
            return false;
        if (isOperator(infix[i]) && isOperator(infix[i+1]))
            return false;
        if (infix[i] == '(' && isOperator(infix[i+1]))
            return false;
        if (infix[i] == '(' && infix[i+1] == ')')
            return false;
    }
    
    if (numClosePar != numOpenPar || numOperator != numOperand-1 || numOperand == 0)
        return false;
    if (parenthesis != "" && (parenthesis[0] != '(' || parenthesis[parenthesis.length()-1] != ')'))
        return false;
    
    return true;
}

// check if values in infix are in map
bool valuesInMap(const Map& values, string infix)
{
    for (int i = 0; i < infix.length(); i++)
        if (isOperand(infix[i]) && !values.contains(infix[i]))
            return false;
        
    return true;
}

// turn infix to postfix
void inToPost(const string infix, string& postfix)
{
    postfix = "";
    stack<char> operatorStack;
    
    for (int i = 0; i < infix.length(); i++) {
        if (isOperand(infix[i]))
            postfix += infix[i];
        else if (infix[i] == '(')
            operatorStack.push(infix[i]);
        else if (infix[i] == ')') {
            while (operatorStack.top() != '(') {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();
        }
        else if (isOperator(infix[i]) && operatorStack.empty())
            operatorStack.push(infix[i]);
        else if (isOperator(infix[i]) && !operatorStack.empty()) {
            while (!operatorStack.empty() && operatorStack.top() != '(' && precedence(operatorStack.top(), infix[i])) {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
        }
    }
    while (!operatorStack.empty()) {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
}

// check if first operator has higher precedence than the second
bool precedence(char f, char s)
{
    if ((f == '+' || f == '-') && (s == '*' || s == '/'))
        return false;
    else
        return true;
}

