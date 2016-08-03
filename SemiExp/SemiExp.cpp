///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design	 //
// Author:		Varun Jindal, vjindal@syr.edu						 //
// Author:      Varun Jindal, Syracuse University,			         //
//              vjindal@syr.edu                                      //
///////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_N0_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

bool SemiExp::get(bool clear)
{
  if (_pToker == nullptr)
    throw(std::logic_error("no Toker reference"));
  _tokens.clear();
  bool flag = 0;
  bool flag2 = false;
  int semiColonCount = 0;
  while (true)
  {
	  std::string token = _pToker->getTok();
	  if (token == "")
		  return false;
	  _tokens.push_back(token);

	  if (token == "{" || token == "}" || token == ";")
		  return true;
	  if (token == "#")
		  flag = 1;
	  if (token == "\n" && flag == 1)
		  return true;

	  if ((token.at(0) == '/' && token.at(1) == '/') || (token.at(0) == '/' && token.at(1) == '*')) {
		  token = _pToker->getTok();
		  _tokens.push_back(token);
		  return true;
	  }

	  flag2 = accessSpec(token);	//Collecting access specifier tokens
	  if (token == ":" && flag2)
		  return true;

	  if (findFor("for") == true) {
		  return forLogic(token);
	  }
  }
  return false;
}

bool SemiExp::checkPound(std::string token)
{
	if (token == "#")
		return true;
}

bool SemiExp::accessSpec(std::string token)
{
	if (token == "public" || token == "protected" || token == "private")
		return true;
}

bool SemiExp::forLogic(std::string token)
{
	int semiColonCount = 0;			// pushing tokens till "{" or count of ";" = 3
	while (true) {
		token = _pToker->getTok();
		_tokens.push_back(token);
		if (token == ";")
			semiColonCount += 1;
		if (token == "{" || semiColonCount == 3)
		{
			return true;
		}
		
	}
}

bool SemiExp::findFor(const std::string& tok) {
	for (size_t i = 0; i<length(); ++i)				// checking for token "for"
		if (tok == _tokens[i])
			return true;
	return false;
}

Token SemiExp::operator[](size_t n)
{
  if (n < 0 || n >= _tokens.size())
    throw(std::invalid_argument("index is out of range"));
  return _tokens[n];
}

/*---------------- Method to check if index is out of bound------------------*/
std::string & SemiExp::operator[](int num)
{
	if (num < 0 || num >= _tokens.size())
		throw(std::invalid_argument("index is out of range"));
	return _tokens[num];
}

size_t SemiExp::length()
{
  return _tokens.size();
}

size_t SemiExp::find(const std::string& tok)
{
	for (size_t i = 0; i<length(); ++i)
		if (tok == _tokens[i])
			return i;
	return length();
}

bool SemiExp::remove(const std::string& tok)
{
	std::vector<std::string>::iterator itd;
	itd = std::find(_tokens.begin(), _tokens.end(), tok);
	if (itd != _tokens.end())
	{
		_tokens.erase(itd);
		return true;
	}
	return false;
}
//----< remove tok at specified indes >------------------------


bool SemiExp::remove(size_t x)			//remove items from vector
{
	if (x<0 || _tokens.size() <= x)
		return false;
	std::vector<std::string>::iterator itd = _tokens.begin();
	_tokens.erase(itd + x);
	return true;
}


void SemiExp::toLower()			//Convert to lower case
{
	for (size_t i = 0; i<length(); ++i)
	{
		for (size_t j = 0; j<(*this)[i].length(); ++j)
		{
			(*this)[i][j] = tolower((*this)[i][j]);
		}
	}
}

void SemiExp::trimFront()   //Trim function
{
	while (_tokens.size() > 0 && (_tokens[0] == "\n" || _tokens[0] == ""))
		remove(0);
}

bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok)
{
	return true;
}

std::string SemiExp::show(bool showNewLines)  //Show Function
{
  std::cout << "\n  ";
  for (auto token : _tokens)
    if(token != "\n")
      std::cout << token << " ";		//Printing tokens 
  std::cout << "\n";
  return "";
}

bool SemiExp::PrintSemiExps(Toker* toker,std::string fileSpec,bool storeFlag)
{
	clear();
	std::cout << std::endl << "Input Text File present in Project Directory : " << fileSpec;
	//std::string out = "C:/Users/Varun/Desktop/Project1/Output/Req7_8Result.txt";		//Output file & directory
	std::string out = "./Req7_8Result.txt";		//Output file & directory
	if(storeFlag)
	std::cout << std::endl << "Output Text File will be generated in Project Directory: " << out<<std::endl;
	FILE *fptr = nullptr;
	if (storeFlag)
		fptr = freopen(out.c_str(), "w", stdout);

	std::fstream in(fileSpec);
	if (!in.good())					//Checking file to be read
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker->attach(&in);
	_pToker = toker;
	
	while (get())
	{
		std::cout << "\n  -- semiExpression --";
		show();
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		show();
		std::cout << "\n\n";
	}
	
	return true;
}

#ifdef TEST_SEMIEXP
int main()
{
  Toker toker;
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  std::string fileSpec = "../Tokenizer/Test.txt";

  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);

  SemiExp semi(&toker);
  while(semi.get())
  {
    std::cout << "\n  -- semiExpression --";
    semi.show();
  } 
  /*
     May have collected tokens, but reached end of stream
     before finding SemiExp terminator.
   */
  if (semi.length() > 0)
  {
    std::cout << "\n  -- semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }
  return 0;
}
#endif