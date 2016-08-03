#ifndef TESTEXECUTIVE_H
#define TESTEXECUTIVE_H
///////////////////////////////////////////////////////////////////////
// TestExecutive.h - Automated test suite for Lexical Scanner        //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Varun Jindal, vjindal@syr.edu                        //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides the imolemetation for automated test suite
* to test tokenizer and semi expression package
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
*	TestExecutive.h, TestExecutive.cpp
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 09 Feb 2016
*/
#pragma once
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"

using namespace Scanner;

class TestExecutive
{
public:
	
	static void Title(std:: string str );
	static void Req123();
	static void Req4();
	static void Req56();
	static void Req78();
	static void Req9_10();

	static Toker* getTokenizer();
	static SemiExp* getSemiExp();

	static void setTokenizer(Toker* toker);
	static void setSemiExp(SemiExp* semiExp);

	static void setIPFilename(std::string ipFile) { ipFileName = ipFile; }
	static std::string getIPFilename() { return ipFileName; }

	static void setStoreFlag(bool storeFlag) { storeResults = storeFlag; }
	static bool getStoreFlag() { return storeResults; }

	static void setCommentFlag(bool comment_flag) { commentFlag = comment_flag; }
	static bool getCommentFlag() { return commentFlag; }

	static void setArgs(int arg, char** argv);


protected:
	static Toker* _Tokenizer;
	static SemiExp* _SemiExp;
	static std::string ipFileName;
	static bool storeResults;
	static bool commentFlag;
};

#endif