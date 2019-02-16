#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int data;
	struct node *next;
} node;

node* insert(node *, int);

void print(node *);

node* delete(node *, int);

int main(int argc, char *argv[]) {
	node *root;
	root = NULL;
	int value;
	char command;
	while(scanf(" %c %d", &command, &value) == 2) {
		if (command != 'i' && command != 'd') {
			return(-1);
		}
		if (command == 'i') {
			root = insert(root, value);
			print(root);
		} else if(command == 'd') {
			root = delete(root, value);
			print(root);
		} else {
			return 0;
		}
	}
	return 0;
}

void print(node *ptr) {
	node *temp = ptr;
	int counter = 1;
	if(temp == NULL) {
		printf("0 :\n");
		return;
	}
	while(temp->next != NULL) {
		temp = temp->next;
		counter++;
	}
	printf("%d : ", counter);
	temp = ptr;
	while(temp->next != NULL) {
		printf("%d ", temp->data);
		temp = temp->next;
	}
	printf("%d\n", temp->data);
	return;
}

node* insert(node *ptr, int newValue) {
	node *temp = ptr;
	node *prev = NULL;
	node *new_node;
	new_node = malloc(sizeof(node));
	new_node->data = newValue;
        new_node->next = NULL;
	if(temp == NULL) {
		ptr = new_node;
	} else if(temp->next == NULL) {
		if(temp->data < newValue) {
			temp->next = new_node;
		} else if(temp->data > newValue) {
			new_node->next = temp;
			ptr = new_node;
		}
	} else {
		while(temp->next != NULL) {
			if(temp->data > newValue) {
				if(prev == NULL) {
					new_node->next = temp;
					ptr = new_node;
					return ptr;
				}
				new_node->next = temp;
				prev->next = new_node;
				return ptr;
			} else if(temp->data == newValue) {
				return ptr;
			}
			prev = temp;
			temp = temp->next;
		}
                if(temp->data > newValue) {
                        new_node->next = temp;
                        prev->next = new_node;
                        return ptr;
                } else if(temp->data == newValue) {
                	return ptr;
                }
		if(temp->next == NULL) {
                	if(temp->data < newValue) {
                        	temp->next = new_node;
                	}else if(temp->data > newValue) {
                        	new_node->next = temp;
                        	prev = new_node;
                	}
		}
		
	}
	return ptr;
}


node* delete(node *ptr, int value) {
	node *temp = ptr;
	node *prev = NULL;
	if(temp == NULL) {
		return ptr;	
	} else if(temp->next == NULL) {
		if(temp->data == value) {
			return NULL;
		}
	}
	while(temp->next != NULL) {
		if(temp->data == value) {
			if(prev != NULL) {
				prev->next = temp->next;
				temp = NULL;
				return ptr;
			} else {
				ptr = ptr->next;
				return ptr;
			}
		}
		prev = temp;
		temp = temp->next;
	}
	if(temp->data == value) {
		if(prev != NULL) {
			prev->next = NULL;
			temp = NULL;
			return ptr;
		} else {
			ptr = NULL;
			return ptr;
		}
	}
	return ptr;
}
