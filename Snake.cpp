#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>

#define SCREEN_HEIGHT 500 //設定遊戲視窗高度 
#define SCREEN_WIDTH 500 //設定遊戲視窗寬度
#define GRID_SIDE 40 //設定遊戲方陣每邊格子數量 
#define LEFT_MARGINE 30 //設定左邊界 
#define TOP_MARGINE 70 //設定上邊界 
#define FOOD_AMOUT 1 //設定每次產生食物數量 
#define Protect_amount 1
#define explode_amount 1 
#define BULLET 5 //每次吃到都有5發子彈 
#define INIT_SPEED  80 //設定初始貪食蛇移動速度 

//宣告前進方向列舉函數 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//宣告遊戲場出現物體列舉函數 
enum Object {
	EMPTY, //空白 0
	WALL, //牆或障礙物 1
	FOOD, //食物 2
	EXPLOSION,//爆炸食物 3
	PROTECT,//防護罩 4
	TP,//傳送點5 
	bomb,//炸彈6 
	maze//迷宮7 
};

//宣告貪食蛇身體節點結構 
struct Node {
	int row; //節點位在第幾行 
	int col; //節點位在第幾列 
	Direction direct; //該節點的前進方向 
	struct Node *next;	//指向下一個節點 
}; 

//定義指向節點結構的指標變數 
typedef struct Node *NodePointer;

void openWindow(); //開啟游戲視窗 
void closeGame(NodePointer snake, NodePointer snake2); //處理遊戲結束邏輯 
int playGame(int field[][GRID_SIDE], NodePointer snake2, NodePointer snake); //遊戲進行邏輯 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]); //貪食蛇是否死亡(死亡條件：撞牆和撞到自己身體) 
int showGameOverMsg(); //遊戲結束訊息
int showGameOverMsg2();
int showGameOverMsg3();
void showInfo(); //顯示遊戲相關資訊 
void drawGameField(int field[][GRID_SIDE]); //繪製遊戲區域 
void drawSquare(int row, int col, int color); //繪製方塊 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake, int key); //讀取鍵盤方向輸入，並設定到所有貪食蛇節點 
void controlSnake(int field[][GRID_SIDE], NodePointer snake, int key);
void moveNext(NodePointer snake,int field[][GRID_SIDE], int protect_time); //繪製貪食蛇每前進一步的身體改變  
void createFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2 ); //產生食物 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //判斷是否撞到牆  
bool IsAtSnake(NodePointer snake, int row, int col); //判斷是否撞到貪食蛇自己的身體
void eatFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2); //處理吃到食物邏輯 
void addNode(NodePointer snake); //增加貪食蛇身體節點

//爆炸食物 
void createExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2);//產生爆炸食物 
void eatExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2); //處理吃到食物邏輯  
//-------爆炸食物end 

//防護罩 
void createProtect(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2); //產生防護罩 
bool eatProtect(int field[][GRID_SIDE], NodePointer snake);
int protect_time = 0;
int protect_time2 = 0;
int protect_1 = 0;
int protect_2 = 0;
//-------防護罩end 

//傳送點
bool IsAtTP(int row,int col,int field[][GRID_SIDE]);
NodePointer anotherTP(int row,int col,int field[][GRID_SIDE]);
//-------傳送點 end

//爆爆爆
void createbomb(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2);
bool eatbomb(int field[][GRID_SIDE], NodePointer snake); //處理吃到食物邏輯
void clear(int field[][GRID_SIDE], NodePointer snake , NodePointer snake2 , int bullet, int protect);
int bullet_p1 = 0 ;
int bullet_p2 = 0;
void drawSquare2(int row, int col, int color);
void drawSquare3(int row, int col, int color);
void delNode(NodePointer snake);
//-------爆爆爆 


Direction AI(int field[][GRID_SIDE], NodePointer snake);//實作電腦控制貪食蛇邏輯 

int speed; //貪食蛇移動速度 
//int scoreSum = 0; //紀錄分數 
int totalTime = 100000; //紀錄遊戲時間 
int const scorePerFood = 1; //每一份食物可得分數 
bool meetSelfWillDie = false; //決定貪食蛇撞到自己身體是否會死掉 
bool IFPlayAI = false; //是否開啟AI模式
int length(NodePointer snake);
int length_p1 = 0;
int length_p2 = 0; 
      
