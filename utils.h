//
// Created by Apale on 2020/9/29.
//

#ifndef COMPILERSPRINCIPLE_UTILS_H
#define COMPILERSPRINCIPLE_UTILS_H

#include <string>
#include <stack>

using std::string, std::stack;

bool isOperator(char c)
{
    switch (c)
    {
        case '+':
        case '*':
        case '|':
        case '^':
            return true;
        default:
            return false;
    }
}


int getPriority(int c)
{
    int level = 0; // 优先级

    switch (c)
    {
        case '(':
            level = 1;
            break;
        case '|':
            level = 2;
            break;
        case '^':
            level = 3;
            break;
        case '*':
        case '+':
            level = 4;
            break;
        default:
            break;
    }
    return level;
}

string toSuffix(const string &expr)
{
    stack<char> op;
    string suffix;
    for (const auto &c: expr)
    {
        if (isOperator(c))
        {
            if (op.empty())
                op.emplace(c);
            else
            {
                while (!op.empty())
                {
                    int t = op.top();
                    if (getPriority(c) <= getPriority(t))
                    {
                        op.pop();
                        suffix.push_back(t);
                    }
                    else
                        break;
                }
                op.emplace(c);
            }
        }
        else
        {
            if (c == '(')
                op.emplace(c);
            else if (c == ')')
            {
                while (op.top() != '(')
                {
                    suffix.push_back(op.top());
                    op.pop();
                }
                op.pop();
            }
            else if (c != ' ')
                suffix.push_back(c);
        }
    }
    while (!op.empty())
    {
        suffix.push_back(op.top());
        op.pop();
    }
    return suffix;
}

#endif //COMPILERSPRINCIPLE_UTILS_H

