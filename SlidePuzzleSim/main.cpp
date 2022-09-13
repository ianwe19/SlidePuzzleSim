#pragma warning(disable:4996)

#include <iostream>				// for general IO
#include <iomanip>				// for setw()

#include <string.h>				// for strcpy()   UNUSED
#include <conio.h>				// for getche()

#include <windows.h>			// for COLOR!

#include <time.h>               // for pseudorandom with time()
#include <stdlib.h>


#define NUM_ROWS		3		// should not be changed for this solution
#define NUM_COLS		3		// should not be changed for this soultion
#define MAX_NUM         NUM_ROWS * NUM_COLS // highest number on board to be changed to *
#define NUM_SCRAMBLE    1000000 // number of times to scramble board

#define PIVOT -1				// used to mark the pivot spot (blank area) on the puzzle   UNUSED
#define PIVOT_SYMBOL	"*"		// used to show the pivot location when drawing the board

// direction codes (part of the slideTile() interface)
#define SLIDE_UP		1		// pass to slideTile() to trigger UP movement
#define SLIDE_DOWN		2		// pass to slideTile() to trigger DOWN movement
#define SLIDE_LEFT		3		// pass to slideTile() to trigger LEFT movement
#define SLIDE_RIGHT		4		// pass to slideTile() to trigger RIGHT movement
#define BRUTE_FORCE     999

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
void inputLoop(int theBoard[NUM_ROWS][NUM_COLS], int keyStroke, int directionCode);


int main() { // use loop
	// Declarations
	int slidingBoard[NUM_ROWS][NUM_COLS];		// the board that holds the sliding tiles
	char keyStroke = ' ';						// holds the user's keystrokes as they come in
	int directionCode = UNSET;					// used to build a direction code to be sent to slideTile()

	// Seed the Pseudo-Random Number Generator (system clock)
	srand(time(NULL));

	InitializeBoard(slidingBoard);

	while (isBoardSolved(slidingBoard)) { // to make sure the scrambled board isnt a solved board
		scrambleBoard(slidingBoard);
	}

	while(1) {
		directionCode = UNSET;

		PrintBoard(slidingBoard);

		inputLoop(slidingBoard, keyStroke, directionCode);
		
		system("cls"); // probably bad

		// win condition check
		if (isBoardSolved(slidingBoard)) {                    
			PrintBoard(slidingBoard);
			std::cout << "\nboard is solved";
			break;
		}
	}
	return 0;
}


void InitializeBoard(int theBoard[NUM_ROWS][NUM_COLS]) {
	int i, j;
	int x = int(1);
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			theBoard[i][j] = x;
			x++;
		}
	}
}


void PrintBoard(int theBoard[NUM_ROWS][NUM_COLS]) {
	int i, j;
	int counter = int(1);

	// get the handle for the console
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COLOR_GREEN);

	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (theBoard[i][j] == MAX_NUM) {
				if (counter == MAX_NUM) { // print PIVOT_SYMBOL in proper color
					SetConsoleTextAttribute(hConsole, COLOR_GREEN);
				}
				else {
					SetConsoleTextAttribute(hConsole, COLOR_RED);
				}
				std::cout << std::setw(3) << PIVOT_SYMBOL;
				SetConsoleTextAttribute(hConsole, COLOR_DEFAULT); // reset color to default
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


bool slideTile(int theBoard[NUM_ROWS][NUM_COLS], int slideDirection) { // return true if successful move
	int i, j;
	int* spacePos = NULL; // unused so far
	int spaceRow = int(NUM_ROWS);
	int spaceCol = int(NUM_COLS);

	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (theBoard[i][j] == MAX_NUM) {
				spacePos = &theBoard[i][j];
				spaceRow = i;
				spaceCol = j;
			}
		}
	}

	if (spaceRow == NUM_ROWS - 1 && slideDirection == SLIDE_DOWN) { // detect and prevent illegal moves
		slideDirection = UNSET;
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
		case SLIDE_UP    :
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow - 1][spaceCol];
			theBoard[spaceRow - 1][spaceCol] = MAX_NUM;
			break;
		case SLIDE_DOWN  :
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow + 1][spaceCol];
			theBoard[spaceRow + 1][spaceCol] = MAX_NUM;
			break;
		case SLIDE_LEFT  :
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow][spaceCol - 1];
			theBoard[spaceRow][spaceCol - 1] = MAX_NUM;
			break;
		case SLIDE_RIGHT :
			theBoard[spaceRow][spaceCol] = theBoard[spaceRow][spaceCol + 1];
			theBoard[spaceRow][spaceCol + 1] = MAX_NUM;
			break;
		case UNSET       : // unset if illegal move
			return false;
	}
	return true;
}


void scrambleBoard(int theBoard[NUM_ROWS][NUM_COLS]) { // 10k - 100k random legal moves
	int i = int(0);

	while (i < NUM_SCRAMBLE) {
		randomMove(theBoard); // feels wrong but might as well just use this
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


void inputLoop(int theBoard[NUM_ROWS][NUM_COLS], int keyStroke, int directionCode) {
	while (directionCode == UNSET) {
		std::cout << "Input swap direction with WASD, or B to solve with highly advanced AI algorithms\r";
		keyStroke = _getch();

		switch (keyStroke) {
		case 'w':
			directionCode = SLIDE_UP;
			slideTile(theBoard, SLIDE_UP);
			break;
		case 'a':
			directionCode = SLIDE_LEFT;
			slideTile(theBoard, SLIDE_LEFT);
			break;
		case 's':
			directionCode = SLIDE_DOWN;
			slideTile(theBoard, SLIDE_DOWN);
			break;
		case 'd':
			directionCode = SLIDE_RIGHT;
			slideTile(theBoard, SLIDE_RIGHT);
			break;
		case 'b':
			while (!isBoardSolved(theBoard)) {
				randomMove(theBoard);
			}
			system("cls");
			directionCode = BRUTE_FORCE;
			break;
		default:
			directionCode = UNSET;
		}
	}
}


void randomMove(int theBoard[NUM_ROWS][NUM_COLS]) { // because i am lazy, also this doesnt work sometimes for brute force solving
	switch (rand() % 4 + 1) {                       // possibly because some boards are unsolvable?

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
