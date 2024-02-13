#include <iostream>
#include "raylib.h"
#include <vector>
using namespace std;

const int screenWidth = 500;
const int screenHeight =500;
Color color ;
bool writtenInFile = false;

class Node {
	public:
		char d;
		Node* next;
		int x;
		Color c;
	public:
		Node() {
			d = ' ';
			next = NULL;
		}
		Node(char key) {
			d = key;
			next = NULL;
		}
};

class LinkedList {
	private:
		Node* head;

	public:
		LinkedList() {
			head = NULL;
		}

		void push(int key,int &letterCount) {
			if (head == NULL) {
				head = new Node(key);
			} 
			else {
				Node* newNode = new Node(key);
				Node* tmp = head;
				while (tmp->next != NULL) {
					tmp = tmp->next;
				}
				tmp->next = newNode;
			}
			letterCount++;
		}

		void printName() {
			Node* tmp = head;
			int xPos = screenWidth/2-60;
			int yPos = screenHeight/2;
			while (tmp != NULL) {
				char charString[2] = {tmp->d, '\0'}; 		// Create a null-terminated string
				DrawText(charString, xPos, yPos, 15, WHITE);
				xPos += 11; 		// Adjust the x-position for the next character
				tmp = tmp->next;
			}
		}

		void deleteLast(int & letterCount) {
			Node* tmp = head;
			Node* prev = NULL;
			while (tmp->next != NULL) {
				prev = tmp;
				tmp = tmp->next;
			}
			if (tmp == head) {
				head = NULL;
				delete tmp;
			} 
			else {
				delete tmp;
				prev->next = NULL;
			}
			letterCount--;
			cout<<letterCount;
		}

		void makeCols() {
			head = new Node();
			head->c = RED;
			head->x = 0;
		}

		void insertStar(int x) {
			Node* tmp = head;
			while(tmp->next!= NULL ){
				tmp = tmp->next;
			}
			if( x == 0 ){
				head->c = color;
				head->d = '*';
				return;
			}
			tmp->next = new Node('*');
			tmp->next->x = x;
			tmp->next->c = color;
		}

		void printCol(int i) {
			Node* tmp = head;
//			int j = 0;
			while(tmp!=NULL){
				if( tmp->d == '*' ) {
					DrawRectangle(tmp->x*20+20,i*20,20,20,tmp->c);
				}
				tmp = tmp->next;
			}	
		}

		char checkNext(int x) {
			Node* tmp = head;
			while(tmp!=NULL){
				if(tmp->x == (x-1) && tmp->d == '*') {
					return '*';
				}
				tmp = tmp->next;
			}
			return ' ';
		}

		int isStar() {
			Node* tmp = head;
			int count = 0 ;
			while(tmp!=NULL){
				if(tmp->d == '*') {
					count++;
				}
				tmp = tmp->next;
			}
			return count;
		}

		Node* gethead() {
			return head;
		}

		void sethead(Node* h) {
			head = h;
		}

		void clearList() {
			Node* current = head;
			Node* next = NULL;

			while (current != NULL) {
				next = current->next;
				delete current;
				current = next;
			}
			head = NULL;
		}
		
		void makeNewHead(){
			head = new Node();
			head->c = RED;
			head->x = 0;
		}
};

class PlayerInfo {
	private:
		char name[256]; 		// Using char array instead of string for fwrite/fread
		int score;

	public:
		void setName(LinkedList n) {
			Node *tmp = n.gethead();
			int i = 0;
			while (tmp != NULL && i < 255) {
				name[i++] = tmp->d;
				tmp = tmp->next;
			}
			name[i] = '\0'; 	// Null-terminate the char array
		}

		void setScore(int s) {
			score = s;
		}

		void writeScoretoFile() {
			FILE *scoreFile = fopen("HighScores.dat", "ab");

			if (scoreFile != NULL) {
				fwrite(this, sizeof(PlayerInfo), 1, scoreFile);
				fclose(scoreFile);
				writtenInFile = true;
			} 
			else {
				cout << "Unable to open file!" << endl;
			}
		}

		static vector<PlayerInfo> retrieveScorefromFile() {
			vector<PlayerInfo> scores;
			FILE *scoreFile = fopen("HighScores.dat", "rb");

			if (scoreFile != NULL) {
				PlayerInfo temp;
				while (fread(&temp, sizeof(PlayerInfo), 1, scoreFile)) {
					scores.push_back(temp);
				}
				fclose(scoreFile);
			}
			return scores;
		}

		const char *getName() const {
			return name;
		}

		int getScore() const {
			return score;
		}
};