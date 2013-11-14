/*
 * Game Engine for Maze game
 *
 * Purpose : CS2110 Project
 *
 * Created By : Minions 
 *
 * Date Created : 13-11-2013
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include "Queue3.h"
//#include "bot1.h"
#include "bot2.h"
#include "bot3.h"

#define NUM 50       //Number of small bonus
#define SMAX 10      //small bonus points
#define MAX 50       //reward points
#define WALLMAX 450  //no of walls
#define DIST_SIZE 2500
#define MAXSIZE 40   //size of matrix
#define TIME_DELAY_CONST 150000000 //time delay loop for display

//coloured text in terminal
#define KN  "\x1B[0m"
#define KR  "\x1B[31m"
#define KG  "\x1B[32m"
#define KY  "\x1B[33m"
#define KB  "\x1B[34m"
#define KM  "\x1B[35m"
#define KC  "\x1B[36m"
#define KW  "\x1B[37m"

/*
 * Function to update distance matrix(for final reward) for the maze recursively
 * @param maze
 * @param distance matrix
 * @param the queue
 */
void bfR_search(int  **maze,int **distance,Queue3 *bfs){
	
	
	if(bfs->first!=NULL){
		
			if(maze[bfs->first->x+1][bfs->first->y]!=-1){
				if(distance[bfs->first->x+1][bfs->first->y]==-1){
					distance[bfs->first->x+1][bfs->first->y]=distance[bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x+1,bfs->first->y);
				}
			}
		
		
			if(maze[bfs->first->x][bfs->first->y+1]!=-1){
				if(distance[bfs->first->x][bfs->first->y+1]==-1){
					distance[bfs->first->x][bfs->first->y+1]=distance[bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x,bfs->first->y+1);
				}
			}
		
		
			if(maze[bfs->first->x-1][bfs->first->y]!=-1){
				if(distance[bfs->first->x-1][bfs->first->y]==-1){
					distance[bfs->first->x-1][bfs->first->y]=distance[bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x-1,bfs->first->y);
				}
			}
		
			if(maze[bfs->first->x][bfs->first->y-1]!=-1){
				if(distance[bfs->first->x][bfs->first->y-1]==-1){
					distance[bfs->first->x][bfs->first->y-1]=distance[bfs->first->x][bfs->first->y]+1;
					bfs=queue_push3(bfs,bfs->first->x,bfs->first->y-1);
				}
			}
		
		
		bfs=queue_pop3(bfs);
		bfR_search(maze,distance,bfs);
		
		
	}
}

