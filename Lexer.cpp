/*TITLE
-----
CPSC 323-01 COMPILERS AND LANGUAGES: PROJECT PART #1- LEXER
Group: BCJ
Members: Jared Fipps, Brian Gilman, Carmine Infusino


CONTACT INFORMATION
-------------------
Jared Fipps: jfipps1995@csu.fullerton.edu

Brian Gilman: bjgilman@csu.fullerton.edu

Carmine Infusino: c.infusino@csu.fullerton.edu




DESCRIPTION/RUN INFORMATION
---------------------------
-The user enters a series of lines to be tokenized followed by a '$'(terminal/EOF) symbol on a new line.
-The program will tokenize the entered strings using regular expressions and outputs the tokens.
-If there is a lexical error, the program terminates and outputs an error message.
-The current version of this project does NOT account for syntax errors.*/
#include "Lexer.h"
#include "Token.h"
#include <stack>
#pragma region Regex

//Regex Checking
regex ident("^\_?([a-z A-Z]*|[0-9]*)$");// identelse ifier
regex LU("^\_[a-z A-Z]$");// Letter-Underscore
regex LUD("^_([a-z A-Z])([0-9])$");// Letter-Underscore-Digit
regex DIGIT("^[0-9]$"); //Only 1 Digit
regex NDIGIT("^\-[0-9]$"); //Only 1 Negative Digit
regex integer("^(\-?[0-9]+)$"); //Many Digits that include pos or neg
regex floatValue("(^\-?[0-9]+)\.([0-9]+)$"); //Float value, Takes interger followed by a . then 1 digit
regex stringValue("\".*\""); //String value

#pragma endregion

//Functions for checking input
bool check_current(int i, int& j, vector<string> lines, string delim[], string& word, bool& symbolcheck);
bool peek(int i, int& j, vector<string> lines, string& word, string delim[], vector<vector<Token>> output, bool& commentcheck);

vector<vector<Token>> lexer()
{
	//Arrays to check for keywords and symbols
	string kwd[15] = { "prog", "main", "fcn", "class",
		"float", "int", "string", "elseif",
		"if", "else", "while", "input",
		"print", "new", "return" };
	string delim[18] = { "(", ")", "<", ">", "{", "}", "[", "]", "*", "^", ":", ".",
		"=", "-", "+", "/", ",", ";" };
	string delimstring[18] = { "paren1", "paren2", "angle1", "angle2", "brace1", "brace2",
		"bracket1", "bracket2", "aster", "caret", "colon", "dot", "equal", "minus", "plus", "slash",
		"comma", "semi" };

	//input and output of code and tokens
	vector<string> lines;
	vector<Token> outputa;
	vector<vector<Token>> output;
	string input;
	string word = "";
	bool identcheck;
	bool symcheck;
	bool symbolcheck;
	bool commentcheck;

	cout << "Enter in a string for the lexer. $ on newline will end input." << endl;
	//input taken in
	while (getline(cin, input, '\n') && input != "$")
	{
		lines.push_back(input + '\n');
		output.push_back(outputa);
	}

	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = 0; j < lines[i].length(); j++)
		{
			//removes extra white spaces
			while ((lines[i][j] == ' ' && word == "") || (lines[i][j] == '\t' && word == ""))
			{
				j++;
			}

			//stops whitespace from printing out at end of the line
			if (lines[i][j] == '\n' && word == "")
			{
				break;
			}

			if (peek(i, j, lines, word, delim, output, commentcheck) || check_current(i, j, lines, delim, word, symbolcheck))
			{
				//Checks word against Regex statements.
				if (commentcheck)
				{
					word = "";
					break;
				}
				else if (regex_match(word, integer) || regex_match(word, DIGIT))
				{
					Token tempToken;
					tempToken.setToken("kwdint");
					tempToken.tokenValue = word;
					output[i].push_back(tempToken);
				}
				else if (regex_match(word, floatValue))
				{
					Token tempToken;
					tempToken.setToken("kwdfloat");
					tempToken.tokenValue = word;
					output[i].push_back(tempToken);
				}
				else if (regex_match(word, stringValue))
				{
					Token tempToken;
					tempToken.setToken("kwdstring");
					tempToken.tokenValue = word;
					output[i].push_back(tempToken);
				}
				else if (regex_match(word, ident))
				{
					identcheck = true;
					for (int p = 0; p < 15; p++)
					{
						if (kwd[p] == word)
						{
							Token tempToken;
							string toke = "kwd" + kwd[p];
							tempToken.setToken(toke);
							output[i].push_back(tempToken);
							toke = "";
							identcheck = false;
							break;
						}
					}
					if (identcheck)
					{
						Token tempToken;
						tempToken.setToken("id");
						tempToken.tokenValue = word;
						output[i].push_back(tempToken);
					}
				}
				else if (symbolcheck)
				{
					symcheck = false;
					for (int o = 0; o < 18; o++)
					{
						if (delim[o] == word)
						{
							Token tempToken;
							tempToken.setToken(delimstring[o]);
							output[i].push_back(tempToken);
							symcheck = true;
						}
					}
					if (!symcheck)
					{
						cout << "Lexical Error on line: " << i + 1 << endl;
						Token errorToken;
						errorToken.setToken("ERROR");
						output[0][0] = errorToken;
						return output;
					}
				}
				else
				{
					cout << "Lexical Error on line: " << i + 1 << endl;
					Token errorToken;
					errorToken.setToken("ERROR");
					output[0][0] = errorToken;
					return output;
				}
				word = "";
			}

			else
			{
				word += lines[i][j]; //Adds current character to word string;
			}
		}
	}

