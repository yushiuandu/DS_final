#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>

#define SCREEN_HEIGHT 500 //�]�w�C���������� 
#define SCREEN_WIDTH 500 //�]�w�C�������e��
#define GRID_SIDE 40 //�]�w�C����}�C���l�ƶq 
#define LEFT_MARGINE 30 //�]�w����� 
#define TOP_MARGINE 40 //�]�w�W��� 
#define FOOD_AMOUT 1 //�]�w�C�����ͭ����ƶq 
#define INIT_SPEED  0 //�]�w��l�g���D���ʳt�� (80)

//�ŧi�e�i��V�C�|��� 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//�ŧi�C�����X�{����C�|��� 
enum Object {
	EMPTY, //�ť� 
	WALL, //��λ�ê�� 
	FOOD //���� 
};

//�ŧi�g���D����`�I���c 
struct Node {
	int row; //�`�I��b�ĴX�� 
	int col; //�`�I��b�ĴX�C 
	Direction direct; //�Ӹ`�I���e�i��V 
	struct Node *next;	//���V�U�@�Ӹ`�I 
	struct Node *nexthead; //������ 
}; 

//�w�q���V�`�I���c�������ܼ� 
typedef struct Node *NodePointer;


//�ŧi���|�`�I 
struct path{
	int row; //�`�I��b�ĴX�� 
	int col; //�`�I��b�ĴX�C 
	int g_cost;  
	int h_cost; 
	int f_cost;
	struct path *parent;  //���V���`�I 
}; 
//�w�q���V�`�I���c�������ܼ� 
typedef struct path *pathptr;

void openWindow(); //�}�Ҵ������� 
void closeGame(NodePointer snake); //�B�z�C�������޿� 
int playGame(int field[][GRID_SIDE], NodePointer snake); //�C���i���޿� 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]); //�g���D�O�_���`(���`����G����M����ۤv����) 
int showGameOverMsg(); //�C�������T��
void showInfo(); //��ܹC��������T 
void drawGameField(int field[][GRID_SIDE]); //ø�s�C���ϰ� 
void drawSquare(int row, int col, int color); //ø�s��� 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake); //Ū����L��V��J�A�ó]�w��Ҧ��g���D�`�I 
void moveNext(NodePointer snake); //ø�s�g���D�C�e�i�@�B���������  
void createFood(int field[][GRID_SIDE], NodePointer snake,int foodamount); //���ͭ��� 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //�P�_�O�_������  
bool IsAtSnake(NodePointer snake, int row, int col); //�P�_�O�_����g���D�ۤv������
void eatFood(int field[][GRID_SIDE], NodePointer snake); //�B�z�Y�쭹���޿� 
void addNode(NodePointer snake); //�W�[�g���D����`�I 

//--------------------------------------AI--------------------------------------
Direction AI(int field[][GRID_SIDE], NodePointer snake);//��@�q������g���D�޿�
Direction shortestpath(NodePointer snake,NodePointer target,int field[][GRID_SIDE],int *flag);//�M�w�̵u���|����V 
void deletefood(NodePointer head,NodePointer snake);//�R�������`�I 
void addfoodnode(NodePointer head,int row,int col);//�s�W�����`�I 
NodePointer snakecopy(NodePointer snack);//�ƻs�@�������D 
void fake_moveNext(NodePointer snake);//��������D���� 
void control_fake_SnakeDirection(int field[][GRID_SIDE], NodePointer snake);//�M�w�����D��V 
bool fake_eatFood(int field[][GRID_SIDE], NodePointer snake);//�P�_�����D���S���Y�쭹�� 
NodePointer findtail(NodePointer snake);//��X�����D������ 
Direction tailpath(NodePointer snake,NodePointer target,int field[][GRID_SIDE],int *flag);//��X������D���ڪ����| 
Direction farfromfood(NodePointer snake,int field[][GRID_SIDE]);//�������̻�����l 
pathptr farfromfoodnode(int row,int col,NodePointer target);//�������̻�����l���I
bool Isinfoodlist(NodePointer head,int row,int col);//�P�_�Ӯy�ЬO�_������ 
bool IsAtSnake_fake(NodePointer snake, int row, int col);//�P�_�O�_���I������D������ 
void freesnake(NodePointer snake);//��������D���O����Ŷ� 
void control_fake_SnakeDirection_farfromfood(NodePointer snake, int key);//�M�w�����D�n�����F�~�`�I 
void freeclosedlist(int n);//����close list���O����Ŷ� 
void freeopenlist(int n);//����open lsit���O����Ŷ� 
//------------------------function for shortest path-------------------------------
pathptr removefromopen_min();//�q�̤p��n��open list������ 
void addtoclosed(pathptr newadd);//�[�J��close list�� 
void addtoopen_min(pathptr newadd);//�[�J��̤p��n��open list�� 
pathptr create(int row,int col,pathptr current,NodePointer target);//�s�W�F�~�`�I 
int isinclose(pathptr detect);//�P�_�O�_�s�bclose list�� 
int isinopen(pathptr detect);//�P�_�O�_�s�bopen list��		
int h_cost(pathptr detect,NodePointer target); //���oh_cost 
int getoldpath(pathptr detect);//���o���e�����| 
void updatepath(pathptr detect,pathptr current,NodePointer target); //��s�̵u���|
//------------------------function for shortest path-------------------------------


