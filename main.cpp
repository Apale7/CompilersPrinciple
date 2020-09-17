#include "Preprocessor.h"

#define FILENAME "main.cpp"

int main()
{
    //this is a comment
    /*this is another comment*/
    auto p = Preprocessor(FILENAME);
    auto &&code = p.pretreatment();
    cout << code;
    return 0;
}
