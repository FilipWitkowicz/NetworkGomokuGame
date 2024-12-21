#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 

#include <pthread.h>
#include "shared.h"

int turn = 0;
char ack2[5];


char checkWinner(char board[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] != '-') {
                // Sprawdzanie poziomej linii
                if (j + 4 < n) {
                    if (board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] &&
                        board[i][j] == board[i][j + 3] && board[i][j] == board[i][j + 4]) {
                        return board[i][j];
                    }
                }

                // Sprawdzanie pionowej linii
                if (i + 4 < n) {
                    if (board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] &&
                        board[i][j] == board[i + 3][j] && board[i][j] == board[i + 4][j]) {
                        return board[i][j];
                    }
                }

                // Sprawdzanie ukośnej linii (w prawo w dół)
                if (i + 4 < n && j + 4 < n) {
                    if (board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] &&
                        board[i][j] == board[i + 3][j + 3] && board[i][j] == board[i + 4][j + 4]) {
                        return board[i][j];
                    }
                }

                // Sprawdzanie ukośnej linii (w lewo w dół)
                if (i + 4 < n && j - 4 >= 0) {
                    if (board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] &&
                        board[i][j] == board[i + 3][j - 3] && board[i][j] == board[i + 4][j - 4]) {
                        return board[i][j];
                    }
                }
            }
        }
    }
    return '-';
}

int checkDraw(char board[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == '-') {
                return 0;
            }
        }
    }
    return 1;
}

void makeBoard(char board[n][n]) {
    printf("    A B C D E F G H I J K L M N O\n");
    for (int i = 0; i < n; i++) {
        if (i < 9) {
            printf("0%d  ", i + 1);
        } else {
            printf("%d  ", i + 1);
        }

        for (int j = 0; j < n; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}



int moveValidation(char board[n][n], int row, int column){
    if (row < 0 || row >= n || column < 0 || column >= n) {
        return 0; 
    }
    if (board[row][column] == '-') {
        if (turn) {
            board[row][column] = 'B';
            turn = 0;
        } else {
            board[row][column] = 'W';
            turn = 1;
        }
        return 1;
    }
    return 0;
}

int sendMsg(int socket, const char* message) {
    int size = send(socket, message, strlen(message), 0);
    return size;
}

int handleTurn(int playing, int waiting, char board[n][n]) {
    char move[4];
    memset(move, 0, sizeof move);
    int size;
    int row, column;

    size = sendMsg(waiting, "It's your opponent's turn!\n");
    if(size <= 0){
        return 2;
    }

    size = sendMsg(playing, "It's your turn!\n");
    if(size <= 0){
        return 1;
    }

    size = recv(playing, move, sizeof move, 0);
    if (size <= 0) {
        return 1;
    }
    printf("Move: %s\n", move);

    if (size == 4) {
        row = (move[1] - '0') * 10 + (move[2] - '0') - 1;
        column = move[0] - 'A';

        if (moveValidation(board, row, column)) {
            size = sendMsg(playing, "Valid move!\n");
            if (size <= 0) {
                return 1;
            }
            size = recv(playing, ack2, sizeof ack2, 0);
            if (size <= 0) {
                return 1;
            }
        } else {
            size = sendMsg(playing, "Invalid move!\nIt's still your turn!\n");
            if (size <= 0) {
                return 1;
            }
            printf("Invalid move!\n");
            size = recv(playing, ack2, sizeof ack2, 0);
            if (size <= 0) {
                return 1;
            }
            return 0;
        }
    } else {
        size = sendMsg(playing, "Invalid move!\nIt's still your turn!\n");
        if (size <= 0) {
            return 1;
        }
        printf("Invalid move!\n");
        size = recv(playing, ack2, sizeof ack2, 0);
        if (size <= 0) {
            return 1;
        }
        return 0;
    }
    return 0;
}
