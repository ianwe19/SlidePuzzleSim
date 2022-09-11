#pragma warning(disable:4996)

#include <iostream>				// for general IO
#include <iomanip>				// for setw()

#include <string.h>				// for strcpy()
#include <conio.h>				// for getche()

#include <windows.h>			// for COLOR!

#define NUM_ROWS		3		// should not be changed for this solution
#define NUM_COLS		3		// should not be changed for this soultion

#define PIVOT -1				// used to mark the pivot spot (blank area) on the puzzle
#define PIVOT_SYMBOL	"*"		// used to show the pivot location when drawing the board

// direction codes (part of the slideTile() interface)
#define SLIDE_UP		1		// pass to slideTile() to trigger UP movement
#define SLIDE_DOWN		2		// pass to slideTile() to trigger DOWN movement
#define SLIDE_LEFT		3		// pass to slideTile() to trigger LEFT movement
#define SLIDE_RIGHT		4		// pass to slideTile() to trigger RIGHT movement

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

// DEVELOPMENT EXTRAS
void printTheRainbow();								// A little reminder on how to do color with the Windows API.


int main() { // use loop
	// Declarations
	int slidingBoard[NUM_ROWS][NUM_COLS];		// the board that holds the sliding tiles
	char keyStroke = ' ';						// holds the user's keystrokes as they come in
	int directionCode = UNSET;					// used to build a direction code to be sent to slideTile()

	// Seed the Pseudo-Random Number Generator (system clock)

	// Driver Logic
	// 1.) This is the part where you show the board, get the moves, process the moves, and re-draw
	//  the board each time something changes.  This is the core logic of the simulation and
	//  none of the problem specific calculations should take place in main().  Instead,
	//  main() should consist of a series of controlled calls to your functions that
	//  orchestrate the top-level behavior of the simulation.

	InitializeBoard(slidingBoard);
	PrintBoard(slidingBoard);

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
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			std::cout << std::setw(3) << theBoard[i][j];
		}
		std::cout << "\n";
	}
}

bool slideTile(int theBoard[NUM_ROWS][NUM_COLS], int slideDirection) { // return true if successful move
	// YOUR IMPLEMENTATION GOES HERE...
	return false;
}

void scrambleBoard(int theBoard[NUM_ROWS][NUM_COLS]) { // 10k - 100k random legal moves
	// YOUR IMPLEMENTATION GOES HERE...
}

bool isBoardSolved(int amISolved[NUM_ROWS][NUM_COLS]) {
	// YOUR IMPLEMENTATION GOES HERE...
	return false;
}

// EXTRAS
void printTheRainbow() {
	int currentColor = 7;

	// get the handle for the console
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// exhaustively print the colors
	std::cout << std::left;
	for (currentColor = 0; currentColor <= 255; currentColor++) {
		SetConsoleTextAttribute(hConsole, currentColor);
		std::cout << std::setw(5) << currentColor << "Look at the pretty COLORS!\n";
	}
}