int main( )
{  	
	openWindow();
	while(TRUE){
	
		Node head = {2, 2, RIGHT, NULL}; //設定蛇頭初始位置和方向 player1
		Node head2 = {37, 37, LEFT, NULL}; //設定蛇頭初始位置和方向 player2
		NodePointer snake = &head;
		NodePointer snake2 = &head2;
		speed = INIT_SPEED;
		
		//設定遊戲場和障礙物 
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
		key = playGame(field, snake, snake2); //進行遊戲
		if (key == 'q' || key == 'Q')
			closeGame(snake,snake2); //如果玩家輸入'q'離開遊戲	
		else if (key == 'r' || key == 'R')
			totalTime = 100000;
			protect_1 = 0;
			protect_2 = 0; 
			bullet_p1 = 0; 
			bullet_p2 = 0;
			fflush(stdin);
			continue; //如果玩家輸入's' 繼續遊戲 		    
	}
}

//開啟游戲視窗
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry Snake Game");
}

//處理遊戲結束邏輯 
void closeGame(NodePointer snake, NodePointer snake2){
	free(snake);
	free(snake2);
	exit(0);
}

//遊戲進行邏輯
int playGame(int field[][GRID_SIDE], NodePointer snake2, NodePointer snake){
	
	drawGameField(field); //繪製遊戲區域 
	createFood(field, snake,snake2); //產生第一份食物
	int key;
	printf("orginal:the player1 has %d bullet(s) and player2 has %d bullet(s)\n",bullet_p2,bullet_p1);
	while(true){
//		player1
		controlSnakeDirection(field, snake,key); //讀取玩家輸入方向鍵，並將新方向設定到各貪食蛇節點 
		moveNext(snake, field, protect_1); //依據節點的方向，繪製新的貪食蛇位置
		//player2
		controlSnake(field, snake2 , key);
		moveNext(snake2, field, protect_2);
		
		eatFood(field, snake,snake2); //判斷是否有吃到食物，如果有新增節點到尾巴 
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
		
		//判斷吃到子彈沒 
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
		//----------------子彈end
		length_p1 = length(snake);//計算P1 HP 
		length_p2 = length(snake2);//計算P2 HP 
		
		showInfo(); //顯示時間和分數資訊 
		
		if(IsGameOver(snake, field) || IsGameOver(snake2, field)) //判斷是否符合遊戲結束條件， 
		{	
			if(IsGameOver(snake, field)){
				return showGameOverMsg3();
			}else if(IsGameOver(snake2, field)){ 
				return showGameOverMsg2();
			}
		}//顯示遊戲結束訊息，並等待玩家輸入選項 
		
//===========道具區=========== 
		//除了吃到食物會產生新食物，系統也隨機產生新食物 
		srand((unsigned)time(NULL));
		if (rand() % 30 == 0)
			createFood(field, snake,snake2);
		//---爆炸食物 
		if(rand()% 50 == 0){
			createExplode(field, snake,snake2);
		}	
		//---防護罩 
		if(rand()% 40 == 0 ){
			createProtect(field, snake,snake2);
		}
		//P1時間到了防護罩就沒了
		if(0 < protect_time){  
			protect_time--;
		}else{
			protect_1 = 0;
		}
		//P2時間到了防護罩就沒了 
		if(0 < protect_time2){ 
			protect_time2--;
		}else{
			protect_2 = 0;
		}
		//產生子彈 
		if(rand()% 25 == 0){
			createbomb(field,snake,snake2);
		}	
//============道具區end==============	
		
		delay(speed); //決定貪食蛇移動速度，speed越小移動越快 
		
		//讀取非方向鍵的其他鍵盤輸入 
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
			else if (key == 'z' || key == 'Z') //決定是否改變模式 ，主要有玩家模式和電腦操控的AI模式 
				IFPlayAI = !IFPlayAI;
		}					
	}		
}