int speed; //�g���D���ʳt�� 
int scoreSum = 0; //�������� 
int totalTime = 0; //�����C���ɶ� 
int const scorePerFood = 1; //�C�@�������i�o���� 
bool meetSelfWillDie = TRUE; //�M�w�g���D����ۤv����O�_�|���� 

//------------------------------------AI----------------------------------------
bool IFPlayAI = TRUE; //�O�_�}��AI�Ҧ� 
pathptr open[GRID_SIDE*GRID_SIDE];	      //A* algorithm
pathptr closed[GRID_SIDE*GRID_SIDE];	  //A* algorithm
int on=0;	//����open list�̪��`�I�� 
int cn=0;	//����closed list�̪��`�I�� 
pathptr neighbor[4];	//��e�`�I���F�~(4�Ӥ�V) 
NodePointer foodlisthead=NULL; //������C�_�I 
NodePointer snakehead=NULL;
      
int main( )
{  	
	openWindow();
	while(TRUE){
		
		Node head = {2, 2, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V 
		NodePointer snake = &head;       //���V�D�Y������ 
		snakehead = &head;	//�]�w�D�Y�����ܼ� 
		speed = INIT_SPEED;				//�]�w��l�t�� 
		foodlisthead=NULL;              //�]�w��l�����`�I 
		//�]�w�C�����M��ê�� 
	   	int field[GRID_SIDE][GRID_SIDE] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		   								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};  
	   	
		char key;
		key = playGame(field, snake); //�i��C��
		if (key == 'q' || key == 'Q')
			closeGame(snake); //�p�G���a��J'q'���}�C��	
		else if (key == 's' || key == 'S')
			scoreSum = 0;
			totalTime = 0;
			continue; //�p�G���a��J's' �~��C�� 		    
	}
}

//�}�Ҵ�������
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE *3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry Snake Game");
}

//�B�z�C�������޿� 
void closeGame(NodePointer snake){
	free(snake);
	exit(0);
}

//�C���i���޿�
int playGame(int field[][GRID_SIDE], NodePointer snake){
	
	drawGameField(field); //ø�s�C���ϰ� 
	createFood(field, snake,FOOD_AMOUT); //���ͲĤ@������ 
	while(true){	
		controlSnakeDirection(field, snake); //Ū�����a��J��V��A�ñN�s��V�]�w��U�g���D�`�I 
		moveNext(snake); //�̾ڸ`�I����V�Aø�s�s���g���D��m 
		eatFood(field, snake); //�P�_�O�_���Y�쭹���A�p�G���s�W�`�I����� 
		showInfo(); //��ܮɶ��M���Ƹ�T 
		
		if(IsGameOver(snake, field)) //�P�_�O�_�ŦX�C����������A 
			return showGameOverMsg(); //��ܹC�������T���A�õ��ݪ��a��J�ﶵ 
		
		//���F�Y�쭹���|���ͷs�����A�t�Τ]�H�����ͷs���� 
		/*
		srand((unsigned)time(NULL));
		if (rand() % 20 == 0)
			createFood(field, snake);		
		*/
		delay(speed);//�M�w�g���D���ʳt�סAspeed�V�p���ʶV�� 
		//Ū���D��V�䪺��L��L��J 
		if(kbhit()){
			int key;
			key = getch();
			//�u����J�j�p�g��a, q �M s�A�t�Τ~�������ñN��J���G�ߨ�~�h���ݳB�z 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 		
			else if (key == 'a') //�M�w�O�_���ܼҦ� �A�D�n�����a�Ҧ��M�q���ޱ���AI�Ҧ� 
				IFPlayAI = !IFPlayAI;
		}	
	}
			
}

