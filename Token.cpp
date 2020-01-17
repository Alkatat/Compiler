#include "Token.h"

Token::Token()
{
	tokenID = -1;
	tokenName = "";
	tokenValue = "";
}

void Token::setToken(string token_name)
{
	tokenName = token_name;
}

void Token::print()
{
	cout << "Token ID: " << tokenID << endl;
	cout << "Token Name: " << tokenName << endl;
}