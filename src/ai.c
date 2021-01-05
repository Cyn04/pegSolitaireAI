/**
 * Cynthia Joseph (938358)
 * COMP20003 Algorithms and Data Structures
 * Second (Spring) Semester 2020 [Assignment 3] - 1/11/2020
 * AI Solver for Peg Solitaire
*/



#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"
#include "assert.h"


void copy_state(state_t* dst, state_t* src){
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
    new_n->num_child = 0;
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
*/
node_t* applyAction(node_t* n, state_t *s, position_s* selected_peg, move_t action ){
		//allocates memory and updates new node.
    node_t *new_node = (node_t*)malloc(sizeof(*new_node));
		assert(new_node);
    new_node->depth = n->depth + 1;
    new_node->move = action;
    copy_state(&(new_node->state), s);
    new_node->parent = n;
		new_node->num_child = 0;
		n->num_child++;
		return new_node;
}


/**
 * Find a solution path as per algorithm description in the handout
 */

void find_solution( state_t* init_state  ){
	//Initializes stack, hashtable and starting node
	initialize_stack();
	node_t* n = create_init_node( init_state );
	HashTable table;
	ht_setup( &table, sizeof(int8_t) * SIZE * SIZE, sizeof(int), 16769023);
	stack_push(n);
	int remaining_pegs = num_pegs(&(n->state));

	while (!is_stack_empty()){
		//Used as "value" in hashtable
    int dummy = 0;

		//Takes node from top of stack
    node_t *node = stack_top();
    stack_pop();
		expanded_nodes++;

		//Updates best solution if lesser remaining pegs found
		if (num_pegs(&(node->state))<remaining_pegs){
			save_solution(node);
			remaining_pegs = num_pegs(&(node->state));
		}

		//Traverses the board for possible moves
		for (int i = 0; i<SIZE; i++ ){
			for (int j = 0; j<SIZE; j++){
				for (int k = left; k<=down; k++){
					position_s selected_peg;
					selected_peg.x = i;
					selected_peg.y = j;
					if ( can_apply((&(node->state)), &selected_peg, k)){

						//Changes the state of the board
            state_t tmp;
						copy_state(&tmp, &(node->state));
						tmp.cursor.x = selected_peg.x;
						tmp.cursor.y = selected_peg.y;
						execute_move_t( &tmp, &selected_peg, k );
						generated_nodes++;

						//Only calls applyAction if a new node is pushed into stack (to save malloc calls).
						if (won(&tmp)){
							//Saves winning solution before freeing everything
            	node_t *new_node = applyAction(node, &tmp, &selected_peg, k);
              assert(new_node);
              stack_push(new_node);
							save_solution(new_node);
							remaining_pegs = num_pegs(&(new_node->state));
							ht_destroy(&table);
							free_stack();
							return;
						}
						//Inserts unique states to stack and hashtable.
						if (!(ht_contains(&table, &tmp))){
            	node_t *new_node = applyAction(node, &tmp, &selected_peg, k);
            	assert(new_node);
							stack_push(new_node);
							ht_insert(&table, &(new_node->state), &dummy);
						}
					}
				}
			}
		}

		//Frees the nodes using freenodes function in stack.h
    if (node->num_child == 0){
			freenodes(node);
    }

		//Returns if expanded nodes exceeded given budget
		if (expanded_nodes>= budget){
			ht_destroy(&table);
			free_stack();
			return;
		}
	}
}