//ø�s�C���ϰ�A�̾ڹC�����x�}�]�wø�s���� 
void drawGameField(int field[][GRID_SIDE]){
   	int row = 0, col = 0;
   	cleardevice(); //�M�z�ù��e�� 
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
				switch(field[row][col])	{
					case WALL: //��b�x�}�����ȬO1 
						drawSquare(row, col, YELLOW);
						break;
					case FOOD: //�����b�x�}�����ȬO2
						drawSquare(row, col, GREEN);
						break;	
				}	
		   }
	}
}

//ø�s���
void drawSquare(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //�]�wø�s���󪺬���ߩM�C�� 
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}

//ø�s�g���D�C�e�i�@�B��������� 
void moveNext(NodePointer snake){
	int currRow, currCol;
	NodePointer head = snake;
	
	while (snake != NULL){
		currRow = snake->row;
		currCol = snake->col;
		
		//�̾ڸ`�I����V�ݩʡA�]�w���ʤU�@�B����m	
		switch(snake->direct){
			case RIGHT:
				snake->col++;
				break;			
			case LEFT:
				snake->col--;
				break;
			case UP:
				snake->row--;
				break;			
			case DOWN:
				snake->row++;
				break;
		}
		
		drawSquare(snake->row, snake->col, RED);
		
		if (!IsAtSnake(snake, currRow, currCol))
			drawSquare(currRow, currCol, BLACK);
			
		snake = snake -> next;	
	}
	drawSquare(head->row, head->col, BLUE);
}

//�g���D�O�_���`(���`����G����M����ۤv����) 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]){
	NodePointer head = snake;
	
	//�P�_�O�_������ 
	if (IsAtWall(field, head->row, head->col))
		return true;
	
	//�P�_�O�_����g���D�ۤv 
	if(meetSelfWillDie)
		return IsAtSnake(snake->next, head->row, head->col);
	
	return false;		
}

//�P�_�O�_������
bool IsAtWall(int field[][GRID_SIDE], int row, int col){
		if (field[row][col] == WALL)
			return true;
		return false;
}

//�P�_�O�_����g���D�ۤv������
bool IsAtSnake(NodePointer snake, int row, int col){

	while(snake != NULL){
		if (row == snake->row && col == snake->col)
			return true;
		snake = snake -> next;		
	}
	return false;
}

//�C�������T��
int showGameOverMsg(){
   	//cleardevice(); //�M�z�Ҧ��ù���ơA�p�G�Ʊ�u��ܰT���ɡA�������� 
	int i = 0;
	char msg1[15] = "Game Over!!";
	char msg2[40] = "press [q] to quit or [s] to restart!!";
   	for(; ; i++){
	   	setcolor(i%14);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 7);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);
	   	
	   	setcolor(WHITE);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 2);
	   	outtextxy(20, SCREEN_HEIGHT / 2 + 70, msg2);
	   	
	   	delay(200);
	   	
	   	setcolor(BLACK);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 7);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);	   
		   	
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S') {
				return key; 				
			}
		}
	}	
}

