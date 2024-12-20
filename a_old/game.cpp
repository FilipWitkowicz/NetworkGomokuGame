#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 

#include <pthread.h>
using namespace std;
//gcc -Wall game.cpp -o game && ./game

#define n 15

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

void makeBoard(char board[n][n]){
    printf("    A B C D E F G H I J K L M N O\n");
        for(int i=0; i<n; i++){
            if(i < 9){
                printf("0%d  ", i+1);
            }
            else{
                printf("%d  ", i+1);
            }

            for(int j=0; j<n; j++){
                printf("%c ", board[i][j]);
            }
            printf("\n");
        }
}
void initializeGame(){
    memset(board, '-', sizeof board);
    memset(move, 0, sizeof move);

    printf("Welcome to Gomoku!\nTo place a move type board coordinates like 'A02'\nTo Start a game type 'RDY'\n");
    for(;;){
        read(0, move, sizeof move);
        if(strcmp(move, "RDY\n") == 0){
            break;
        }
    }
}

int main() {

    initializeGame();

    for(;;){
        memset(move, 0, sizeof move);
        
        makeBoard(board);
        if(winner == 'B'){
            printf("Black won!\n");
            break;
        }
        else if(winner == 'W'){
            printf("White won!\n");
            break;
        }
        
        // tu beda sockety
        if(turn){
            printf("Black to move!\n");
        }
        else{
            printf("White to move!\n");
        }

        int size = read(0, move, sizeof move);
        if(size != 4){
            printf("Invalid move!\n");
            continue;
        }
        row = (move[1] - '0')*10 + (move[2] - '0') -1;
        column = move[0] - 'A';

        if (board[row][column] == '-'){
            if(turn){
                board[row][column] = 'B';
                turn = 0;
            }
            else{
                board[row][column] = 'W';
                turn = 1;
            }
        }
        else{
            printf("Invalid move!\n");
            continue;
        }
        
        winner = checkWinner(board);
        printf("Winner: %c\n", winner);
        
        
    }

    return 0;
}

