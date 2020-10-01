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
#include <map>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <fstream>

using std::string, std::vector, std::unordered_map, std::unordered_set, std::set, std::map;

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
        set<int> stats;
        int id;

        bool operator<(const DStat &d) const
        {
            return stats < d.stats;
        }

        bool operator==(const DStat &d) const
        {
            return stats == d.stats;
        }
    };

    struct DFA
    {
        map<int, map<char, int>> G;
        int start;
        unordered_set<int> end;
        bool match(const string &s,char (*type)(char c))
        {
            int now = 0;
            for (auto &i: s)
            {
                char c = type(i);
                if (G[now].count(c))
                    now = G[now][c];
                else
                    return false;
            }
            return end.count(now);
        }

    };


    unordered_set<char> charSet;
    set<char> inputSet;
    string reg;

    DStat e_closure(int s);

    DStat e_closure(DStat T);

    DStat move(const DStat &, char);

public:
    NFA nfa;
    DFA dfa;

    Lex(string reg, string inputset);

    Lex(string charset);

    void setReg(const string &reg);

    void preprocess();

    inline char statusCode(char a);

    void buildNFA();

    void buildDFA();

    void showNFA();

    void printToFile(DFA dfa, const string &filename);
};


#endif //COMPILERSPRINCIPLE_LEX_H
