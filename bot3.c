/*
 * Bot for Maze game using Minimax algorithm with alpha beta pruning
 *
 * Purpose : CS2110 Project
 *
 * Created By : Minions 
 *
 * Date Created : 13-11-2013
 *
 */
#include<stdio.h>
#include "Queue3.h"
#include "bot3.h"

int dist3[MBON3][SIZE3][SIZE3]; //distance of every point from a bonus 
int labelvalue3[MBON3];         //value of bonus corresponding to that label
int label3[SIZE3][SIZE3];       // Mapping from bonus to integers
/*
 * Function to do BFS
 * @param maze, the distance matrix, size, queue, the label of the bonus for which bfs is done
 * 
  */
void bf_search3(int **maze,int distance[MBON3][SIZE3][SIZE3],int N,Queue3 *bfs,int blabel){
	
	
	if(bfs->first!=NULL){

		
		
		if(bfs->first->x<SIZE3-1){
			if(maze[bfs->first->x+1][bfs->first->y]!=-1){
				if(dist3[blabel][bfs->first->x+1][bfs->first->y]==-1){
					dist3[blabel][bfs->first->x+1][bfs->first->y]=dist3[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x+1,bfs->first->y);
				}
			}
		}
		
		if(bfs->first->y<N-1){
			if(maze[bfs->first->x][bfs->first->y+1]!=-1){
				if(dist3[blabel][bfs->first->x][bfs->first->y+1]==-1){
					dist3[blabel][bfs->first->x][bfs->first->y+1]=dist3[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x,bfs->first->y+1);
				}
			}
		}
		
		if(bfs->first->x>0){
			if(maze[bfs->first->x-1][bfs->first->y]!=-1){
				if(dist3[blabel][bfs->first->x-1][bfs->first->y]==-1){
					dist3[blabel][bfs->first->x-1][bfs->first->y]=dist3[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x-1,bfs->first->y);
				}
			}
		}
		
		if(bfs->first->y>0){
			if(maze[bfs->first->x][bfs->first->y-1]!=-1){
				if(dist3[blabel][bfs->first->x][bfs->first->y-1]==-1){
					dist3[blabel][bfs->first->x][bfs->first->y-1]=dist3[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x,bfs->first->y-1);
				}
			}
		}
		
		bfs=queue_pop3(bfs);
		bf_search3(maze,dist3,SIZE3,bfs,blabel);
		
		
	}
}

/*
 function to initialise distances from all bonus to all other squares.
  this function also creates a mapping from bonus to a subset of integers
  * @params the maze
   */
void initialise3(int **maze){
//fill label value,label,dist3
	int i,j,k;
	int lval=0;
	for(i=0;i<SIZE3;i++)
		for(j=0;j<SIZE3;j++){
			
			if(maze[i][j]==SBONUS3){
				label3[i][j]=lval;
				labelvalue3[lval]=SBONUS3;
				lval++;
			}
			else if(maze[i][j]==REWARD3){
				label3[i][j]=lval;
				labelvalue3[lval]=REWARD3;
				lval++;
			}
			else
				label3[i][j]=-1;
	}
	for(i=0;i<MBON3;i++)
		for(j=0;j<SIZE3;j++)
			for(k=0;k<SIZE3;k++)
				dist3[i][j][k]=-1;
	
		
	for(i=0;i<SIZE3;i++)
		for(j=0;j<SIZE3;j++){
			int blabel=label3[i][j];
	
			if(maze[i][j]==SBONUS3||maze[i][j]==REWARD3){
				Queue3 *bfs=queue_new3();
				bfs=queue_push3(bfs,i,j);
				dist3[blabel][i][j]=0;
				bf_search3(maze,dist3,SIZE3,bfs,blabel);
				queue_delete3(bfs);
			}
			
			
		}


}
/*
 Function to return if the game is over
 * @params the current state in game s
 * @returns an integer
  */
int gameover3(node3 s){
	if((s.x1==SIZE3-1&& s.y1==SIZE3-1)|| (s.x2==SIZE3-1&& s.y2==SIZE3-1))
		return 1;
	return 0;
}

