#ifndef LLPARSER_H
#define LLPARSER_H
#include <vector>
#include <map>
#include <stack>
#include <string>
#include "lexical-tests.h"
using namespace std;

enum Symbols {
	// the symbols:
	// Terminal symbols:
	TS_ID,       //id 0
	TS_PLUS,     // + 1
	TS_MINUS,    // - 2
	TS_MUL,      // * 3
	TS_DIV,      // / 4
  TS_L_PR,     // ( 5
  TS_R_PR,     // ) 6
  TS_EOS,      // $ 7

  TS_EQUALS,   // = 8
  TS_EMPTY,    // Empty 9
  TS_TYPE,    // type 10
	TS_INVALID,	// invalid 11
  TS_NUM,  //12 integer

	// Non-terminal symbols:
	NTS_E,		// E 12
  NTS_S,    // S 13
  NTS_A,    // A 14
  NTS_D,    // D 15
	NTS_EP,   // E Prime 16
  NTS_T,    // T 17
  NTS_TP,   // T Prime 18
  NTS_F,     // F 19
  NTS_TYPE,   // 20
  NTS_ID,     //21
};

//DEFINITION OF LL PARSER
class LLParser {
public:
    LLParser(const string& expression);
    void analyzer();
    Symbols lex(Token t);
    void next();
    // void generateSymbolTable(Token t);


private:
    string m_expression;
    vector<Token> m_tokens;
    map< Symbols, map<Symbols, int> > m_table;

    stack<Symbols> m_ss;
    Token m_current;
    int m_index;
};
#endif
