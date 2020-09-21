//
// Created by apale on 2020/9/21.
//

#ifndef COMPILERSPRINCIPLE_LEXICALANALYZER_H
#define COMPILERSPRINCIPLE_LEXICALANALYZER_H
#include "Preprocessor.h"

struct codeVal
{
    char code;
    string *val;

    codeVal();
    codeVal &operator=(const codeVal &cv);
    ~codeVal();
};

class LexicalAnalyzer
{
    static unordered_map<string, char> mp;
    static unordered_set<char> charSet;
public:
    codeVal scanner(const string &code, int &i);
};


#endif //COMPILERSPRINCIPLE_LEXICALANALYZER_H
