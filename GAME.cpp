#include <iostream>
#include "raylib.h"
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <vector>
#include "header.hpp"
using namespace std;

//-------------------------Resources-------------------------

const int gridWidth = 260;
const int gridHeight =360;
const int gridSize = 20;

bool gameRunning = false;
bool showMenu = true;
bool showInputScreen = false;
bool highscoreScreen = false;
bool sentFirst = false;
bool gamePaused = false;

int score = 0 ;
int tet1 ;
int tet2 ;
int degree = 0 ;
Color color2 ;
LinkedList grid1[18];
PlayerInfo player;
LinkedList name;
int letterCount = 0;

//---------------------------TETRIS FUNCTIONS---------------------------

// Grid Making
void DrawTetrisGrid() {
	// Draw vertical lines
	for (int x = 0; x <= gridWidth; x += gridSize) {
		for (int y = 0; y <= gridHeight; y += gridSize) {
			if (x == 0 || x == gridWidth || y == gridHeight) {
				// Draw side and bottom boxes in white
				DrawRectangle(x, y, gridSize, gridSize, WHITE);
			} 
			else {
				// Leave the middle boxes unfilled
				DrawRectangleLines(x, y, gridSize, gridSize, WHITE);
			}
		}
	}
}

// Printing array
void print() {
	for ( int i = 0 ; i < 18 ; i++ ) {
		grid1[i].printCol(i);
		cout<<endl;
	}
}

// Swapping completed line by above line
void swap(int i) {
	for ( i ; i > 0 ; i-- ) {
		grid1[i].sethead(grid1[i-1].gethead());
		if( grid1[i].isStar() == 0 ){
			grid1[i].makeNewHead();
		}
	}
}

// Checking for line completion
void check() {
	for ( int i = 0 ; i < 18 ; i++ ) {
		if (grid1[i].isStar()==12) {
			swap(i);
			score+=100;
			print();
		}
	}
}

// Generating Random Shapes & colors
int generateShapes() {
	int type = rand () % 5;
	int colorVar = rand () % 6;
	switch(colorVar){
		case 0 :
			color=MAROON;
			break;
		case 1 :
			color = { 0, 255, 255, 225 };
			break;
		case 2 :
			color=ORANGE;
			break;
		case 3 :
			color=GREEN;
			break;
		case 4 :
			color=VIOLET;
			break;
		case 5 :
			color=DARKBLUE;
			break;					
	}
	return type;
}

void generateColor() {
	int colorVar2 = rand () % 6;
	switch(colorVar2){
		case 0 :
			color2=MAROON;
			break;
		case 1 :
			color2={ 0, 255, 255, 225 };
			break;
		case 2 :
			color2=ORANGE;
			break;
		case 3 :
			color2=GREEN;
			break;
		case 4 :
			color2=VIOLET;
			break;
		case 5 :
			color2=DARKBLUE;
			break;					
	}
}

// Creating Shapes
void instruction(int tet2) {
	DrawRectangleLines(300, 85, 180, 120, WHITE);
	DrawText(TextFormat("CONTROLS:"),310,100,15,RAYWHITE);
	DrawText(TextFormat("' D ' TO MOVE RIGHT"),310,125,15,RAYWHITE);
	DrawText(TextFormat("' A ' TO MOVE LEFT", score),310,145,15,RAYWHITE);
	DrawText(TextFormat("' S ' TO MOVE DOWN", score),310,165,15,RAYWHITE);

	// Draw a box on the side with white borders
	DrawRectangleLines(300, 220, 180, 120, WHITE);

	// Draw four red boxes inside the white-bordered box
	switch(tet2) {
		case 0: //"****"
			DrawRectangle(340, 240, 20, 20, color2);
			DrawRectangle(361, 240, 20, 20, color2);
			DrawRectangle(382, 240, 20, 20, color2);
			DrawRectangle(403, 240, 20, 20, color2);
			break;
		case 1:	// L 
			DrawRectangle(340, 230, 20, 20, color2);
			DrawRectangle(340, 251, 20, 20, color2);
			DrawRectangle(340, 272, 20, 20, color2);
			DrawRectangle(361, 272, 20, 20, color2);
			break;
		case 2://	"D"
			DrawRectangle(340, 240, 20, 20, color2);
			DrawRectangle(361, 240, 20, 20, color2);
			DrawRectangle(340, 261, 20, 20, color2);
			DrawRectangle(361, 261, 20, 20, color2);
			break;
		case 3:	//   ***
			//        *
			DrawRectangle(340, 240, 20, 20, color2);
			DrawRectangle(361, 240, 20, 20, color2);
			DrawRectangle(382, 240, 20, 20, color2);
			DrawRectangle(361, 261, 20, 20, color2);
			break;
		case 4:	//   *
			//       **
			//        *
			DrawRectangle(340, 235, 20, 20, color2);
			DrawRectangle(340, 256, 20, 20, color2);
			DrawRectangle(361, 256, 20, 20, color2);
			DrawRectangle(361, 277, 20, 20, color2);
			break;	

	}
}