/*
 * Function that returns gain (or loss) as a function of distances from each bonus times a weightage
   given to the bonus
 * @params the x and y co-ordinate of the point and the bonus array saying which all bonus are still present in the matrix
 * @returns an integer the auxilary gain i.e.weighted distance from rewards
    */
int gain_aux3R(int x,int y,int bonus[]){
	int sum=0,i,j,k;
	
	
	
	for(i=0;i<MBON3;i++){
		
		sum+=(labelvalue3[i]*bonus[i])*dist3[i][x][y];
		if(labelvalue3[i]==REWARD3)
			sum+=7*labelvalue3[i]*bonus[i]*dist3[i][x][y];
	}

	return sum;	
}




/*
 Computes the gain for the particular state st
 * @params the state of game st
 * @returns the gain
  */
int gain3(node3 st){
	
		return (gain_aux3R(st.x2,st.y2,st.bonus)-gain_aux3R(st.x1,st.y1,st.bonus)+MULCONST3*(st.s1-st.s2));
}


/*
 This function is called by the game engine and it returns the direction
 * @params maze,current scores of players,current locations of players
 * @returns direction
 */
int bestmove3(int**maze,int s1,int s2,int x1,int y1,int x2,int y2){
	
	node3 root;
	int direction;
	int i,j,depth=0;
	for(i=0;i<MBON3;i++)
		root.bonus[i]=0;
	for(i=0;i<SIZE3;i++)
		for(j=0;j<SIZE3;j++)
			if(maze[i][j]!=-1 && maze[i][j]!=0)
				root.bonus[label3[i][j]]=1;
	root.s1=s1;root.s2=s2;root.x1=x1;root.y1=y1;
	root.x2=x2;root.y2=y2;
	int alpha=-INF3,beta=INF3;
	maxim3(maze,root,alpha,beta,depth,&direction);
	return direction;
}
/*
 * function to copy a node
  */
void copy3(node3* a,node3 * b){
	int i; 
	for(i=0;i<MBON3;i++)
		(*a).bonus[i]=(*b).bonus[i];
	a->s1=b->s1;
	a->s2=b->s2;
	a->x1=b->x1;
	a->y1=b->y1;
	a->x2=b->x2;
	a->y2=b->y2;
	}
	
int splcase3(int ** maze,int x,int y){
    int ct=0;
    if(maze[x+1][y]==-1)
                        ct++;
    if(maze[x-1][y]==-1)
                        ct++;
    if(maze[x][y+1]==-1)
                        ct++;
    if(maze[x][y-1]==-1)
                        ct++;
    if(ct>2 && maze[x][y]<=0)
            return 0;
    return 1;
}

/*
 * Function which given a state finds the maximum depth our player can get if its our move
 * @params maze,state of game, alpha (lower limit of score possible),beta (upper limit of score possible)
 * depth gone,direction
 * @returns Max gain possible for player 1
 */
int maxim3(int ** maze,node3 state,int alpha,int beta, int depth,int* dir){
	if(gameover3(state) || depth>DEP3)
		return (gain3(state)-SBONUS3*2*depth);
	int bestmove=-INF3;
	node3 temp;int direction;
	if(maze[state.x1+1][state.y1]!=-1 && splcase3(maze,state.x1+1,state.y1)){
		copy3(&temp,&state);
		temp.x1++;
		if(maze[state.x1+1][state.y1]>0){
			temp.bonus[label3[state.x1+1][state.y1]]=0;
			temp.s1+=labelvalue3[label3[state.x1+1][state.y1]];
			
		}
			
		int tmp=minim3(maze,temp,alpha,beta,depth,&direction);
		
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=1;
			if(bestmove>alpha)
				alpha=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x1][state.y1-1]!=-1 && splcase3(maze,state.x1,state.y1-1)){
		copy3(&temp,&state);
		temp.y1--;
		if(maze[state.x1][state.y1-1]>0){
			temp.bonus[label3[state.x1][state.y1-1]]=0;
			temp.s1+=labelvalue3[label3[state.x1][state.y1-1]];
			
		}
			
		int tmp=minim3(maze,temp,alpha,beta,depth,&direction);
		
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=0;
			if(bestmove>alpha)
				alpha=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 
	
	if(maze[state.x1][state.y1+1]!=-1 && splcase3(maze,state.x1,state.y1+1)){
		copy3(&temp,&state);
		temp.y1++;
		if(maze[state.x1][state.y1+1]>0){
			temp.bonus[label3[state.x1][state.y1+1]]=0;
			temp.s1+=labelvalue3[label3[state.x1][state.y1+1]];
		
		}
			
		int tmp=minim3(maze,temp,alpha,beta,depth,&direction);
		
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=2;
			if(bestmove>alpha)
				alpha=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x1-1][state.y1]!=-1 && splcase3(maze,state.x1-1,state.y1)){
		copy3(&temp,&state);
		temp.x1--;
		if(maze[state.x1-1][state.y1]>0){
			temp.bonus[label3[state.x1-1][state.y1]]=0;
			temp.s1+=labelvalue3[label3[state.x1-1][state.y1]];
		}
			
		int tmp=minim3(maze,temp,alpha,beta,depth,&direction);
		
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=3;
			if(bestmove>alpha)
				alpha=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 

	return bestmove;
	
}
	

