/*TITLE
-----
CPSC 323-01 COMPILERS AND LANGUAGES: PROJECT PART #2- Parser
Group: BCJM
Members: Jared Fipps, Brian Gilman, Carmine Infusino, Mustafa Alkatat


CONTACT INFORMATION
-------------------
Jared Fipps: jfipps1995@csu.fullerton.edu

Brian Gilman: bjgilman@csu.fullerton.edu

Carmine Infusino: c.infusino@csu.fullerton.edu

Mustafa Alkatat: alkatat@csu.fullerton.edu

*/
#include "Lexer.h"
#include "Symbol.h"
#include "Rule.h"
#include "Token.h"
#include "Node.h"
#include <typeinfo>
#include <stack>

//enum declarations
enum nonTerm {Pgm = 1, Block, Stmts, Stmt, Astmt, Ostmt, Wstmt, Fstmt, Y, Else2, Elist, Elist2, E, T, F, Pexpr, Fatom, Opadd, Opmul, X, Z};
enum term { kwdprog = 1, brace1, brace2, semi, id, equal, kwdinput, kwdprint, paren1, paren2, kwdwhile, kwdif, kwdelseif, kwdelse, comma, kwdint, kwdfloat, kwdstring, plus, minus, aster, slash, caret, eps, $};

//Function Declarations
void symArraySetUp(Symbol SymArray[]);
void ruleArraySetUp(Symbol SymArray[], Rule RuleArray[]);
void vector_convertor(vector<vector<string>>, vector<int>&);
void termArraySetUp(Symbol findX[]);
int main()
{
	//Variable Declarations:
	vector<vector<Token>> lex_tokens;
	stack<Token> reverse_stack;
	stack<Token> token_stack;
	stack<Symbol> rule_stack;
	stack<Symbol> temp_stack;
	Token tokenArrayTemp[100];
	Symbol symArray[100];
	Rule ruleArray[100];
	Symbol xIndexSearch[50];
	Node *head_parse_tree = new Node();
	Node *traverse = head_parse_tree;
	Node *traverse_parent = NULL;
	Node *traverse_kids[10];
	int id;
	int tokenStackCount = 0;
	int yIndex = 0;
	int xIndex = 0;
	int ruleIndex = 0;
	int rhsKidCount = 0;
	//Fill Symbol and Rule Arrays
	symArraySetUp(symArray);
	ruleArraySetUp(symArray, ruleArray);

	//Filling xIndexSearch array
	termArraySetUp(xIndexSearch);

	//Runs lexer() to tokenize user input
	lex_tokens = lexer();
	if (lex_tokens[0][0].tokenName == "ERROR")
	{
		cout << "Error" << endl;
		cout << lex_tokens[0][0].tokenName << endl;
		system("pause");
		return 0; // Exits if a lexical error occurred
	}

	//Prints out tokens from vector before pushing on stack
	cout << "\nprinting token vector\n";
	for (int i = 0; i < lex_tokens.size(); i++)
	{
		for (int j = 0; j < lex_tokens[i].size(); j++)
		{
			cout << lex_tokens[i][j].tokenName << "\t" << lex_tokens[i][j].tokenValue << endl;
		}

	}

	//Stores token vector values into reverse_stack for reversal
	//Increments tokenStackCount
	for (int i = 0; i < lex_tokens.size(); i++)
	{
		for (int j = 0; j < lex_tokens[i].size(); j++)
		{
			reverse_stack.push(lex_tokens[i][j]);
			tokenStackCount++;
		}
	}

	//Stores reverse_stack values into tokenArrayTemp for symbol id assignment
	for (int i = 0; i < tokenStackCount; i++)
	{
		Token temp = reverse_stack.top();
		Token tempToken;
		tempToken.setToken(temp.tokenName);
		tempToken.tokenValue = temp.tokenValue;
		reverse_stack.pop();
		tokenArrayTemp[i] = tempToken;
	}

	//Setting symbol ids to token array
	for (int i = 0; i < tokenStackCount; i++)
	{
		for (int j = 0; j < 45; j++)
		{
			if (tokenArrayTemp[i].tokenName == symArray[j].sym)
			{
				tokenArrayTemp[i].tokenID = symArray[j].symID;
			}
		}
	}

	//Stores tokenArrayTemp values into token_stack
	for (int i = 0; i < tokenStackCount; i++)
	{
		token_stack.push(tokenArrayTemp[i]);
	}

	//outputs top of token_stack for checking
	cout << "\ntoken top:" << token_stack.top().tokenName;
	cout << endl;

	//push on $
	rule_stack.push(symArray[45]);

	//push on Pgm
	rule_stack.push(symArray[42]);

	//LL parse matrix
	int MTX[21][25] = {
		/*PGM*/{ 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 24 },
		/*Block*/{ -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Stmts*/{ -1, -1, 3, -1, 2, -1, -1, 2, -1, -1, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Stmt*/{ -1, -1, -1, -1, 4, -1, -1, 5, -1, -1, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Astmt*/{ -1, -1, -1, -1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Ostmt*/{ -1, -1, -1, -1, -1, -1, -1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Wstmt*/{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Fstmt*/{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Y*/{ -1, -1, -1, -1, 13, -1, 12, -1, 13, -1, -1, -1, -1, -1, -1, 13, 13, 13, -1, -1, -1, -1, -1, -1, -1 },
		/*Else2*/{ -1, -1, -1, 16, -1, -1, -1, -1, -1, -1, -1, -1, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Elist*/{ -1, -1, -1, -1, 17, -1, -1, -1, 17, 18, -1, -1, -1, -1, -1, 17, 17, 17, -1, -1, -1, -1, -1, -1, -1 },
		/*Elist2*/{ -1, -1, -1, -1, -1, -1, -1, -1, -1, 20, -1, -1, -1, -1, 19, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*E*/{ -1, -1, -1, -1, 21, -1, -1, -1, 21, -1, -1, -1, -1, -1, -1, 21, 21, 21, -1, -1, -1, -1, -1, -1, -1 },
		/*T*/{ -1, -1, -1, -1, 22, -1, -1, -1, 22, -1, -1, -1, -1, -1, -1, 22, 22, 22, -1, -1, -1, -1, -1, -1, -1 },
		/*F*/{ -1, -1, -1, -1, 23, -1, -1, -1, 24, -1, -1, -1, -1, -1, -1, 23, 23, 23, -1, -1, -1, -1, -1, -1, -1 },
		/*Pexpr*/{ -1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		/*Fatom*/{ -1, -1, -1, -1, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, 28, 29, -1, -1, -1, -1, -1, -1, -1 },
		/*Opadd*/{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, 31, -1, -1, -1, -1, -1 },
		/*Opmul*/{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, 33, 34, -1, -1 },
		/*X*/{ -1, -1, -1, 36, -1, -1, -1, -1, -1, 36, -1, -1, -1, -1, 36, -1, -1, -1, 35, 35, 36, 36, 36, -1, -1 },
		/*Z*/{ -1, -1, -1, 38, -1, -1, -1, -1, -1, 38, -1, -1, -1, -1, 38, -1, -1, -1, 38, 38, 37, 37, 37, -1, -1 },
};
	//LL Parser
	while (!token_stack.empty())
	{
		//cout << "RuleTop: " << rule_stack.top().sym << "\t" << "TokenTop: " << token_stack.top().tokenName << endl;
		//if top==front, put into parse tree and pop off stack
		while (rule_stack.top().sym == "eps" || rule_stack.top().sym == "")
		{
			//traverse->createNode((rule_stack.top()).sym, 0);

			//for (int i = 0; i < 10; i++)
			//{
			//	traverse_kids[i] = NULL;
			//}
			//traverse->setLinks(traverse_parent, traverse_kids);

			///*id = traverse->kid_id;
			//delete traverse_parent->kids[id];

			//traverse_parent->kidcnt = (traverse_parent->kidcnt) - 1;*/
			/*
			id = traverse->kid_id;
			id = id + 1;

		
			if (traverse_parent->kids[id])
			{
				traverse = traverse_parent->kids[id];
			}
			else
			{
				traverse->findNext(head_parse_tree, head_parse_tree);
				traverse_parent = traverse->parent;
			}*/
			/*traverse = traverse->parent->parent->kids[2];
			traverse_parent = traverse->parent;*/
			rule_stack.pop();
		}
		if (rule_stack.top().sym == token_stack.top().tokenName)
		{
			//cout << "RulePop: " << rule_stack.top().sym << "\t" << "TokenPop: " << token_stack.top().tokenName << endl;
			traverse->createNode((rule_stack.top()).sym, 0);

			for (int i = 0; i < 10; i++)
			{
				traverse_kids[i] = NULL;
			}
			traverse->setLinks(traverse_parent, traverse_kids);

			id = traverse->kid_id; 
			id = id + 1;

			if (traverse_parent->kids[id])
			{
				traverse = traverse_parent->kids[id];
			}
			/*else
			{
				traverse->findNext(head_parse_tree, head_parse_tree);
				traverse_parent = traverse->parent;
			}*/
			
			rule_stack.pop();
			token_stack.pop();
		}
		else
		{
			//if rule_stack.top() is terminal and does not match token_stack.top(), give error and end pgm
			if (rule_stack.top().symKind == "term" && rule_stack.top().sym != token_stack.top().tokenName)
			{
				cout << "Rule Stack Error: " << endl;
				while (!rule_stack.empty())
				{
					cout << rule_stack.top().sym << endl;
					rule_stack.pop();
				}
				cout << "Parsing Error" << endl;
				system("pause");
				return 0;
			}
			//if rule_stack.top() is nonterm, expand with rule_stack.top() RHS and pop current rule off stack
			else
			{
				//cout << "RuleStackTop: " << rule_stack.top().sym << "\t" << "TokenStackTop: " << token_stack.top().tokenName << endl;
				//Finds y index for LL Parse Matrix
				for (int i = 0; i < 38; i++)
				{
					if (rule_stack.top().sym == ruleArray[i].LHS.sym)
					{
						yIndex = ruleArray[i].ruleID - 1;
					}
				}
				//Finds x index for LL Parse Matrix
				for (int i = 0; i < 25; i++)
				{
					if (token_stack.top().tokenName == xIndexSearch[i].sym)
					{
						xIndex = i;
					}
				}

				ruleIndex = MTX[yIndex][xIndex];
				//cout << "Y: " << yIndex << "\t" << "X: " << xIndex << endl;
				//If cell is empty in LL Parse Matrix, error is given
				if (ruleIndex == -1)
				{
					while (!rule_stack.empty())
					{
						cout << rule_stack.top().sym << endl;
						rule_stack.pop();
					}
					cout << "Cell Empty... \t Parser Error";
					system("pause");
					return 0;
				}

				rhsKidCount = ruleArray[ruleIndex].rhsCount;
				//Places rule's RHS in temporary stack
				for (int i = 0; i < rhsKidCount; i++)
				{
					traverse_kids[i] = new Node();
					traverse_kids[i]->setKidId(i);
					temp_stack.push(ruleArray[ruleIndex].RHS[i]);
				}

				
				traverse->createNode((rule_stack.top()).sym, rhsKidCount);
				traverse->setLinks(traverse_parent, traverse_kids);

				traverse_parent = traverse;
				traverse = traverse_kids[0];
				
				//Pops current rule off stack
				rule_stack.pop();

				//Places new symbols on top of stack in reverse
				for (int i = 0; i < rhsKidCount; i++)
				{
					rule_stack.push(temp_stack.top());
					temp_stack.pop();
				}
			}
		}
	}
	
	cout << "Parser Tree" << endl;
	head_parse_tree->display(head_parse_tree);

	system("pause");
	return 0;
}

//Assigns symbols to symArray
#pragma region
void symArraySetUp(Symbol SymArray[])
{
	SymArray[0].setSym("kwdprog", 0, "term");
	SymArray[1].setSym("Block", 1, "NonT");
	SymArray[2].setSym("brace1", 2, "term");
	SymArray[3].setSym("Stmts", 3, "NonT");
	SymArray[4].setSym("brace2", 4, "NonT");
	SymArray[5].setSym("Stmt", 5, "NonT");
	SymArray[6].setSym("semi", 6, "term");
	SymArray[7].setSym("Astmt", 7, "NonT");
	SymArray[8].setSym("Ostmt", 8, "NonT");
	SymArray[9].setSym("Wstmt", 9, "NonT");
	SymArray[10].setSym("Fstmt", 10, "NonT");
	SymArray[11].setSym("id", 11, "term");
	SymArray[12].setSym("equal", 12, "term");
	SymArray[13].setSym("Y", 13, "NonT");
	SymArray[14].setSym("kwdinput", 14, "term");
	SymArray[15].setSym("E", 15, "NonT");
	SymArray[16].setSym("kwdprint", 16, "term");
	SymArray[17].setSym("paren1", 17, "term");
	SymArray[18].setSym("Elist", 18, "NonT");
	SymArray[19].setSym("paren2", 19, "term");
	SymArray[20].setSym("kwdwhile", 20, "term");
	SymArray[21].setSym("Pexpr", 21, "NonT");
	SymArray[22].setSym("kwdif", 22, "term");
	SymArray[23].setSym("Else2", 23, "NonT");
	SymArray[24].setSym("kwdelseif", 24, "term");
	SymArray[25].setSym("kwdelse", 25, "term");
	SymArray[26].setSym("Elist2", 26, "NonT");
	SymArray[27].setSym("comma", 27, "term");
	SymArray[28].setSym("T", 28, "NonT");
	SymArray[29].setSym("Opadd", 29, "NonT");
	SymArray[30].setSym("F", 30, "NonT");
	SymArray[31].setSym("Opmul", 31, "NonT");
	SymArray[32].setSym("Fatom", 32, "NonT");
	SymArray[33].setSym("kwdint", 33, "term");
	SymArray[34].setSym("kwdfloat", 34, "NonT");
	SymArray[35].setSym("kwdstring", 35, "term");
	SymArray[36].setSym("plus", 36, "term");
	SymArray[37].setSym("minus", 37, "term");
	SymArray[38].setSym("aster", 38, "term");
	SymArray[39].setSym("slash", 39, "term");
	SymArray[40].setSym("caret", 40, "term");
	SymArray[41].setSym("eps", 41, "term");
	SymArray[42].setSym("Pgm", 42, "NonT");
	SymArray[43].setSym("x", 43, "NonT");
	SymArray[44].setSym("z", 44, "NonT");
	SymArray[45].setSym("$", 45, "term");
}
#pragma endregion
//Assigns rules to ruleArray
#pragma region
void ruleArraySetUp(Symbol SymArray[], Rule RuleArray[])
{
	//PGM = kwdprog Block
	RuleArray[0].setRule(SymArray[42], 0, 2, 1);
	RuleArray[0].RHS[0] = SymArray[0];
	RuleArray[0].RHS[1] = SymArray[1];

	//Block = brace1 Stmts brace2
	RuleArray[1].setRule(SymArray[1], 1, 3, 2);
	RuleArray[1].RHS[0] = SymArray[2];
	RuleArray[1].RHS[1] = SymArray[3];
	RuleArray[1].RHS[2] = SymArray[4];

	//Stmts = Stmt semi Stmts
	RuleArray[2].setRule(SymArray[3], 2, 3, 3);
	RuleArray[2].RHS[0] = SymArray[5];
	RuleArray[2].RHS[1] = SymArray[6];
	RuleArray[2].RHS[2] = SymArray[3];

	//Stmt = eps
	RuleArray[3].setRule(SymArray[3], 3, 1, 3);
	RuleArray[3].RHS[0] = SymArray[41];

	//Stmt = Astmt
	RuleArray[4].setRule(SymArray[5], 4, 1, 4);
	RuleArray[4].RHS[0] = SymArray[7];

	//Stmt = Ostmt
	RuleArray[5].setRule(SymArray[5], 5, 1, 4);
	RuleArray[5].RHS[0] = SymArray[8];

	//Stmt = Wstmt
	RuleArray[6].setRule(SymArray[5], 6, 1, 4);
	RuleArray[6].RHS[0] = SymArray[9];

	//Stmt = Fstmt
	RuleArray[7].setRule(SymArray[5], 7, 1, 4);
	RuleArray[7].RHS[0] = SymArray[10];

	//Astmt = id equal Y
	RuleArray[8].setRule(SymArray[7], 8, 3, 5);
	RuleArray[8].RHS[0] = SymArray[11];
	RuleArray[8].RHS[1] = SymArray[12];
	RuleArray[8].RHS[2] = SymArray[13];

	//Ostmt = kwdprint paren1 Elist paren2
	RuleArray[9].setRule(SymArray[8], 11, 4, 6);
	RuleArray[9].RHS[0] = SymArray[16];
	RuleArray[9].RHS[1] = SymArray[17];
	RuleArray[9].RHS[2] = SymArray[18];
	RuleArray[9].RHS[3] = SymArray[19];

	//Wstmt = kwdwhile Pexpr Block
	RuleArray[10].setRule(SymArray[9], 12, 3, 7);
	RuleArray[10].RHS[0] = SymArray[20];
	RuleArray[10].RHS[1] = SymArray[21];
	RuleArray[10].RHS[2] = SymArray[1];

	//Fstmt = kwdif Pexpr Block Else2
	RuleArray[11].setRule(SymArray[10], 13, 4, 8);
	RuleArray[11].RHS[0] = SymArray[22];
	RuleArray[11].RHS[1] = SymArray[21];
	RuleArray[11].RHS[2] = SymArray[1];
	RuleArray[11].RHS[3] = SymArray[23];

	//Y = kwdinput
	RuleArray[12].setRule(SymArray[13], 9, 1, 9);
	RuleArray[12].RHS[0] = SymArray[14];

	//Y = E
	RuleArray[13].setRule(SymArray[13], 10, 1, 9);
	RuleArray[13].RHS[0] = SymArray[15];

	//Else2 = kwdelseif Pexpr Blcok Else2
	RuleArray[14].setRule(SymArray[23], 14, 4, 10);
	RuleArray[14].RHS[0] = SymArray[24];
	RuleArray[14].RHS[1] = SymArray[21];
	RuleArray[14].RHS[2] = SymArray[1];
	RuleArray[14].RHS[3] = SymArray[23];

	//Else2 = kwdelse Block
	RuleArray[15].setRule(SymArray[23], 15, 2, 10);
	RuleArray[15].RHS[0] = SymArray[25];
	RuleArray[15].RHS[1] = SymArray[1];

	//Else2 = eps
	RuleArray[16].setRule(SymArray[23], 16, 1, 10);
	RuleArray[16].RHS[0] = SymArray[41];

	//Elist = E Elist
	RuleArray[17].setRule(SymArray[18], 17, 2, 11);
	RuleArray[17].RHS[0] = SymArray[15];
	RuleArray[17].RHS[1] = SymArray[26];

	//Elist = eps
	RuleArray[18].setRule(SymArray[18], 18, 1, 11);
	RuleArray[18].RHS[0] = SymArray[41];

	//Elist2 = comma Elist
	RuleArray[19].setRule(SymArray[26], 19, 2, 12);
	RuleArray[19].RHS[0] = SymArray[27];
	RuleArray[19].RHS[1] = SymArray[18];

	//Elist2 = eps
	RuleArray[20].setRule(SymArray[26], 20, 1, 12);
	RuleArray[20].RHS[0] = SymArray[41];

	//E = Tx
	RuleArray[21].setRule(SymArray[15], 21, 2, 13);
	RuleArray[21].RHS[0] = SymArray[28];
	RuleArray[21].RHS[1] = SymArray[43];

	//T = Fz
	RuleArray[22].setRule(SymArray[28], 22, 2, 14);
	RuleArray[22].RHS[0] = SymArray[30];
	RuleArray[22].RHS[1] = SymArray[44];

	//F = Fatom
	RuleArray[23].setRule(SymArray[30], 23, 1, 15);
	RuleArray[23].RHS[0] = SymArray[32];

	//F = Pexpr
	RuleArray[24].setRule(SymArray[30], 24, 1, 15);
	RuleArray[24].RHS[0] = SymArray[21];


	//Pexpr = paren1 E paren2
	RuleArray[25].setRule(SymArray[21], 25, 3, 16);
	RuleArray[25].RHS[0] = SymArray[17];
	RuleArray[25].RHS[1] = SymArray[15];
	RuleArray[25].RHS[2] = SymArray[19];

	//Fatom = id
	RuleArray[26].setRule(SymArray[32], 26, 1, 17);
	RuleArray[26].RHS[0] = SymArray[11];

	//Fatom = kwdint
	RuleArray[27].setRule(SymArray[32], 27, 1, 17);
	RuleArray[27].RHS[0] = SymArray[33];

	//Fatom = float
	RuleArray[28].setRule(SymArray[32], 28, 1, 17);
	RuleArray[28].RHS[0] = SymArray[34];

	//Fatom = string
	RuleArray[29].setRule(SymArray[32], 29, 1, 17);
	RuleArray[29].RHS[0] = SymArray[35];

	//Opadd = plus
	RuleArray[30].setRule(SymArray[29], 30, 2, 18);
	RuleArray[30].RHS[0] = SymArray[36];

	//Opadd = minus
	RuleArray[31].setRule(SymArray[29], 31, 1, 18);
	RuleArray[31].RHS[0] = SymArray[37];

	//Opmul = aster
	RuleArray[32].setRule(SymArray[31], 32, 1, 19);
	RuleArray[32].RHS[0] = SymArray[38];

	//Opmul = slash
	RuleArray[33].setRule(SymArray[31], 33, 1, 19);
	RuleArray[33].RHS[0] = SymArray[39];

	//Opmul = caret
	RuleArray[34].setRule(SymArray[31], 34, 1, 19);
	RuleArray[34].RHS[0] = SymArray[40];

	//x = Opadd T x
	RuleArray[35].setRule(SymArray[43], 35, 3, 20);
	RuleArray[35].RHS[0] = SymArray[29];
	RuleArray[35].RHS[1] = SymArray[28];
	RuleArray[35].RHS[2] = SymArray[43];

	//x = eps
	RuleArray[36].setRule(SymArray[43], 36, 1, 20);
	RuleArray[36].RHS[0] = SymArray[41];


	//z = Opmul F z
	RuleArray[37].setRule(SymArray[44], 37, 3, 21);
	RuleArray[37].RHS[0] = SymArray[31];
	RuleArray[37].RHS[1] = SymArray[30];
	RuleArray[37].RHS[2] = SymArray[44];

	//z = eps
	RuleArray[38].setRule(SymArray[44], 38, 1, 21);
	RuleArray[38].RHS[0] = SymArray[41];
}

#pragma region
void termArraySetUp(Symbol findX[])
{
	findX[0].setSym("kwdprog", 0, "term");
	findX[1].setSym("brace1", 1, "term");
	findX[2].setSym("brace2", 2, "term");
	findX[3].setSym("semi", 3, "term");
	findX[4].setSym("id", 4, "term");
	findX[5].setSym("equal", 5, "term");
	findX[6].setSym("kwdinput", 6, "term");
	findX[7].setSym("kwdprint", 7, "term");
	findX[8].setSym("paren1", 8, "term");
	findX[9].setSym("paren2", 9, "term");
	findX[10].setSym("kwdwhile", 10, "term");
	findX[11].setSym("kwdif", 11, "term");
	findX[12].setSym("kwdelseif", 12, "term");
	findX[13].setSym("kwdelse", 13, "term");
	findX[14].setSym("comma", 14, "term");
	findX[15].setSym("kwdint", 15, "term");
	findX[16].setSym("kwdfloat", 16, "term");
	findX[17].setSym("kwdstring", 17, "term");
	findX[18].setSym("plus", 18, "term");
	findX[19].setSym("minus", 19, "term");
	findX[20].setSym("aster", 20, "term");
	findX[21].setSym("slash", 21, "term");
	findX[22].setSym("caret", 22, "term");
	findX[23].setSym("eps", 23, "term");
	findX[24].setSym("$", 24, "term");
}
#pragma endregion