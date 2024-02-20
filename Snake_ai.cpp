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
#define TOP_MARGINE 40 //設定上邊界 
#define FOOD_AMOUT 1 //設定每次產生食物數量 
#define INIT_SPEED  0 //設定初始貪食蛇移動速度 (80)

//宣告前進方向列舉函數 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//宣告遊戲場出現物體列舉函數 
enum Object {
	EMPTY, //空白 
	WALL, //牆或障礙物 
	FOOD //食物 
};

//宣告貪食蛇身體節點結構 
struct Node {
	int row; //節點位在第幾行 
	int col; //節點位在第幾列 
	Direction direct; //該節點的前進方向 
	struct Node *next;	//指向下一個節點 
	struct Node *nexthead; //未完成 
}; 

//定義指向節點結構的指標變數 
typedef struct Node *NodePointer;


//宣告路徑節點 
struct path{
	int row; //節點位在第幾行 
	int col; //節點位在第幾列 
	int g_cost;  
	int h_cost; 
	int f_cost;
	struct path *parent;  //指向母節點 
}; 
//定義指向節點結構的指標變數 
typedef struct path *pathptr;

void openWindow(); //開啟游戲視窗 
void closeGame(NodePointer snake); //處理遊戲結束邏輯 
int playGame(int field[][GRID_SIDE], NodePointer snake); //遊戲進行邏輯 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]); //貪食蛇是否死亡(死亡條件：撞牆和撞到自己身體) 
int showGameOverMsg(); //遊戲結束訊息
void showInfo(); //顯示遊戲相關資訊 
void drawGameField(int field[][GRID_SIDE]); //繪製遊戲區域 
void drawSquare(int row, int col, int color); //繪製方塊 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake); //讀取鍵盤方向輸入，並設定到所有貪食蛇節點 
void moveNext(NodePointer snake); //繪製貪食蛇每前進一步的身體改變  
void createFood(int field[][GRID_SIDE], NodePointer snake,int foodamount); //產生食物 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //判斷是否撞到牆  
bool IsAtSnake(NodePointer snake, int row, int col); //判斷是否撞到貪食蛇自己的身體
void eatFood(int field[][GRID_SIDE], NodePointer snake); //處理吃到食物邏輯 
void addNode(NodePointer snake); //增加貪食蛇身體節點 

//--------------------------------------AI--------------------------------------
Direction AI(int field[][GRID_SIDE], NodePointer snake);//實作電腦控制貪食蛇邏輯
Direction shortestpath(NodePointer snake,NodePointer target,int field[][GRID_SIDE],int *flag);//決定最短路徑的方向 
void deletefood(NodePointer head,NodePointer snake);//刪除食物節點 
void addfoodnode(NodePointer head,int row,int col);//新增食物節點 
NodePointer snakecopy(NodePointer snack);//複製一條虛擬蛇 
void fake_moveNext(NodePointer snake);//控制虛擬蛇移動 
void control_fake_SnakeDirection(int field[][GRID_SIDE], NodePointer snake);//決定虛擬蛇方向 
bool fake_eatFood(int field[][GRID_SIDE], NodePointer snake);//判斷虛擬蛇有沒有吃到食物 
NodePointer findtail(NodePointer snake);//找出虛擬蛇的尾巴 
Direction tailpath(NodePointer snake,NodePointer target,int field[][GRID_SIDE],int *flag);//找出到虛擬蛇尾巴的路徑 
Direction farfromfood(NodePointer snake,int field[][GRID_SIDE]);//離食物最遠的格子 
pathptr farfromfoodnode(int row,int col,NodePointer target);//離食物最遠的格子結點
bool Isinfoodlist(NodePointer head,int row,int col);//判斷該座標是否有食物 
bool IsAtSnake_fake(NodePointer snake, int row, int col);//判斷是否有碰到虛擬蛇的身體 
void freesnake(NodePointer snake);//釋放虛擬蛇的記憶體空間 
void control_fake_SnakeDirection_farfromfood(NodePointer snake, int key);//決定虛擬蛇要走的鄰居節點 
void freeclosedlist(int n);//釋放close list的記憶體空間 
void freeopenlist(int n);//釋放open lsit的記憶體空間 
//------------------------function for shortest path-------------------------------
pathptr removefromopen_min();//從最小堆積的open list中移除 
void addtoclosed(pathptr newadd);//加入到close list中 
void addtoopen_min(pathptr newadd);//加入到最小堆積的open list中 
pathptr create(int row,int col,pathptr current,NodePointer target);//新增鄰居節點 
int isinclose(pathptr detect);//判斷是否存在close list中 
int isinopen(pathptr detect);//判斷是否存在open list中		
int h_cost(pathptr detect,NodePointer target); //取得h_cost 
int getoldpath(pathptr detect);//取得之前的路徑 
void updatepath(pathptr detect,pathptr current,NodePointer target); //更新最短路徑
//------------------------function for shortest path-------------------------------


