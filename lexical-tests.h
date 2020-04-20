#ifndef LEXICAL_TESTS_H
#define LEXICAL_TESTS_H
#include <string>
#include <vector>
using namespace std;
struct Token {
  string tokenName;
  string  lexemeName;
};
vector<Token> lexer(const string& exp);
string getLexemeName(int);
int getColumn(char);
bool isSeperator(char);


#endif
