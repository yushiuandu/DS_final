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
#define TOP_MARGINE 70 //�]�w�W��� 
#define FOOD_AMOUT 1 //�]�w�C�����ͭ����ƶq 
#define Protect_amount 1
#define explode_amount 1 
#define BULLET 5 //�C���Y�쳣��5�o�l�u 
#define INIT_SPEED  80 //�]�w��l�g���D���ʳt�� 

//�ŧi�e�i��V�C�|��� 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//�ŧi�C�����X�{����C�|��� 
enum Object {
	EMPTY, //�ť� 0
	WALL, //��λ�ê�� 1
	FOOD, //���� 2
	EXPLOSION,//�z������ 3
	PROTECT,//���@�n 4
	TP,//�ǰe�I5 
	bomb,//���u6 
	maze//�g�c7 
};

//�ŧi�g���D����`�I���c 
struct Node {
	int row; //�`�I��b�ĴX�� 
	int col; //�`�I��b�ĴX�C 
	Direction direct; //�Ӹ`�I���e�i��V 
	struct Node *next;	//���V�U�@�Ӹ`�I 
}; 

//�w�q���V�`�I���c�������ܼ� 
typedef struct Node *NodePointer;

void openWindow(); //�}�Ҵ������� 
void closeGame(NodePointer snake, NodePointer snake2); //�B�z�C�������޿� 
int playGame(int field[][GRID_SIDE], NodePointer snake2, NodePointer snake); //�C���i���޿� 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]); //�g���D�O�_���`(���`����G����M����ۤv����) 
int showGameOverMsg(); //�C�������T��
int showGameOverMsg2();
int showGameOverMsg3();
void showInfo(); //��ܹC��������T 
void drawGameField(int field[][GRID_SIDE]); //ø�s�C���ϰ� 
void drawSquare(int row, int col, int color); //ø�s��� 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake, int key); //Ū����L��V��J�A�ó]�w��Ҧ��g���D�`�I 
void controlSnake(int field[][GRID_SIDE], NodePointer snake, int key);
void moveNext(NodePointer snake,int field[][GRID_SIDE], int protect_time); //ø�s�g���D�C�e�i�@�B���������  
void createFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2 ); //���ͭ��� 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //�P�_�O�_������  
bool IsAtSnake(NodePointer snake, int row, int col); //�P�_�O�_����g���D�ۤv������
void eatFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2); //�B�z�Y�쭹���޿� 
void addNode(NodePointer snake); //�W�[�g���D����`�I

//�z������ 
void createExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2);//�����z������ 
void eatExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2); //�B�z�Y�쭹���޿�  
//-------�z������end 

//���@�n 
void createProtect(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2); //���ͨ��@�n 
bool eatProtect(int field[][GRID_SIDE], NodePointer snake);
int protect_time = 0;
int protect_time2 = 0;
int protect_1 = 0;
int protect_2 = 0;
//-------���@�nend 

//�ǰe�I
bool IsAtTP(int row,int col,int field[][GRID_SIDE]);
NodePointer anotherTP(int row,int col,int field[][GRID_SIDE]);
//-------�ǰe�I end

//�z�z�z
void createbomb(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2);
bool eatbomb(int field[][GRID_SIDE], NodePointer snake); //�B�z�Y�쭹���޿�
void clear(int field[][GRID_SIDE], NodePointer snake , NodePointer snake2 , int bullet, int protect);
int bullet_p1 = 0 ;
int bullet_p2 = 0;
void drawSquare2(int row, int col, int color);
void drawSquare3(int row, int col, int color);
void delNode(NodePointer snake);
//-------�z�z�z 


Direction AI(int field[][GRID_SIDE], NodePointer snake);//��@�q������g���D�޿� 

int speed; //�g���D���ʳt�� 
//int scoreSum = 0; //�������� 
int totalTime = 100000; //�����C���ɶ� 
int const scorePerFood = 1; //�C�@�������i�o���� 
bool meetSelfWillDie = false; //�M�w�g���D����ۤv����O�_�|���� 
bool IFPlayAI = false; //�O�_�}��AI�Ҧ�
int length(NodePointer snake);
int length_p1 = 0;
int length_p2 = 0; 
      
