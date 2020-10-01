#include "Preprocessor.h"
#include "LexicalAnalyzer.h"
#include "Lex.h"

#define FILENAME "test.txt"

char type(char c)
{
    if (isdigit(c))
        return 'a';
    return c;
}

int main()
{
    //this is a comment
    /*this is another comment*/
//    auto p = Preprocessor(FILENAME);
//    auto &&code = p.pretreatment();
//    cout << code << "\n\n";
//    auto lex = LexicalAnalyzer();
//    int i = 0;
//    codeVal t;
//    do
//    {
//        while (code[i] == ' ')++i;
//        t = lex.scanner(code ,i);
//        printf("(%c, %s)\n", t.code, t.val->empty() ? "NULL" : t.val->c_str());
//    }while (t.code != '#');
//    auto lex = Lex("(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*($|.(0|1|2|3|4|5|6|7|8|9)*)", "0123456789.$");
//    string reg = "($|+|-)aa*($|.a*)($|e($|+|-)aa*)|($|+|-)a*(a|.aa*)($|e($|+|-)aa*)";//a: 0-9
    /*
     * ($|+|-)aa*($|.a*)($|e($|+|-)aa*)
     * ($|+|-)a*(a|.aa*)($|e($|+|-)aa*)
     */
//    string inputSet = "a.e+-";
    string reg = "(a|b)*abb", inputSet = "ab";
    auto lex = Lex(reg, inputSet);
    lex.preprocess();
    lex.buildNFA();
    lex.showNFA();
//    lex.showNFA();
    lex.buildDFA();
    lex.printToFile(lex.dfa, "number.txt");
//    string s;
//    while (std::cin >> s)
//        cout << lex.dfa.match(s, type) << '\n';
    return 0;
}