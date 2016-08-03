/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:		Varun Jindal, vjindal@syr.edu					   //
// Author:      Varun Jindal, Syracuse University,			         //
//              vjindal@syr.edu                                      //
///////////////////////////////////////////////////////////////////////
/*
Helper code that does not attempt to handle all tokenizing
special cases like escaped characters.
Added functionalities to handle all the project #1 requirements
*/
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_N0_WARNINGS
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

namespace Scanner
{
	class ConsumeState
	{
	public:
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		void setSpecialSingleChars(std::string);
		void setSpecialCharPairs(std::string);


		bool IsSpecialSingle(std::string str);
		bool IsSpecialDouble(std::string str);
		ConsumeState* checkIsSpecial();
		ConsumeState* checkQuotation();
		ConsumeState* checkComments();
		static void reset();

		bool getCommentFlag() { return commentFlag; }
		void setCommentFlag(bool value) { commentFlag = value; }

	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static const char doubleQuote;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pQuotedString;
		static ConsumeState* _pSpclSingleCharToken;
		static ConsumeState* _pEatSpecialOneChar;
		static ConsumeState* _pEatSpecialTwoChar;
		static ConsumeState* _pSingleQuotedString;
		static bool ConsumeState::first_constructor;
		static bool ConsumeState::first_destructor;
		static bool commentFlag;
	};
}

using namespace Scanner;

bool ConsumeState::commentFlag = false;
std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pQuotedString = nullptr;
ConsumeState* ConsumeState::_pSpclSingleCharToken = nullptr;
ConsumeState* ConsumeState::_pEatSpecialOneChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialTwoChar = nullptr;
ConsumeState* ConsumeState::_pSingleQuotedString = nullptr;
const char ConsumeState::doubleQuote = '"';
std::vector<std::string> singleVec = { "<", ">","[","]", "(",")","{" , "}", ":", "=", "+", "-", "*", "\n" };
std::vector<std::string> doubleVec = { "<<" , ">>" , "::", "++", "--", "==" , "+=", "-=", "*=", "/=" };
std::string temp1;
std::string temp2;
bool ConsumeState::first_constructor = true;
bool ConsumeState::first_destructor = true;

void testLog(const std::string& msg);

void ConsumeState::reset()
{
	first_destructor = first_constructor = true;
}

void ConsumeState::setSpecialSingleChars(std::string ssc)
{	
	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;
	while ((pos = ssc.find(delimiter)) != std::string::npos) {
		token = ssc.substr(0, pos);
		//std::cout << token << std::endl;

		std::vector<std::string>::iterator it;
		it = find(singleVec.begin(), singleVec.end(), token);
		if (it != singleVec.end())
			continue;
		singleVec.push_back(token);
		ssc.erase(0, pos + delimiter.length());
	}
}


void ConsumeState::setSpecialCharPairs(std::string scp)
{
	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;
	while ((pos = scp.find(delimiter)) != std::string::npos) {
		token = scp.substr(0, pos);
		//std::cout << token << std::endl;

		std::vector<std::string>::iterator it;
		it = find(doubleVec.begin(), doubleVec.end(), token);
		if (it != doubleVec.end())
			continue;
		doubleVec.push_back(token);
		scp.erase(0, pos + delimiter.length());
	}
}


bool ConsumeState::IsSpecialSingle(std::string match) //function to check if the token is the special one character token
{
	std::vector<std::string>::iterator it;
	it = find(singleVec.begin(), singleVec.end(), match);
	if (it != singleVec.end())
	{
		return true;
	}
	return false;
}
bool ConsumeState::IsSpecialDouble(std::string temp2) //function to check if the token is the special two character token
{
	std::vector<std::string>::iterator itd = std::find(doubleVec.begin(), doubleVec.end(), temp2);
	if (itd != doubleVec.end())
	return true;

	return false;
}

ConsumeState* ConsumeState::checkIsSpecial() //helper function for NextState() to check if currchar is special char
{
	std::string match(1, (char)currChar);    /// hadling special one & two characters
	temp1 = match;
	std::string second(1, (char)_pIn->peek());
	temp2 = temp1 + second;
	if (IsSpecialDouble(temp2))
		return _pEatSpecialTwoChar;
	else if (IsSpecialSingle(match))
		return _pEatSpecialOneChar;
	else
		return nullptr;
}


ConsumeState* ConsumeState::checkQuotation()  //helper function for NextState() to check if currchar is double or single quote
{
	if ((currChar == '"'))				// Double Quoted strings
		return _pQuotedString;
	
	else if ((currChar == '\''))		//Single Quoted strings
		return _pSingleQuotedString;
	else
		return nullptr;
}

ConsumeState* ConsumeState::checkComments() //helper function for NextState() to check if currchar is c or cpp comment
{
	int chNext = _pIn->peek();
	if (currChar == '/' && chNext == '/') // C++ comments
		return _pEatCppComment;

	else if (currChar == '/' && chNext == '*') // C comments
		return _pEatCComment;
	else
		return nullptr;
}

ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good()))
		return nullptr;
	int chNext = _pIn->peek();

	if (chNext == EOF)
	_pIn->clear();

	if ((std::isspace(currChar) && currChar != '\n') || (!_pIn->good())) // White space
		return _pEatWhitespace;

	ConsumeState * comments = checkComments(); // check for comments
	if (comments != nullptr)
		return comments;
	

	if (currChar == '\n' || (currChar == '\\' && _pIn->peek() == 'n')) // New Line
		return _pEatNewline;

	ConsumeState * specialChar = checkIsSpecial(); // special characters
	if (specialChar != nullptr)
		return specialChar;

	if (std::isalnum(currChar)) //Alphanumeric characters
		return _pEatAlphanum;
	
	ConsumeState * quotation = checkQuotation();  // check for quotations
	if (quotation != nullptr)
		return quotation;
	
	if (ispunct(currChar) && currChar != ':')	// Punctuators
	return _pEatPunctuator;	
	

	throw(std::logic_error("invalid type"));
}