// Placing and generating
bool placeTet(int &x1,int &x2,int &x3,int &x4,int &y1,int &y2,int &y3,int &y4) {
	
	if(y4==340 || y3 == 340	 || (grid1[y1/20+1].checkNext(x1/20)=='*') || (grid1[y2/20+1].checkNext(x2/20)=='*') || (grid1[y3/20+1].checkNext(x3/20)=='*')
	   || (grid1[y4/20+1].checkNext(x4/20)=='*')) {
		grid1[(y1)/20].insertStar((x1-20)/20);
		grid1[(y2)/20].insertStar((x2-20)/20);
		grid1[(y3)/20].insertStar((x3-20)/20);
		grid1[(y4)/20].insertStar((x4-20)/20);
		
		degree = 0;
		
		check();
		
		if(y1==0||y2==0||y3==0||y4==0) {
			gameRunning = false;
			showMenu = false;
		}
		return true;
	} 
	else {
		return false;
	}
}

//Check for NoMovement/RotationRoom
bool checkMove(int &x1,int &x2,int &x3,int &x4,int &y1,int &y2,int &y3,int &y4){
	if(x1-20<20||x2-20<20||x3-20<20||x4-20<20) return false;
	return true;
}

//Check After Rotation Tetris doesn't cross boundary
bool checkRotateMove(int &x1,int &x2,int &x3,int &x4,int &y1,int &y2,int &y3,int &y4){
	if(x1>19&&x2>19&&x3>19&&x4>19&&x1<241&&x2<241&&x3<241&&x4<241) return false;
	return true;
}

//Rotate Function
void rotate(int &x1,int &x2,int &x3,int &x4,int &y1,int &y2,int &y3,int &y4){
	switch(tet1){
		case 0 : { //"****"
			switch(degree){
				case 0 :
					degree++;
					x1 = x2;
					y1 = y2-20;
					x3 = x2;
					y3 = y2+20;
					x4 = x2;
					y4 = y2+40;
				break;
				case 1 :
					x1 = x2-20;
					y1 = y2;
					x3 = x2+20;
					y3 = y2;
					x4 = x2 +40;
					y4 = y2;
					degree--;
				break;
			}
		break;
		}	
		case 1 : {// L
			switch(degree){
				case 0 :
					degree++;
					x1 = x2-20;
					y1 = y2;
					x3 = x2+20;
					y3 = y2;
					x4 = x3;
					y4 = y2-20;
				break;
				case 1 :
					degree++;
					x1 = x2-20;
					y1 = y2;
					x3 = x2+20;
					y3 = y2;
					x4 = x2-20;
					y4 = y1+20;
				break;
				case 2 :
					degree++;
					x1 = x2;
					y1 = y2-20;
					x3 = x2;
					y3 = y2+20;
					x4 = x2-20;
					y4 = y1;
				break;
				case 3:
					x1 = x2 ;
					y1 = y2 - 20;
					x3 = x2 ;
					y3 = y2 + 20;
					x4 = x2 + 20;
					y4 = y2 + 20;
					degree = 0;
					break;
			}	
		break;
		}
		case 3 : {	
					//   ***
				//        *
			switch(degree){
				case 0 :
					degree++;
					x1 = x2;
					y1 = y2-20;
					x3 = x2;
					y3 = y2+20;
					x4 = x2+20;
					y4 = y2;
				break;
				case 1 :
					degree++;
					x1 = x2-20;
					y1 = y2;
					x3 = x2+20;
					y3 = y2;
					x4 = x2;
					y4 = y2-20;
				break;
				case 2 :
					degree++;
					x1 = x2;
					y1 = y2-20;
					x3 = x2;
					y3 = y2+20;
					x4 = x2-20;
					y4 = y2;
				break;
				case 3:
					x1 = x2-20;
					y1 = y2;
					x3 = x2+20;
					y3 = y2;
					x4 = x2;
					y4 = y2+20;
					degree = 0;
					break;
			}	
		break;
		}
		case 4 : {	
			switch(degree){
				case 0 :
					degree++;
					x3 = x1+20;
					y3 = y1;
					x4 = x2-20;
					y4 = y2;
				break;
				case 1 :
					degree--;
					x3 = x2+20;
					y3 = y2;
					x4 = x2+20;
					y4 = y2+20;
				break;
			}	
		break;
		}
		default:
			break;	
	}
}