//��ܹC��������T
void showInfo(){
	totalTime += speed;
	totalTime += 15;
	char timeMsg[45] = " Time:";
	char scoreMsg[45] = "Score:";
	char modeMsg[20] = "";
	char optMsg1[50] = "press [q] to quit, [s] to restart or";
	char optMsg2[50] = "press [a] to change mode.";
	
	char time[10];
	char score[10];
	
	sprintf(time, "%5d", totalTime/1000);
	strcat(timeMsg, time);
	strcat(timeMsg, " sec.");
	
	setcolor(WHITE); //�]�w��r�C�� 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1); //�]�w�r���A�����Ϋ����M�r���j�p 
   	outtextxy(0, 0, timeMsg); //�̾ڧ��п�X��r��ù�	
	
	sprintf(score, "%5d", scoreSum);
	strcat(scoreMsg, score);
	strcat(scoreMsg, " point.");

	setcolor(WHITE);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, 19, scoreMsg);	
   	
   	if(IFPlayAI){
   		strcat(modeMsg, "AI Mode    ");  		
	}else{
   		strcat(modeMsg, "Player Mode");  		
	}
			
   	setcolor(LIGHTMAGENTA);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(SCREEN_HEIGHT - LEFT_MARGINE * 2, 0, modeMsg);
   	
   	setcolor(LIGHTGREEN);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE, optMsg1);
	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE + 20, optMsg2);	      	
}

//Ū����L��V��J�A�ó]�w��Ҧ��g���D�`�I 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake){
	
	Direction snakeDirect = snake -> direct;
	
	//get key code by pressing keybord
	int key;
	if(kbhit()){
		key = getch();
	}
		
	//decide snake's moving direction	
	switch(key){
		case KEY_RIGHT:
			snakeDirect = RIGHT;
			break;			
		case KEY_LEFT:
			snakeDirect = LEFT;
			break;		
		case KEY_UP:
			snakeDirect = UP;
			break;				
		case KEY_DOWN:
			snakeDirect = DOWN;
			break;				
	}
	
	if (IFPlayAI){
		snakeDirect = AI(field, snake);
	}

	//���F�F�칳�D�@�˲��ʤ覡�A��D���ܤ�V�ɡA�ä��O��Ө���@�_���ܡA
	//�ӬO����ʦa�A�U�Ӹ`�I�v�@���ܡA��@�Ӹ`�I����s���e�i��V�ɡA
	//�|�N�ۤv�ثe����V�ǵ��U�@�ӡC 
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}

//���ͭ���
void createFood(int field[][GRID_SIDE], NodePointer snake,int foodamount){
	int row, col, i;
	NodePointer temp=foodlisthead;
	srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 
	for(i=0; i< foodamount; i++){
		//�p�G�üƲ��ͪ���m�O�b��M�D���魫�|�A�h���s���͡A����ŦX���󬰤� 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || Isinfoodlist(temp,row,col));
		field[row][col] = FOOD;
		drawSquare(row,col, GREEN);	
		addfoodnode(temp,row,col);	
	}
}

//�B�z�Y�쭹���޿�
void eatFood(int field[][GRID_SIDE], NodePointer snake){
	//�p�G�D�Y�P������m���|�A�N�O�Y�쭹�� 
	if(field[snake->row][snake->col] == FOOD){
		field[snake->row][snake->col] = EMPTY; //�N�ӭ����M��
		printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
		addNode(snake); //�W�[�`�I 
		scoreSum = scoreSum + scorePerFood; //�������� 
		//�R���ӭ����`�I 
		NodePointer head=foodlisthead;
		deletefood(head,snake);
		createFood(field,snake,1); //���ͷs������ 
	}
}

//�W�[�g���D����`�I
void addNode(NodePointer snake){
	NodePointer tail = NULL;
	NodePointer newNode = (NodePointer) malloc(sizeof(Node)); //��l�Ƥ@�ӷs�`�I 
	//�M��D������ 
	while(snake != NULL){
		tail = snake;
		snake = snake -> next;
	}
	//�N�D���ڪ���V�ݩʵ��s�`�I 
	newNode -> direct = tail -> direct;
	
	//�M�w�s�`�I����m�A�̾ڧ��ڪ����ʤ�V�ӱ��W�s�`�I 
	switch(tail->direct){
		case RIGHT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col - 1;
			break;			
		case LEFT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col + 1;			
			break;
		case UP:
			newNode -> row = tail -> row + 1;
			newNode -> col = tail -> col;			
			break;			
		case DOWN:
			newNode -> row = tail -> row - 1;
			newNode -> col = tail -> col;			
			break;
	}
	
	tail -> next = newNode; //�N���ڸ`�I�s����s�`�I 
	newNode -> next = NULL;		
}

