//
// Created by apale on 2020/9/21.
//

#include "LexicalAnalyzer.h"


codeVal::~codeVal()
{
    delete this->val;
}

codeVal::codeVal()
{
    code = 0;
    val = new string();
}

codeVal &codeVal::operator=(const codeVal &cv)
{
    if (this != &cv)
    {
        this->val = new string (*cv.val);
        this->code = cv.code;
    }
    return *this;
}



unordered_map<string, char> LexicalAnalyzer::mp = {
        {"begin",   '{'},
        {"end",     '}'},
        {"integer", 'a'},
        {"real",    'c'},
        {"BEGIN",   '{'},
        {"END",     '}'},
        {"INTEGER", 'a'},
        {"REAL",    'c'},
};
unordered_set<char> LexicalAnalyzer::charSet = {
        ',', ';', '(', ')', '=', '+', '*', '#'
};

codeVal LexicalAnalyzer::scanner(const string &code, int &i)
{
    codeVal t;
    string token;
    if (isalpha(code[i]))//标识符或基本字
    {
        while (isalnum(code[i]))
            token.push_back(code[i++]);
        if (!mp.count(token))
        {
            *t.val = token;
            t.code = 'i';
        }
        else
            t.code = mp[token];
    }
    else if (isdigit(code[i]))
    {
        while (isdigit(code[i]))
            token.push_back(code[i++]);
        if (code[i] == '.')
        {
            token.push_back(code[i++]);
            while (isdigit(code[i]))
                token.push_back(code[i++]);
            t.code = 'y';
        }
        else
            t.code = 'x';
        *t.val = token;
    }
    else if (code[i] == '.')//.开头的实数
    {
        token.push_back('.');
        ++i;
        if (isdigit(code[i]))
        {
            while (isdigit(code[i]))
                token.push_back(code[i++]);
            t.code = 'y';
            *t.val = token;
        }
        else
        {
            cout << "Error word->" << token << std::endl;
            exit(-1);
        }
    }
    else if (!charSet.count(code[i]))
    {
        cout << "Error word->" << token << std::endl;
        exit(-1);
    }
    else if (code[i] == '+' && code[i + 1] == '+')
    {
        t.code = '$';
        i += 2;
    }
    else
        t.code = code[i++];
    return t;
}