/*
 * Function that finds the maximum gain that our bot is assured if its player 2's move in the given state
 * @params maze,state of game, alpha (lower limit of score possible),beta (upper limit of score possible)
 * depth gone,direction
 * @returns the max gain possible for player 1 if player 2 has to play in this given state
  */
int minim3(int ** maze,node3 state,int alpha,int beta, int depth,int* dir){
	depth++;
	if(gameover3(state) || depth>=DEP3)
		return (gain3(state)-DEEPCONST3*2*(depth+1));
	
	int bestmove=INF3;
	node3 temp;int direction;
	if(maze[state.x2+1][state.y2]!=-1 && splcase3(maze,state.x2+1,state.y2)){
		copy3(&temp,&state);
		temp.x2++;
		if(maze[state.x2+1][state.y2]>0){
			temp.bonus[label3[state.x2+1][state.y2]]=0;
			temp.s2+=labelvalue3[label3[state.x2+1][state.y2]];
		}
			
		int tmp=maxim3(maze,temp,alpha,beta,depth,&direction);
		
		if(tmp<bestmove){
			bestmove=tmp;
			*dir=1;
			if(bestmove<beta)
				beta=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x2][state.y2-1]!=-1 &&  splcase3(maze,state.x2,state.y2-1)){
		copy3(&temp,&state);
		temp.y2--;
		if(maze[state.x2][state.y2-1]>0){
			temp.bonus[label3[state.x2][state.y2-1]]=0;
			temp.s2+=labelvalue3[label3[state.x2][state.y2-1]];
		}
			
		int tmp=maxim3(maze,temp,alpha,beta,depth,&direction);
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=0;
			if(bestmove<beta)
				beta=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 
	
	if(maze[state.x2][state.y2+1]!=-1 && splcase3(maze,state.x2,state.y2+1)){
		copy3(&temp,&state);
		temp.y2++;
		if(maze[state.x2][state.y2+1]>0){
			temp.bonus[label3[state.x2][state.y2+1]]=0;
			temp.s2+=labelvalue3[label3[state.x2][state.y2+1]];
		}
			
		int tmp=maxim3(maze,temp,alpha,beta,depth,&direction);
		if(tmp<bestmove){
			bestmove=tmp;
			*dir=2;
			if(bestmove<beta)
				beta=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x2-1][state.y2]!=-1 && splcase3(maze,state.x2-1,state.y2)){
		copy3(&temp,&state);
		temp.x2--;
		if(maze[state.x2-1][state.y2]>0){
			temp.bonus[label3[state.x2-1][state.y2]]=0;
			temp.s2+=labelvalue3[label3[state.x2-1][state.y2]];
		}
			
		int tmp=maxim3(maze,temp,alpha,beta,depth,&direction);
		if(tmp<bestmove){
			bestmove=tmp;
			*dir=3;
			if(bestmove<beta)
				beta=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 

	return bestmove;
	
}
	