int main( )
{  	
	openWindow();
	while(TRUE){
	
		Node head = {2, 2, RIGHT, NULL}; //�]�w�D�Y��l��m�M��V player1
		Node head2 = {37, 37, LEFT, NULL}; //�]�w�D�Y��l��m�M��V player2
		NodePointer snake = &head;
		NodePointer snake2 = &head2;
		speed = INIT_SPEED;
		
		//�]�w�C�����M��ê�� 
	   	int field[GRID_SIDE][GRID_SIDE] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		   								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,7,7,7,7,0,0,7,7,7,7,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,7,0,0,7,0,0,0,0,0,0,1},
										   {1,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,5,0,0,7,0,0,7,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,7,0,0,7,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,7,0,0,7,7,7,7,0,0,0,1},
										   {1,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,7,0,0,0,0,0,7,4,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,7,0,0,0,0,0,7,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,7,0,0,0,0,0,7,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,7,7,7,7,0,0,7,7,7,7,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,7,7,7,7,7,7,7,0,0,7,0,0,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,7,0,0,7,0,0,7,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,7,0,0,7,0,0,7,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,7,0,0,7,0,0,7,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,7,0,0,7,0,0,7,0,0,7,0,0,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,7,0,0,7,0,0,7,0,0,7,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,7,0,0,7,0,0,7,0,0,7,0,0,0,0,0,0,7,0,0,2,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,1},
										   {1,0,0,7,0,0,7,0,0,7,0,0,7,0,0,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
	   	
		char key;
		key = playGame(field, snake, snake2); //�i��C��
		if (key == 'q' || key == 'Q')
			closeGame(snake,snake2); //�p�G���a��J'q'���}�C��	
		else if (key == 'r' || key == 'R')
			totalTime = 100000;
			protect_1 = 0;
			protect_2 = 0; 
			bullet_p1 = 0; 
			bullet_p2 = 0;
			fflush(stdin);
			continue; //�p�G���a��J's' �~��C�� 		    
	}
}

//�}�Ҵ�������
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry Snake Game");
}

//�B�z�C�������޿� 
void closeGame(NodePointer snake, NodePointer snake2){
	free(snake);
	free(snake2);
	exit(0);
}

