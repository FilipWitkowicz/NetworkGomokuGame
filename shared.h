#ifndef GAME_H
#define GAME_H

// rozmiar tablicy
#define n 15

extern int turn;

// Deklaracje funkcji
char checkWinner(char board[n][n]);
void makeBoard(char board[n][n]);
int moveValidation(char board[n][n], int row, int column);
int sendMsg(int socket, const char* message);
int handleTurn(int playing, int waiting, char board[n][n]);
int checkDraw(char board[n][n]);

#endif
