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
    inputSet.insert('$');
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
    //a( aa )a *a
    for (int i = 0; i < reg.size() - 1; ++i)
    {
        int a = statusCode(reg[i]), b = statusCode(reg[i + 1]);
        if (a == 'a' && b == '(' || a == 'a' && b == 'a' || a == ')' && b == 'a' || a == '*' && b == 'a' ||
            a == '*' && b == '(' || a == ')' && b == '(')
            reg.insert(i + 1, "^");
    }
    std::cout << reg << '\n';
    reg = toSuffix(reg);
    std::cout << reg << '\n';
}

Lex::Lex(string charset) : charSet(charset.begin(), charset.end())
{}

char Lex::statusCode(char a)
{
    if (inputSet.find(a) != inputSet.end())
        return 'a';
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
            a.end = 1;
            a[0][c] = {a.end};
            stk.emplace(a);//每次新产生的子NFA都放入栈中
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
                    tmp[tmp.start]['$'].emplace_back(a.start + size1);

                    size_t size2 = tmp.size();
                    tmp += b;
                    tmp[tmp.start]['$'].emplace_back(b.start + size2);
                    tmp[a.end + size1]['$'] = {int(tmp.size())};
                    tmp[b.end + size2]['$'] = {int(tmp.size())};
                    tmp.end = tmp.size();
                    tmp.G.emplace_back(unordered_map<char, vector<int>>());
                    stk.emplace(tmp);//每次新产生的子NFA都放入栈中
                    break;
                }
                case '*':
                {
                    NFA tmp;
                    NFA a = stk.top();
                    stk.pop();
                    size_t size1 = tmp.size();
                    tmp += a;
                    tmp[tmp.start]['$'].emplace_back(a.start + size1);
                    tmp[a.end + size1]['$'].emplace_back(a.start + size1);
                    size_t s = tmp.size();
                    tmp[a.end + size1]['$'].emplace_back(s);
                    tmp.end = s;
                    tmp.G.emplace_back(unordered_map<char, vector<int>>());
                    tmp[tmp.start]['$'].emplace_back(s);
                    stk.emplace(tmp);//每次新产生的子NFA都放入栈中
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
                    a.end = b.end + s;
                    stk.emplace(a);//每次新产生的子NFA都放入栈中
                    break;
                }
                default:
                    assert(0);
            }
        }
    }
    nfa = stk.top();
}

void Lex::showNFA()
{
    std::cout << "-------------------------------------------\n";
    std::cout << "NFA\n";
    for (int i = 0; i < nfa.size(); ++i)
    {
        for (auto &k: nfa[i])
            for (auto &v: k.second)
                std::cout << i << ' ' << k.first << ' ' << v << std::endl;
    }
    std::cout << "-------------------------------------------\n";
}

void Lex::buildDFA()
{
    vector<DStat> Dstats;
    Dstats.emplace_back(e_closure(nfa.start));
    set<DStat> vis;
    vis.insert(Dstats[0]);
    for (int i = 0; i < Dstats.size(); ++i)
    {
        for (auto c: inputSet)
        {
            if (c == '$')
                continue;
            auto U = e_closure(move(Dstats[i], c));
            if (U.stats.empty())
                continue;
            if (vis.find(U) == vis.end())
            {
                Dstats.emplace_back(U);
                if (U.stats.find(nfa.end) != U.stats.end())
                    dfa.end.insert(std::find(Dstats.begin(), Dstats.end(), U) - Dstats.begin());//U在DStat中的下标
                vis.insert(U);
            }
            dfa.G[i][c] = std::find(Dstats.begin(), Dstats.end(), U) - Dstats.begin();//U在DStat中的下标
        }
    }
    for (int i = 0; i < Dstats.size(); ++i)
    {
        std::cout << i << ": {";
        for (auto &j:Dstats[i].stats)
            std::cout << j << ' ';
        std::cout << "}\n";
    }
    for (auto &i: dfa.G)
        for (auto &j: i.second)
        {
            std::cout << i.first << ' ' << j.first << ' ' << j.second << '\n';
        }
}

Lex::DStat Lex::e_closure(int s)
{
    DStat stat;
    stat.stats.insert(s);
    stack<int> stk;
    stk.emplace(s);
    stat.stats.insert(s);
    while (!stk.empty())
    {
        int i = stk.top();
        stk.pop();
        for (auto &v: nfa[i]['$'])
        {
            if (stat.stats.find(v) != stat.stats.end()) continue;
            stk.emplace(v);
            stat.stats.insert(v);
        }
    }
    return stat;
}

Lex::DStat Lex::move(const Lex::DStat &T, char a)
{
    DStat stat;
    for (auto &i: T.stats)
        if (nfa[i].find(a) != nfa[i].end())
            for (auto &v: nfa[i][a])
            {
                if (stat.stats.find(v) == stat.stats.end())
                {
                    stat.stats.insert(v);
                }
            }
    return stat;
}

Lex::DStat Lex::e_closure(Lex::DStat T)
{
    DStat stat;
    for (auto &i: T.stats)
    {
        auto tmp = e_closure(i);
        for (auto &v: tmp.stats)
            stat.stats.insert(v);
    }
    return stat;
}

void Lex::printToFile(Lex::DFA dfa, const string &filename)
{
    std::ofstream out(filename);
    for (auto &i : dfa.G)
    {
        out << i.first << ' ';
        for (auto &j: inputSet)
        {
            if (i.second.count(j))
                out << i.second[j] << ' ';
            else
                out << -1 << ' ';
        }
        out << '\n';
    }
    for (auto &i: dfa.end)
        out << i << ' ';
    out << '\n';
    for (auto &i:inputSet)
    {
        out << i << ' ';
    }
}
