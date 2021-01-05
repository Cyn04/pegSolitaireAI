#include<stdio.h>
#include "stack.h"
#include "utils.h"
#include "hashtable.h"

void initialize_stack(){ stack_top_idx = -1; }

void stack_push(node_t* n) {
  if(stack_top_idx == STACK_SIZE -1) { // overflow case.
		printf("Error: stack overflow\n");
		return;
	}
	stack[++stack_top_idx] = n;
}

void stack_pop() {
	if(stack_top_idx == -1) { // If stack is empty, pop should throw error.
		printf("Error: No element to pop\n");
		return;
	}
	stack_top_idx--;
}

node_t* stack_top() { return stack[stack_top_idx]; }

int is_stack_empty() {
    if(stack_top_idx == -1) return 1;
    return 0;
}

//CHANGED - function for printing node states for checking purposes
void print_stack() {
  printf("\n\n\n\n\n\n\n");
	printf("Stack: ");
	for(int i = 0;i<=stack_top_idx;i++){
    printf("{{{{%d}}}}\n",i);
		for (int y = 0; y < SIZE; y++){
      for (int x = 0; x < SIZE; x++){
        printf("[%c]", stack[i]->state.field[x][y]);
      }
      printf("\n");
    }
  }
	printf("\n");
}

//CHANGED - frees a node and its parents.
void freenodes(node_t* n){
	node_t *curr = n, *tmp;
    while(curr != NULL){
        tmp = curr;
        curr = curr->parent;
        if (tmp->num_child == 0){
            if (tmp->parent!=NULL && tmp->parent->num_child > 0) tmp->parent->num_child--;
            free(tmp);
        }
    }
}

//CHANGED - frees the nodes inside the stack.
void free_stack() {
    for (int i = 0; i <= stack_top_idx; i++){
        freenodes(stack[i]);
    }
    stack_top_idx = -1;
	printf("\n");
}
