#include "syntax.h"

#include <iostream>
#include <vector>
using namespace std;




SyntaxAnalyzer::SyntaxAnalyzer(const std::string& expression)
{
    m_expression = expression;
    //Use our previously made lexer function to get the tokens
    m_tokens = lexer(expression);

    //Current index is used to get the next token in vector
    m_index = -1;

    //Call next to get
    next();
}


/**
* Starts checking for expression or assignment or declarations
**/
void SyntaxAnalyzer::analyzer() {
  if (m_expression.find('=') != std::string::npos) {
    statement();
  }

  else if(m_expression.find('+') != std::string::npos || m_expression.find('-') != std::string::npos || m_expression.find('*') != std::string::npos || m_expression.find('/') != std::string::npos) {
    expression();
  }

  else if(m_expression.find("int ") != std::string::npos || m_expression.find("string ") != std::string::npos || m_expression.find("bool ") != std::string::npos || m_expression.find("float ") != std::string::npos|| m_expression.find("double ") != std::string::npos) {
    declaration();
  }
  else {
    for(unsigned x = 0; x < m_tokens.size(); ++x)
    {
        cout<<m_tokens[x].lexemeName<<":   "
            <<m_tokens[x].tokenName<<endl;
    }
  }

}



/**
*checks if token is IDENTIFIER
**/
bool SyntaxAnalyzer::id() {

  cout << "<ID> -> id" << endl;

  if(m_current.lexemeName == "Identifier"){
    return  true;
  }

  return false;
}


/**
*Moves to the next token
**/
void SyntaxAnalyzer::next() {
  m_index++;
  if(m_index >= m_tokens.size()) {
    m_current = {"", ""};
  }
  else {
    m_current = m_tokens.at(m_index);
  }
}


/**
*checks if token is expression
**/
bool SyntaxAnalyzer::expression() {
cout << "<Expression> -> <Term> <Expression Prime>" << endl;
  if(term()) {
    if(expressionPrime()) {
      return true;
    }
    else {
      cout << "ERROR: expected expressionPrime" << endl;
      return false;
    }
  }
  return false;
}

/**
*checks if expressionPrime
**/
bool SyntaxAnalyzer::expressionPrime() {
    if(m_current.tokenName == "+" || m_current.tokenName == "-") {
      print();

      next();
      cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
      if(term()) {
        if(expressionPrime()){
          return true;
        }
        else {
          cout << "ERROR: expected expressionPrime" << endl;
          return false;
        }
      }

    }



  return true;

}

/**
*checks if term
**/
bool SyntaxAnalyzer::term() {
  cout << "<Term> -> <Factor> <TermPrime>" << endl;
  if(factor()) {
    next();
    if(termPrime()){
      return true;
    }
    else {
      cout << "ERROR: expected <termPrime>" << endl;
      return false;
    }
  }
  else {
    cout << "ERROR: expected <term>" << endl;
    return false;
  }

}

/**
*checks if termPrime
**/
bool SyntaxAnalyzer::termPrime(){
    if(m_current.tokenName == "*") {
      print();
      next();
      cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
      if(factor()) {
        next();
        if(termPrime()){
          return true;
        }
        return false;
      }
      else {
        cout <<  "ERROR: expected <factor>" << endl;
			return false;
      }

    }
    else if(m_current.tokenName == "/") {
      print();
      next();
      cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
      if(factor()) {
        next();
        if(termPrime()){
          return true;
        }
        return false;
      }
      else {
        cout <<  "ERROR: expected <factor>" << endl;
      return false;
      }

    }

    return true;

}

/**
*checks if factor
**/
bool SyntaxAnalyzer::factor() {
  //Check for (expression) inside paranthesis
   if(m_current.tokenName == "(") {
    print();
    cout << "<Factor> -> (<Expression>)" << endl;

    next();
    if(expression()){
        //check for closing paranthesis
        if(m_current.tokenName == ")") {
            print();
            return true;
        }
        else {
          cout << "ERROR: Missing closing paranthesis" << endl;
          return false;
        }

    }
    else {
      cout << "ERROR: Expected <Expression>" << endl;
      return false;
    }
  }

  else {
    print();
    cout << "<Factor> -> <ID>" << endl;
    if(id()) {
      return true;

    }
  }
  return false;
}


bool SyntaxAnalyzer::statement() {
  cout << "<Statement> -> <Assign>" << endl;
  print();
  if(assignment()) {
    return true;
  }
  else {

    cout << "ERROR: Expected <Assign>" << endl;
    return false;
  }
}
bool SyntaxAnalyzer::assignment() {
  cout << "<Assign> -> <ID> = <Expression>" << endl;
  if(id()) {
    next();
    if(m_current.tokenName == "=") {
      next();
      if(expression()) {
        return true;
      }
      else {
        cout << "ERROR: Expected <Expression>" << endl;
        return false;
      }
    }
    else {
      cout << "ERROR: Expected assignment operator>" << endl;
      return false;
    }
  }
  else {
    cout << "ERROR: Expected <ID>" << endl;
    return false;
  }
  return false;

}


bool SyntaxAnalyzer::declaration() {
  cout << "<Statement> -> <Declarative> " << endl;
  print();
  if(declarative()) {
    return true;
  }
  else {

    cout << "ERROR: Expected <Declarative>" << endl;
    return false;
  }
}

bool SyntaxAnalyzer::declarative() {
  cout << "<Declarative> -> <Type> <ID>" << endl;
  if(m_current.lexemeName == "Keyword") {
    next();
    if(id()) {
      return true;
    }
    else {
      cout << "ERROR: Expected <ID>" << endl;
      return false;
    }
  }
  else {
    cout << "ERROR: Expected Keyword" << endl;
    return false;
  }
  return false;

}



//Prints token
void SyntaxAnalyzer::print() {
  cout  << m_current.lexemeName << ":      " << m_current.tokenName << endl;

}
