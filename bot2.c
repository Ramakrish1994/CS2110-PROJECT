#include<stdio.h>
#include "Queue3.h"
#define INF 1000000000
#define MBON 51
#define DEP 5
#define MULCONST 500
#define SIZE 40
#define SBONUS 10
#define REWARD 50
#define DEEPCONST 30
typedef struct node_ node; 
struct node_{
	int bonus[MBON];
	int s1,s2,x1,y1,x2,y2;
};
//int visited[SIZE][SIZE];
int dist[MBON][SIZE][SIZE];
int labelvalue[MBON];//value of bonus corresponding to that label
int label[SIZE][SIZE];// Mapping from bonus to integers

void bf_search(int **maze,int distance[MBON][SIZE][SIZE],int N,Queue3 *bfs,int blabel){
	
	
	if(bfs->first!=NULL){

		
		
		if(bfs->first->x<SIZE-1){
			if(maze[bfs->first->x+1][bfs->first->y]!=-1){
				if(dist[blabel][bfs->first->x+1][bfs->first->y]==-1){
					dist[blabel][bfs->first->x+1][bfs->first->y]=dist[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x+1,bfs->first->y);
				}
			}
		}
		
		if(bfs->first->y<N-1){
			if(maze[bfs->first->x][bfs->first->y+1]!=-1){
				if(dist[blabel][bfs->first->x][bfs->first->y+1]==-1){
					dist[blabel][bfs->first->x][bfs->first->y+1]=dist[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x,bfs->first->y+1);
				}
			}
		}
		
		if(bfs->first->x>0){
			if(maze[bfs->first->x-1][bfs->first->y]!=-1){
				if(dist[blabel][bfs->first->x-1][bfs->first->y]==-1){
					dist[blabel][bfs->first->x-1][bfs->first->y]=dist[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x-1,bfs->first->y);
				}
			}
		}
		
		if(bfs->first->y>0){
			if(maze[bfs->first->x][bfs->first->y-1]!=-1){
				if(dist[blabel][bfs->first->x][bfs->first->y-1]==-1){
					dist[blabel][bfs->first->x][bfs->first->y-1]=dist[blabel][bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x,bfs->first->y-1);
				}
			}
		}
		
		bfs=queue_pop3(bfs);
		bf_search(maze,dist,SIZE,bfs,blabel);
		
		
	}
}


void initialise2(int **maze){
//fill label value,label,dist
	int i,j,k;
	int lval=0;
	for(i=0;i<SIZE;i++)
		for(j=0;j<SIZE;j++){
			//visited[i][j]=0;
			if(maze[i][j]==SBONUS){
				label[i][j]=lval;
				labelvalue[lval]=SBONUS;
				lval++;
			}
			else if(maze[i][j]==REWARD){
				label[i][j]=lval;
				labelvalue[lval]=REWARD;
				lval++;
			}
			else
				label[i][j]=-1;
	}
	for(i=0;i<MBON;i++)
		for(j=0;j<SIZE;j++)
			for(k=0;k<SIZE;k++)
				dist[i][j][k]=-1;
	
		
	for(i=0;i<SIZE;i++)
		for(j=0;j<SIZE;j++){
			int blabel=label[i][j];
	
			if(maze[i][j]==SBONUS||maze[i][j]==REWARD){
				Queue3 *bfs=queue_new3();
				bfs=queue_push3(bfs,i,j);
				dist[blabel][i][j]=0;
				bf_search(maze,dist,SIZE,bfs,blabel);
				queue_delete3(bfs);
			}
			
			
		}


}

int gameover(node s){
	if((s.x1==SIZE-1&& s.y1==SIZE-1)|| (s.x2==SIZE-1&& s.y2==SIZE-1))
		return 1;
	return 0;
}

int gain_aux(int x,int y,int bonus[]){
	int sum=0,i;
	for(i=0;i<MBON;i++){
		sum+=(labelvalue[i]*bonus[i]/*+(visited[x][y])*(visited[x][y])*(visited[x][y])*/)*dist[i][x][y];
		if(labelvalue[i]==REWARD)
			sum+=7*labelvalue[i]*bonus[i]*dist[i][x][y];
		}

	return sum;	
}

int gain(node st){
	return (gain_aux(st.x2,st.y2,st.bonus)-gain_aux(st.x1,st.y1,st.bonus))+MULCONST*(st.s1-st.s2);
}



int bestmove2(int**maze,int s1,int s2,int x1,int y1,int x2,int y2){
	//visited[x1][y1]++;
	node root;
	int direction;
	int i,j,depth=0;
	for(i=0;i<MBON;i++)
		root.bonus[i]=0;
	for(i=0;i<SIZE;i++)
		for(j=0;j<SIZE;j++)
			if(maze[i][j]!=-1 && maze[i][j]!=0)
				root.bonus[label[i][j]]=1;
	root.s1=s1;root.s2=s2;root.x1=x1;root.y1=y1;
	root.x2=x2;root.y2=y2;
	int alpha=-INF,beta=INF;
	maxim(maze,root,alpha,beta,depth,&direction);
	return direction;
}

