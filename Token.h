#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
using namespace std;
class Token{
public:
	int tokenID;
	string tokenName;
	string tokenValue;
	Token();
	void setToken(string token_name);
	void print();
};
#endif TOKEN_H