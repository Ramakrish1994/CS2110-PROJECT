typedef struct node_ node; 
#define MBON 51
struct node_{
	int bonus[MBON];
	int s1,s2,x1,y1,x2,y2;
};

int bestmove2(int**maze,int s1,int s2,int x1,int y1,int x2,int y2);