//��@�g���D�����޿� 
Direction AI(int field[][GRID_SIDE], NodePointer snake){
	//���o�����`�I 
	NodePointer food = foodlisthead;
	//�P�_���L�����| 
	int canfindpath=1,canfindtail=1;
	//��X�Y�������̵u���|p1 
	Direction p1 = shortestpath(snake,food,field,&canfindpath);
	//���@�������D 
	NodePointer snake2=snakecopy(snake);
	
	if(canfindpath){
		while(fake_eatFood(field,snake2)){//���ʵ����D�h�Y���� 		
			control_fake_SnakeDirection(field, snake2); //��������D��V 
			fake_moveNext(snake2); //�����D����
		}
		//��X������D���ڪ����| 
		NodePointer tail2 = findtail(snake2);	
		tailpath(snake2,tail2,field,&canfindtail);
		//�p�G�����D�Y�������i�H���h�ۤv���ڪ����|�A�h��p1���� 
		if(canfindtail){
			freesnake(snake2);
			return p1;
		}
	} 
	//�p�G�����D�䤣��ۤv�����کθ��|p1���s�b�A�h�������̻�����V���� 
	if(!canfindpath || !canfindtail){
		freesnake(snake2);
		return farfromfood(snake,field);
	}
}

NodePointer snakecopy(NodePointer snake){
	NodePointer snake2=NULL,trail=NULL,lead;
	while(snake){
		NodePointer temp=(NodePointer) malloc (sizeof(Node));
		temp->row=snake->row;
		temp->col=snake->col;
		temp->direct=snake->direct;
		temp->next=NULL;
		if(!snake2){
			snake2=temp;
			lead=temp;
			trail=temp;
		}
		else{
			lead=temp;
			trail->next=lead;
			trail=lead;
		}
		snake=snake->next;
	}
	return snake2;
}

Direction shortestpath(NodePointer snake,NodePointer target,int field[][GRID_SIDE],int *flag){
	pathptr foodpath[10];
	int i,j,k;
	//���O�p��U�ӭ������̵u���| 
	for(j=0;target;j++,target=target->next){
		on=0;
		cn=0;
		//�]�w�_�l�`�I 
		pathptr start=(pathptr) malloc(sizeof(path));
		start->row=snake->row;
		start->col=snake->col;
		start->g_cost=0;
		start->h_cost=0;
		start->f_cost=0;
		start->parent=NULL;
		//�N�_�l�I�[�Jopen�� 
		addtoopen_min(start);
		while(1){
			pathptr current;
			current=removefromopen_min();
			addtoclosed(current);
			if(on==-1){  //�䤣����| 
				*flag=0;
				return RIGHT;
			}
			//�����I 
			if(current->row==target->row && current->col==target->col){
				break;
			}
			//�]�w�F�~�`�I 
			neighbor[0]=create(current->row-1,current->col,current,target);//�W 
			neighbor[1]=create(current->row,current->col+1,current,target);//�k 
			neighbor[2]=create(current->row+1,current->col,current,target);//�U 
			neighbor[3]=create(current->row,current->col-1,current,target);//�� 
			for(i=0;i<4;i++){
				if(field[neighbor[i]->row][neighbor[i]->col] == WALL || isinclose(neighbor[i]) || IsAtSnake(snake->next,neighbor[i]->row,neighbor[i]->col)){
					free(neighbor[i]);
					continue;
				}
				int oldpath=getoldpath(neighbor[i]);
				//�p�G���I��f_cost��p�A���ª�f_cost��s
				//�p�G�٤��bopen list�̭��A�⥦�[�i�h 
				if(neighbor[i]->f_cost <= oldpath || !isinopen(neighbor[i])){
					if(!isinopen(neighbor[i])){
						neighbor[i]->parent=current;
						addtoopen_min(neighbor[i]);
						continue;
					}
					updatepath(neighbor[i],current,target);
				}
				free(neighbor[i]);
			}
		}
		foodpath[j]=closed[cn];	
	}
	//��X���D�Y�̪񪺭��� 
	int min,pos=0;
	min=foodpath[0]->f_cost;
	for(k=1;k<j;k++){
		if(foodpath[k]->f_cost < min){
			min=foodpath[k]->f_cost;
			pos=k;
		}
	}
	//��X�Ĥ@�B���`�I 
	pathptr temp= NULL;
	while(foodpath[pos]->parent != NULL){
		temp = foodpath[pos];
		foodpath[pos]=foodpath[pos]->parent;
	}
	foodpath[pos]=temp;
	if(foodpath[pos]->row==snake->row && foodpath[pos]->col==snake->col+1){
		freeopenlist(on);
		freeclosedlist(cn);
		return RIGHT;
	}
	else if(foodpath[pos]->row==snake->row && foodpath[pos]->col==snake->col-1){
		freeopenlist(on);
		freeclosedlist(cn);
		return LEFT;
	}
	else if(foodpath[pos]->row==snake->row+1 && foodpath[pos]->col==snake->col){
		freeopenlist(on);
		freeclosedlist(cn);
		return DOWN;
	}
	else if(foodpath[pos]->row==snake->row-1 && foodpath[pos]->col==snake->col){
		freeopenlist(on);
		freeclosedlist(cn);
		return UP;
	}
}