//�C���i���޿�
int playGame(int field[][GRID_SIDE], NodePointer snake2, NodePointer snake){
	
	drawGameField(field); //ø�s�C���ϰ� 
	createFood(field, snake,snake2); //���ͲĤ@������
	int key;
	printf("orginal:the player1 has %d bullet(s) and player2 has %d bullet(s)\n",bullet_p2,bullet_p1);
	while(true){
//		player1
		controlSnakeDirection(field, snake,key); //Ū�����a��J��V��A�ñN�s��V�]�w��U�g���D�`�I 
		moveNext(snake, field, protect_1); //�̾ڸ`�I����V�Aø�s�s���g���D��m
		//player2
		controlSnake(field, snake2 , key);
		moveNext(snake2, field, protect_2);
		
		eatFood(field, snake,snake2); //�P�_�O�_���Y�쭹���A�p�G���s�W�`�I����� 
		eatFood(field, snake2, snake);
		
		eatExplode(field,snake,snake2);
		eatExplode(field,snake2 , snake);
		
		if(eatProtect(field,snake)){
			protect_1 = 1;
			protect_time += 40;
		}
		if(eatProtect(field,snake2)){
			protect_2 = 1;
			protect_time2 += 40;
		}
		
		//�P�_�Y��l�u�S 
		if(eatbomb(field,snake)){
			if(bullet_p1 < 10){
				bullet_p1 += BULLET;
				if(bullet_p1 > 10){
					bullet_p1 = 10; 
				}
			}
			
			printf("eat:the player1 has %d bullet(s) and player2 has %d bullet(s)\n",bullet_p2,bullet_p1);
		}
		if(eatbomb(field,snake2)){
			if(bullet_p2 < 10){
				bullet_p2 += BULLET;
				if(bullet_p2 > 10){
					bullet_p2 = 10; 
				}
			}
			printf("eat:the player2 has %d bullet(s) and player2 has %d bullet(s)\n",bullet_p2,bullet_p1);
		}
		//----------------�l�uend
		length_p1 = length(snake);//�p��P1 HP 
		length_p2 = length(snake2);//�p��P2 HP 
		
		showInfo(); //��ܮɶ��M���Ƹ�T 
		
		if(IsGameOver(snake, field) || IsGameOver(snake2, field)) //�P�_�O�_�ŦX�C����������A 
		{	
			if(IsGameOver(snake, field)){
				return showGameOverMsg3();
			}else if(IsGameOver(snake2, field)){ 
				return showGameOverMsg2();
			}
		}//��ܹC�������T���A�õ��ݪ��a��J�ﶵ 
		
//===========�D���=========== 
		//���F�Y�쭹���|���ͷs�����A�t�Τ]�H�����ͷs���� 
		srand((unsigned)time(NULL));
		if (rand() % 30 == 0)
			createFood(field, snake,snake2);
		//---�z������ 
		if(rand()% 50 == 0){
			createExplode(field, snake,snake2);
		}	
		//---���@�n 
		if(rand()% 40 == 0 ){
			createProtect(field, snake,snake2);
		}
		//P1�ɶ���F���@�n�N�S�F
		if(0 < protect_time){  
			protect_time--;
		}else{
			protect_1 = 0;
		}
		//P2�ɶ���F���@�n�N�S�F 
		if(0 < protect_time2){ 
			protect_time2--;
		}else{
			protect_2 = 0;
		}
		//���ͤl�u 
		if(rand()% 25 == 0){
			createbomb(field,snake,snake2);
		}	
//============�D���end==============	
		
		delay(speed); //�M�w�g���D���ʳt�סAspeed�V�p���ʶV�� 
		
		//Ū���D��V�䪺��L��L��J 
	   	if(kbhit()) {
			key = getch();	
			
			
			if (key == 'q' || key == 'Q' || key == 'r' || key == 'R')
				return key; 				
			
			else if(key == 'e' || key == 'E'){
				if(bullet_p2 > 0){
				clear(field,snake2 ,snake,bullet_p1, protect_1);
				bullet_p2--;
				printf("clear:the player1 has %d bullet(s) and player2 has %d bullet(s)\n",bullet_p2,bullet_p1);
				}		
			}
			else if(key == 32 && bullet_p1 > 0){
				clear(field,snake ,snake2,bullet_p1, protect_2);
				bullet_p1--;
				printf("clear:the player1 has %d bullet(s) and player2 has %d bullet(s)\n",bullet_p2,bullet_p1);
			}
			else if (key == 'z' || key == 'Z') //�M�w�O�_���ܼҦ� �A�D�n�����a�Ҧ��M�q���ޱ���AI�Ҧ� 
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
					case EXPLOSION:
						drawSquare(row, col, 5);
						break;
					case PROTECT:
						drawSquare(row, col, WHITE);
						break;
					case TP:
						drawSquare(row, col, BLUE);
						break;
					case bomb:
						drawSquare(row, col, BROWN);
						break;
					case maze:
						drawSquare(row, col, YELLOW);
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
void moveNext(NodePointer snake,int field[][GRID_SIDE], int protect_time){
	int currRow, currCol;
	NodePointer tp;
	
	while (snake != NULL){
		currRow = snake->row;
		currCol = snake->col;
		
		//�̾ڸ`�I����V�ݩʡA�]�w���ʤU�@�B����m	
		switch(snake->direct){
			case RIGHT:
				
				if(field[snake->row][snake->col+1] == maze){
					snake->direct = LEFT;
					snake->col--;
				}else{
					snake->col++;
				}
				
				if(IsAtTP(snake->row,snake->col,field)){
					tp=anotherTP(snake->row,snake->col,field);//���t�@��TP�I 
					snake->row=tp->row;//���Ӹ`�IX�y�� 
					snake->col=tp->col;//���Ӹ`�IY�y�� 
				}
				break;			
			case LEFT:
				
				if(field[snake->row][snake->col-1] == maze){
					snake->direct = RIGHT;
					snake->col++;
				}else{
					snake->col--;
				}
				
				if(IsAtTP(snake->row,snake->col,field)){
					tp=anotherTP(snake->row,snake->col,field);//���t�@��TP�I 
					snake->row=tp->row;//���Ӹ`�IX�y�� 
					snake->col=tp->col;//���Ӹ`�IY�y�� 
				}
				break;
			case UP:
				if(field[snake->row-1][snake->col] == maze){
					snake->direct = DOWN;
					snake->row++;
				}else{
					snake->row--;
				}
				
				if(IsAtTP(snake->row,snake->col,field)){
					tp=anotherTP(snake->row,snake->col,field);//���t�@��TP�I 
					snake->row=tp->row;//���Ӹ`�IX�y�� 
					snake->col=tp->col;//���Ӹ`�IY�y�� 
				}
				
				break;			
			case DOWN:
				if(field[snake->row+1][snake->col] == maze){
					snake->direct = UP;
					snake->row--;
				}else{
					snake->row++;
				}
				
				if(IsAtTP(snake->row,snake->col,field)){
					tp=anotherTP(snake->row,snake->col,field);//���t�@��TP�I 
					snake->row=tp->row;//���Ӹ`�IX�y�� 
					snake->col=tp->col;//���Ӹ`�IY�y�� 
				}
				break;
		}

		//���@�n 
		if(protect_time > 0 ){
			drawSquare(snake->row, snake->col, WHITE);
		}
		else{
			drawSquare(snake->row, snake->col, RED);
		}
		//���@�nend
		
		if (!IsAtSnake(snake, currRow, currCol) && !IsAtTP(currRow,currCol,field))
			drawSquare(currRow, currCol, BLACK);
		
		if(IsAtTP(snake->row,snake->col,field)){
			drawSquare(snake->row, snake->col, BLUE);
		}
			
		snake = snake -> next;	
	}
}

//�g���D�O�_���`(���`����G����M����ۤv����) 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]){
	NodePointer head = snake;
	
	//�P�_�O�_������ 
	if (IsAtWall(field, head->row, head->col) || snake == NULL)
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
	char msg2[40] = "press [q] to quit or [r] to restart!!";
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
			if (key == 'q' || key == 'Q' || key == 'r' || key == 'R') {
				protect_1 = 0;protect_2 = 0;
				
				return key; 				
			}
		}
	}	
}

