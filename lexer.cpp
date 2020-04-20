#include "lexical-tests.h"
#include <cctype>
#include <iostream>
#include <algorithm>
using namespace std;


//Enum for table transitions instead of numbers to make it more readable
enum TRANSITIONS
{
    INITIAL = 0,
    INTEGER,
    FLOAT,
    OPERATOR,
    IDENTIFIER,
    UNKNOWN,
    SPACE,
    SEPERATOR
};
        //                0       1          2        3          4          5         6        7
int stateTable[][8] = {{INITIAL, INTEGER,  FLOAT,  OPERATOR,  IDENTIFIER, UNKNOWN,  SPACE, SEPERATOR},
/* STATE 1 */   {INTEGER,  INTEGER,  FLOAT,   INITIAL,   INITIAL,  INITIAL,  INITIAL, INITIAL},
/* STATE 2 */   {FLOAT,       FLOAT,  UNKNOWN, INITIAL,   INITIAL,  INITIAL,  INITIAL, INITIAL},
/* STATE 3 */   {OPERATOR,  INITIAL, INITIAL,  INITIAL,   IDENTIFIER,  INITIAL,  INITIAL, INITIAL},
/* STATE 4 */   {IDENTIFIER,    IDENTIFIER, INITIAL,  IDENTIFIER,   IDENTIFIER,  INITIAL,  INITIAL, INITIAL},
/* STATE 5 */   {UNKNOWN,  UNKNOWN, UNKNOWN, UNKNOWN,  UNKNOWN, UNKNOWN, INITIAL, UNKNOWN},
/* STATE 6 */   {SPACE,     INITIAL, INITIAL,  INITIAL,   INITIAL,  INITIAL,  INITIAL, INITIAL},
/* STATE 7 */   {SEPERATOR, INITIAL, INITIAL, INITIAL, INITIAL, INITIAL, INITIAL, SEPERATOR}};

//Keywords
string keywords[] = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or", "not"};


/**
*Finds the lexems of a line
*@PARAM exp: a line of code from file
*@RETURN: A vecotr containing lexeme information
**/
vector<Token> lexer(const string& exp) {
  char initalChar = exp[0];
  //If exp starts with a ! assume it is a comment and stop reading further
  if(initalChar == '!') {
    return {};
  }
    Token access;
     vector<Token> tokens;
     char currentChar = ' ';
     int col = INITIAL;
     int currentState = INITIAL;
     int prevState = INITIAL;
     string currentToken = "";

     // Using FSM to find the remaining tokens
     for(unsigned x = 0; x < exp.length();)
     {
         currentChar = exp[x];


         col = getColumn(currentChar);




         currentState = stateTable[currentState][col];



         if(currentState == INITIAL)
         {
              //Skip whitespace
             if(prevState != SPACE)
             {

                 access.tokenName = currentToken;

                //Check if token is a keyword
                 if(prevState == IDENTIFIER && std::find(std::begin(keywords), std::end(keywords), currentToken) != std::end(keywords)) {
                   access.lexemeName = "Keyword";
                 }

                 else {
                   access.lexemeName = getLexemeName(prevState);
                 }

                 tokens.push_back(access);

             }
             currentToken = "";
         }
         else
         {
             currentToken += currentChar;
             ++x;
         }

         //Keep track of previous state
         prevState = currentState;

     }
     //Make sure final token is accounted for
     if(currentState != SPACE && currentToken != "")
     {
         access.tokenName = currentToken;

         access.lexemeName = getLexemeName(currentState);
         tokens.push_back(access);
     }
     return tokens;
}



/**
*Gets the name of a lexeme using it's number
*@PARAM lexeme: an integer
*@RETURN: Name of the lexeme corresponding to the integer value
**/
string getLexemeName(int lexeme)
{
    switch(lexeme)
    {
        case INTEGER:
           return "Integer";
           break;
        case FLOAT:
           return "Float";
           break;
        case OPERATOR:
           return "Operator";
           break;
        case IDENTIFIER:
           return "Identifier";
           break;
        case UNKNOWN:
           return "Unkonwn";
           break;
        case SEPERATOR:
          return "Seperator";
          break;
        case SPACE:
           return "Whitespace";
           break;
        default:
           return "ERROR";
           break;
    }
}


/**
*Gets column of character in State Table
*@PARAM currentChar: a character
*@RETURN: Number of column in state table
**/
int getColumn(char currentChar)
{

    if(isspace(currentChar))
    {
        return SPACE;
    }


    else if(isdigit(currentChar))
    {
        return INTEGER;
    }


    else if(currentChar == '.')
    {
        return FLOAT;
    }


    else if(isalpha(currentChar))
    {
        return IDENTIFIER;
    }

    else if(isSeperator(currentChar)) {
      return SEPERATOR;
    }


    else if(ispunct(currentChar))
    {
        return OPERATOR;
    }
    return UNKNOWN;
}


/**
*Checks if a character is a seperator
*@PARAM chr: a character
*@RETURN: TRUE if the chr is a seperator, FALSE if chr is not a seperator
**/
bool isSeperator(char chr) {
  char seperators[] = {'\'', '(', ')', '{', '}', '[', ']', ',', '.', ':', ';'};
  if(std::find(std::begin(seperators), std::end(seperators), chr) != std::end(seperators)) {
    return true;
  }
  else return false;

}
