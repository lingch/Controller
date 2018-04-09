

Node* addNode(Node *head, void *data){
	Node *newNode = (Node*)malloc(sizeof(Node)/sizeof(u8));

	if(head == NULL){
		newNode->prev = newNode->next = NULL;
	}else{
		newNode->next = head;
		newNode->prev = NULL;
		head->prev = newNode;
	}

	return newNode;
}

Node* deleteNode(Node *head, void *data){
	
	Node *p = head;

	while(p){
		
	}

	//head is the only element in this list
	if(head->prev == head->next == NULL){
		head = NULL;
	}

	if(p->prev){
		//p is not the head
		((TimerTask*)(p->prev))->next = p->next;
	}else{
		//p is the head
		*ppTaskHead = p->next;
	}

	if(p->next){
		//p is not the tail
		((TimerTask*)(p->next))->prev=p->prev;
	}

	free(p);
}