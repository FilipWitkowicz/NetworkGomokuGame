#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define n 15

//gcc player.c -o player && ./player 127.0.0.1 3000

char info_buffer[1024]; 
char board[n][n];
char move[4];

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

int main(int argc, char* argv[]) {

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    int player_socket = socket(AF_INET, SOCK_STREAM, 0);

    connect(player_socket, (struct sockaddr *)&server_addr, sizeof server_addr);



    int msg_size = read(player_socket, info_buffer, sizeof info_buffer);
    printf("%s\n", info_buffer);
    bzero(info_buffer, sizeof info_buffer);

    msg_size = read(player_socket, info_buffer, sizeof info_buffer);
    printf("%s\n", info_buffer);

    for(;;){
        bzero(info_buffer, sizeof info_buffer);
        printf("Current board state:\n");
        msg_size = read(player_socket, board, sizeof board);
        if(msg_size == 0){
            printf("Game over!\n");
            close(player_socket);
            break;
        }
        makeBoard(board);
        msg_size = read(player_socket, info_buffer, sizeof info_buffer);
        printf("%s\n", info_buffer);
        if(msg_size == 31){
            printf("Enter your move: \n");
            msg_size = read(0, move, sizeof move);
            printf("%d\n", msg_size);
            write(player_socket, move, sizeof move);
        }
        else{
            printf("Waiting for other player to make a move...\n");
        }

    }
  
}