void copy(node* a,node * b){
	int i; 
	for(i=0;i<MBON;i++)
		(*a).bonus[i]=(*b).bonus[i];
	a->s1=b->s1;
	a->s2=b->s2;
	a->x1=b->x1;
	a->y1=b->y1;
	a->x2=b->x2;
	a->y2=b->y2;
	}

int maxim(int ** maze,node state,int alpha,int beta, int depth,int* dir){
	if(gameover(state) || depth>DEP)
		return (gain(state)-DEEPCONST*2*depth);
	int bestmove=-INF;
	node temp;int direction;
	if(maze[state.x1+1][state.y1]!=-1){
		copy(&temp,&state);
		temp.x1++;
		if(maze[state.x1+1][state.y1]>0){
			temp.bonus[label[state.x1+1][state.y1]]=0;
			temp.s1+=labelvalue[label[state.x1+1][state.y1]];
		}
			
		int tmp=minim(maze,temp,alpha,beta,depth,&direction);
		//if(depth==0)
		//printf("%d %d %d \n",tmp,state.x1+1,state.y1);
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=1;
			if(bestmove>alpha)
				alpha=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x1][state.y1-1]!=-1){
		copy(&temp,&state);
		temp.y1--;
		if(maze[state.x1][state.y1-1]>0){
			temp.bonus[label[state.x1][state.y1-1]]=0;
			temp.s1+=labelvalue[label[state.x1][state.y1-1]];
		}
			
		int tmp=minim(maze,temp,alpha,beta,depth,&direction);
		//if(depth==0)
		//printf("%d %d %d \n",tmp,state.x1,state.y1-1);
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=0;
			if(bestmove>alpha)
				alpha=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 
	
	if(maze[state.x1][state.y1+1]!=-1){
		copy(&temp,&state);
		temp.y1++;
		if(maze[state.x1][state.y1+1]>0){
			temp.bonus[label[state.x1][state.y1+1]]=0;
			temp.s1+=labelvalue[label[state.x1][state.y1+1]];
		}
			
		int tmp=minim(maze,temp,alpha,beta,depth,&direction);
		//if(depth==0)
		//printf("%d %d %d \n",tmp,state.x1,state.y1+1);
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=2;
			if(bestmove>alpha)
				alpha=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x1-1][state.y1]!=-1){
		copy(&temp,&state);
		temp.x1--;
		if(maze[state.x1-1][state.y1]>0){
			temp.bonus[label[state.x1-1][state.y1]]=0;
			temp.s1+=labelvalue[label[state.x1-1][state.y1]];
		}
			
		int tmp=minim(maze,temp,alpha,beta,depth,&direction);
		//if(depth==0)		
		//printf("%d %d %d \n",tmp,state.x1-1,state.y1);
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
	


int minim(int ** maze,node state,int alpha,int beta, int depth,int* dir){
	depth++;
	if(gameover(state) || depth>=DEP)
		return (gain(state)-DEEPCONST*2*(depth+1));
	
	int bestmove=INF;
	node temp;int direction;
	if(maze[state.x2+1][state.y2]!=-1){
		copy(&temp,&state);
		temp.x2++;
		if(maze[state.x2+1][state.y2]>0){
			temp.bonus[label[state.x2+1][state.y2]]=0;
			temp.s2+=labelvalue[label[state.x2+1][state.y2]];
		}
			
		int tmp=maxim(maze,temp,alpha,beta,depth,&direction);
		
		if(tmp<bestmove){
			bestmove=tmp;
			*dir=1;
			if(bestmove<beta)
				beta=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x2][state.y2-1]!=-1){
		copy(&temp,&state);
		temp.y2--;
		if(maze[state.x2][state.y2-1]>0){
			temp.bonus[label[state.x2][state.y2-1]]=0;
			temp.s2+=labelvalue[label[state.x2][state.y2-1]];
		}
			
		int tmp=maxim(maze,temp,alpha,beta,depth,&direction);
		if(tmp>bestmove){
			bestmove=tmp;
			*dir=0;
			if(bestmove<beta)
				beta=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 
	
	if(maze[state.x2][state.y2+1]!=-1){
		copy(&temp,&state);
		temp.y2++;
		if(maze[state.x2][state.y2+1]>0){
			temp.bonus[label[state.x2][state.y2+1]]=0;
			temp.s2+=labelvalue[label[state.x2][state.y2+1]];
		}
			
		int tmp=maxim(maze,temp,alpha,beta,depth,&direction);
		if(tmp<bestmove){
			bestmove=tmp;
			*dir=2;
			if(bestmove<beta)
				beta=bestmove;
		}
		if(alpha>beta)
			return bestmove;
	} 


	if(maze[state.x2-1][state.y2]!=-1){
		copy(&temp,&state);
		temp.x2--;
		if(maze[state.x2-1][state.y2]>0){
			temp.bonus[label[state.x2-1][state.y2]]=0;
			temp.s2+=labelvalue[label[state.x2-1][state.y2]];
		}
			
		int tmp=maxim(maze,temp,alpha,beta,depth,&direction);
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
	
