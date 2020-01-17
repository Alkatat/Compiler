#ifndef LEXER_CPP_INCLUDED
#define LEXER_CPP_INCLUDED

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include "Token.h"
using namespace std;

vector<vector<Token>> lexer();
bool check_current(int i, int& j, vector<string> lines, string delim[], string& word, bool& symbolcheck);
bool peek(int i, int& j, vector<string> lines, string& word, string delim[], vector<vector<Token>> output, bool& commentcheck);

#endif