int showGameOverMsg2(){
   	//cleardevice(); //�M�z�Ҧ��ù���ơA�p�G�Ʊ�u��ܰT���ɡA�������� 
	int i = 0;
	char msg1[15] = "player2 win!!";
	char msg2[40] = "press [q] to quit or [r] to restart!!";
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
			if (key == 'q' || key == 'Q' || key == 'r' || key == 'R') {
				protect_1 = 0;protect_2 = 0;
				
				return key; 				
			}
		}
	}	
}

int showGameOverMsg3(){
   	//cleardevice(); //�M�z�Ҧ��ù���ơA�p�G�Ʊ�u��ܰT���ɡA�������� 
	int i = 0;
	char msg1[15] = "player1 win!!";
	char msg2[40] = "press [q] to quit or [r] to restart!!";
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
			if (key == 'q' || key == 'Q' || key == 'r' || key == 'R') {
				protect_1 = 0;protect_2 = 0;
				
				return key; 				
			}
		}
	}	
}
//��ܹC��������T
void showInfo(){

	int i;
	setcolor(BLUE); //�]�w��r�C�� 
   	settextstyle(BOLD_FONT , HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   	char player[] = "P1";
   	outtextxy(0, 15, player); //�̾ڧ��п�X��r��ù�
	   
	setcolor(GREEN); //�]�w��r�C�� 
   	settextstyle(BOLD_FONT , HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   	char player2[] = "P2";
	outtextxy(320, 15, player2); //�̾ڧ��п�X��r��ù�	
   	
	totalTime -= speed;
	if(totalTime<=0){
		if(length_p2 > length_p1){
			showGameOverMsg2();
		}else if (length_p1 == length_p2){
			showGameOverMsg();
		}else{
			showGameOverMsg3();
		}
		fflush(stdin);
	}
	char timeMsg[45] = "";
	char scoreMsg[45] = " HP:";
	char scoreMsg2[45] = " HP:";
	char optMsg1[50] = "press [q] to quit, [r] to restart or";
	char optMsg2[50] = "press [z] to change mode.";
	
	char time[10];
	char score[10];
//	Time
	sprintf(time, "%d", totalTime/1000);
	strcat(timeMsg, time);
	
	setcolor(WHITE); //�]�w��r�C�� 
   	settextstyle(4 , HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   	outtextxy(245, 14, timeMsg); //�̾ڧ��п�X��r��ù�
	
//	P1���� 
	sprintf(score, "%3d", length_p2);
	strcat(scoreMsg, score);
	strcat(scoreMsg, " point.");

	setcolor(12);
   	settextstyle(4 , HORIZ_DIR , 2);
   	outtextxy(50, 13, scoreMsg);
	   
//	P2���� 
	sprintf(score, "%3d", length_p1);
	strcat(scoreMsg2, score);
	strcat(scoreMsg2, " point.");
	setcolor(12);
   	settextstyle(4 , HORIZ_DIR , 2);
   	outtextxy(380, 13, scoreMsg2);

	//�l�u P1 
	for(i=3;i<bullet_p2+3;i++){
		drawSquare(-2, i, RED);
	}
	for(i = bullet_p2+3 ; i < 13 ; i++){
		drawSquare(-2, i, BLACK);
	}
	//�l�u P2 
	for(i=28;i<bullet_p1+28;i++){
			drawSquare(-2, i, RED);
	}
	for(i = bullet_p1+28 ; i < 38 ; i++){
		drawSquare(-2, i, BLACK);
	}


//  �U��   
   	setcolor(LIGHTGREEN);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE, optMsg1);
	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE + 20, optMsg2);	     
}

//Ū����L��V��J�A�ó]�w��Ҧ��g���D�`�I 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake, int key){
	
	Direction snakeDirect = snake -> direct;
	
	//get key code by pressing keybord
		
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
	
	if (IFPlayAI) 
		snakeDirect = AI(field, snake);

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

void controlSnake(int field[][GRID_SIDE], NodePointer snake,int key){
	Direction snakeDirect = snake -> direct;
	
	//get key code by pressing keybord
	if(key == 'a' || key == 'A'){
		snakeDirect = LEFT;
	}else if(key == 'd' || key == 'D'){
		snakeDirect = RIGHT;
	}else if(key == 'w' || key == 'W'){
		snakeDirect = UP;
	}else if(key == 's' || key == 'S'){
		snakeDirect = DOWN;
	}
	//decide snake's moving direction	
	
	if (IFPlayAI) 
		snakeDirect = AI(field, snake);

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
void createFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 

	for(i=0; i< foodAmount; i++){
		//�p�G�üƲ��ͪ���m�O�b��M�D���魫�|�A�h���s���͡A����ŦX���󬰤� 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = FOOD;
		drawSquare(row,col, GREEN);			
	}
}

//�B�z�Y�쭹���޿�
void eatFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	//�p�G�D�Y�P������m���|�A�N�O�Y�쭹�� 
	if(field[snake->row][snake->col] == FOOD){
		field[snake->row][snake->col] = EMPTY; //�N�ӭ����M�� 
		printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
		addNode(snake); //�W�[�`�I 
		createFood(field, snake,snake2); //���ͷs������
	}
}

