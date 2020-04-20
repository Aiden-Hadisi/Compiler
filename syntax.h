#ifndef SYNTAX_H
#define SYNTAX_H
#include <vector>
#include <string>
#include "lexical-tests.h"
using namespace std;

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(const string& expression);
    void analyzer();
    void next();
    bool id();
    bool term();
    bool termPrime();
    bool expression();
    bool expressionPrime();
    bool factor();
    void print();
    bool statement();
    bool assignment();
    bool declaration();
    bool declarative();



private:
    string m_expression;
    vector<Token> m_tokens;
    Token m_current;
    int m_index;
};
#endif
