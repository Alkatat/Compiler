#include "Symbol.h"
#include <iostream>
#include <string>

using namespace std;

Symbol::Symbol()
{
	sym = "";
	symID = 0;
	symKind = "";
}

void Symbol::setSym(string Sym, int Symid, string Symkind)
{
	sym = Sym;
	symID = Symid;
	symKind = Symkind;
}

void Symbol::printSym()
{
	cout << sym << endl;
	cout << symID << endl;
	cout << symKind << endl;
}