//�z������
//�����z������ 
void createExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){

	int row, col, i, explodeAmount = explode_amount;
	srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 

	for(i=0; i< explodeAmount; i++){
		//�p�G�üƲ��ͪ���m�O�b��M�D���魫�|�A�h���s���͡A����ŦX���󬰤� 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = EXPLOSION;
		drawSquare(row,col, 5);			
	}
}

void eatExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	//�p�G�D�Y�P������m���|�A�N�O�Y�쭹��
	int i; 
	if(field[snake->row][snake->col] == EXPLOSION){
		field[snake->row][snake->col] = EMPTY; //�N�ӭ����M�� 
		printf("The snake has eaten explode food at row: %d, col: %d\n", snake->row, snake->col);
		int row, col, i ;
		 srand(time(NULL));
		int foodAmount = rand()*rand()%5 +1;
		srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 

		for(i=0; i< foodAmount; i++){
				//�p�G�üƲ��ͪ���m�O�b��M�D���魫�|�A�h���s���͡A����ŦX���󬰤� 
			do{
				row = rand() % GRID_SIDE;
				col = rand() % GRID_SIDE;	
			}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) ||IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
		
			field[row][col] = FOOD;
			drawSquare(row,col, GREEN);			
		}
	}
} 
//-------------�z������end 