class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C++ comment";
		do {
			token += currChar;
			if (currChar != '/') {
				//token += currChar;
			}
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\n');

		if (!commentFlag)
			token.clear();
	}
};

class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C comment";
		
		do {
			token += currChar;
			if (currChar != '*' && currChar != '/') {
				//token += currChar;
			}
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += '*'; token += '/';

		if (!commentFlag)
			token.clear();

		_pIn->get();
		currChar = _pIn->get();
	}
};

class EatPunctuator : public ConsumeState
{
public:
	int flag = 0;
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  Inside eat punctuator";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			
			std::vector<std::string>::iterator it;

			std::string match(1, (char)currChar);
			//std::cout << "\nMatch ::" << match;

			it = find(singleVec.begin(), singleVec.end(), match);
			if (it != singleVec.end())
				flag = 1;

		} while (ispunct(currChar) && flag!=1 && currChar!='\'');
	}
};

class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};


class EatDoubleQuote : public ConsumeState {
public:
	virtual void eatChars()
	{

		token.clear();
		if (!_pIn->good())  // end of stream
			return;

		currChar = _pIn->get();
		if (currChar == '"')// for 
		{
			currChar = _pIn->get();
			return;
		}
		do {
			if (currChar == '\\' && _pIn->peek() == '"')
			{
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				continue;
			}
			if (currChar == '\\' && _pIn->peek() == '\\')
			{	token += currChar;
				currChar = _pIn->get();
			}
			if (currChar == '\\' && _pIn->peek() == '\'')
			{
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				continue;
			}
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '"');
		currChar = _pIn->get();
	}
};

class EatSingleQuote : public ConsumeState {
	public:
		virtual void eatChars()
		{
			token.clear();
			if (!_pIn->good())
				return;
			currChar = _pIn->get();
			if (currChar == '\'')
			{
				currChar = _pIn->get();
				return;
			}
			if (currChar == '\\')
			{
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				currChar = _pIn->get();
				return;
			}
			do {
				if (currChar == '\\' && _pIn->peek() == '\'')
				{
					currChar = _pIn->get();
					//	currChar = _pIn->get();
					token += currChar;
					currChar = _pIn->get();
					continue;
				}
				token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			} while (currChar != '\'');
			currChar = _pIn->get();
		}	
};

class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		if (currChar == '\\')
			prevChar = currChar;
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		if (prevChar == '\\' && currChar == 'n')
		{
			token += currChar;
			currChar = _pIn->get();
		}
	}
};


class EatSpecialOneChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		return;
	}
};

class EatSpecialTwoChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";

		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		token += currChar;
		currChar = _pIn->get();
		return;
	}
};

ConsumeState::ConsumeState()
{
	static bool first = true;
	if (first_constructor)
	{
		first_constructor = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pQuotedString = new EatDoubleQuote();
		_pEatSpecialOneChar = new EatSpecialOneChar();
		_pEatSpecialTwoChar = new EatSpecialTwoChar();
		_pSingleQuotedString = new EatSingleQuote();
		_pState = _pEatWhitespace;
	}
}

ConsumeState::~ConsumeState()
{
	static bool first = true;
	if (first_destructor)
	{
		first_destructor = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pQuotedString;
		delete _pEatSpecialOneChar;
		delete _pEatSpecialTwoChar;
		delete _pSingleQuotedString;
	}
}
//Fuction called from TestExecutive
bool Toker::PrintTokens(std:: string fileSpec,bool storeFlag, bool commentFlag, std::string out)
{
	std::cout << std::endl<<"Input Text File present in Project Directory : "<<fileSpec;
	//std::string out = "C:/Users/Varun/Desktop/Project1/Output/Req4Result.txt";
	if(storeFlag)
	std::cout << std::endl << "Output Text File will be generated in Project Directory : " << out;
	//C:\Users\Varun\Desktop\Project1\Output
	
	FILE *fptr = nullptr;						
	if (storeFlag)
		fptr = freopen(out.c_str(), "w", stdout);  //Writing to the file
	
	std::ifstream in(fileSpec);

	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}

	Toker toker;

	toker.setCommentFlag(commentFlag);

	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n") {
			tok = "newline";
		}
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n";
	
	return true;
}


Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void Toker::reset()
{
	ConsumeState::reset();
}

void Toker::setCommentFlag(bool value)
{
	pConsumer->setCommentFlag(value);
}

void Scanner::Toker::setSpecialSingleChars(std::string ssc)
{
	pConsumer->setSpecialSingleChars(ssc);
}

void Scanner::Toker::setSpecialCharPairs(std::string scp)
{
	pConsumer->setSpecialCharPairs(scp);
}

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}



//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n") {
			tok = "newline";
		}
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n";
	return 0;
}
#endif