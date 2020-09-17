//
// Created by Apale on 2020/9/13.
//
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
using std::ifstream;
using std::string;
using std::ios;
using std::cout;
using std::unordered_map;
#ifndef COMPILERSPRINCIPLE_PREPROCESSOR_H
#define COMPILERSPRINCIPLE_PREPROCESSOR_H


class Preprocessor
{
    ifstream sourceFile;
public:
    Preprocessor() = delete;

    string &&pretreatment();

    explicit Preprocessor(const string &filePath);
};


#endif //COMPILERSPRINCIPLE_PREPROCESSOR_H
