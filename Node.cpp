#include "Node.h"

Node::Node()
{
	for (int i = 0; i < 10; i++)
	{
		kids[i] = NULL;
	}
	sym = "";
	kidcnt = 0;
}

void Node::createNode(string symb, int kcnt)
{
	sym = symb;
	kidcnt = kcnt;
}

void Node::setKidId(int k)
{
	kid_id = k;
}

void Node::setLinks(Node *p, Node *k[])
{
	parent = p;
	for (int i = 0; i < 10; i++)
	{
		kids[i] = k[i];
	}
}

void Node::display(Node * head)
{
	Node *traverse = head;
	cout << "Node: " << traverse->sym;
	if(head->parent)
		cout << " | Parent = " << head->parent->sym << " | Node'skid id: " << traverse->kid_id << endl;
	else
		cout << " | No Parent, Node's kid id: " << traverse->kid_id << endl;
	cin.get();
	for (int i = 0; i < 10; i++)
	{
		if (traverse->kids[i])
		{
			display(traverse->kids[i]);
		}
		else
		{
			break;
		}
	}
}

// 
bool  Node::findNext(Node *traverse, Node *&currentNode)
{
	bool key = false;
	if (traverse->sym.compare("") == 0)
	{
		currentNode = traverse;
		key = true;
	}

	if (key != true)
	{
		for (int i = 0; i < 10; i++)
		{
			if (traverse->kids[i])
			{
				key = findNext(traverse->kids[i], currentNode);
				if (key == true)
				{
					break;
				}
			}
		}
	}
	return key;
}