void generateTet(int &x1,int &x2,int &x3,int &x4,int &y1,int &y2,int &y3,int &y4,bool &sent,float &timer) {
	timer += GetFrameTime();
	ClearBackground({ 0,0,0, 255 });

	if (IsKeyPressed(KEY_D)) {
		if(x4+20<241 && x3+20<241 && x2+20<241 && x1+20<241  && grid1[y1/20].checkNext(x1/20+1) != '*' &&  grid1[y2/20].checkNext(x2/20+1) != '*' 
		&&  grid1[y3/20].checkNext(x3/20+1) != '*' && grid1[y4/20].checkNext(x4/20+1) != '*') {
			x1+=20;
			x2+=20;
			x3+=20;
			x4+=20;
		}
	}
	if (IsKeyPressed(KEY_A)) {
		if(x1-20>19 && x2-20>19 && x3-20>19 && x4-20>19 && grid1[y1/20].checkNext(x1/20-1) != '*' &&  grid1[y2/20].checkNext(x2/20-1) != '*' &&  grid1[y3/20].checkNext(x3/20-1) != '*'
		&& grid1[y4/20].checkNext(x4/20-1) != '*') {
			x1-=20;
			x2-=20;
			x3-=20;
			x4-=20;
		}
	}
	
	if (IsKeyPressed(KEY_S)) {
		if(y4+20 <= 340) {
			y1+=20;
			y2+=20;
			y3+=20;
			y4+=20;
		}
	}
	if (IsKeyPressed(KEY_W)){
		int tempx1=x1,tempx2=x2,tempx3=x3,tempx4=x4,tempy1=y1,tempy2=y2,tempy3=y3,tempy4=y4;
		rotate(x1,x2,x3,x4,y1,y2,y3,y4);
		if(checkRotateMove(x1,x2,x3,x4,y1,y2,y3,y4)==true){
			x1 = tempx1 ,x2=tempx2,x3=tempx3,x4=tempx4,y1=tempy1,y2=tempy2,y3=tempy3,y4=tempy4;
		}
	}
	if (IsKeyPressed(KEY_P)) {
		gamePaused = true;
	}

	DrawRectangle(x1,y1,20,20,color);
	DrawRectangle(x2,y2,20,20,color);
	DrawRectangle(x3,y3,20,20,color);
	DrawRectangle(x4,y4,20,20,color);

	if(placeTet(x1,x2,x3,x4,y1,y2,y3,y4)==true) {
		sent = false;
	} 
	else {
		if(timer > 0.25f) {
			y1+=20;
			y2+=20;
			y3+=20;
			y4+=20;
			timer = 0;
		}
	}
}