int speed; //貪食蛇移動速度 
int scoreSum = 0; //紀錄分數 
int totalTime = 0; //紀錄遊戲時間 
int const scorePerFood = 1; //每一份食物可得分數 
bool meetSelfWillDie = TRUE; //決定貪食蛇撞到自己身體是否會死掉 

//------------------------------------AI----------------------------------------
bool IFPlayAI = TRUE; //是否開啟AI模式 
pathptr open[GRID_SIDE*GRID_SIDE];	      //A* algorithm
pathptr closed[GRID_SIDE*GRID_SIDE];	  //A* algorithm
int on=0;	//紀錄open list裡的節點數 
int cn=0;	//紀錄closed list裡的節點數 
pathptr neighbor[4];	//當前節點的鄰居(4個方向) 
NodePointer foodlisthead=NULL; //食物串列起點 
NodePointer snakehead=NULL;
      
int main( )
{  	
	openWindow();
	while(TRUE){
		
		Node head = {2, 2, RIGHT, NULL}; //設定蛇頭初始位置和方向 
		NodePointer snake = &head;       //指向蛇頭的指標 
		snakehead = &head;	//設定蛇頭全域變數 
		speed = INIT_SPEED;				//設定初始速度 
		foodlisthead=NULL;              //設定初始食物節點 
		//設定遊戲場和障礙物 
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
		key = playGame(field, snake); //進行遊戲
		if (key == 'q' || key == 'Q')
			closeGame(snake); //如果玩家輸入'q'離開遊戲	
		else if (key == 's' || key == 'S')
			scoreSum = 0;
			totalTime = 0;
			continue; //如果玩家輸入's' 繼續遊戲 		    
	}
}

//開啟游戲視窗
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE *3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry Snake Game");
}

//處理遊戲結束邏輯 
void closeGame(NodePointer snake){
	free(snake);
	exit(0);
}

