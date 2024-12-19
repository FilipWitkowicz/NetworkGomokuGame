#ifndef GAME_H
#define GAME_H

// rozmiar tablicy
#define n 15

extern int turn;

// Deklaracje funkcji
char checkWinner(char board[n][n]);
void makeBoard(char board[n][n]);
int moveValidation(char board[n][n], int row, int column);
void sendMsg(int socket, const char* message);
void handleTurn(int player1_socket, int player2_socket, char board[n][n]);

#endif