Direction tailpath(NodePointer snake,NodePointer target,int field[][GRID_SIDE],int *flag){
		int i;
		pathptr foodpath;
		on=0;
		cn=0;
		//�]�w�_�l�`�I 
		pathptr start=(pathptr) malloc(sizeof(path));
		start->row=snake->row;
		start->col=snake->col;
		start->g_cost=0;
		start->h_cost=0;
		start->f_cost=0;
		start->parent=NULL;
		//�N�_�l�I�[�Jopen�� 
		addtoopen_min(start);
		while(1){
			pathptr current;
			current=removefromopen_min();
			addtoclosed(current);
			if(on==-1){  //�䤣����| 
				*flag=0;
				return RIGHT;
			}
			//�����I 
			if(current->row==target->row && current->col==target->col){
				break;
			}
			//�]�w�F�~�`�I 
			neighbor[0]=create(current->row-1,current->col,current,target);
			neighbor[1]=create(current->row,current->col+1,current,target);
			neighbor[2]=create(current->row+1,current->col,current,target);
			neighbor[3]=create(current->row,current->col-1,current,target);
			for(i=0;i<4;i++){
				if(field[neighbor[i]->row][neighbor[i]->col] == WALL || isinclose(neighbor[i]) || IsAtSnake_fake(snake,neighbor[i]->row,neighbor[i]->col)){
					free(neighbor[i]);
					continue;
				}
				int oldpath=getoldpath(neighbor[i]);
				if(neighbor[i]->f_cost < oldpath || !isinopen(neighbor[i])){
					if(!isinopen(neighbor[i])){
						neighbor[i]->parent=current;
						addtoopen_min(neighbor[i]);
						continue;
					}
					updatepath(neighbor[i],current,target);
				}
				free(neighbor[i]);
			}
		}
	foodpath=closed[cn];
	//��X�Ĥ@�B���`�I 
	pathptr temp=NULL;
	while(foodpath->parent != NULL){
		temp = foodpath;
		foodpath=foodpath->parent;
	}
	foodpath = temp;
	if(foodpath->row==snake->row && foodpath->col==snake->col+1){
		freeopenlist(on);
		freeclosedlist(cn);
		return RIGHT;
	}
	else if(foodpath->row==snake->row && foodpath->col==snake->col-1){
		freeopenlist(on);
		freeclosedlist(cn);
		return LEFT;
	}
	else if(foodpath->row==snake->row+1 && foodpath->col==snake->col){
		freeopenlist(on);
		freeclosedlist(cn);
		return DOWN;
	}
	else if(foodpath->row==snake->row-1 && foodpath->col==snake->col){
		freeopenlist(on);
		freeclosedlist(cn);
		return UP;
	}
}

