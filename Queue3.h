/*
 * Queue Data Structure
 *
 * Purpose : CS2110 Project
 *
 * Created By : Minions 
 *
 * Date Created : 13-11-2013
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node_3 Node3;
struct Node_3{
	int x,y;
	Node3 *next;
};

Node3* node_new3(int x,int y,Node3 *next);

typedef struct Queue_3 {
	Node3 *first;
	Node3 *last;

} Queue3;




// Create a new empty queue
Queue3* queue_new3();
// Deletes the queue, frees memory.
void queue_delete3( Queue3* q );

// Inserts @val to the back of the queue
Queue3* queue_push3( Queue3* q, int x,int y );

// Remove the element at the front of the queue - also frees memory
Queue3* queue_pop3( Queue3* q );

#endif
