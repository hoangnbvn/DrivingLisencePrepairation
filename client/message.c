#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>
#include"message.h"
#include<gtk/gtk.h>

int connectServer(int client_sock,struct sockaddr_in server_addr){
    RequestForm data_recv,data_sent;
    int bytes_sent;
    if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
        printf("\nError!Can not connect to sever! Client exit imediately! ");
        return 0;
    }
    data_sent.type = HELLOSERVER;
    strcpy(data_sent.string1,"hello server");
    bytes_sent = send(client_sock, (void *) &data_sent, sizeof(data_sent), 0);  if (bytes_sent < 0) perror("\nError: ");

    return 1;
}

createRoomResponse * CreateRoomRequest(int client_sock,RequestForm data_sent){
    createRoomResponse * quesList;
    int bytes_sent;
    
    bytes_sent = send(client_sock, (void *) &data_sent, sizeof(data_sent), 0); 
        if (bytes_sent < 0) perror("\nError: ");
    
    bytes_sent = recv(client_sock, (void *) quesList, sizeof(createRoomResponse), 0); 
        if (bytes_sent < 0) perror("\nError: ");
        g_print("RoomID: %d",quesList->RoomID);
    return  quesList;
}

void disconnectServer(int client_sock){
    RequestForm data_recv,data_sent;
    int bytes_sent;
    data_sent.type = BYESERVER;
    strcpy(data_sent.string1,"bye server");
    bytes_sent = send(client_sock, (void *) &data_sent, sizeof(data_sent), 0); 
        if (bytes_sent < 0) perror("\nError: ");
}