pathptr removefromopen_min(){
	pathptr removed=open[1];
	pathptr temp=open[on--];
	int parent=1;
	int child=2;
	while(child<=on){
		if(child<on && open[child]->f_cost > open[child+1]->f_cost){
			child+=1;
		}
		if(temp->f_cost <= open[child]->f_cost){
			break;
		}
		open[parent]=open[child];
		parent=child;
		child*=2;
	}
	open[parent]=temp;
	return removed;
}

void addtoclosed(pathptr newadd){
	closed[++cn]=newadd;
}

void addtoopen_min(pathptr newadd){
	int i=++on;
	while(i!=1 && newadd->f_cost < open[i/2]->f_cost){
		open[i]=open[i/2];
		i=i/2;
	}
	open[i]=newadd;
}

pathptr create(int row,int col,pathptr current,NodePointer target){
	pathptr newpath=(pathptr) malloc(sizeof(path));
	newpath->row=row;
	newpath->col=col;
	newpath->g_cost=current->g_cost+1;
	newpath->h_cost=h_cost(newpath,target);
	newpath->f_cost=newpath->g_cost+newpath->h_cost;
	return newpath;
}

int isinclose(pathptr detect){
	for(int i=1;i<=cn;i++){
		if(closed[i]->row==detect->row && closed[i]->col==detect->col){
			return 1;
		}
	}
	return 0;
}

int isinopen(pathptr detect){
	for(int i=1;i<=on;i++){
		if(open[i]->row==detect->row && open[i]->col==detect->col){
			return 1;
		}
	}
	return 0;
}

int h_cost(pathptr detect,NodePointer target){
	int row2 = target->row-detect->row;
	int col2 = target->col-detect->col;
	return abs(row2)+abs(col2);  
}

int getoldpath(pathptr detect){
	for(int i=1;i<on;i++){
		if(open[i]->row==detect->row && open[i]->col==detect->col){
			return open[i]->h_cost;
		}
	}
	return 0;
}

void updatepath(pathptr detect,pathptr current,NodePointer target){
	for(int i=1;i<on;i++){
		if(open[i]->row==detect->row && open[i]->col==detect->col){
			open[i]->g_cost=current->g_cost+1;
			open[i]->h_cost=h_cost(neighbor[i],target);
			open[i]->f_cost=open[i]->g_cost+open[i]->h_cost;
			open[i]->parent=current;
		}
	}
}

void deletefood(NodePointer head,NodePointer snake){
	NodePointer trail=NULL;
	if(head->row==snake->row && head->col==snake->col){
		foodlisthead=head->next;
		free(head);
		return;
	}
	while(head){
		if(head->row==snake->row && head->col==snake->col){
			trail->next=head->next;
			free(head);
		}
		trail=head;
		head=head->next;
	}
}

void addfoodnode(NodePointer head,int row,int col){
	NodePointer headd = foodlisthead;
	NodePointer temp=(NodePointer) malloc (sizeof(Node));
	temp->row=row;
	temp->col=col; 
	temp->next=NULL;
	if(!headd){
		foodlisthead=temp;
	}
	else{
		while(headd->next){
			headd=headd->next;
		}
		headd->next=temp;
	}
}

void fake_moveNext(NodePointer snake){
	while (snake != NULL){
		//�̾ڸ`�I����V�ݩʡA�]�w���ʤU�@�B����m	
		switch(snake->direct){
			case RIGHT:
				snake->col++;
				break;			
			case LEFT:
				snake->col--;
				break;
			case UP:
				snake->row--;
				break;			
			case DOWN:
				snake->row++;
				break;
		}
		snake = snake -> next;	
	}
}

void control_fake_SnakeDirection(int field[][GRID_SIDE], NodePointer snake){
	int nouse=1;
	NodePointer food = foodlisthead;
	Direction snakeDirect = snake -> direct;
	snakeDirect = shortestpath(snake,food,field,&nouse);
	//���F�F�칳�D�@�˲��ʤ覡�A��D���ܤ�V�ɡA�ä��O��Ө���@�_���ܡA
	//�ӬO����ʦa�A�U�Ӹ`�I�v�@���ܡA��@�Ӹ`�I����s���e�i��V�ɡA
	//�|�N�ۤv�ثe����V�ǵ��U�@�ӡC 
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}

