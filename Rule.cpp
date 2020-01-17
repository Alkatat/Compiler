#include "Rule.h"
#include <iostream>
#include <string>

using namespace std;

Rule::Rule()
{
	LHS.setSym("", 0, "");
	RHS[0] = {};
	ruleIndexNumber = 0;
	rhsCount = 0;
	ruleID = 0;
}

void Rule::setRule(Symbol lhs, int index, int count, int id)
{
	LHS = lhs;
	rhsCount = count;
	ruleIndexNumber = index;
	ruleID = id;
}

void Rule::printRule()
{
	cout << "LHS:" << endl;
	LHS.printSym();
	cout << endl;
	cout << "RHS: " << endl;
	for (int i = 0; i < rhsCount; i++)
	{
		RHS[i].printSym();
	}
	cout << endl;
	cout << ruleIndexNumber << endl;
	cout << rhsCount << endl;
	cout << "ID: " << ruleID << endl;
}