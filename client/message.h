#ifndef __MESSAGE_H__
#define __MESSAGE_H__
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
typedef enum {
	HELLOSERVER,
	JOINROOM,
	CREATEROOM,
	STARTEXAM,
	FINISHEXAM,
	CANCELROOM,
    LEFTROOM,
	SHOWRESULTSOFROOM,
	SHOWROOMLIST,
    BYESERVER
}messageType;

typedef struct RequestForm
{
	messageType type;
	char string1[200];
    char string2[200];
    int number1;
    int number2;
    int number3;
}RequestForm;

typedef struct createRoomResponse
{
	messageType type;
    int RoomID;
	int question[100];
    int numberQues;
    int time;
    int level;
}createRoomResponse;

typedef struct showRoomListResponse
{
	messageType type;
	char roomname[20][50];
    int roomID[20];
    int numberroom;
    int level[20];
}showRoomListResponse;

typedef struct joinRoomReponse
{
    messageType type;
    char roomName[50];
    int numberUser;
    int numberQuestion;
    int time;
    int level;
}joinRoomReponse;

typedef struct roomResultsResponse
{
	messageType type;
    char usename[50][50];
    int score[50];
    int time[50];
    int numberUser;
}roomResultsResponse;

typedef struct question
    {
    	char q[500];
	char answer1[200];
    	char answer2[200];
	char answer3[200];
	char answer4[200];
	int correctAnswer;
	char imagelink[50];
	char hint[300];
    }question;
    
    typedef struct Exam
    {
    	question question[100];
    	int numberQuestion;
    }Exam;

int connectServer(int client_sock,struct sockaddr_in server_addr);
createRoomResponse * CreateRoomRequest(int client_sock,RequestForm data_sent);
void disconnectServer(int client_sock);

#endif
