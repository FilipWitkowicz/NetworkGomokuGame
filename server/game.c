#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 

#include <pthread.h>
#include "game.h"


char move[10];
char board[n][n];
int turn = 0;
int row, column;
char winner = '-';

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

void initializeGame() {
    memset(board, '-', sizeof board);
    memset(move, 0, sizeof move);

    printf("Game has been started!\n");

}
