#ifndef SYMBOL_H
#define SYMBOL_H



#include <iostream>
#include <string>
using namespace std;

/*BEGINNING OF SYMBOL CLASS*/
class Symbol {
public:
	string sym;
	int symID;
	string symKind;
	Symbol();
	void setSym(string Sym, int Symid, string Symkind);
	void printSym();
};
/*class Token {
	int tok_id;
	string name;
	int tok_cnt;
	int line;
};*/
#endif SYMBOL_H
