#ifndef RULE_H
#define RULE_H

#include <iostream>
#include <string>
#include "Symbol.h"
using namespace std;
/*BEGINNING OF RULE CLASS*/
class Rule {
public:
	Symbol LHS;
	Symbol RHS[10];
	int ruleIndexNumber;
	int ruleID;
	int rhsCount;
	Rule();
	void setRule(Symbol lhs, int index, int count, int id);
	void printRule();
};





#endif RULE_H