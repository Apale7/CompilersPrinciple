//
// Created by Apale on 2020/9/29.
//

#include "Lex.h"
#include <cassert>
#include "utils.h"

Lex::Lex(string reg, string inputset) : reg(std::move(reg))
{
    charSet.insert(inputset.begin(), inputset.end());
    string op = "+()*|";
    charSet.insert(op.begin(), op.end());
    inputSet.insert(inputset.begin(), inputset.end());
}

void Lex::setReg(const string &reg)
{
    Lex::reg = reg;
}

void Lex::preprocess()
{
    assert(reg.size());
    int l = 0;
    for (const auto &item : reg)
    {
        assert(charSet.find(item) != charSet.end());
        if (item == '(') ++l;
        else if (item == ')') --l;
        assert(l >= 0);
    }
    assert(l == 0);
    for (int i = 0; i < reg.size() - 1; ++i)
    {
        int a = statusCode(reg[i]), b = statusCode(reg[i + 1]);
        if ((a == 'a' || a == 'd') && (b == 'a' || b == 'd') || (a == '*' && (b == 'a' || b == 'd')) ||
            ((a == 'a' || a == 'd') && b == '(') || (a == ')' && (b == 'a' || b == 'd')))
            reg.insert(i + 1, "^");
    }
    reg = toSuffix(reg);
    std::cout << reg << '\n';
}

Lex::Lex(string charset) : charSet(charset.begin(), charset.end())
{}

int Lex::statusCode(char a)
{
    if (isalpha(a))
        return 'a';
    if (isdigit(a))
        return 'd';
    return a;
}

void Lex::buildNFA()
{
    stack<NFA> stk;
    for (auto &c: reg)
    {
        if (!isOperator(c))
        {
            NFA a;
            a.G.resize(2);
            a[0][c] = {1};
            a.end = 1;
            stk.emplace(a);
        }
        else
        {
            switch (c)
            {
                case '|':
                {
                    NFA tmp;
                    NFA b = stk.top();
                    stk.pop();
                    NFA a = stk.top();
                    stk.pop();
                    size_t size1 = tmp.size();
                    tmp += a;
                    tmp[tmp.start]['e'].emplace_back(a.start + size1);

                    size_t size2 = tmp.size();
                    tmp += b;
                    tmp[tmp.start]['e'].emplace_back(b.start + size2);
                    tmp[a.end + size1]['e'] = {int(tmp.size())};
                    tmp[b.end + size2]['e'] = {int(tmp.size())};
                    tmp.end = tmp.size();
                    tmp.G.emplace_back(unordered_map<char, vector<int>>());
                    stk.emplace(tmp);
                    break;
                }
                case '*':
                {
                    NFA tmp;
                    NFA a = stk.top();
                    stk.pop();
                    size_t size1 = tmp.size();
                    tmp += a;
                    tmp[tmp.start]['e'].emplace_back(a.start + size1);
                    tmp[a.end + size1]['e'].emplace_back(a.start + size1);
                    size_t s = tmp.size();
                    tmp[a.end + size1]['e'].emplace_back(s);
                    tmp.end = s;
                    tmp.G.emplace_back(unordered_map<char, vector<int>>());
                    tmp[tmp.start]['e'].emplace_back(s);
                    stk.emplace(tmp);
                    break;
                }
                case '+':
                {
                    NFA a = stk.top();
                    NFA b = a, tmp;
                    stk.pop();
                    size_t size1 = tmp.size();
                    tmp += a;
                    tmp[tmp.start]['e'].emplace_back(a.start + size1);
                    tmp[a.end + size1]['e'].emplace_back(a.start + size1);
                    size_t s = tmp.size();
                    tmp[a.end + size1]['e'].emplace_back(s);
                    tmp.end = s;
                    tmp.G.emplace_back(unordered_map<char, vector<int>>());
                    tmp[tmp.start]['e'].emplace_back(s);
                    stk.emplace(tmp);
                    b.G.pop_back();
                    b += tmp;
                    b.end = tmp.end;
                    stk.emplace(b);
                    break;
                }
                case '^':
                {
                    NFA b = stk.top();
                    stk.pop();
                    NFA a = stk.top();
                    stk.pop();
                    a.G.pop_back();
                    size_t s = a.size();
                    a += b;
                    a.end = b.end;
                    stk.emplace(a);
                    break;
                }
            }
        }
    }
    nfa = stk.top();
}

void Lex::showNFA()
{
    for (int i = 0; i < nfa.size(); ++i)
    {
        for (auto &k: nfa[i])
            for (auto &v: k.second)
                std::cout << i << ' ' << k.first << ' ' << v << std::endl;
    }
}

void Lex::buildDFA()
{
    move(e_closure(6), 'a');
//    vector<DStat> Dstats;
//    Dstats.emplace_back(e_closure(nfa.start));
//    Dstats.back().id = 0;
//    set<DStat> vis;
//    for (int i = 0; i < Dstats.size(); ++i)
//    {
//        vis.insert(Dstats[i]);
//        for (auto c: inputSet)
//        {
//            c
//        }
//    }
}

Lex::DStat Lex::e_closure(int s)
{
    DStat stat;
    stat.stats.emplace_back(s);
    stack<int> stk;
    stk.emplace(s);
    unordered_set<int> vis;
    vis.insert(s);
    while (!stk.empty())
    {
        int i = stk.top();
        stk.pop();
        for (auto &v: nfa[i]['e'])
        {
            if (vis.find(v) != vis.end()) continue;
            stat.stats.emplace_back(v);
            stk.emplace(v);
            vis.insert(v);
        }
    }
    std::sort(stat.stats.begin(), stat.stats.end());
    for (auto &i: stat.stats)
        std::cout << i << ' ';
    std::cout << '\n';
    return stat;
}

Lex::DStat Lex::move(const Lex::DStat &T, char a)
{
    DStat stat;
    unordered_set<int> vis;
    for (auto &i: T.stats)
        if (nfa[i].find('a') != nfa[i].end())
            for (auto &v: nfa[i]['a'])
            {
                if (vis.find(v) == vis.end())
                {
                    vis.insert(v);
                    stat.stats.emplace_back(v);
                }
            }
    std::sort(stat.stats.begin(), stat.stats.end());
    for (auto &i: stat.stats)
        std::cout << i << ' ';
    return stat;
}
