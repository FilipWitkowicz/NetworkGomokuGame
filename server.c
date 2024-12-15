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

//gcc server.c game.c -o main && ./main

int main() {
    initializeGame();

    for (;;) {
        memset(move, 0, sizeof move);
        makeBoard(board);

        if (winner == 'B') {
            printf("Black won!\n");
            break;
        } else if (winner == 'W') {
            printf("White won!\n");
            break;
        }

        // tu beda sockety
        if (turn) {
            printf("Black to move!\n");
        } else {
            printf("White to move!\n");
        }

        int size = read(0, move, sizeof move);
        if (size != 4) {
            printf("Invalid move!\n");
            continue;
        }
        
        row = (move[1] - '0') * 10 + (move[2] - '0') - 1;
        column = move[0] - 'A';

        if (board[row][column] == '-') {
            if (turn) {
                board[row][column] = 'B';
                turn = 0;
            } else {
                board[row][column] = 'W';
                turn = 1;
            }
        } else {
            printf("Invalid move!\n");
            continue;
        }

        winner = checkWinner(board);
        printf("Winner: %c\n", winner);
    }

    return 0;
}