//遊戲進行邏輯
int playGame(int field[][GRID_SIDE], NodePointer snake){
	
	drawGameField(field); //繪製遊戲區域 
	createFood(field, snake,FOOD_AMOUT); //產生第一份食物 
	while(true){	
		controlSnakeDirection(field, snake); //讀取玩家輸入方向鍵，並將新方向設定到各貪食蛇節點 
		moveNext(snake); //依據節點的方向，繪製新的貪食蛇位置 
		eatFood(field, snake); //判斷是否有吃到食物，如果有新增節點到尾巴 
		showInfo(); //顯示時間和分數資訊 
		
		if(IsGameOver(snake, field)) //判斷是否符合遊戲結束條件， 
			return showGameOverMsg(); //顯示遊戲結束訊息，並等待玩家輸入選項 
		
		//除了吃到食物會產生新食物，系統也隨機產生新食物 
		/*
		srand((unsigned)time(NULL));
		if (rand() % 20 == 0)
			createFood(field, snake);		
		*/
		delay(speed);//決定貪食蛇移動速度，speed越小移動越快 
		//讀取非方向鍵的其他鍵盤輸入 
		if(kbhit()){
			int key;
			key = getch();
			//只有輸入大小寫的a, q 和 s，系統才有反應並將輸入結果拋到外層等待處理 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 		
			else if (key == 'a') //決定是否改變模式 ，主要有玩家模式和電腦操控的AI模式 
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
void moveNext(NodePointer snake){
	int currRow, currCol;
	NodePointer head = snake;
	
	while (snake != NULL){
		currRow = snake->row;
		currCol = snake->col;
		
		//依據節點的方向屬性，設定移動下一步的位置	
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

//貪食蛇是否死亡(死亡條件：撞牆和撞到自己身體) 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]){
	NodePointer head = snake;
	
	//判斷是否撞到牆 
	if (IsAtWall(field, head->row, head->col))
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

//顯示遊戲相關資訊
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
	
	setcolor(WHITE); //設定文字顏色 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1); //設定字型，水平或垂直和字型大小 
   	outtextxy(0, 0, timeMsg); //依據坐標輸出文字到螢幕	
	
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

//讀取鍵盤方向輸入，並設定到所有貪食蛇節點 
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
void createFood(int field[][GRID_SIDE], NodePointer snake,int foodamount){
	int row, col, i;
	NodePointer temp=foodlisthead;
	srand((unsigned)time(NULL)); //取目前系統時間作為亂數種子 
	for(i=0; i< foodamount; i++){
		//如果亂數產生的位置是在牆和蛇身體重疊，則重新產生，直到符合條件為止 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col) || Isinfoodlist(temp,row,col));
		field[row][col] = FOOD;
		drawSquare(row,col, GREEN);	
		addfoodnode(temp,row,col);	
	}
}

//處理吃到食物邏輯
void eatFood(int field[][GRID_SIDE], NodePointer snake){
	//如果蛇頭與食物位置重疊，就是吃到食物 
	if(field[snake->row][snake->col] == FOOD){
		field[snake->row][snake->col] = EMPTY; //將該食物清空
		printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
		addNode(snake); //增加節點 
		scoreSum = scoreSum + scorePerFood; //紀錄分數 
		//刪除該食物節點 
		NodePointer head=foodlisthead;
		deletefood(head,snake);
		createFood(field,snake,1); //產生新的食物 
	}
}

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

//實作貪食蛇控制邏輯 
Direction AI(int field[][GRID_SIDE], NodePointer snake){
	//取得食物節點 
	NodePointer food = foodlisthead;
	//判斷有無找到路徑 
	int canfindpath=1,canfindtail=1;
	//找出吃食物的最短路徑p1 
	Direction p1 = shortestpath(snake,food,field,&canfindpath);
	//做一條虛擬蛇 
	NodePointer snake2=snakecopy(snake);
	
	if(canfindpath){
		while(fake_eatFood(field,snake2)){//移動虛擬蛇去吃食物 		
			control_fake_SnakeDirection(field, snake2); //控制虛擬蛇方向 
			fake_moveNext(snake2); //虛擬蛇移動
		}
		//找出到虛擬蛇尾巴的路徑 
		NodePointer tail2 = findtail(snake2);	
		tailpath(snake2,tail2,field,&canfindtail);
		//如果虛擬蛇吃完食物可以找到去自己尾巴的路徑，則朝p1移動 
		if(canfindtail){
			freesnake(snake2);
			return p1;
		}
	} 
	//如果虛擬蛇找不到自己的尾巴或路徑p1不存在，則往食物最遠的方向移動 
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
	//分別計算各個食物的最短路徑 
	for(j=0;target;j++,target=target->next){
		on=0;
		cn=0;
		//設定起始節點 
		pathptr start=(pathptr) malloc(sizeof(path));
		start->row=snake->row;
		start->col=snake->col;
		start->g_cost=0;
		start->h_cost=0;
		start->f_cost=0;
		start->parent=NULL;
		//將起始點加入open中 
		addtoopen_min(start);
		while(1){
			pathptr current;
			current=removefromopen_min();
			addtoclosed(current);
			if(on==-1){  //找不到路徑 
				*flag=0;
				return RIGHT;
			}
			//找到終點 
			if(current->row==target->row && current->col==target->col){
				break;
			}
			//設定鄰居節點 
			neighbor[0]=create(current->row-1,current->col,current,target);//上 
			neighbor[1]=create(current->row,current->col+1,current,target);//右 
			neighbor[2]=create(current->row+1,current->col,current,target);//下 
			neighbor[3]=create(current->row,current->col-1,current,target);//左 
			for(i=0;i<4;i++){
				if(field[neighbor[i]->row][neighbor[i]->col] == WALL || isinclose(neighbor[i]) || IsAtSnake(snake->next,neighbor[i]->row,neighbor[i]->col)){
					free(neighbor[i]);
					continue;
				}
				int oldpath=getoldpath(neighbor[i]);
				//如果該點的f_cost更小，把舊的f_cost更新
				//如果還不在open list裡面，把它加進去 
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
	//找出離蛇頭最近的食物 
	int min,pos=0;
	min=foodpath[0]->f_cost;
	for(k=1;k<j;k++){
		if(foodpath[k]->f_cost < min){
			min=foodpath[k]->f_cost;
			pos=k;
		}
	}
	//找出第一步的節點 
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
		//設定起始節點 
		pathptr start=(pathptr) malloc(sizeof(path));
		start->row=snake->row;
		start->col=snake->col;
		start->g_cost=0;
		start->h_cost=0;
		start->f_cost=0;
		start->parent=NULL;
		//將起始點加入open中 
		addtoopen_min(start);
		while(1){
			pathptr current;
			current=removefromopen_min();
			addtoclosed(current);
			if(on==-1){  //找不到路徑 
				*flag=0;
				return RIGHT;
			}
			//找到終點 
			if(current->row==target->row && current->col==target->col){
				break;
			}
			//設定鄰居節點 
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
	//找出第一步的節點 
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
		//依據節點的方向屬性，設定移動下一步的位置	
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

bool fake_eatFood(int field[][GRID_SIDE],NodePointer snake){
	//如果蛇頭與食物位置重疊，就是吃到食物 
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
	farr[0]=farfromfoodnode(snake->row-1,snake->col,target);//上 
	farr[1]=farfromfoodnode(snake->row+1,snake->col,target);//下 
	farr[2]=farfromfoodnode(snake->row,snake->col-1,target);//左 
	farr[3]=farfromfoodnode(snake->row,snake->col+1,target);//右 

	int min=0,pos;
	int canfindtail;
	NodePointer tail2; 
	for(int i=0;i<4;i++){
			if(field[farr[i]->row][farr[i]->col] == WALL || IsAtSnake_fake(snake,farr[i]->row,farr[i]->col)){
				continue;
			}
			canfindtail=1;
			NodePointer snake2 = snakecopy(snake);  //複製一條虛擬蛇 
			control_fake_SnakeDirection_farfromfood(snake2,i);//決定蛇頭的方向 
			fake_moveNext(snake2);//朝該方向移動 
			tail2 = findtail(snake2);//找到虛擬蛇的尾巴 
			tailpath(snake2,tail2,field,&canfindtail);//檢查是否有走到自己尾巴的路徑 
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


