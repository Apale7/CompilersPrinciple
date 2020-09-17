//
// Created by Apale on 2020/9/13.
//

#include "Preprocessor.h"

Preprocessor::Preprocessor(const string &filePath)
{
    sourceFile.open(filePath, ios::in);
}

string &&Preprocessor::pretreatment()
{
    auto *result = new string;
    char c0, c1 = '$';
    bool inComment = false;
    while (c0 = c1, ((c1 = sourceFile.get()) != EOF))
    {
        if (!inComment)
        {
            if (c0 == '/' && c1 == '*')
            {
                result->pop_back();
                inComment = true;
                continue;
            }
            else if (c0 == '/' && c1 == '/')
            {
                result->pop_back();
                while (c1 = sourceFile.get(), c1 != EOF && c1 != '\n');
            }
            else
            {
                if (c0 == '\\' && c1 == '\n')
                    result->pop_back();
                else
                {
                    if (c1 == '\t' || c1 == '\n')
                        c1 = ' ';
                    result->push_back(c1);
                }
            }
        }
        else
        {
            if (c0 == '*' && c1 == '/')
                inComment = false;
        }
    }
    result->push_back('#');
    return std::move(*result);
}
