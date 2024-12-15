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

//gcc server.c game.c -o game && ./game

int size;

int moveValidation(int row, int column){
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

int main() {


    struct sockaddr_in server_addr, player1_addr, player2_addr;
    memset(&server_addr, 0, sizeof server_addr);
    memset(&player1_addr, 0, sizeof player1_addr);
    memset(&player2_addr, 0, sizeof player2_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3003);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof server_addr);
    listen(server_socket, 10);

    printf("Server is running!\nWaiting for players...\n");

    socklen_t struct_size = sizeof player1_addr;


    int player1_socket = accept(server_socket, (struct sockaddr*)&player1_addr, &struct_size);
    printf("Player 1 connected!\n");
    write(player1_socket, "Welcome to Gomoku!\nYou are playing white!\nWaiting for second player!\n", 69);
    int player2_socket = accept(server_socket, (struct sockaddr*)&player2_addr, &struct_size);
    printf("Player 2 connected!\n");
    write(player2_socket, "Welcome to Gomoku!\nYou are playing black!\n", 43);

    write(player1_socket, "Both players has connected!\nTo play a move type board coordinates like 'A02'\n", 78);
    write(player2_socket, "Both players has connected!\nTo play a move type board coordinates like 'A02'\n", 78);

    initializeGame();


    // game loop
    for (;;) {
        memset(move, 0, sizeof move);
        makeBoard(board);
        size = write(player1_socket, board, sizeof board);
        printf("%d\n", size);
        size = write(player2_socket, board, sizeof board);
        printf("%d\n", size);


        // checking if game is over
        if (winner == 'B') {
            printf("Black won!\n");
            close(player1_socket);
            close(player2_socket);
            close(server_socket);
            break;
        } else if (winner == 'W') {
            close(player1_socket);
            close(player2_socket);
            close(server_socket);
            printf("White won!\n");
            break;
        }

        // make a move
        if (turn) {
            printf("Black to move!\n");
            write(player2_socket, "Black to move!\nIt's your turn!\n", 31);
            write(player1_socket, "Black to move!\nIt's your opponent's turn!\n", 41);

            size = read(player2_socket, move, sizeof move);
            printf("Move: %s\n", move);
            if(size == 4){
                row = (move[1] - '0') * 10 + (move[2] - '0') - 1;
                column = move[0] - 'A';
                if(moveValidation(row, column)){
                    write(player2_socket, "Valid move!\n", 12);
                }
                else{
                    write(player2_socket, "Invalid move!\nIt's still your turn!\n", 35);
                    printf("Invalid move!\n");
                    continue;
                }
            }
            else{
                write(player2_socket, "Invalid move!\nIt's still your turn!\n", 35);
                printf("Invalid move!\n");
                continue;
            }
     
        } else {
            printf("White to move!\n");
            write(player1_socket, "White to move!\nIt's your turn!\n", 31);
            write(player2_socket, "White to move!\nIt's your opponent's turn!\n", 41);

            size = read(player1_socket, move, sizeof move);
            printf("Move: %s\n", move);
            if(size == 4){
                row = (move[1] - '0') * 10 + (move[2] - '0') - 1;
                column = move[0] - 'A';
                if(moveValidation(row, column)){
                    write(player1_socket, "Valid move!\n", 12);
                }
                else{
                    write(player1_socket, "Invalid move!\nIt's still your turn!\n", 35);
                    printf("Invalid move!\n");
                    continue;
                }
            }
            else{
                write(player1_socket, "Invalid move!\nIt's still your turn!\n", 35);
                printf("Invalid move!\n");
                continue;
            }
        }


        winner = checkWinner(board);
    }

    return 0;
}
