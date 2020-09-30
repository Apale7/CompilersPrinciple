//
// Created by Apale on 2020/9/29.
//

#ifndef COMPILERSPRINCIPLE_LEX_H
#define COMPILERSPRINCIPLE_LEX_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>
#include <cassert>
#include <algorithm>

using std::string, std::vector, std::unordered_map, std::unordered_set, std::set;

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

    struct DStat
    {
        vector<int> stats;
        int id;

        bool operator<(const DStat &d) const
        {
            return stats < d.stats;
        }
    };

    struct DFA
    {
        vector<unordered_map<char, int>> G;
        int start, end;
    };


    unordered_set<char> charSet;
    unordered_set<char> inputSet;
    string reg;

    DStat e_closure(int s);
    DStat e_closure(DStat T);
    DStat move(const DStat&, char);
public:
    NFA nfa;

    Lex(string reg, string inputset);

    Lex(string charset);

    void setReg(const string &reg);

    void preprocess();

    static inline int statusCode(char a);

    void buildNFA();

    void buildDFA();

    void showNFA();
};


#endif //COMPILERSPRINCIPLE_LEX_H
