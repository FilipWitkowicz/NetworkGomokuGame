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

//g++ player.c shared.c -o player && ./player 127.0.0.1 3000

char info_buffer[1024]; 
char board[n][n];
char move[4];
int size;
int player_socket;
struct sockaddr_in server_addr;



int main(int argc, char* argv[]) {
    memset(&server_addr, 0, sizeof server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    player_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (player_socket == -1) {
        perror("Error creating socket!\n");
        return 0;
    }

    if(connect(player_socket, (struct sockaddr *)&server_addr, sizeof server_addr) != -1){
        printf("Connected to server!\n");
    }
    else{
        printf("Connection failed!\n");
        return 0;
    }

    //welcome message
    size = recv(player_socket, info_buffer, sizeof info_buffer, 0);
    if (size <= 0) {
        printf("Server disconnected before game even started!\n");
        close(player_socket);
        return 0;
    }
    size = send(player_socket, "ack\n", sizeof "ack\n", 0);
    if (size <= 0) {
        printf("Server disconnected before game even started!\n");
        close(player_socket);
        return 0;
    }

    printf("%s\n", info_buffer);
    memset(info_buffer, 0, sizeof info_buffer);

    //both players connected msg
    size = recv(player_socket, info_buffer, sizeof info_buffer, 0);
    if(size <= 0){
        printf("Server disconnected before game even started!\n");
        close(player_socket);
        return 0;
    }
    size = send(player_socket, "ack\n", sizeof "ack\n", 0);
    if (size <= 0) {
        printf("Server disconnected before game even started!\n");
        close(player_socket);
        return 0;
    }

    printf("%s\n", info_buffer);
    memset(info_buffer, 0, sizeof info_buffer);

    for(;;){
        memset(info_buffer, 0, sizeof info_buffer);
        size = recv(player_socket, board, sizeof board, 0);
        if (size <= 0) {
            printf("Opponent disconected, you won!\n");
            close(player_socket);
            return 0;
        }
        


        makeBoard(board);

        size = recv(player_socket, info_buffer, sizeof info_buffer, 0);
        if (size <= 0) {
            printf("Opponent disconected, you won!\n");
            close(player_socket);
            return 0;
        }
        printf("%s\n", info_buffer);
        if(size == 16){
            memset(info_buffer, 0, sizeof info_buffer);
            printf("Enter your move: \n");
            scanf("%s", move);
            size = send(player_socket, move, sizeof move, 0);
            if (size <= 0) {
                printf("Opponent disconected, you won!\n");
                close(player_socket);
                return 0;
            }

            //move validation info
            size = recv(player_socket, info_buffer, sizeof info_buffer, 0);
            if (size <= 0) {
                printf("Opponent disconected, you won!\n");
                close(player_socket);
                return 0;
            }
            printf("%s\n", info_buffer);
            size = send(player_socket, "ack\n", sizeof "ack\n", 0);
            if (size <= 0) {
                printf("Opponent disconected, you won!\n");
                close(player_socket);
                return 0;
            }

        }
        else{
            printf("Waiting for other player to make a move...\n");
        }

        memset(info_buffer, 0, sizeof info_buffer);
        size = recv(player_socket, info_buffer, sizeof info_buffer, 0);
        if (size <= 0) {
            printf("Opponent disconected, you won!\n");
            close(player_socket);
            return 0;
        }
        printf("%s\n", info_buffer);
        if(size < 11){
            break;
        }


        size = send(player_socket, "ack\n", sizeof "ack\n", 0);
        if (size <= 0) {
            printf("Opponent disconected, you won!\n");
            close(player_socket);
            return 0;
        }


    }
  return 0;
}

