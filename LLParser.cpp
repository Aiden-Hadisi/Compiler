#include "LLParser.h"

#include <iostream>
#include <vector>
using namespace std;
map<string, int> symbolTable;
int initalMemory = 5000;


//Checks The symbol table and inserts data if needed
void symbolTableCheck(Token t) {

  if(symbolTable[t.tokenName]) {
    cout << "ERROR: Redeclaration of variable " << t.tokenName << endl;
    exit(1);
  }
  symbolTable[t.tokenName] = initalMemory++;
}

template<typename K, typename V>
void print_map(std::map<K,V> const &m)
{
    for (auto const& pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}
LLParser::LLParser(const std::string& expression)
{
    m_expression = expression + "$";

    //Use our previously made lexer function to get the tokens
    m_tokens = lexer(expression);



    //SET UP our parse Table
    m_table[NTS_E][TS_ID] = 1;
    m_table[NTS_E][TS_L_PR] = 1;
    m_table[NTS_EP][TS_PLUS] = 2;
    m_table[NTS_EP][TS_MINUS] = 3;


    m_table[NTS_EP][TS_EOS] = 4;
    m_table[NTS_T][TS_ID] = 5;
    m_table[NTS_T][TS_L_PR] = 5;
    m_table[NTS_TP][TS_PLUS] = 8;
    m_table[NTS_TP][TS_MINUS] = 8;

    m_table[NTS_TP][TS_MUL] = 6;

    m_table[NTS_TP][TS_DIV] = 7;
    m_table[NTS_TP][TS_R_PR] = 8;
    m_table[NTS_TP][TS_EOS] = 8;
    m_table[NTS_F][TS_ID] = 10;
    m_table[NTS_F][TS_L_PR] = 9;
    m_table[NTS_S][TS_TYPE] = 11;

    m_table[NTS_D][TS_TYPE]= 12;
    m_table[NTS_TYPE][TS_TYPE] = 13;
    m_table[NTS_ID][TS_ID] = 14;
    m_table[NTS_S][TS_ID] = 15;
    m_table[NTS_A][TS_ID] = 16;
    m_table[NTS_E][TS_NUM] = 1;
    m_table[NTS_T][TS_NUM] = 5;
    m_table[NTS_F][TS_NUM] = 17;
    //Current index is used to get the next token in vector
    m_index = -1;

    //Call next to go to index 0
    next();
    //push the $ to the stack
    m_ss.push(TS_EOS);
    if(m_current.lexemeName == "Keyword") {

      //IF IT IS KEYWORD TREAT IT AS declaration and PUSH S TO STACK S -> D
      m_ss.push(NTS_S);
      symbolTableCheck(m_tokens.at(1));
    }

    else if (m_expression.find('=') != std::string::npos) {
      //IF IT CONTAINS = TREAT AS ASSIGNMENT AND PUSH S TO STACK S -> A
      m_ss.push(NTS_S);
    }

    else {
      //TREAT AS EXPRESSION AND PUSH E TO STACK
      m_ss.push(NTS_E);
    }
}


void LLParser::analyzer() {
  if(m_current.tokenName == "$") {
    return;
  }
  cout << "=================================================" << endl;
  cout << endl;
  for(unsigned x = 0; x < m_tokens.size(); ++x)
  {
      cout<<m_tokens[x].lexemeName<<":   "
          <<m_tokens[x].tokenName<<endl;
  }

  while(m_ss.size() > 0) {
    if(lex(m_current) == m_ss.top()) {
      cout << "Symbols matched: " << m_current.tokenName  << endl;
      next();
      m_ss.pop();
    }
    else {

      // cout << "TOP OF STACK IS "<< m_ss.top() << endl;
      // cout << "CURRENT LEXEME IS "  << lex(m_current) << endl;
      // cout << "Tokens are: " << m_current.tokenName  << " " << m_current.lexemeName << endl;
      // cout << m_table[m_ss.top()][lex(m_current)] << endl;
      switch (m_table[m_ss.top()][lex(m_current)]) {
        case 1:

          cout << "E -> TE'" << endl;
          m_ss.pop();
          m_ss.push(NTS_EP);
          m_ss.push(NTS_T);

          break;
        case 2:
          cout << "E' -> +TE'" << endl;
          m_ss.pop();
          m_ss.push(NTS_EP);
          m_ss.push(NTS_T);
          m_ss.push(TS_PLUS);
          break;
        case 3:
          cout << "E' -> -TE'" << endl;
          m_ss.pop();
          m_ss.push(NTS_EP);
          m_ss.push(NTS_T);
          m_ss.push(TS_MINUS);
          break;
        case 4:
          cout << "E' -> Empty" << endl;
          m_ss.pop();
          break;
        case 5:
          cout << "T -> FT'" << endl;
          m_ss.pop();
            m_ss.push(NTS_TP);
          m_ss.push(NTS_F);


          break;
        case 6:
          cout << "T' -> *FT'" << endl;
          m_ss.pop();
          m_ss.push(NTS_TP);
          m_ss.push(NTS_F);
          m_ss.push(TS_MUL);
          break;
        case 7:
          cout << "T' -> /FT'" << endl;
          m_ss.pop();
          m_ss.push(NTS_TP);
          m_ss.push(NTS_F);
          m_ss.push(TS_DIV);
          break;
        case 8:
          cout << "T' -> Empty" << endl;
          m_ss.pop();
          break;

        case 9:
          cout << "F -> ( E )" << endl;
          m_ss.pop();
          m_ss.push(TS_R_PR);
          m_ss.push(NTS_E);
          m_ss.push(TS_L_PR);
          break;
        case 10:
          cout << "F -> id" << endl;
          m_ss.pop();
          m_ss.push(TS_ID);
          break;
        case 11:
          cout << "S -> D" << endl;
          m_ss.pop();
          m_ss.push(NTS_D);
          break;
        case 12:
          cout << "D -> <TYPE> <ID>" << endl;
          m_ss.pop();
          m_ss.push(NTS_ID);
          m_ss.push(NTS_TYPE);
          break;
        case 13:
          cout << "TYPE -> type" << endl;

          m_ss.pop();
          m_ss.push(TS_TYPE);
          break;
        case 14:
          cout << "ID -> id" << endl;
          m_ss.pop();
          m_ss.push(TS_ID);
          break;
        case 15:
          cout << "S -> A" << endl;
          m_ss.pop();
          m_ss.push(NTS_A);
          break;
        case 16:
          cout << "A -> <ID> = <E>" << endl;
          m_ss.pop();
          m_ss.push(NTS_E);
          m_ss.push(TS_EQUALS);
          m_ss.push(NTS_ID);
          break;
        case 17:
          cout << "F -> Number" << endl;
          m_ss.pop();
          m_ss.push(TS_NUM);
          break;
        default:
          cout << "Parser defaulted." << endl;
          exit(0);
      }
    }
  }
  cout << "SYMBOL TABLE:  " << endl;
  cout << "------------------------" << endl;
  print_map(symbolTable);
cout << "------------------------" << endl;

}

Symbols LLParser::lex(Token t) {

  if(t.lexemeName == "Identifier") {

    return TS_ID;
  }

  if(t.lexemeName == "Keyword") {
    return TS_TYPE;
  }
  if(t.lexemeName == "") {
    return TS_EMPTY;
  }
  if(t.tokenName == "+") {

    return TS_PLUS;
  }
  if(t.tokenName == "-") {
    return TS_MINUS;
  }
  if(t.tokenName == "/") {
    return TS_DIV;
  }
  if(t.tokenName == "(") {
    return TS_L_PR;
  }
  if(t.tokenName == ")") {
    return TS_R_PR;
  }
  if(t.tokenName == "$") {
    return TS_EOS;
  }
  if(t.tokenName == "=") {
    return TS_EQUALS;
  }
  if(t.lexemeName == "Integer") {
    return TS_NUM;
  }

return TS_EMPTY;

}

void LLParser::next() {
  m_index++;
  if(m_index >= m_tokens.size()) {
    m_current = {"$", "$"};
  }
  else {
    m_current = m_tokens.at(m_index);
  }
}