//���@�n 
//���ͨ��@�n 
void createProtect(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	int row, col, i, protectAmount = Protect_amount;
	srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 

	for(i=0; i< protectAmount; i++){
		//�p�G�üƲ��ͪ���m�O�b��M�D���魫�|�A�h���s���͡A����ŦX���󬰤� 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = PROTECT;
		drawSquare(row,col, WHITE);			
	}
}

//�p�G�Y�쨾�@�n 
bool eatProtect(int field[][GRID_SIDE], NodePointer snake){
	//�p�G�D�Y�P������m���|�A�N�O�Y�쭹��
	
	if(field[snake->row][snake->col] == PROTECT){
		field[snake->row][snake->col] = EMPTY; //�N�ӭ����M�� 
		printf("The snake has eaten protect food at row: %d, col: %d\n", snake->row, snake->col);	
		return true;
	}
	return false;
}
//--------------end

//�ǰe�I 
bool IsAtTP(int row,int col,int field[][GRID_SIDE]){
	if(field[row][col]==TP){
		return true;
	}
	return false;
}
NodePointer anotherTP(int row,int col,int field[][GRID_SIDE]){
	NodePointer temp = (NodePointer) malloc (sizeof(Node));
	int i,j;
	for(i=0;i<GRID_SIDE;i++){
		for(j=0;j<GRID_SIDE;j++){
			if(field[i][j]==TP && (row!=i || col!=j)){
				temp->row=i;
				temp->col=j;
				return temp;
			}
		}
	}
}
//----------�ǰe�Iend 

//----���O�H����
//����bomb
void createbomb(int field[][GRID_SIDE], NodePointer snake , NodePointer snake2){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 

	for(i=0; i< foodAmount; i++){
		//�p�G�üƲ��ͪ���m�O�b��M�D���魫�|�A�h���s���͡A����ŦX���󬰤� 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = bomb;
		drawSquare(row,col, BROWN);
	}
}

//�Y�쬵�u 
bool eatbomb(int field[][GRID_SIDE], NodePointer snake){
	//�p�G�D�Y�P������m���|�A�N�O�Y�쭹�� 
	if(field[snake->row][snake->col] == bomb){
		field[snake->row][snake->col] == EMPTY;
		printf("The snake has eaten bomb at row: %d, col: %d\n", snake->row, snake->col);
		return true;
	}
	return false;
}

