#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define n 15

//gcc player.c -o player && ./player 127.0.0.1 3000

char info_buffer[1024]; 
char board[n][n];
char move[4];
int player_socket;

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("Server has been stopped!\n");
        close(player_socket); // Zamknięcie gniazda
        exit(0);
    }
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


int main(int argc, char* argv[]) {
    memset(board, '-', sizeof board);
    

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    player_socket = socket(AF_INET, SOCK_STREAM, 0);
    signal(SIGINT, handle_signal);
    int opt = 1;
    setsockopt(player_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(connect(player_socket, (struct sockaddr *)&server_addr, sizeof server_addr) != -1){
        printf("Connected to server!\n");
    }
    else{
        printf("Connection failed!\n");
        return 0;
    }



    int msg_size = read(player_socket, info_buffer, sizeof info_buffer);
    printf("%s\n", info_buffer);
    bzero(info_buffer, sizeof info_buffer);

    msg_size = read(player_socket, info_buffer, sizeof info_buffer);
    printf("%s\n", info_buffer);

    for(;;){
        printf("--------------------------------\n");
        bzero(info_buffer, sizeof info_buffer);
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
            bzero(info_buffer, sizeof info_buffer);
            printf("Enter your move: \n");
            msg_size = read(0, move, sizeof move);
            write(player_socket, move, sizeof move);
            msg_size = read(player_socket, info_buffer, sizeof info_buffer);
            printf("%s\n", info_buffer);
        }
        else{
            printf("Waiting for other player to make a move...\n");
        }

    }
  return 0;
}

