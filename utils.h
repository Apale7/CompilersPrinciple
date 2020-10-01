//
// Created by Apale on 2020/9/29.
//

#ifndef COMPILERSPRINCIPLE_UTILS_H
#define COMPILERSPRINCIPLE_UTILS_H

#include <string>
#include <stack>

using std::string, std::stack;

bool isOperator(char c)
{//判断是不是运算符
    switch (c)
    {
        case '*':
        case '|':
        case '^':
            return true;
        default:
            return false;
    }
}


int getPriority(int c)
{//运算符的优先级
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
        {//是运算符
            if (op.empty())//栈空，直接入栈
                op.emplace(c);
            else
            {//优先级更大的运算符全部出栈
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
            if (c == '(')//左括号直接入栈
                op.emplace(c);
            else if (c == ')')
            {//遇到右括号，一直出栈，直到遇到左括号
                while (op.top() != '(')
                {
                    suffix.push_back(op.top());
                    op.pop();
                }
                op.pop();
            }
            else
                suffix.push_back(c);//操作数直接放入表达式中
        }
    }
    while (!op.empty())
    {//取出剩余的运算符
        suffix.push_back(op.top());
        op.pop();
    }
    return suffix;
}

#endif //COMPILERSPRINCIPLE_UTILS_H

