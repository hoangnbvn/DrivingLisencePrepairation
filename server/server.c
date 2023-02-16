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
#include <pthread.h>

#include<string.h>
#include<ctype.h>
#include"fields.h"

#define BACKLOG 20   /* Number of allowed connections */
#define PORT 5500
#define BUFF_SIZE 1024
// Biến Global
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
// Biến Global
// Message
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
// Message

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
roomResultsResponse response;



// Online User
typedef struct User
{
	int sockfd;
	int uid;
	char name[32];
}User;

User onlUserList[20];
int numberOnlUser = 0;

int addUser(char * name,int sockfd){
    int i=0;
    if(numberOnlUser<20){
        for( i=0;i<20;i++){
            if(onlUserList[i].uid<0){
                strcpy(onlUserList[numberOnlUser].name,name);
                onlUserList[numberOnlUser].sockfd = sockfd;
                onlUserList[numberOnlUser].uid = numberOnlUser;
                break;
            }
        }
    numberOnlUser++;
    return i;
    }
    else return -1;
}
void deleteUser(int index){
    strcpy(onlUserList[index].name,"");
    onlUserList[index].sockfd = -1;
    onlUserList[index].uid = -1;
    numberOnlUser--;
}
//Online User

//Room
typedef struct ROOM
{
    int roomID;
    char name[100];
    int numberQuestion;
    int time;
    char filename[100];
    int state; // 1: sẵn sàng | 2: đang thi | 3: đã kết thúc
    int level; // Cap Do: 1,2,3
    int userList[20];
    int numberUser;
    int QuestionList[100]; 
}ROOM;
ROOM roomList[20];
int numberRoom=0;
void addRoom(char *roomname,int time,int numQes,char*filename,int state){
    roomList[numberRoom].roomID= numberRoom+1;
    strcpy(roomList[numberRoom].name,roomname);
    strcpy(roomList[numberRoom].filename,filename);
    roomList[numberRoom].time = time;
    roomList[numberRoom].numberQuestion = numQes;
    roomList[numberRoom].state = state;
    roomList[numberRoom].numberUser = 1;
    numberRoom++;
}
void createRoom(char *roomname,int time,int numQes,int level,int owner,int random){
    char filename[100];
    int r;
    sprintf(filename,"results/%s_%d.txt",roomname,numberRoom);
    addRoom(roomname,time,numQes,filename,1);
    roomList[numberRoom-1].level = level;
    roomList[numberRoom-1].numberUser = 1;
    roomList[numberRoom-1].userList[0]=owner;
    for(int i=0;i<numQes;i++){
        r = rand();
        roomList[numberRoom-1].QuestionList[i]=(r*random*(numberRoom-1))%100;
    }
}
void readRoomfile(){
    IS is;
    is=new_inputstruct("room.txt");
    if(is->f!=NULL){
        while(1){
            get_line(is);
            if(is->NF<0) break;
            else if(is->NF == 6){
            	int roomID =atoi(is->fields[0]);
	        char * roomName = is->fields[1];
	        int question = atoi(is->fields[2]);
	        int time = atoi(is->fields[3]);
	        int state  = atoi(is->fields[4]);
	        char* filename  = is->fields[5]; 
	        addRoom(roomName,time,question,filename,state);
            }
            
        }
        jettison_inputstruct(is);
    }
}

//Room

void *handleClient(void *);
void removeFrUserList(int,int);
int main()
{ 
	int listenfd, *connfd;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in *client; /* client's address information */
	int sin_size;
	pthread_t tid;
	FILE *fp;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT); 
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   

	if(bind(listenfd,(struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listenfd, BACKLOG) == -1){  
		perror("\nError: ");
		return 0;
	}
	
	sin_size=sizeof(struct sockaddr_in);
	client = malloc(sin_size);
    for(int i=0;i<20;i++){
        onlUserList[i].uid = -1;
        strcpy(onlUserList[i].name,"");
        onlUserList[i].sockfd = -1;
    }
    readRoomfile();
	printf("Server Start \n");
	while(1){		
		connfd = malloc(sizeof(int));
		if ((*connfd = accept(listenfd, (struct sockaddr *)client, &sin_size)) ==- 1)
			perror("\nError: ");
				
		printf("You got a connection from [%s]-[%d]\n", inet_ntoa(client->sin_addr),ntohs(client->sin_port) ); /* prints client's IP */
		/* For each client, spawns a thread, and the thread handles the new client */
		pthread_create(&tid, NULL, &handleClient, connfd);	
	}
	close(listenfd);
	return 0;
}