void swapTet(int &tet1,int &tet2) {
	int temp = tet2;
	tet2 = generateShapes();
	tet1 = temp;
	color = color2;
	generateColor();
}


// ---------------------------SORTING HIGHSCORE---------------------------

int partition(vector<PlayerInfo> &arr, int low, int high) {
	int pivot = arr[high].getScore();
	int i = low - 1;

	for (int j = low; j <= high - 1; j++) {
		if (arr[j].getScore() >= pivot) {
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return i + 1;
}

void quickSort(vector<PlayerInfo> &arr, int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high);

		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}


//---------------------------SCREEN FUNCTIONS---------------------------

void menu(Texture2D background) {
	DrawTexture(background,0,0, WHITE);
	Rectangle buttonRegion1 = {screenWidth/2-85,screenHeight/2+9, 167, 59};
	Rectangle buttonRegion2 = {screenWidth/2-85,screenHeight/2+85, 167, 59};

	if(CheckCollisionPointRec(GetMousePosition(), buttonRegion1)||CheckCollisionPointRec(GetMousePosition(), buttonRegion2)) {
		SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
	} 
	else {
		SetMouseCursor(MOUSE_CURSOR_ARROW);
	}
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), buttonRegion1)) {
		SetMouseCursor(MOUSE_CURSOR_ARROW);
		showMenu=false;
		showInputScreen = true;
	}
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), buttonRegion2)) {
		SetMouseCursor(MOUSE_CURSOR_ARROW);
		DrawText("HighSCORE!", 10, 200, 20, RED);
		showMenu=false;
		showInputScreen=false;
		highscoreScreen=true;
	}
}

void inputScreen() {
	ClearBackground(BLACK);
	DrawText(TextFormat("Enter Name:"),screenWidth/2-150,screenHeight/2,15,RAYWHITE);
	int key = GetKeyPressed();

	if(key !=0 && key > 64 && key < 100) {
		ClearBackground(BLACK);
		name.push(key,letterCount);
		name.printName();
	} 
	else if((key == KEY_ENTER || key == KEY_KP_ENTER) && letterCount > 0) {
		letterCount = 0;
		showMenu = false;
		gameRunning = true;
		showInputScreen = false;
		player.setName(name);
	} 
	else if(key == KEY_BACKSPACE && letterCount > 0) {
		name.deleteLast(letterCount);
	}
	name.printName();
}

void highScoreScreen() {
    ClearBackground(BLACK);
    DrawText("HIGHSCORES!", 120, 170, 35, RED);

    vector<PlayerInfo> sortedScores = PlayerInfo::retrieveScorefromFile();
    int n = sortedScores.size();

    quickSort(sortedScores, 0, n - 1);

    int rank = 1;
    for (int i = 0; i < n; ++i) {
        const PlayerInfo &player = sortedScores[i];
        DrawText(TextFormat("%d) %s - %d", rank, player.getName(), player.getScore()), 120, 220 + (rank - 1) * 40, 20, RED);
        rank++;
        if (rank > 3)
            break;
    }
}

void pauseScreen() {
    ClearBackground({ 0, 0, 0, 150 });
    DrawText("PAUSED", screenWidth / 2 - 50, screenHeight / 2 - 20, 30, RED);
    DrawText("Press 'P' to Resume", screenWidth / 2 - 80, screenHeight / 2 + 20, 15, WHITE);
    if (IsKeyPressed(KEY_P)) {
		gamePaused = false;
	}
}


