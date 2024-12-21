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

//g++ server.c shared.c -o game && ./game

int size;
int server_socket;
char board[n][n];
char winner = '-';
char ack[5];

struct sockaddr_in server_addr, player1_addr, player2_addr;

int main() {

    memset(board, '-', n * n * sizeof(char));
    memset(&server_addr, 0, sizeof server_addr);
    memset(&player1_addr, 0, sizeof player1_addr);
    memset(&player2_addr, 0, sizeof player2_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket!\n");
        return 0;
    }
    int debug = bind(server_socket, (struct sockaddr*)&server_addr, sizeof server_addr);
    if (debug == -1) {
        perror("Error binding socket!\n");
        return 0;
    }
    int debug2 = listen(server_socket, 10);
    if (debug2 == -1) {
        perror("Error listening on socket!\n");
        return 0;
    }

    printf("Server is running!\nWaiting for players...\n");

    socklen_t struct_size = sizeof player1_addr;


    int player1_socket = accept(server_socket, (struct sockaddr*)&player1_addr, &struct_size);
    if (player1_socket == -1) {
        perror("Error accepting player 1!\n");
        close(server_socket);
        return 0;
    }
    printf("Player 1 connected!\n");
    size = sendMsg(player1_socket, "Welcome to Gomoku!\nYou are playing white!\nWaiting for second player!\n");
    if(size <= 0){
        printf("Player 1 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        return 0;
    }


    int player2_socket = accept(server_socket, (struct sockaddr*)&player2_addr, &struct_size);
    if (player2_socket == -1) {
        perror("Error accepting player 2!\n");
        close(server_socket);
        close(player1_socket);
        return 0;
    }
    printf("Player 2 connected!\n");
    size = sendMsg(player2_socket, "Welcome to Gomoku!\nYou are playing black!\n");
    if(size <= 0){
        printf("Player 2 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        close(player2_socket);
        return 0;
    }

    size = recv(player1_socket, ack, sizeof ack, 0);
    if (size <= 0) {
        printf("Player 1 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        close(player2_socket);
        return 0;
    }
    recv(player2_socket, ack, sizeof ack, 0);
    if (size <= 0) {
        printf("Player 2 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        close(player2_socket);
        return 0;
    }


    //usleep(500000);

    size = sendMsg(player1_socket, "Both players has connected!\nTo play a move type board coordinates like 'A02'\n");
    if(size <= 0){
        printf("Player 1 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        close(player2_socket);
        return 0;
    }
    size = sendMsg(player2_socket, "Both players has connected!\nTo play a move type board coordinates like 'A02'\n");
    if(size <= 0){
        printf("Player 2 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        close(player2_socket);
        return 0;
    }

    size = recv(player1_socket, ack, sizeof ack, 0);
    if (size <= 0) {
        printf("Player 1 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        close(player2_socket);
        return 0;
    }
    size = recv(player2_socket, ack, sizeof ack, 0);
    if (size <= 0) {
        printf("Player 2 disconnected before game even started! \n");
        close(server_socket);
        close(player1_socket);
        close(player2_socket);
        return 0;
    }

    printf("Game has been started!\n");

    // usleep(500000);

    // game loop
    for (;;) {
        // usleep(500000);
        if (winner == '-') {
            makeBoard(board);
        }


        
        size = send(player1_socket, board, sizeof board, 0);
        if(size <= 0){
            printf("White disconnected!\n");
            printf("Black won!\n");
            winner = 'B';

        }
        size = send(player2_socket, board, sizeof board, 0);
        if(size <= 0){
            printf("Black disconnected!\n");
            printf("White won!\n");
            winner = 'W';

        }


        
        if(turn){
            printf("Black to move!\n");
            size = handleTurn(player2_socket, player1_socket, board);
            if(size == 1){
                printf("Black disconnected!\n");
                printf("White won!\n");
                winner = 'W';

            }
            else if(size == 2){
                printf("White disconnected!\n");
                printf("Black won!\n");
                winner = 'B';

            }
        }
        else{
            printf("White to move!\n");
            size = handleTurn(player1_socket, player2_socket, board);
            if(size == 1){
                printf("White disconnected!\n");
                printf("Black won!\n");
                winner = 'B';

            }
            else if(size == 2){
                printf("Black disconnected!\n");
                printf("White won!\n");
                winner = 'W';

            }
        }


        winner = checkWinner(board);
        if (winner == 'B') {
            printf("Black won!\n");
            sendMsg(player1_socket, "You lost!\n");
            sendMsg(player2_socket, "You won!\n");
            close(player1_socket);
            close(player2_socket);
            close(server_socket);
            break;
        }
        else if (winner == 'W') {
            printf("White won!\n");
            sendMsg(player1_socket, "You won!\n");
            sendMsg(player2_socket, "You lost!\n");
            close(player1_socket);
            close(player2_socket);
            close(server_socket);
            break;
        }
        else if(checkDraw(board)){
            printf("Draw!\n");
            sendMsg(player1_socket, "Draw!\n");
            sendMsg(player2_socket, "Draw!\n");
            close(player1_socket);
            close(player2_socket);
            close(server_socket);
            break;
        }
        else{
            size = sendMsg(player1_socket, "next round!\n");
            if(size <= 0){
                printf("White disconnected!\n");
                printf("Black won!\n");
                winner = 'B';

            }
            size = sendMsg(player2_socket, "next round!\n");
            if (size <= 0) {
                printf("Black disconnected!\n");
                printf("White won!\n");
                winner = 'W';
            }
            size = recv(player1_socket, ack, sizeof ack, 0);
            if (size <= 0) {
                printf("White disconnected!\n");
                printf("Black won!\n");
                winner = 'B';
            } 
            size = recv(player2_socket, ack, sizeof ack, 0);
            if (size <= 0) {
                printf("Black disconnected!\n");
                printf("White won!\n");
                winner = 'W';
            }
        }
      
    }


    return 0;
}
