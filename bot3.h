/*
 * bot data stucture and functions 
 *
 * Purpose : CS2110 Project
 *
 * Created By : Minions 
 *
 * Date Created : 13-11-2013
 *
 */
 

#define INF3 1000000000
#define MBON3 51//Max number of small bonus
#define DEP3 6 // max depth it should go
#define MULCONST3 500 
#define SIZE3 40 // Size of maze
#define SBONUS3 10 //small bonus points
#define REWARD3 50 // big reward points
#define DEEPCONST3 30
typedef struct node_3 node3; 
struct node_3{
	int bonus[MBON3];
	int s1,s2,x1,y1,x2,y2;
};

int bestmove3(int**maze,int s1,int s2,int x1,int y1,int x2,int y2);

void initialise3(int **maze);