//�M�z 
void  clear(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2 ,int bullet,int protect){
	int currRow, currCol;
	currRow = snake->row;
	currCol = snake->col;
	int i = 0;
	int test;
	NodePointer head = snake2;
	
	
	if(snake!=NULL) {
		
		switch(snake->direct){
			delay(200);
			case RIGHT :
				//�P�_���S���g���D 
				while(snake2){
					test = snake2->row;
					if(test == currRow)break;
					snake2 = snake2->next;
				}
				//�P�_�g�D�����|���S���D�� 
				while((currCol + i) != test){
					if(field[currRow][currCol+i] != EMPTY)break;
					i++;
				}
				//�p�G���g���D
				if(test == currRow && (currCol + i) == test){
					for(int j = currCol ; j < snake2->col ;j++){
						drawSquare2(currRow,j,5);
					} 
					delay(40);
					for(int j = currCol ; j < snake2->col ;j++){
						drawSquare2(currRow,j,BLACK);
					} 
					//�p�G�S�����@�n
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//�p�G�S���g���D
					for(int j = currCol+1 ; j < currCol+3 ;j++){
						if(field[currRow][j] == EMPTY){
							drawSquare2(currRow,j,5);
						}
					} 
					delay(40);
					for(int j = currCol+1 ; j < currCol+3 ;j++){
						if(field[currRow][j] == EMPTY){
							drawSquare2(currRow,j,BLACK);
						}
					} 
				}
				break;
							
			case LEFT :
				//�P�_���S���g���D 
				while(snake2){
					test = snake2->row;
					if(test == currRow)break;
					snake2 = snake2->next;
				}
				//�P�_�g�D�����|���S���D�� 
				while((currCol - i) != test){
					if(field[currRow][currCol - i] != EMPTY)break;
					i++;
				}
				//�p�G���g���D
				if( test == currRow && currCol - i == test){
					for(int j = currCol ; j > snake2->col ;j--){
						drawSquare2(currRow,j,5);
					}
					delay(40);
					for(int j = currCol ; j > snake2->col ;j--){
						drawSquare(currRow,j,BLACK);
					} 
					//�p�G�S�����@�n�A�A�R���`�I 
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//�p�G�S���g���D
					for(int j = currCol-1 ; j > currCol-3 ;j--){
						if(field[currRow][j] == EMPTY){
							drawSquare2(currRow,j,5);
						}
					}
					delay(40);
					for(int j = currCol-1 ; j > currCol-3 ;j--){
						if(field[currRow][j] == EMPTY){
							drawSquare2(currRow,j,BLACK);
						}
					} 
				}
				break;
				
			case UP:
				//�P�_���S���g���D 
				while(snake2){
					test = snake2->col;
					if(test == currCol)break;
					snake2 = snake2->next;
				}
				//�P�_�g�D�����|���S���D�� 
				while((currRow + i) != test){
					if(field[currRow+ i][currCol] != EMPTY)break;
					i++;
				}
				//�p�G���g���D
				if(test == currCol && (currRow + i) == test){
					for(int j = currRow ; j > snake2->row ;j--){
						drawSquare3(j,currCol,5);
					}
					delay(40);
					for(int j = currRow ; j > snake2->row ;j--){
						drawSquare(j,currCol,BLACK);
					}
					//�p�G�S�����@�n�A�h���R���@�Ӹ`�I 
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//�p�G�S���g���D
					for(int j = currRow-1 ; j > currRow-3 ;j--){
						if(field[j][currCol] == EMPTY){
							drawSquare3(j,currCol,5);
						}
						
					}
					delay(40);
					for(int j = currRow-1 ; j > currRow-3 ;j--){
						if(field[j][currCol] == EMPTY){
							drawSquare(j,currCol,BLACK);
						}
						
					}
				}
				break;
							
			case DOWN:
				//�P�_���S���g���D 
				while(snake2){
					test = snake2->col;
					if(test == currCol)break;
					snake2 = snake2->next;
				}
				//�P�_�g�D�����|���S���D�� 
				while((currRow - i) != test){
					if(field[currRow - i][currCol] != EMPTY)break;
					i++;
				}
				//�p�G���g���D 
				if( test == currCol && (currRow - i) == test){
					for(int j = currRow ; j < snake2->row ;j++){
						drawSquare3(j,currCol,5);
					}
					delay(40);
					for(int j = currRow ; j < snake2->row ;j++){
						drawSquare(j,currCol,BLACK);
					}
					//�p�G�S�����@�n�A�h���֤@�Ӹ`�I 
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//�p�G�S���g���D 
					for(int j = currRow + 1 ; j < currRow + 3 ;j++){
						if(field[j][currCol] == EMPTY){
							drawSquare3(j,currCol,5);
						}
					}
					delay(40);
					for(int j = currRow + 1 ; j < currRow + 3 ;j++){
						if(field[j][currCol] == EMPTY){
							drawSquare(j,currCol,BLACK);
						}
					}
				}
				break;
			
		}	
		
	}

}
//�S�ĳ� ��V 
void drawSquare2(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE / 2;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight * 2 + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //�]�wø�s���󪺬���ߩM�C�� 
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}
//�S�ĳ� ���V 
void drawSquare3(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE / 2;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth * 2+ col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //�]�wø�s���󪺬���ߩM�C�� 
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}

void delNode(NodePointer snake){
	NodePointer head = snake;
	NodePointer tail = head;
	NodePointer newNode = head;
	int length = 0;
	//�M��D������ 
	while(snake != NULL){
		tail = snake;
		snake = snake -> next;
		length++;
	}
	if(length == 1){
		showGameOverMsg();
	}
	while(newNode->next != tail){
		newNode = head;
		head = head->next;
	}
	
	drawSquare(tail->row,tail->col,BLACK);
	newNode->next = NULL;
	free(tail);
}


//------���O�Hend 




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

int length(NodePointer snake){
	int Length = 0;
	while(snake){
		Length++;
		snake = snake->next;
	}
	return Length;
}

//��@�g���D�����޿� 
Direction AI(int field[][GRID_SIDE], NodePointer snake){
	//�Цb�o�̹�@�X�g���D�����޿�A��������ۤv�M�䭹���M���׻�ê�� 
	Direction snakeDirect = snake -> direct;
	return snakeDirect;
}