bool fake_eatFood(int field[][GRID_SIDE],NodePointer snake){
	//�p�G�D�Y�P������m���|�A�N�O�Y�쭹�� 
	if(field[snake->row][snake->col] == FOOD){
		addNode(snake);
		return 0;
	}
	return 1;
}

NodePointer findtail(NodePointer snake){
	while(snake->next){
		snake=snake->next;
	}
	return snake;
} 

Direction farfromfood(NodePointer snake,int field[][GRID_SIDE]){
	NodePointer target = foodlisthead;	
	pathptr farr[4];
	farr[0]=farfromfoodnode(snake->row-1,snake->col,target);//�W 
	farr[1]=farfromfoodnode(snake->row+1,snake->col,target);//�U 
	farr[2]=farfromfoodnode(snake->row,snake->col-1,target);//�� 
	farr[3]=farfromfoodnode(snake->row,snake->col+1,target);//�k 

	int min=0,pos;
	int canfindtail;
	NodePointer tail2; 
	for(int i=0;i<4;i++){
			if(field[farr[i]->row][farr[i]->col] == WALL || IsAtSnake_fake(snake,farr[i]->row,farr[i]->col)){
				continue;
			}
			canfindtail=1;
			NodePointer snake2 = snakecopy(snake);  //�ƻs�@�������D 
			control_fake_SnakeDirection_farfromfood(snake2,i);//�M�w�D�Y����V 
			fake_moveNext(snake2);//�¸Ӥ�V���� 
			tail2 = findtail(snake2);//�������D������ 
			tailpath(snake2,tail2,field,&canfindtail);//�ˬd�O�_������ۤv���ڪ����| 
			if(farr[i]->f_cost>=min && canfindtail){
				min=farr[i]->f_cost;
				pos=i;
			}
			free(farr[i]);
			freesnake(snake2);
		}
	switch(pos){
		case 0:
			return UP;
		case 1:
			return DOWN;
		case 2:
			return LEFT;
		case 3:
			return RIGHT;
	}
}

bool Isinfoodlist(NodePointer head,int row,int col){
	while(head){
		if(head->row==row && head->col==col){
			return 1;
		}
		head=head->next;
	}
	return 0;
}

bool IsAtSnake_fake(NodePointer snake, int row, int col){
	while(snake->next != NULL){
		if (row == snake->row && col == snake->col)
			return true;
		snake = snake -> next;		
	}
	return false;
}

pathptr farfromfoodnode(int row,int col,NodePointer target){
	pathptr temp=(pathptr) malloc(sizeof(path));
	temp->row=row;
	temp->col=col;
	temp->parent=NULL;
	temp->f_cost=abs(row-target->row)+abs(col-target->col);
	return temp;
}

void freesnake(NodePointer snake){
	NodePointer temp;
	while(snake){
		temp=snake;
		snake=snake->next;
		free(temp);
	}
}

void control_fake_SnakeDirection_farfromfood(NodePointer snake, int key){
	Direction snakeDirect = snake->direct;
	switch(key){
		case 0:
			snakeDirect=UP;
			break; 
		case 1:
			snakeDirect=DOWN;
			break; 
		case 2:
			snakeDirect=LEFT;
			break; 
		case 3:
			snakeDirect=RIGHT;
			break; 
	}
	//���F�F�칳�D�@�˲��ʤ覡�A��D���ܤ�V�ɡA�ä��O��Ө���@�_���ܡA
	//�ӬO����ʦa�A�U�Ӹ`�I�v�@���ܡA��@�Ӹ`�I����s���e�i��V�ɡA
	//�|�N�ۤv�ثe����V�ǵ��U�@�ӡC 
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}

void freeclosedlist(int n){
	for(int i=1;i<n;i++){
		free(closed[i]);
	}
}

void freeopenlist(int n){
	for(int i=1;i<n;i++){
		free(open[i]);
	}
}


