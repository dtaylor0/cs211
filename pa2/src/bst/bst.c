#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1

typedef struct node{
	int data;
	struct node *right;
	struct node *left;
} node;

node* insert(node *, int);
node* delete(node *, int);
void search(node *, int);
void print(node *);
int main() {
	node *root;
	root = NULL;
        int value;
        char command;
	while(scanf(" %c", &command) == 1) {
		if (command != 'i' && command != 'd' && command != 'p' && command != 's') {
			return(-1);
		}
		if (command == 'p') {
			print(root);
			printf("\n");
		} else {
			if(scanf(" %d", &value) != 1) {
				return -1;
			}
			if(command == 'd') {
				root = delete(root, value);
			} else if(command == 's') {
				search(root, value);
			} else if(command == 'i') {
				root = insert(root, value);
			} else {
				return 0;
			}
		}
	}
	return 0;
}

void print(node *root) {
	node *ptr;
	ptr = root;
	if(ptr == NULL) {
		char empty[] = "\0";
		printf("%s", empty);
		return;
	} else {
		printf("(");
		print(ptr->left);
		printf("%d", ptr->data);
		print(ptr->right);
		printf(")");
	}
	return;
}

node* insert(node *root, int new_value) {
	node *new;
	new = (node *)malloc(sizeof(node));
	new->data = new_value;
	new->left = NULL;
	new->right = NULL;
	node *ptr = root;
	if(root == NULL) {
		printf("inserted\n");
		return new;
	} else if(root->left == NULL && root->right == NULL) {
		if(root->data < new->data) {
			root->right = new;
			printf("inserted\n");
		} else if(root->data > new->data) {
			root->left = new;
			printf("inserted\n");
		} else {
			printf("duplicate\n");
			free(new);
			return root;
		}
		return root;
	} else {
		while(TRUE) {
			if(new->data > ptr->data) {
				if(ptr->right == NULL) {
					ptr->right = new;
					printf("inserted\n");
					return root;
				} else {
					ptr = ptr->right;
					continue;
				}
			} else if(new->data < ptr->data) {
				
				if(ptr->left == NULL) {
					ptr->left = new;
					printf("inserted\n");
					return root;
				} else {
					ptr = ptr->left;
					continue;
				}
			} else {
				printf("duplicate\n");
				free(new);
				return root;
			}
		}
	}
	return NULL;
}

node* delete(node *root, int value) {
	node *ptr;
	ptr = root;
	node *parent;
	parent = NULL;
	if(ptr == NULL) {
		printf("absent\n");
	} else {
		while(TRUE) {
			if(ptr->data == value) {
				if(ptr->left != NULL && ptr->right != NULL) {
					node *mini_ptr;
					mini_ptr = ptr->left;
					node *mini_parent;
					mini_parent = ptr;
					while(mini_ptr->right != NULL) {
						mini_parent = mini_ptr;
						mini_ptr = mini_ptr->right;
					}
					if(mini_parent == root) {
						mini_ptr->right = root->right;
						root = mini_ptr;
						printf("deleted\n");
					} else {
						ptr->data = mini_ptr->data;
						mini_parent->right = mini_ptr->left;
						mini_ptr = NULL;
						printf("deleted\n");
					}
					return root;
				} else if(ptr->left != NULL) {
					if(parent == NULL) {
						ptr = ptr->left;
						printf("deleted\n");
						return ptr;
					} 
					if(parent->left == ptr) {
						parent->left = ptr->left;
						printf("deleted\n");
					} else if(parent->right == ptr) {
						parent->right = ptr->left;
						printf("deleted\n");
					}
					ptr = NULL;
					return root;
				} else if(ptr->right != NULL) {
					if(parent == NULL) {
						ptr = ptr->right;
						printf("deleted\n");
						return ptr;
					} 
					if(parent->left == ptr) {
						parent->left = ptr->right;
						printf("deleted\n");
					} else if(parent->right == ptr) {
						parent->right = ptr->right;
						printf("deleted\n");
					}
					ptr = NULL;
					return root;
					
				} else {
					if(parent == NULL) {
						ptr = NULL;
						printf("deleted\n");
						return ptr;
					}
					if(parent->left == ptr) {
						parent->left = NULL;
						printf("deleted\n");
					} else if(parent->right == ptr) {
						parent->right = NULL;
						printf("deleted\n");
					}
					ptr = NULL;
					return root;
				}
			} else if(ptr->data < value) {
				parent = ptr;
				ptr = ptr->right;
				if(ptr == NULL) {
					printf("absent\n");
					return root;
				}
				continue;
			} else if(ptr->data > value) {
				parent = ptr;
				ptr = ptr->left;
				if(ptr == NULL) {
					printf("absent\n");
					return root;
				}
				continue;
				
			}
			return root;
		}
	}
	return root;
}

void search(node *root, int value) {
	node *ptr;
	ptr = root;
	if(ptr == NULL) {
		printf("absent\n");
		return;
	} else {
		while(TRUE) {
			if(ptr->data == value) {
				printf("present\n");
				return;
			} else {
				if(ptr->data < value) {
					ptr = ptr->right;
					if(ptr == NULL) {
						printf("absent\n");
						return;
					}
				} else if(ptr->data > value) {
					ptr = ptr->left;
					if(ptr == NULL) {
						printf("absent\n");
						return;
					}
				}
			}
		}
	}
	return;
}
