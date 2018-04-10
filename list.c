#include "list.h"
#include <stdlib.h>

Node* addNode(Node *head, void *pData){
	Node *newNode = (Node*)malloc(sizeof(Node)/sizeof(u8));
	newNode->pData = pData;
	
	if(head == NULL){
		newNode->prev = newNode->next = NULL;
	}else{
		newNode->next = head;
		newNode->prev = NULL;
		head->prev = newNode;
	}

	return newNode;
}

Node* deleteNode(Node *head, void *pData){
	
	Node *p = head;

	while(p){
		if(p->pData == pData){
			//target node found
			if(head == p){
				head = NULL;
			}else{
				p->prev->next = p->next;
				if(p->next){
					p->next->prev = p->prev;
				}
			}
			free(p);
			return head;
		}
		p=p->next;
	}
}