//繪製遊戲區域，依據遊戲場矩陣設定繪製物件 
void drawGameField(int field[][GRID_SIDE]){
   	int row = 0, col = 0;
   	cleardevice(); //清理螢幕畫面 
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
				switch(field[row][col])	{
					case WALL: //牆在矩陣中的值是1 
						drawSquare(row, col, YELLOW);
						break;
					case FOOD: //食物在矩陣中的值是2
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

//繪製方塊
void drawSquare(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //設定繪製物件的為實心和顏色 
   	bar(left , top, right, bottom);	//設定繪製方塊的位置 
}

//繪製貪食蛇每前進一步的身體改變 
void moveNext(NodePointer snake,int field[][GRID_SIDE], int protect_time){
	int currRow, currCol;
	NodePointer tp;
	
	while (snake != NULL){
		currRow = snake->row;
		currCol = snake->col;
		
		//依據節點的方向屬性，設定移動下一步的位置	
		switch(snake->direct){
			case RIGHT:
				
				if(field[snake->row][snake->col+1] == maze){
					snake->direct = LEFT;
					snake->col--;
				}else{
					snake->col++;
				}
				
				if(IsAtTP(snake->row,snake->col,field)){
					tp=anotherTP(snake->row,snake->col,field);//找到另一個TP點 
					snake->row=tp->row;//更改該節點X座標 
					snake->col=tp->col;//更改該節點Y座標 
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
					tp=anotherTP(snake->row,snake->col,field);//找到另一個TP點 
					snake->row=tp->row;//更改該節點X座標 
					snake->col=tp->col;//更改該節點Y座標 
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
					tp=anotherTP(snake->row,snake->col,field);//找到另一個TP點 
					snake->row=tp->row;//更改該節點X座標 
					snake->col=tp->col;//更改該節點Y座標 
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
					tp=anotherTP(snake->row,snake->col,field);//找到另一個TP點 
					snake->row=tp->row;//更改該節點X座標 
					snake->col=tp->col;//更改該節點Y座標 
				}
				break;
		}

		//防護罩 
		if(protect_time > 0 ){
			drawSquare(snake->row, snake->col, WHITE);
		}
		else{
			drawSquare(snake->row, snake->col, RED);
		}
		//防護罩end
		
		if (!IsAtSnake(snake, currRow, currCol) && !IsAtTP(currRow,currCol,field))
			drawSquare(currRow, currCol, BLACK);
		
		if(IsAtTP(snake->row,snake->col,field)){
			drawSquare(snake->row, snake->col, BLUE);
		}
			
		snake = snake -> next;	
	}
}

//貪食蛇是否死亡(死亡條件：撞牆和撞到自己身體) 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]){
	NodePointer head = snake;
	
	//判斷是否撞到牆 
	if (IsAtWall(field, head->row, head->col) || snake == NULL)
		return true;
	
	//判斷是否撞到貪食蛇自己 
	if(meetSelfWillDie)
		return IsAtSnake(snake->next, head->row, head->col);
	
	return false;		
}

//判斷是否撞到牆
bool IsAtWall(int field[][GRID_SIDE], int row, int col){
		if (field[row][col] == WALL)
			return true;
		return false;
}

//判斷是否撞到貪食蛇自己的身體
bool IsAtSnake(NodePointer snake, int row, int col){

	while(snake != NULL){
		if (row == snake->row && col == snake->col)
			return true;
		snake = snake -> next;		
	}
	return false;
}

//遊戲結束訊息
int showGameOverMsg(){
   	//cleardevice(); //清理所有螢幕資料，如果希望只顯示訊息時，取消註解 
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
   	//cleardevice(); //清理所有螢幕資料，如果希望只顯示訊息時，取消註解 
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
   	//cleardevice(); //清理所有螢幕資料，如果希望只顯示訊息時，取消註解 
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
//顯示遊戲相關資訊
void showInfo(){

	int i;
	setcolor(BLUE); //設定文字顏色 
   	settextstyle(BOLD_FONT , HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   	char player[] = "P1";
   	outtextxy(0, 15, player); //依據坐標輸出文字到螢幕
	   
	setcolor(GREEN); //設定文字顏色 
   	settextstyle(BOLD_FONT , HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   	char player2[] = "P2";
	outtextxy(320, 15, player2); //依據坐標輸出文字到螢幕	
   	
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
	
	setcolor(WHITE); //設定文字顏色 
   	settextstyle(4 , HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   	outtextxy(245, 14, timeMsg); //依據坐標輸出文字到螢幕
	
//	P1分數 
	sprintf(score, "%3d", length_p2);
	strcat(scoreMsg, score);
	strcat(scoreMsg, " point.");

	setcolor(12);
   	settextstyle(4 , HORIZ_DIR , 2);
   	outtextxy(50, 13, scoreMsg);
	   
//	P2分數 
	sprintf(score, "%3d", length_p1);
	strcat(scoreMsg2, score);
	strcat(scoreMsg2, " point.");
	setcolor(12);
   	settextstyle(4 , HORIZ_DIR , 2);
   	outtextxy(380, 13, scoreMsg2);

	//子彈 P1 
	for(i=3;i<bullet_p2+3;i++){
		drawSquare(-2, i, RED);
	}
	for(i = bullet_p2+3 ; i < 13 ; i++){
		drawSquare(-2, i, BLACK);
	}
	//子彈 P2 
	for(i=28;i<bullet_p1+28;i++){
			drawSquare(-2, i, RED);
	}
	for(i = bullet_p1+28 ; i < 38 ; i++){
		drawSquare(-2, i, BLACK);
	}


//  下面   
   	setcolor(LIGHTGREEN);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE, optMsg1);
	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE + 20, optMsg2);	     
}

//讀取鍵盤方向輸入，並設定到所有貪食蛇節點 
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

	//為了達到像蛇一樣移動方式，當蛇改變方向時，並不是整個身體一起改變，
	//而是延遲性地，各個節點逐一改變，當一個節點收到新的前進方向時，
	//會將自己目前的方向傳給下一個。 
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

	//為了達到像蛇一樣移動方式，當蛇改變方向時，並不是整個身體一起改變，
	//而是延遲性地，各個節點逐一改變，當一個節點收到新的前進方向時，
	//會將自己目前的方向傳給下一個。 
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}

//產生食物
void createFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //取目前系統時間作為亂數種子 

	for(i=0; i< foodAmount; i++){
		//如果亂數產生的位置是在牆和蛇身體重疊，則重新產生，直到符合條件為止 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = FOOD;
		drawSquare(row,col, GREEN);			
	}
}

//處理吃到食物邏輯
void eatFood(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	//如果蛇頭與食物位置重疊，就是吃到食物 
	if(field[snake->row][snake->col] == FOOD){
		field[snake->row][snake->col] = EMPTY; //將該食物清空 
		printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
		addNode(snake); //增加節點 
		createFood(field, snake,snake2); //產生新的食物
	}
}

//爆炸食物
//產生爆炸食物 
void createExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){

	int row, col, i, explodeAmount = explode_amount;
	srand((unsigned)time(NULL)); //取目前系統時間作為亂數種子 

	for(i=0; i< explodeAmount; i++){
		//如果亂數產生的位置是在牆和蛇身體重疊，則重新產生，直到符合條件為止 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = EXPLOSION;
		drawSquare(row,col, 5);			
	}
}

