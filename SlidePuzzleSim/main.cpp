#pragma warning(disable:4996)

#include <iostream>				// for general IO
#include <iomanip>				// for setw()

#include <conio.h>				// for getche()

#include <windows.h>			// for COLOR!

#include <time.h>               // for pseudorandom with time()
#include <stdlib.h>


#define NUM_ROWS		3		// should not be changed for this solution
#define NUM_COLS		3		// should not be changed for this soultion
#define PIVOT         NUM_ROWS * NUM_COLS // highest number on board to be changed to *
#define NUM_SCRAMBLE    1000000 // number of times to scramble board

#define PIVOT_SYMBOL	"*"		// used to show the pivot location when drawing the board

// direction codes (part of the slideTile() interface)
#define SLIDE_UP		1		// pass to slideTile() to trigger UP movement
#define SLIDE_DOWN		2		// pass to slideTile() to trigger DOWN movement
#define SLIDE_LEFT		3		// pass to slideTile() to trigger LEFT movement
#define SLIDE_RIGHT		4		// pass to slideTile() to trigger RIGHT movement
#define BRUTE_FORCE     999     // pass to inputLoop() to solve with RNG

#define UNSET			-1		// used to arbitrarily indicate an undetermined state in a constuct

#define COLOR_DEFAULT	7
#define COLOR_RED		12
#define COLOR_GREEN		10


// PROTOTYPES
void InitializeBoard(int[NUM_ROWS][NUM_COLS]);
void PrintBoard(int[NUM_ROWS][NUM_COLS]);
bool slideTile(int[NUM_ROWS][NUM_COLS], int);
void scrambleBoard(int[NUM_ROWS][NUM_COLS]);		// depends upon slideTile()
bool isBoardSolved(int[NUM_ROWS][NUM_COLS]);		// indicates if the board is in the SOLVED state
void randomMove(int[NUM_ROWS][NUM_COLS]);
int inputLoop(int theBoard[NUM_ROWS][NUM_COLS], int keyStroke, int directionCode);


int main() {
	// Declarations
	int slidingBoard[NUM_ROWS][NUM_COLS];		// the board that holds the sliding tiles
	char keyStroke = ' ';						// holds the user's keystrokes as they come in
	int directionCode = UNSET;					// used to build a direction code to be sent to slideTile()
	int totalMoveCount = int(0);

	// Seed the Pseudo-Random Number Generator (system clock)
	srand(time(NULL));

	InitializeBoard(slidingBoard);

	while (isBoardSolved(slidingBoard)) { // to make sure the scrambled board isnt a solved board
		scrambleBoard(slidingBoard);
	}

	while(1) {
		directionCode = UNSET;

		PrintBoard(slidingBoard);

		totalMoveCount += inputLoop(slidingBoard, keyStroke, directionCode);
		
		system("cls"); // clear console window

		// win condition check
		if (isBoardSolved(slidingBoard)) {                    
			PrintBoard(slidingBoard);
			std::cout << "\nboard is solved\ntook " << totalMoveCount << " moves";
			break;
		}
	}
	return 0;
}


void InitializeBoard(int theBoard[NUM_ROWS][NUM_COLS]) {
	int i, j;
	int counter = int(1);
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			theBoard[i][j] = counter;
			counter++;
		}
	}
}


void PrintBoard(int theBoard[NUM_ROWS][NUM_COLS]) {
	int i, j;
	int counter = int(1);

	HANDLE hConsole; // get the handle for the console
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COLOR_GREEN);

	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (theBoard[i][j] == PIVOT) {
				SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
				std::cout << std::setw(3) << PIVOT_SYMBOL;
			}
			else {
				if (theBoard[i][j] == counter) { // print numbers in proper color
					SetConsoleTextAttribute(hConsole, COLOR_GREEN);
				}
				else {
					SetConsoleTextAttribute(hConsole, COLOR_RED);
				}
				std::cout << std::setw(3) << theBoard[i][j];
				SetConsoleTextAttribute(hConsole, COLOR_DEFAULT); // reset color to default
			}
			counter++;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}


