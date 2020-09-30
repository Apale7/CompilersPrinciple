#include "Preprocessor.h"
#include "LexicalAnalyzer.h"
#include "Lex.h"

#define FILENAME "test.txt"

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
    auto lex = Lex("(a|b)*abb", "ab");
    lex.preprocess();
    lex.buildNFA();
//    lex.showNFA();
    lex.buildDFA();
    return 0;
}
