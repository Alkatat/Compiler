#ifndef NODE_H 
#define NODE_H

#include <iostream>
#include <string>
#include "Symbol.h"
#include "Rule.h"
using namespace std;

class Node {

public:
	string sym;
	Node * parent;
	Node * kids[10];
	int kidcnt;
	int kid_id;

	Node();
	void createNode(string symb, int kcnt);
	void setKidId(int k);
	void setLinks(Node *p, Node *k[]);
	void display(Node *head);
	bool findNext(Node *traverse, Node *&currentNode);


};

/*class Symbol {
public:
	string sym;
	int symID;
	string symKind;


	Symbol();
	void setSym(string Sym, int Symid, string Symkind);
	void printSym();
};*/











#endif 