int main(int argc,char *argv[]){

	srand(time(NULL));

	int **maze;
	char **cmaze;
	
	maze=(int **)malloc(MAXSIZE*sizeof(int *));
	int i;
	for(i=0;i<MAXSIZE;i++)
		maze[i]=malloc(MAXSIZE*sizeof(int));
	int j;
	for(i=0;i<MAXSIZE;i++)
		for(j=0;j<MAXSIZE;j++)
			maze[i][j]=0;
	
	//create boundary around the maze
	for(i=0;i<MAXSIZE;i++){
		maze[i][0]=-1;
		maze[i][MAXSIZE-1]=-1;
		maze[0][i]=-1;
		maze[MAXSIZE-1][i]=-1;
	}
	
	//generate bonuses in the maze using rand()
	for(i=0;i<NUM;i++)
		maze[rand()%(MAXSIZE-2)+1][rand()%(MAXSIZE-2)+1]=SMAX;
		
	//generate walls in the maze	
	for(i=0;i<WALLMAX;i++)
		if(maze[rand()%(MAXSIZE-2)+1][rand()%(MAXSIZE-2)+1]==0)
			maze[rand()%(MAXSIZE-2)+1][rand()%(MAXSIZE-2)+1]=-1;
	
	//check that final reward is not cut off
	if(maze[MAXSIZE-2][MAXSIZE-3]==-1 && maze[MAXSIZE-3][MAXSIZE-2]==-1)
		maze[MAXSIZE-2][MAXSIZE-3]=0;
	maze[MAXSIZE-2][MAXSIZE-2]=MAX;
	
	int direction;
	int winner=0;	
	int score1=0,score2=0;
	int px1,py1,px2,py2;
	int dist;
	Queue3 *bfs=queue_new3();
	bfs=queue_push3(bfs,MAXSIZE-2,MAXSIZE-2);
	int **distance;
	
	distance=(int**)malloc((MAXSIZE)*sizeof(int));

	
	if(distance==NULL){
			return 0;
	}
		
	// generate character maze
	for(i=0;i<MAXSIZE;i++){
		distance[i]=(int*)malloc(MAXSIZE*sizeof(int));
		if(distance[i]==NULL){
			
			return 0;
		}
	}	
	cmaze=(char**)malloc((MAXSIZE)*sizeof(char*));

	
	if(cmaze==NULL){
			return 0;
	}
		

	for(i=0;i<MAXSIZE;i++){
		cmaze[i]=(char*)malloc(MAXSIZE*sizeof(char));
		if(cmaze[i]==NULL){
			
			return 0;
		}
	}
	for(i=0;i<MAXSIZE;i++){
		for(j=0;j<MAXSIZE;j++){
			if(i==px1 && j==py1)
				cmaze[i][j]='.';
			else if (i==px2 && j==py2)
				cmaze[i][j]='.';
			else if(maze[i][j]==0)
				cmaze[i][j]='.';

			if(maze[i][j]==-1)
				cmaze[i][j]='#';
			
			if(maze[i][j]>0){
				if(maze[i][j]==10)
					cmaze[i][j]='B';
				else
					cmaze[i][j]='R';
			}
		}
		
	}
	
	//update distance matrix with respect to final reward
	for(i=0;i<MAXSIZE;i++){
		for(j=0;j<MAXSIZE;j++){
			if(i==MAXSIZE-2&&j==MAXSIZE-3)
				distance[i][j]=0;
			else
				distance[i][j]=-1;
		}
	}
		       
	bfR_search(maze,distance,bfs);
	queue_delete3(bfs);
	
	
	//generate position of player1 and player2 which are equidistant from final reward
	//and at a cerain minimum distance
	int distq[DIST_SIZE][2];
	int size=0,i1,j1;
	px1=py1=px2=py2=0;
	while(!(px2+py2)||!(px1+py1)){
		dist=rand()%40+MAXSIZE-2;
		size=0;
		for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
			//put all the points at this randomly generated distance in a array
				if(distance[i][j]==dist){
					distq[size][0]=i;
					distq[size][1]=j;
					size++;
									
				}
			}	
		}
		//choose two points from the array
		//done to ensure that player1 and player2 start from different points
		if(size>=2){
			i1=rand()%size;
			j1=size-i1-1;
			px1=distq[i1][0];
			py1=distq[i1][1];
			px2=distq[j1][0];
			py2=distq[j1][1];
		}

	}
	int px1i,py1i,px2i,py2i;
	px1i=px1;
	px2i=px2;
	py1i=py1;
	py2i=py2;
	maze[px1][py1]=0;
	maze[px2][py2]=0;
	
	for(i=0;i<MAXSIZE;i++)
		free(distance[i]);
	free(distance);




	initialise3(maze);
	initialise2(maze);

	//check which two bots are playing
	if(atoi(argv[1])==1 && atoi(argv[2])==2 ||atoi(argv[2])==1 && atoi(argv[1])==2){ 	
	while(1){

		direction=bestmove3(maze,score1,score2,px1,py1,px2,py2);

		if(direction==3){
			if(maze[px1-1][py1]==-1){
				winner=2;
				break;
			}
			px1--;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;
		}
		if(direction==2){
			if(maze[px1][py1+1]==-1){
				winner=2;
				break;
			}
			py1++;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;
		
		}
		if(direction==1){
			if(maze[px1+1][py1]==-1){
				winner=2;
				break	;		
			}
			px1++;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;

		}
		if(direction==0){
			if(maze[px1][py1-1]==-1){
				winner=2;
				break;
			}
			py1--;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;

		}
		
		if((px1==(MAXSIZE-2)&&py1==(MAXSIZE-2))){
			if(score1>score2)
				winner=1;
			else if(score1<score2)
				winner=2;
			else
				winner=0;
			break;
		}
		
				
		for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s1 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s2 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					else
						printf("%sR %s",KM,KW);
				}
			}
			printf("\n");
		}
		printf("1:%d 2:%d\n",score1,score2);
		for(i=0;i<TIME_DELAY_CONST;i++);

		direction=bestmove2(maze,score2,score1,px2,py2,px1,py1);



		if(direction==3){
			if(maze[px2-1][py2]==-1){
				winner=1;
				break;
			}
			px2--;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;

		}
		if(direction==2){
			if(maze[px2][py2+1]==-1){
				winner=1;
				break;
			}
			py2++;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;

		}
		if(direction==1){
			if(maze[px2+1][py2]==-1){
				winner=1;
				break;
			}
			px2++;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;
	
		}
		if(direction==0){
			if(maze[px2][py2-1]==-1){
				winner=1;
				break;
			}
			py2--;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;
	
		}
		if((px2==(MAXSIZE-2)&&py2==(MAXSIZE-2))){
			if(score1>score2)
				winner=1;
			else if(score1<score2)
				winner=2;
			else
				winner=0;
			break;
		}
		for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s1 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s2 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					else
						printf("%sR %s",KM,KW);
				}
			}
			printf("\n");
		}
	
		printf("1:%d 2:%d\n",score1,score2);

	    for(i=0;i<TIME_DELAY_CONST;i++);   
	}

	for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s1 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s2 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					
				}
			}
			printf("\n");
	}
	for(i=0;i<TIME_DELAY_CONST;i++);   
	printf("%sScore for 1:%d %sScore for 2:%d\n",KB,score1,KB,score2);
	if(winner)
		printf("%sWINNER IS %d\n%s",KG,winner,KW);
	else
		printf("%sIt's a draw!%s\n",KG,KW);
	}
	
	else if(atoi(argv[1])==2 && atoi(argv[2])==3 ||atoi(argv[2])==2 && atoi(argv[1])==3){ 	
	

	while(1){

		direction=bestmove3(maze,score1,score2,px1,py1,px2,py2);

		if(direction==3){
			if(maze[px1-1][py1]==-1){
				winner=3;
				break;
			}
			px1--;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;
		}
		if(direction==2){
			if(maze[px1][py1+1]==-1){
				winner=3;
				break;
			}
			py1++;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;
		
		}
		if(direction==1){
			if(maze[px1+1][py1]==-1){
				winner=3;
				break	;		
			}
			px1++;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;

		}
		if(direction==0){
			if(maze[px1][py1-1]==-1){
				winner=3;
				break;
			}
			py1--;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;

		}
		if((px1==(MAXSIZE-2)&&py1==(MAXSIZE-2))){
			if(score1>score2)
				winner=2;
			else if(score1<score2)
				winner=3;
			else
				winner=0;
			break;
		}
		
				
		for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s2 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s3 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					else
						printf("%sR %s",KM,KW);
				}
			}
			printf("\n");
		}
		printf("2:%d 3:%d\n",score1,score2);
		for(i=0;i<TIME_DELAY_CONST;i++);

		direction=bestmove2(maze,score2,score1,px2,py2,px1,py1);



		if(direction==3){
			if(maze[px2-1][py2]==-1){
				winner=2;
				break;
			}
			px2--;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;

		}
		if(direction==2){
			if(maze[px2][py2+1]==-1){
				winner=2;
				break;
			}
			py2++;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;

		}
		if(direction==1){
			if(maze[px2+1][py2]==-1){
				winner=2;
				break;
			}
			px2++;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;
	
		}
		if(direction==0){
			if(maze[px2][py2-1]==-1){
				winner=2;
				break;
			}
			py2--;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;
	
		}
		if((px2==(MAXSIZE-2)&&py2==(MAXSIZE-2))){
			if(score1>score2)
				winner=2;
			else if(score1<score2)
				winner=3;
			else
				winner=0;
			break;
		}
		for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s2 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s3 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					else
						printf("%sR %s",KM,KW);
				}
			}
			printf("\n");
		}
	
		printf("2:%d 3:%d\n",score1,score2);

	    	for(i=0;i<TIME_DELAY_CONST;i++);   
	}
	for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s2 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s3 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					
				}
			}
			printf("\n");
	}
	for(i=0;i<TIME_DELAY_CONST;i++);

	printf("%sScore for 2:%d %sScore for 3:%d\n",KB,score1,KB,score2);
	if(winner)
		printf("%sWINNER IS %d\n%s",KG,winner,KW);
	else
		printf("%sIt's a draw!%s\n",KG,KW);
	} 
	
	else if(atoi(argv[1])==3 && atoi(argv[2])==1 ||atoi(argv[2])==3 && atoi(argv[1])==1){ 	
	

	while(1){

		direction=bestmove3(maze,score1,score2,px1,py1,px2,py2);

		if(direction==3){
			if(maze[px1-1][py1]==-1){
				winner=1;
				break;
			}
			px1--;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;
		}
		if(direction==2){
			if(maze[px1][py1+1]==-1){
				winner=1;
				break;
			}
			py1++;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;
		
		}
		if(direction==1){
			if(maze[px1+1][py1]==-1){
				winner=1;
				break	;		
			}
			px1++;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;

		}
		if(direction==0){
			if(maze[px1][py1-1]==-1){
				winner=1;
				break;
			}
			py1--;
			score1+=maze[px1][py1];
			maze[px1][py1]=0;

		}
		if((px1==(MAXSIZE-2)&&py1==(MAXSIZE-2))){
			if(score1>score2)
				winner=3;
			else if(score1<score2)
				winner=1;
			else
				winner=0;
			break;
		}
		
				
		for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s3 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s1 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					else
						printf("%sR %s",KM,KW);
				}
			}
			printf("\n");
		}
		printf("3:%d 1:%d\n",score1,score2);
		for(i=0;i<TIME_DELAY_CONST;i++);

		direction=bestmove2(maze,score2,score1,px2,py2,px1,py1);



		if(direction==3){
			if(maze[px2-1][py2]==-1){
				winner=3;
				break;
			}
			px2--;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;

		}
		if(direction==2){
			if(maze[px2][py2+1]==-1){
				winner=3;
				break;
			}
			py2++;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;

		}
		if(direction==1){
			if(maze[px2+1][py2]==-1){
				winner=3;
				break;
			}
			px2++;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;
	
		}
		if(direction==0){
			if(maze[px2][py2-1]==-1){
				winner=3;
				break;
			}
			py2--;
			score2+=maze[px2][py2];
			maze[px2][py2]=0;
	
		}
		if((px2==(MAXSIZE-2)&&py2==(MAXSIZE-2))){
			if(score1>score2)
				winner=3;
			else if(score1<score2)
				winner=1;
			else
				winner=0;
			break;
		}
		for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s3 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s1 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					else
						printf("%sR %s",KM,KW);
				}
			}
			printf("\n");
		}
	
		printf("3:%d 1:%d\n",score1,score2);

	    	for(i=0;i<TIME_DELAY_CONST;i++);   
	}
	
	for(i=0;i<MAXSIZE;i++){
			for(j=0;j<MAXSIZE;j++){
				if( px1==px2 && py1==py2 && i==px1 && j==py1)
					printf("%sT %s",KY,KW);
				else if(i==px1 && j==py1)
					printf("%s3 %s",KB,KW);
				else if (i==px2 && j==py2)
					printf("%s1 %s",KB,KW);
				else if(maze[i][j]==0)
					printf(". ");

				if(maze[i][j]==-1)
					printf("%s# %s",KR,KW);
			
				if(maze[i][j]>0){
					if(maze[i][j]==10)
						printf("%sB %s",KG,KW);
					
				}
			}
			printf("\n");
	}
	for(i=0;i<TIME_DELAY_CONST;i++);
	printf("%sScore for 3:%d %sScore for 1:%d\n",KB,score1,KB,score2);
	if(winner)
		printf("%sWINNER IS %s%d\n%s",KG,KB,winner,KW);
	else
		printf("%sIt's a draw!%s\n",KG,KW);
	} 
	

	for(i=0;i<MAXSIZE;i++){
		free(maze[i]);
		free(cmaze[i]);
	}
	free(cmaze);
	free(maze);
}






	