#pragma region output
	//converted code is output
	/*for (int i = 0; i < output.size(); i++)
	{
		for (int j = 0; j < output[i].size(); j++)
		{
			cout << "(:token " << (i + 1) << " " << output[i][j].tokenName << " " << ")" << endl;
		}
	}*/
#pragma endregion
	//return output;
	return output;
}

bool check_current(int i, int& j, vector<string> lines, string delim[], string& word, bool& symbolcheck)
{
	symbolcheck = false;
	string checker = "";
	string num = "";
	num += lines[i][j + 1];
	//Checks for float
	if ((lines[i][j] == '.' && regex_match(num, integer)))
	{
		symbolcheck = true;
		return false;
	}
	//Checks for string
	if (lines[i][j] == '"')
	{
		word += lines[i][j];
		j++;
		while (lines[i][j] != '"')
		{
			if (lines[i][j] == '\n')
			{
				cout << "Lexical Error on line: " << i + 1 << endl;
				return 0;
			}
			word += lines[i][j];
			j++;
		}
		word += lines[i][j];
		symbolcheck = true;
		return true;
	}
	//Checks for symbols
	for (int k = 0; k < 18; k++)
	{
		checker += lines[i][j];
		if (checker == delim[k])
		{
			word += lines[i][j];
			symbolcheck = true;
			return true;
		}
		checker = "";
	}
	//Checks for spaces, tabs, or newline chars.
	if (lines[i][j] == ' ' || lines[i][j] == '\t' || lines[i][j] == '\n')
	{
		symbolcheck = true;
		return true;
	}
	else
	{
		symbolcheck = true;
		return false;
	}
}

bool peek(int i, int& j, vector<string> lines, string& word, string delim[], vector<vector<Token>> output, bool& commentcheck)
{
	//Checking for multi-char operators
	commentcheck = false;
	string num = "";
	num += lines[i][j + 1];
	string current = "";
	current += lines[i][j];
	if (lines[i][j] == ' ' || lines[i][j] == '\n' || lines[i][j] == '\t')
	{
		return false;
	}
	if ((lines[i][j] == '-' && lines[i][j + 1] == '>'))
	{
		Token tempToken;
		tempToken.setToken("oparrow");
		output[i].push_back(tempToken);
		word = "";
		j++;
	}
	else if ((lines[i][j] == '=' && lines[i][j + 1] == '='))
	{
		Token tempToken;
		tempToken.setToken("opeq");
		output[i].push_back(tempToken);
		word = "";
		j++;
	}
	else if ((lines[i][j] == '!' && lines[i][j + 1] == '='))
	{
		Token tempToken;
		tempToken.setToken("opne");
		output[i].push_back(tempToken);
		word = "";
		j++;
	}
	else if ((lines[i][j] == '<' && lines[i][j + 1] == '='))
	{
		Token tempToken;
		tempToken.setToken("ople");
		output[i].push_back(tempToken);
		word = "";
		j++;
	}
	else if ((lines[i][j] == '>' && lines[i][j + 1] == '='))
	{
		Token tempToken;
		tempToken.setToken("opge");
		output[i].push_back(tempToken);
		word = "";
		j++;
	}
	else if ((lines[i][j] == '<' && lines[i][j + 1] == '<'))
	{
		Token tempToken;
		tempToken.setToken("opshl");
		output[i].push_back(tempToken);
		word = "";
		j++;
	}
	else if ((lines[i][j] == '>' && lines[i][j + 1] == '>'))
	{
		Token tempToken;
		tempToken.setToken("opshr");
		output[i].push_back(tempToken);
		word = "";
		j++;
	}
	else if ((lines[i][j] == '/' && lines[i][j + 1] == '/'))
	{
		commentcheck = true;
	}
	//Checking for floats
	else if ((lines[i][j] == '.' && regex_match(num, integer)))
	{
		return false;
	}
	else if ((regex_match(current, integer) && lines[i][j + 1] == '.'))
	{
		return false;
	}
	//End checking for floats
	//Checks normal symbols
	else
	{
		string checker = "";
		for (int k = 0; k < 18; k++)
		{
			checker += lines[i][j + 1];
			if (checker == delim[k])
			{
				word += lines[i][j];
				return true;
			}
			checker = "";
		}
	}
	return false;
}
