#include <bits/stdc++.h>

using namespace std;

class Solution
{
public:
    constexpr static const int f[][5] = {
            {1,  2,  3,  4,  -1},
            {-1, -1, 3,  4,  -1},
            {-1, -1, 3,  4,  -1},
            {-1, -1, -1, 5,  -1},
            {-1, -1, 6,  7,  8},
            {-1, -1, -1, 9,  10},
            {-1, -1, -1, 11, 12},
            {-1, -1, 6,  7,  8},
            {13, 14, -1, 15, -1},
            {-1, -1, -1, 9,  10},
            {16, 17, -1, 18, -1},
            {-1, -1, -1, 19, 8},
            {20, 21, -1, 22, -1},
            {-1, -1, -1, 15, -1},
            {-1, -1, -1, 15, -1},
            {-1, -1, -1, 23, -1},
            {-1, -1, -1, 18, -1},
            {-1, -1, -1, 18, -1},
            {-1, -1, -1, 24, -1},
            {-1, -1, -1, 19, 8},
            {-1, -1, -1, 22, -1},
            {-1, -1, -1, 22, -1},
            {-1, -1, -1, 25, -1},
            {-1, -1, -1, 23, -1},
            {-1, -1, -1, 24, -1},
            {-1, -1, -1, 25, -1}};

    int code(char c)
    {
        if (c == '+')
            return 0;
        if (c == '-')
            return 1;
        if (c == '.')
            return 2;
        if (isdigit(c))
            return 3;
        if (c == 'e')
            return 4;
        else
            return 5;
    }

    bool ok(int s)
    {
        vector<int> endStat = {25, 19, 18, 15, 22, 5, 4, 11, 23, 6, 24, 7, 9};
        return find(endStat.begin(), endStat.end(), s) != endStat.end();
    }

    bool isNumber(string s)
    {
        if (s.empty())
            return false;
        int l = 0, r = s.size() - 1;
        while (s[l] == ' ' && l < s.size()) ++l;
        while (s[r] == ' ' && r > l) --r;
        s = s.substr(l, r + 1);
        if (s.empty())
            return false;
        int now = 0;
        for (auto &i: s)
        {
            if (code(i) == 5)
                return false;
            now = f[now][code(i)];
            if (now == -1)
                return false;
        }
        return ok(now);
    }
};