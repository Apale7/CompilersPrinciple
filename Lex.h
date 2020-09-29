//
// Created by Apale on 2020/9/29.
//

#ifndef COMPILERSPRINCIPLE_LEX_H
#define COMPILERSPRINCIPLE_LEX_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cassert>

using std::string, std::vector, std::unordered_map, std::unordered_set;

class Lex
{
    struct NFA
    {
        vector<unordered_map<char, vector<int>>> G;
        int start, end;

        NFA()
        {
            G.resize(1);
            start = end = 0;
        }

        size_t size() const
        {
            return G.size();
        }

        unordered_map<char, vector<int>> &operator[](int n)
        {
            assert(n < G.size());
            return G[n];
        }

        NFA &operator+=(NFA b)
        {
            int offset = size();
            for (int i = 0; i < b.size(); ++i)
            {
                G.push_back(b[i]);
                for (auto &k: G.back())
                    for (auto &v: k.second)
                        v += offset;
            }
            return *this;
        }
    };

    const unordered_set<char> charSet;
    string reg;
public:
    NFA nfa;
    Lex(string reg, string charset);

    Lex(string charset);

    void setReg(const string &reg);

    void preprocess();

    static inline int statusCode(char a);

    void buildNFA();
    void showFA();
};


#endif //COMPILERSPRINCIPLE_LEX_H