int main(void) {
	for( int i = 0 ; i < 18 ; i++ ) {
		grid1[i].makeCols();
	}
	
	InitWindow(screenWidth, screenHeight, "Tetris");
	SetTargetFPS(60);
	
	bool sent = false;
	int type = generateShapes();
	float timer = 0.0f;
	int x1,x2,x3,x4,y1,y2,y3,y4;
	Texture2D background = LoadTexture("mainmenu1.png");
	while(!WindowShouldClose()) {
		BeginDrawing();
		if(gameRunning && gamePaused == false){
			if(sent == false) {
				if(sentFirst == false) {
					tet1 = generateShapes();
					tet2 = generateShapes();
					generateColor();
					sentFirst = true;
				} 
				else {
					swapTet(tet1,tet2);
				}
				int initialX = 100;
				int initialY = 0;
				switch(tet1) {
					case 0: //"****"
						sent = true;
						x1 = initialX;
						y1 = initialY;
						x2 = initialX + 20;
						y2 = initialY;
						x3 = initialX + 40;
						y3 = initialY;
						x4 = initialX + 60;
						y4 = initialY;
						generateTet(x1,x2,x3,x4,y1,y2,y3,y4,sent,timer);
						break;
					case 1:	// L
						sent = true;
						x1 = initialX;
						y1 = initialY;
						x2 = initialX;
						y2 = initialY + 20;
						x3 = initialX;
						y3 = initialY + 40;
						x4 = initialX + 20;
						y4 = initialY + 40;
						generateTet(x1,x2,x3,x4,y1,y2,y3,y4,sent,timer);
						break;
					case 2://	"D"
						sent = true;
						x1 = initialX;
						y1 = initialY;
						x2 = initialX + 20;
						y2 = initialY;
						x3 = initialX;
						y3 = initialY + 20;
						x4 = initialX + 20;
						y4 = initialY + 20;
						generateTet(x1,x2,x3,x4,y1,y2,y3,y4,sent,timer);
						break;
					case 3:	//   ***
						//        *
						sent = true;
						x1 = initialX;
						y1 = initialY;
						x2 = initialX + 20;
						y2 = initialY;
						x3 = initialX + 40;
						y3 = initialY;
						x4 = initialX + 20;
						y4 = initialY + 20;
						generateTet(x1,x2,x3,x4,y1,y2,y3,y4,sent,timer);
						break;
					case 4:	//   *
						//       **
						//        *
						sent = true;
						x1 = initialX;
						y1 = initialY;
						x2 = initialX ;
						y2 = initialY + 20;
						x3 = initialX + 20;
						y3 = initialY + 20;
						x4 = initialX + 20;
						y4 = initialY + 40;
						generateTet(x1,x2,x3,x4,y1,y2,y3,y4,sent,timer);
						break;	
				}
			} 
			else {
				generateTet(x1,x2,x3,x4,y1,y2,y3,y4,sent,timer);
			}
			instruction(tet2);
			print();
			DrawTetrisGrid();
			DrawText(TextFormat("Score: %i", score),320,20,15,RAYWHITE);
		} 
		else if(showMenu == false && gameRunning == false && showInputScreen == false && highscoreScreen == false) {
			if(writtenInFile == false) {
				score = 0;
				player.setScore(score);
				player.writeScoretoFile();
				for( int i = 0 ; i < 18 ; i++ ) {
					grid1[i].makeCols();
				}
			}
			
			ClearBackground({ 0,0,0, 255 });
			DrawText("GAME OVER!",screenWidth/2-150,screenHeight/2-50,50,RED);
			DrawText(TextFormat("SCORE : %i", score),screenWidth/2-150,screenHeight/2,20,RED);
			name.clearList();
			
			DrawText("Press BackSpace to Return to MainMenu", screenWidth / 2 - 150, screenHeight / 2 + 50, 15, RAYWHITE);
			if (IsKeyPressed(KEY_BACKSPACE)) {
				showMenu = true;
				writtenInFile = false;
			}
		} 
		else if(showMenu == true && gameRunning == false) {
			menu(background);
		} 
		else if(showMenu == false && showInputScreen) {
			inputScreen();
		} 
		else if(showMenu == false && showInputScreen == false && gameRunning == false && highscoreScreen == true) {
			highScoreScreen();

			DrawText("Press BackSpace to Return to MainMenu", screenWidth / 2 - 150, screenHeight / 2 +200, 15, RAYWHITE);
			if (IsKeyPressed(KEY_BACKSPACE)) {
				showMenu = true;
				highscoreScreen = false;
			}
		}
		else if(gamePaused == true){
			pauseScreen();
		}

		EndDrawing();
	}
	
	UnloadTexture(background);
	return 0;
}