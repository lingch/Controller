#ifndef _MY_LIST_H_
#define _MY_LIST_H_

#include	"STC15Fxxxx.H"

struct Node;

typedef struct _Node
{
	struct _Node *prev;
	struct _Node *next;
	void *pData;
} Node;

Node* addNode(Node *head, void *pData);
Node* deleteNode(Node *head, void *pData);


#endif
