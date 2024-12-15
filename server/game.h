#ifndef GAME_H
#define GAME_H

#define n 15

// Deklaracje zmiennych globalnych
extern char move[10];
extern char board[n][n];
extern int turn;
extern int row, column;
extern char winner;

// Deklaracje funkcji
char checkWinner(char board[n][n]);
void makeBoard(char board[n][n]);
void initializeGame();


#endif
