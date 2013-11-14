/*
 * Queue Implementation
 * 
 * Purpose : CS2110 Project
 *
 * Created By : Minions 
 *
 * Date Created : 13-11-2013
 *
 */


#include "Queue3.h"
#include<stdlib.h>
/*
 *funcion to create a new node in the queue
 *@param coordinate x
 *@param coordinate y
 *@param next node
 *@return created node
 */
Node3 *node_new3(int x,int y,Node3* next){

	Node3 *new_node=(Node3 *)malloc(sizeof(Node3));
	if(new_node==NULL){
		return NULL;
	}
	new_node->x=x;
	new_node->y=y;
	new_node->next=next;
	return new_node;
}

/*
 *function to create a new queue
 *@return the queue
 */
Queue3 *queue_new3(){

	Queue3* q=(Queue3*)malloc(sizeof(Queue3));
	if(q==NULL){
		
		return NULL;
	}

	q->first=NULL;
	q->last=NULL;
	return q;
}

/*
 *function to delete elements of the queue and free memory
 *@param the queue
 */
void queue_delete3(Queue3 *q){

	Node3 *ptr=q->first;
	
	while(ptr!=NULL){
		Node3 *p=ptr->next;
		free(ptr);
		ptr=p;
	}
	free(q);
}

/*
 *function to push a element to the end of the queue
 *@param the queue
 *@return the queue
 */
Queue3 *queue_push3(Queue3* q,int x,int y){

	if(q->first==NULL){
		q->last=node_new3(x,y,NULL);
		q->first=q->last;
	}
	else{
		Node3 *ptr=node_new3(x,y,NULL);
		q->last->next=ptr;
		q->last=ptr;
	}
	return q;
}

/*
 *function to delete element at the top of the queue
 *@param the queue
 *@return the queue
 */
Queue3* queue_pop3(Queue3 *q){

	if(q->first==NULL){
		return q;
		
	}
	else if(q->first==q->last){
		Node3 *ptr=q->first;
		free(ptr);
		q->first=NULL;
		q->last=NULL;
	}
	else{
		Node3 *ptr=q->first;
		q->first=ptr->next;
		free(ptr);
	}
	return q;

}




        