void eatExplode(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	//如果蛇頭與食物位置重疊，就是吃到食物
	int i; 
	if(field[snake->row][snake->col] == EXPLOSION){
		field[snake->row][snake->col] = EMPTY; //將該食物清空 
		printf("The snake has eaten explode food at row: %d, col: %d\n", snake->row, snake->col);
		int row, col, i ;
		 srand(time(NULL));
		int foodAmount = rand()*rand()%5 +1;
		srand((unsigned)time(NULL)); //取目前系統時間作為亂數種子 

		for(i=0; i< foodAmount; i++){
				//如果亂數產生的位置是在牆和蛇身體重疊，則重新產生，直到符合條件為止 
			do{
				row = rand() % GRID_SIDE;
				col = rand() % GRID_SIDE;	
			}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) ||IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
		
			field[row][col] = FOOD;
			drawSquare(row,col, GREEN);			
		}
	}
} 
//-------------爆炸食物end 

//防護罩 
//產生防護罩 
void createProtect(int field[][GRID_SIDE], NodePointer snake, NodePointer snake2){
	int row, col, i, protectAmount = Protect_amount;
	srand((unsigned)time(NULL)); //取目前系統時間作為亂數種子 

	for(i=0; i< protectAmount; i++){
		//如果亂數產生的位置是在牆和蛇身體重疊，則重新產生，直到符合條件為止 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = PROTECT;
		drawSquare(row,col, WHITE);			
	}
}

//如果吃到防護罩 
bool eatProtect(int field[][GRID_SIDE], NodePointer snake){
	//如果蛇頭與食物位置重疊，就是吃到食物
	
	if(field[snake->row][snake->col] == PROTECT){
		field[snake->row][snake->col] = EMPTY; //將該食物清空 
		printf("The snake has eaten protect food at row: %d, col: %d\n", snake->row, snake->col);	
		return true;
	}
	return false;
}
//--------------end

//傳送點 
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
//----------傳送點end 

//----炸別人喔喔喔
//產生bomb
void createbomb(int field[][GRID_SIDE], NodePointer snake , NodePointer snake2){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //取目前系統時間作為亂數種子 

	for(i=0; i< foodAmount; i++){
		//如果亂數產生的位置是在牆和蛇身體重疊，則重新產生，直到符合條件為止 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || IsAtSnake(snake2, row, col) || field[row][col]!=EMPTY);
	
		field[row][col] = bomb;
		drawSquare(row,col, BROWN);
	}
}

//吃到炸彈 
bool eatbomb(int field[][GRID_SIDE], NodePointer snake){
	//如果蛇頭與食物位置重疊，就是吃到食物 
	if(field[snake->row][snake->col] == bomb){
		field[snake->row][snake->col] == EMPTY;
		printf("The snake has eaten bomb at row: %d, col: %d\n", snake->row, snake->col);
		return true;
	}
	return false;
}