void *handleClient(void *arg){
	int connfd;
    int bytes_sent, bytes_received;
    RequestForm data_recv,data_sent;
	char buff[BUFF_SIZE + 1];
    int index = -1;
    int myroom = -1;
	connfd = *((int *) arg);
	free(arg);
    printf("Thread started\n");
    while (1)
    {
        bytes_received = recv(connfd, (void *) &data_recv, sizeof(data_recv), 0);
            if (bytes_received <= 0) {
            	break;
            }
            
        if(data_recv.type == HELLOSERVER && index==-1){
            printf("[HELLOSERVER]\n");
            printf("Socket number: %d | String: %s\n",connfd, data_recv.string1);
            index = addUser(data_recv.string1,connfd);
            printf("Index: %d\n",index);
            
        }
        else if(data_recv.type == CREATEROOM && myroom ==-1){
            printf("[CREATEROOM]\n");
            createRoom(data_recv.string1,data_recv.number2,data_recv.number1,data_recv.number3,index,connfd);
            myroom = numberRoom-1;
            strcpy(onlUserList[index].name,data_recv.string2);
            printf("nameRoom: %s number Questions: %d time: %d level: %d\n",roomList[myroom].name,roomList[myroom].numberQuestion,roomList[myroom].time,roomList[myroom].level);
            createRoomResponse queslistmeg;
            queslistmeg.type = CREATEROOM;
            queslistmeg.numberQues = roomList[myroom].numberQuestion;
            for(int i=0;i<queslistmeg.numberQues;i++){
                queslistmeg.question[i]=roomList[myroom].QuestionList[i];
            }
            queslistmeg.time = roomList[myroom].time;
            queslistmeg.level = roomList[myroom].level;
            queslistmeg.RoomID = myroom;
            bytes_sent = send(connfd, (createRoomResponse*) &queslistmeg, sizeof(createRoomResponse), 0); // gửi dữ liệu
	            if (bytes_sent < 0) perror("\nError: ");
        }
        else if(data_recv.type ==SHOWROOMLIST){
            if(data_recv.number1 ==1){
                int count = 0;
                printf("[SHOWROOMLIST] state =1\n");
                showRoomListResponse roomlistmeg;
                for(int i=0;i<numberRoom;i++){
                    if(roomList[i].state==1){
                        strcpy(roomlistmeg.roomname[count],roomList[i].name);
                        roomlistmeg.roomID[count] = roomList[i].roomID;
                        roomlistmeg.level[count] = roomList[i].level;
                        count++;
                    }
                }
                roomlistmeg.numberroom =count;
                bytes_sent = send(connfd, (showRoomListResponse*) &roomlistmeg, sizeof(showRoomListResponse), 0); // gửi dữ liệu
	                if (bytes_sent < 0) perror("\nError: ");
                data_recv.number1 =0;
            }
            if(data_recv.number1 ==2){
                int count = 0;
                showRoomListResponse roomlistmeg;
                printf("[SHOWROOMLIST] state =2\n");
                for(int i=0;i<numberRoom;i++){
                    if(roomList[i].state==2){
                        strcpy(roomlistmeg.roomname[count],roomList[i].name);
                        roomlistmeg.roomID[count] = roomList[i].roomID;
                        roomlistmeg.level[count] = 0;
                        count++;
                    }
                }
                roomlistmeg.numberroom =count;
                bytes_sent = send(connfd, (showRoomListResponse*) &roomlistmeg, sizeof(showRoomListResponse), 0); // gửi dữ liệu
	                if (bytes_sent < 0) perror("\nError: ");
                data_recv.number1 =0;
            }
            if(data_recv.number1 ==3){
                int count = 0;
                showRoomListResponse roomlistmeg;
                printf("[SHOWROOMLIST] state =3\n");

                for(int i=0;i<numberRoom;i++){
                    if(roomList[i].state==3){
                        strcpy(roomlistmeg.roomname[count],roomList[i].name);
                        roomlistmeg.roomID[count] = roomList[i].roomID;
                        roomlistmeg.level[count] = 0;
                        count++;
                    }
                }
                roomlistmeg.numberroom =count;
                bytes_sent = send(connfd, (showRoomListResponse*) &roomlistmeg, sizeof(showRoomListResponse), 0); // gửi dữ liệu
	                if (bytes_sent < 0) perror("\nError: ");
                data_recv.number1 =0;
            }
        }
        else if(data_recv.type == JOINROOM){
            int joinroomID = data_recv.number1-1;
            int numberUser = roomList[joinroomID].numberUser;
            char userName[50];
            char tmp[100];
            roomList[joinroomID].userList[numberUser] = index;
            strcpy(onlUserList[index].name,data_recv.string1);
            roomList[joinroomID].numberUser++;
            numberUser++;
            printf("[JOINROOM]\n");
            printf("User %s join %s\n",data_recv.string1,roomList[joinroomID].name);
            
            joinRoomReponse joinroomreponse;
            strcpy(joinroomreponse.roomName,roomList[joinroomID].name);
            joinroomreponse.numberUser = numberUser;
            joinroomreponse.time = roomList[joinroomID].time;
            joinroomreponse.level = roomList[joinroomID].level;
            printf("%d",roomList[joinroomID].time);
            joinroomreponse.numberQuestion = roomList[joinroomID].numberQuestion;
            joinroomreponse.type = JOINROOM;
            printf("number User: %d\n",joinroomreponse.numberUser);
            
            
            for(int i=0;i<numberUser;i++){
                int k = roomList[joinroomID].userList[i];
                int socket = onlUserList[k].sockfd;
                bytes_sent = send(socket, (void*) &joinroomreponse, sizeof(joinRoomReponse), 0); 
	                if (bytes_sent < 0) perror("\nError: ");
	        
            }
        }
        else if(data_recv.type == STARTEXAM){
            int startroomID =data_recv.number1;
            int numberUserInRoom = roomList[startroomID].numberUser;
            if(roomList[startroomID].state == 1){
                joinRoomReponse joinroomreponse;
                
                
                strcpy(joinroomreponse.roomName,roomList[startroomID].name);
                joinroomreponse.numberUser = numberUserInRoom;
                joinroomreponse.time = roomList[startroomID].time;
            	joinroomreponse.numberQuestion = roomList[startroomID].numberQuestion;
                
                joinroomreponse.type = STARTEXAM;
                printf("number User: %d\n",joinroomreponse.numberUser);
                
                for(int i=0;i<numberUserInRoom;i++){// gửi tín hiệu bắt đầu thi đến tất cả User
                    int k = roomList[startroomID].userList[i];
                    int socket = onlUserList[k].sockfd;
                    bytes_sent = send(socket, (void*) &joinroomreponse, sizeof(joinRoomReponse), 0); 
                        if (bytes_sent < 0) perror("\nError: ");
                    printf("sent\n");
                }
                roomList[startroomID].state = 2; // Phòng Đang Thi 
                printf("[STARTEXAM] RoomID: %d roomName:%s \n",startroomID,roomList[startroomID].name);
            }
            else if(roomList[startroomID].state != 1){
                createRoomResponse startexamresponse;
                startexamresponse.RoomID =startroomID;
                startexamresponse.numberQues = roomList[startroomID].numberQuestion;
                startexamresponse.time = roomList[startroomID].time;
                startexamresponse.type = STARTEXAM;
                for(int i=0;i<startexamresponse.numberQues;i++) startexamresponse.question[i] = roomList[startroomID].QuestionList[i];// gửi gói câu hỏi đến User
                bytes_sent = send(connfd, (void*) &startexamresponse, sizeof(createRoomResponse), 0); 
                    if (bytes_sent < 0) perror("\nError: ");
            }  
        }
        else if(data_recv.type ==LEFTROOM){
            int roomID = data_recv.number1;
            int numberUserInRoom = roomList[roomID].numberUser;
            int k =0;
            int i=0;
            joinRoomReponse joinroomreponse,leftroomresponse;
            char UserName[50]; strcpy(UserName,data_recv.string1);
            for(i=0;i<numberUserInRoom;i++){
                k = roomList[roomID].userList[i];
                if(strcmp(UserName,onlUserList[k].name)==0){
                    removeFrUserList(roomID,i);
                    numberUserInRoom--;
                    break;
                }
            }
            printf("[LEFTROOM]-User %s left %s\n",UserName,roomList[roomID].name);
            leftroomresponse.type =LEFTROOM;
            bytes_sent = send(connfd, (void*) &leftroomresponse, sizeof(joinRoomReponse), 0); 
	                if (bytes_sent < 0) perror("\nError: ");


            strcpy(joinroomreponse.roomName,roomList[roomID].name);
            joinroomreponse.numberUser = numberUserInRoom;
            joinroomreponse.type = JOINROOM;
            joinroomreponse.time = roomList[roomID].time;
            joinroomreponse.numberQuestion =roomList[roomID].numberQuestion;

            printf("number User: %d\n",joinroomreponse.numberUser);
            
            for(int i=0;i<numberUserInRoom;i++){
                int k = roomList[roomID].userList[i];
                int socket = onlUserList[k].sockfd;
                bytes_sent = send(socket, (void*) &joinroomreponse, sizeof(joinRoomReponse), 0); 
	                if (bytes_sent < 0) perror("\nError: ");
            }
        }
        else if(data_recv.type == CANCELROOM&& myroom !=-1){
            int cancelRoomID = data_recv.number1;
            int numberUserInRoom = roomList[cancelRoomID].numberUser;
            roomList[cancelRoomID].state = 0; // vô hiệu hóa room bị hủy
            joinRoomReponse cancelroomresponse;
            cancelroomresponse.type = CANCELROOM;
            printf("[CANCELROOM]-%s room is cancel\n",roomList[cancelRoomID].name);
            for(int i=0;i<numberUserInRoom;i++){
                int k = roomList[cancelRoomID].userList[i];
                int socket = onlUserList[k].sockfd;
                bytes_sent = send(socket, (void*) &cancelroomresponse, sizeof(joinRoomReponse), 0); 
	                if (bytes_sent < 0) perror("\nError: ");
            }
        }
        else if(data_recv.type==BYESERVER && index !=-1){
            deleteUser(index);
            break;
        }
        else if(data_recv.type == FINISHEXAM){
            int finishedRoom = data_recv.number3;
            
            sprintf(buff,"%s %d %d\n",data_recv.string1,data_recv.number1,data_recv.number2);
            printf("[FINISHEXAM] %s\n",buff);
            FILE*fp;
            if(roomList[finishedRoom].state == 2){
                fp = fopen(roomList[finishedRoom].filename,"w");
                fputs(buff,fp);
                fclose(fp);
                roomList[finishedRoom].state = 3;
                fp = fopen("room.txt","a");
                sprintf(buff,"%d %s %d %d %d %s\n",finishedRoom,roomList[finishedRoom].name,roomList[finishedRoom].numberQuestion,roomList[finishedRoom].time,roomList[finishedRoom].state, roomList[finishedRoom].filename);
                fputs(buff,fp);
                fclose(fp);
            }
            else{
                fp = fopen(roomList[finishedRoom].filename,"a");
                fputs(buff,fp);
                fclose(fp);
            }
        }
        else if(data_recv.type == SHOWRESULTSOFROOM){
            int showroomID = atoi(data_recv.string1)-1;
            printf("[SHOWRESULTSOFROOM] %s\n",roomList[showroomID].name);
            char filename[50]; 
            
            strcpy(filename,roomList[showroomID].filename);
            printf("%s\n",filename);
            int count = 0;
            IS is;
            is=new_inputstruct(filename);
            if(is->f!=NULL){
                while(1){
                    get_line(is);
                    if(is->NF<1) break;
                    printf("%s %s %s\n",is->fields[0],is->fields[1],is->fields[2]);
                    strcpy(response.usename[count],is->fields[0]);
                    response.score[count] = atoi(is->fields[1]);
                    response.time[count] = atoi(is->fields[2]);
                    count ++;
                }
                response.numberUser =count;
            }
            jettison_inputstruct(is);
            response.type = SHOWRESULTSOFROOM;
            bytes_sent = send(connfd, (void*) &response, sizeof(roomResultsResponse), 0); 
	                if (bytes_sent < 0) perror("\nError: ");
        }
        
    }
	close(connfd);	
    printf("Thread end\n");
	return 0;
}

void removeFrUserList(int roomID, int index){
    int numberUserInRoom = roomList[roomID].numberUser;
    for(int i = index;i<numberUserInRoom-1;i++) roomList[roomID].userList[i] = roomList[roomID].userList[i+1];
    roomList[roomID].numberUser--;
}