bool slideTile(int theBoard[NUM_ROWS][NUM_COLS], int slideDirection) { // return true if legal move, false otherwise
	int i, j;
	int spaceRow = int(NUM_ROWS);
	int spaceCol = int(NUM_COLS);

	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (theBoard[i][j] == PIVOT) {
				spaceRow = i;
				spaceCol = j;
			}
		}
	}

	if (spaceRow == NUM_ROWS - 1 && slideDirection == SLIDE_DOWN) { // detect and prevent illegal moves
		slideDirection = UNSET;                                     // e.g. if pivot is in top row, prevent moving up
	}
	else if (spaceRow == 0 && slideDirection == SLIDE_UP) {
		slideDirection = UNSET;
	}
	else if (spaceCol == NUM_COLS - 1 && slideDirection == SLIDE_RIGHT) {
		slideDirection = UNSET;
	}
	else if (spaceCol == 0 && slideDirection == SLIDE_LEFT) {
		slideDirection = UNSET;
	}

	switch (slideDirection) {
		case SLIDE_UP:
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow - 1][spaceCol];
			theBoard[spaceRow - 1][spaceCol] = PIVOT;
			break;
		case SLIDE_DOWN:
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow + 1][spaceCol];
			theBoard[spaceRow + 1][spaceCol] = PIVOT;
			break;
		case SLIDE_LEFT:
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow][spaceCol - 1];
			theBoard[spaceRow][spaceCol - 1] = PIVOT;
			break;
		case SLIDE_RIGHT:
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow][spaceCol + 1];
			theBoard[spaceRow][spaceCol + 1] = PIVOT;
			break;
		case UNSET: // unset if illegal move
			return false;
	}
	return true;
}


void scrambleBoard(int theBoard[NUM_ROWS][NUM_COLS]) {
	int i = int(0);

	while (i < NUM_SCRAMBLE) {
		randomMove(theBoard);
		i++;
	}
}


bool isBoardSolved(int amISolved[NUM_ROWS][NUM_COLS]) {
	int i, j;
	int counter = int(1);

	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (amISolved[i][j] != counter) {
				return false;
				break;
			}
			counter++;
		}
	}
	return true;
}


int inputLoop(int theBoard[NUM_ROWS][NUM_COLS], int keyStroke, int directionCode) {
	int moveCounter = int(0);
	while (directionCode == UNSET) {
		std::cout << "Input swap direction with WASD, or press B to solve with RNG\r";
		keyStroke = _getch();

		switch (keyStroke) {
		case 'w':
			directionCode = SLIDE_UP;
			slideTile(theBoard, SLIDE_UP);
			moveCounter++;
			break;
		case 'a':
			directionCode = SLIDE_LEFT;
			slideTile(theBoard, SLIDE_LEFT);
			moveCounter++;
			break;
		case 's':
			directionCode = SLIDE_DOWN;
			slideTile(theBoard, SLIDE_DOWN);
			moveCounter++;
			break;
		case 'd':
			directionCode = SLIDE_RIGHT;
			slideTile(theBoard, SLIDE_RIGHT);
			moveCounter++;
			break;
		case 'b':
			while (!isBoardSolved(theBoard)) {
				randomMove(theBoard);
				moveCounter++;
			}
			system("cls");
			directionCode = BRUTE_FORCE;
			break;
		default:
			directionCode = UNSET;
		}
	}
	return moveCounter;
}


void randomMove(int theBoard[NUM_ROWS][NUM_COLS]) { // because i am lazy, also this doesnt work sometimes
	switch (rand() % 4 + 1) {

	case 1:
		slideTile(theBoard, SLIDE_UP);
		break;
	case 2:
		slideTile(theBoard, SLIDE_DOWN);
		break;
	case 3:
		slideTile(theBoard, SLIDE_LEFT);
		break;
	case 4:
		slideTile(theBoard, SLIDE_RIGHT);
		break;
	}
}
