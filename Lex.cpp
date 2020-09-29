//
// Created by Apale on 2020/9/29.
//

#include "Lex.h"
#include <cassert>
#include "utils.h"

Lex::Lex(string reg, string charset) : reg(std::move(reg)), charSet(charset.begin(), charset.end())
{
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
                    NFA a = stk.top();
                    stk.pop();
                    size_t size1 = tmp.size();
                    tmp += a;
                    tmp[tmp.start]['e'].emplace_back(a.start + size1);
                    NFA b = stk.top();
                    stk.pop();
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

void Lex::showFA()
{
    for (int i = 0; i < nfa.size(); ++i)
    {
        for (auto &k: nfa[i])
            for (auto &v: k.second)
                std::cout << i << ' ' << k.first << ' ' << v << std::endl;
    }
}