//清理 
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
				//判斷有沒有射中蛇 
				while(snake2){
					test = snake2->row;
					if(test == currRow)break;
					snake2 = snake2->next;
				}
				//判斷射蛇的路徑有沒有道具 
				while((currCol + i) != test){
					if(field[currRow][currCol+i] != EMPTY)break;
					i++;
				}
				//如果有射中蛇
				if(test == currRow && (currCol + i) == test){
					for(int j = currCol ; j < snake2->col ;j++){
						drawSquare2(currRow,j,5);
					} 
					delay(40);
					for(int j = currCol ; j < snake2->col ;j++){
						drawSquare2(currRow,j,BLACK);
					} 
					//如果沒有防護罩
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//如果沒有射中蛇
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
				//判斷有沒有射中蛇 
				while(snake2){
					test = snake2->row;
					if(test == currRow)break;
					snake2 = snake2->next;
				}
				//判斷射蛇的路徑有沒有道具 
				while((currCol - i) != test){
					if(field[currRow][currCol - i] != EMPTY)break;
					i++;
				}
				//如果有射中蛇
				if( test == currRow && currCol - i == test){
					for(int j = currCol ; j > snake2->col ;j--){
						drawSquare2(currRow,j,5);
					}
					delay(40);
					for(int j = currCol ; j > snake2->col ;j--){
						drawSquare(currRow,j,BLACK);
					} 
					//如果沒有防護罩，再刪除節點 
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//如果沒有射中蛇
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
				//判斷有沒有射中蛇 
				while(snake2){
					test = snake2->col;
					if(test == currCol)break;
					snake2 = snake2->next;
				}
				//判斷射蛇的路徑有沒有道具 
				while((currRow + i) != test){
					if(field[currRow+ i][currCol] != EMPTY)break;
					i++;
				}
				//如果有射中蛇
				if(test == currCol && (currRow + i) == test){
					for(int j = currRow ; j > snake2->row ;j--){
						drawSquare3(j,currCol,5);
					}
					delay(40);
					for(int j = currRow ; j > snake2->row ;j--){
						drawSquare(j,currCol,BLACK);
					}
					//如果沒有防護罩，則對方刪除一個節點 
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//如果沒有射中蛇
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
				//判斷有沒有射中蛇 
				while(snake2){
					test = snake2->col;
					if(test == currCol)break;
					snake2 = snake2->next;
				}
				//判斷射蛇的路徑有沒有道具 
				while((currRow - i) != test){
					if(field[currRow - i][currCol] != EMPTY)break;
					i++;
				}
				//如果有射中蛇 
				if( test == currCol && (currRow - i) == test){
					for(int j = currRow ; j < snake2->row ;j++){
						drawSquare3(j,currCol,5);
					}
					delay(40);
					for(int j = currRow ; j < snake2->row ;j++){
						drawSquare(j,currCol,BLACK);
					}
					//如果沒有防護罩，則對方少一個節點 
					if(protect == 0){
						delNode(head);	
					}
					fflush(stdin);
				}else{//如果沒有射中蛇 
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
//特效喔 橫向 
void drawSquare2(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE / 2;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight * 2 + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //設定繪製物件的為實心和顏色 
   	bar(left , top, right, bottom);	//設定繪製方塊的位置 
}
//特效喔 直向 
void drawSquare3(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE / 2;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth * 2+ col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //設定繪製物件的為實心和顏色 
   	bar(left , top, right, bottom);	//設定繪製方塊的位置 
}

void delNode(NodePointer snake){
	NodePointer head = snake;
	NodePointer tail = head;
	NodePointer newNode = head;
	int length = 0;
	//尋找蛇的尾巴 
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


//------炸別人end 




//增加貪食蛇身體節點
void addNode(NodePointer snake){
	NodePointer tail = NULL;
	NodePointer newNode = (NodePointer) malloc(sizeof(Node)); //初始化一個新節點 
	//尋找蛇的尾巴 
	while(snake != NULL){
		tail = snake;
		snake = snake -> next;
	}
	//將蛇尾巴的方向屬性給新節點 
	newNode -> direct = tail -> direct;
	
	//決定新節點的位置，依據尾巴的移動方向來掛上新節點 
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
	
	tail -> next = newNode; //將尾巴節點連接到新節點 
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

//實作貪食蛇控制邏輯 
Direction AI(int field[][GRID_SIDE], NodePointer snake){
	//請在這裡實作出貪食蛇控制邏輯，必須能夠自己尋找食物和躲避障礙物 
	Direction snakeDirect = snake -> direct;
	return snakeDirect;
}
