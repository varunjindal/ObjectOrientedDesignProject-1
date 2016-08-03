/////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Automated test suite for Lexical Scanner    //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:		Varun Jindal, vjindal@syr.edu					   //
/////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_N0_WARNINGS
#include "TestExecutive.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include<iostream>
#include<algorithm>

using namespace Scanner;

Toker* TestExecutive::_Tokenizer = nullptr;
SemiExp* TestExecutive::_SemiExp = nullptr;
std::string TestExecutive::ipFileName = "";
bool TestExecutive::storeResults = false;
bool TestExecutive::commentFlag = false;


Toker* TestExecutive::getTokenizer()
{
	return _Tokenizer;
}
SemiExp* TestExecutive::getSemiExp()
{
	return _SemiExp;
}

 void TestExecutive::setTokenizer(Toker* toker)
 {
	 _Tokenizer = toker;
 }
 void TestExecutive::setSemiExp(SemiExp* semiExp)
 {
	 _SemiExp = semiExp;
 }

 void TestExecutive::Req123()
 {	
	 freopen("CON", "w", stdout);
	 Title("Demonstrating Requirement 1,2 & 3");
	 std::cout << "This project is developed using Visual Studio 2015" << std::endl;
	 std::cout << "C++ standard library's is used for streams for all I/O and new and delete for all heap-based memory management" << std::endl;
	 std::cout << "Project consist of packages for Tokenizing, collecting SemiExpressions, and a scanner interface, ITokCollection" << std::endl;
	 
 }

 void TestExecutive :: Req4()
 {	 
	 Title("Demonstrating Requirement 4 also setSpecialSingleChars & setSpecialCharPairs");
	 freopen("CON", "w", stdout);
	 //std::string out = "C:/Users/Varun/Desktop/Project1/Output/Req4Result.txt";
	 std::string out = "./Req4Result.txt";
	 //setIPFilename("C:/Users/Varun/Desktop/Project1/Tokenizer/Test.txt");
	 setIPFilename("./test4.txt");
	 //setStoreFlag(true);
	 getTokenizer()->PrintTokens(getIPFilename(),getStoreFlag(),getCommentFlag(),out); // Call for tokenizer requirement
	 setTokenizer(nullptr);	 
 }

 void TestExecutive::Req56()
 {
	 freopen("CON", "w", stdout);	
	 Title("Demonstrating Requirement 5 & 6");
	 std::cout << "For Requirement 5 : Please refer to getTok() function in Tokenizer.cpp file under this project" << std::endl;
	 std::cout << "For Requirement 6 : Please refer to get() function in SemiExp.cpp file under this project" << std::endl;
 }
 
 void TestExecutive::Req78()
 {
	 freopen("CON", "w", stdout);
	 Title("Demonstrating Requirement 7 & 8");
	 setIPFilename("./test78.txt");
	 getSemiExp()->PrintSemiExps(getTokenizer(),getIPFilename(),getStoreFlag());    // Call for semiExpression requirement
	 setTokenizer(nullptr);
 }

 void TestExecutive::Req9_10()
 {
	 freopen("CON", "w", stdout);
	 Title("Demonstrating Requirement 9 & 10");
	 std::cout << "For Requirement 9 : Please refer to the implementation of ITokCollection interface in SemiExp.h and SemiExp.cpp file" << std::endl;
	 std::cout << "****************************************************************" << std::endl;
	 std::cout << "ITokCollection functions are implemented in SemiExp.cpp file from file 97 to 171 -- Please refer the definations" << std::endl;
	 std::cout << "****************************************************************" << std::endl;
	 std::cout << "For Requirement 10 : Automated test suite is implemented using TestExecutive package" << std::endl;
 }

 void TestExecutive::Title(std::string str)
 {
	 std::cout <<std::endl<< "================================================================================"<<std::endl;
	 std::cout << str << std::endl;
 }
 
 void TestExecutive::setArgs(int arg, char** argv)
 {
	 std::string argument(argv[1]);     //set store flag to true from argument 1
	 if (argument == "y")
		 setStoreFlag(true);
	 else
		 setStoreFlag(false);

	 std::string comment(argv[2]); // set comment flag
	 if (comment == "y")
		 setCommentFlag(true);
	 else
		 setCommentFlag(false);
 }

 int main(int arg, char** argv)
 {
	 std::string fileSpec = "../Tokenizer/Test.txt";
	 SemiExp semiExp;
	 Toker toker;
	 TestExecutive::setArgs(arg, argv);
	 TestExecutive::setTokenizer(&toker);
	 TestExecutive::setSemiExp(&semiExp);
	 TestExecutive::Req123();
	 TestExecutive::Req4();
	 TestExecutive::Req56();
	 Toker::reset();
	 Toker toker2;
	 TestExecutive::setTokenizer(&toker2);
	 TestExecutive::Req78();
	 TestExecutive::Req9_10();
	 Toker::reset();
	 Toker toker3;
	 freopen("CON", "w", stdout);
	 TestExecutive::Title("Demonstrating setSpecialSingleChars requirement");
	 TestExecutive::setIPFilename("./test2.txt");
	 toker3.setSpecialSingleChars("@,$");
	 std::string out = "./setSpecialSingle.txt";
	 toker3.PrintTokens(TestExecutive::getIPFilename(), TestExecutive::getStoreFlag(), TestExecutive::getCommentFlag(),out);
	 if (TestExecutive::getStoreFlag())
	 std::cout << std::endl << "Output Text File will be generated in Project Directory : " << out << std::endl;
	 freopen("CON", "w", stdout);
	 TestExecutive::Title("Demonstrating setSpecialCharPair requirement");
	 Toker::reset();
	 Toker toker4;
	 out = "./setSpecialcharPair.txt";
	 toker4.setSpecialCharPairs("@@,$$");
	 toker4.PrintTokens(TestExecutive::getIPFilename(), TestExecutive::getStoreFlag(), TestExecutive::getCommentFlag(), out);
	 if(TestExecutive::getStoreFlag())
	 std::cout << std::endl << "Output Text File will be generated in Project Directory : " << out << "\n"<<std::endl;	 
	 return 0;
 }