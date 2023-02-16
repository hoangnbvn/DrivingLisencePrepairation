#include<gtk/gtk.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include "message.h"
#include "vaothi.h"
#include "lambai.h"

GtkWidget * layout5;
GtkWidget * w5;
int clientsock_3;
char RoomName[50],UserName[50];
int numberQuestion1,maxtime1;
int level1;
int roomID,roomStart;
createRoomResponse * quesList;
int clocktime = 0;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t megRecvThread1;

void draw_Wait_Room_View_1(GtkWidget * window, char *numberUserString);
void CreateRoom();
void * revMsgToServer1(void *data);
void CancelRoom();
void BackToVaoThi2();
void startRoomFunClick();

GtkWidget *roomNameEntry;
GtkWidget *nameEntry;
GtkWidget *numberQuestionEntry;
GtkWidget *timeEntry;
GtkWidget *levelEntry;
void drawSettingRoomLayout(GtkWidget * window) {
    GtkWidget *background;
    GtkWidget *CreateRoomButton;
    GtkWidget *CancelButton;
    
    layout5 = gtk_layout_new(NULL, NULL);
    
    
    background = gtk_image_new_from_file("Pictures/Backgrounds/background4.png");
    gtk_layout_put(GTK_LAYOUT(layout5), background, 0, 0);
    
    roomNameEntry = gtk_entry_new();
	gtk_layout_put(GTK_LAYOUT(layout5), roomNameEntry, 325,111);
	gtk_widget_set_size_request(roomNameEntry,220,50);
	gtk_entry_set_placeholder_text(GTK_ENTRY(roomNameEntry),"T\u00ean Ph\u00f2ng");
    nameEntry = gtk_entry_new();
	gtk_layout_put(GTK_LAYOUT(layout5), nameEntry, 325,242);
	gtk_widget_set_size_request(nameEntry,220,50);
	gtk_entry_set_placeholder_text(GTK_ENTRY(nameEntry),"T\u00ean Ch\u1ee7 Ph\u00f2ng");

    numberQuestionEntry = gtk_entry_new();
	gtk_layout_put(GTK_LAYOUT(layout5), numberQuestionEntry, 565,111);
	gtk_widget_set_size_request(numberQuestionEntry,220,50);
	gtk_entry_set_placeholder_text(GTK_ENTRY(numberQuestionEntry),"S\u1ed1 C\u00e2u H\u1ecfi");

    timeEntry = gtk_entry_new();
	gtk_layout_put(GTK_LAYOUT(layout5), timeEntry, 565,242);
	gtk_widget_set_size_request(timeEntry,220,50);
	gtk_entry_set_placeholder_text(GTK_ENTRY(timeEntry),"Th\u1eddi Gian Thi");
     
     levelEntry = gtk_entry_new();
	gtk_layout_put(GTK_LAYOUT(layout5), levelEntry, 465,370);
	gtk_widget_set_size_request(levelEntry,320,50);
	gtk_entry_set_placeholder_text(GTK_ENTRY(levelEntry),"Độ Khó");	
	

    CreateRoomButton = gtk_button_new_with_label("T\u1ea1o");
    	gtk_layout_put(GTK_LAYOUT(layout5), CreateRoomButton, 625, 525);
    	gtk_widget_set_size_request(CreateRoomButton, 200, 60);
    	gtk_style_context_add_class(gtk_widget_get_style_context(CreateRoomButton),"createroomButton");
    	
    CancelButton = gtk_button_new_with_label("Hủy");
    	gtk_layout_put(GTK_LAYOUT(layout5), CancelButton, 315, 525);
    	gtk_widget_set_size_request(CancelButton, 200, 60);
    	gtk_style_context_add_class(gtk_widget_get_style_context(CancelButton),"cancleroomButton");
    
    gtk_container_add(GTK_CONTAINER (window), layout5);
    gtk_widget_show(layout5);
    g_signal_connect(G_OBJECT(CreateRoomButton),"clicked",G_CALLBACK(CreateRoom),NULL);
    g_signal_connect(G_OBJECT(CancelButton),"clicked",G_CALLBACK(BackToVaoThi2),NULL);
    gtk_widget_show_all(window);
}

void SettingRoom(GtkWidget * window,int socket){
	clientsock_3 = socket;
	w5 = window;
	drawSettingRoomLayout(window);
}
void BackToVaoThi2(){
	gtk_container_remove(GTK_CONTAINER (w5), layout5);
	drawLuyenTap(w5,clientsock_3);
}
GtkWidget *errorFrame;
static gboolean delete_error()
{
    clocktime++;
    if(clocktime <2) return TRUE;
    else{
    	gtk_widget_destroy(errorFrame);
    	gtk_widget_show(layout5);
    	gtk_widget_show_all(w5);
    	return FALSE;
    } 
}
void CreateRoom(){
    const gchar* string;
    int error=0;
    char *buff;
    buff = (char*)calloc(50,sizeof(char));
    
    string = gtk_entry_get_text(GTK_ENTRY(roomNameEntry));
    sprintf(RoomName,"%s",string);
    if(!(strlen(RoomName)>0&&strlen(RoomName)<=50)) error++;
      
    string = gtk_entry_get_text(GTK_ENTRY(nameEntry));
    sprintf(UserName,"%s",string);
    if(!(strlen(UserName)>0&&strlen(UserName)<=50)) error++;
    
    string = gtk_entry_get_text(GTK_ENTRY(numberQuestionEntry));
    sprintf(buff,"%s",string);
    numberQuestion1 = atoi(buff);
    if(!(numberQuestion1>=20&&numberQuestion1<=30)) error++;
    
    string = gtk_entry_get_text(GTK_ENTRY(timeEntry));
    sprintf(buff,"%s",string);
    maxtime1 = 60*atoi(buff);
    if(!(maxtime1>=1200&&maxtime1<=1800)) error++;
    
    string = gtk_entry_get_text(GTK_ENTRY(levelEntry));
    sprintf(buff,"%s",string);
    level1 = atoi(buff);
    if(!(level1==1||level1==2||level1==3)) error++;
    
    
    if(error>0){
    	errorFrame = gtk_button_new_with_label("Tạo Phòng Không Thành Công");
    		gtk_layout_put(GTK_LAYOUT(layout5), errorFrame, 200, 10);
    		gtk_widget_set_size_request(errorFrame, 700, 10);
    		gtk_style_context_add_class(gtk_widget_get_style_context(errorFrame),"ErrorFrame");
    	gtk_widget_show(layout5);
    	gtk_widget_show_all(w5);
    	clocktime = 0;
    	g_timeout_add_seconds(1, delete_error, NULL);
    }
    else{
    	
    	RequestForm data_sent;
    	int bytes_sent;
    	data_sent.type=CREATEROOM;
    	strcpy(data_sent.string1,RoomName);
    	strcpy(data_sent.string2,UserName);
    	data_sent.number1 = numberQuestion1;
    	data_sent.number2 = maxtime1;
    	data_sent.number3 = level1;
    	quesList = (createRoomResponse*)calloc(1,sizeof(createRoomResponse));
    	bytes_sent = send(clientsock_3, (void *) &data_sent, sizeof(data_sent), 0); 
        	if (bytes_sent < 0) perror("\nError: ");
    
    	bytes_sent = recv(clientsock_3, (void *) quesList, sizeof(createRoomResponse), 0); 
        	if (bytes_sent < 0) perror("\nError: ");
        roomStart = 1;
        roomID = quesList->RoomID;
        g_print("RoomID: %d",roomID);
        gtk_container_remove(GTK_CONTAINER (w5), layout5);
        draw_Wait_Room_View_1(w5,"S\u1ed1 Ng\u01b0\u1eddi Tham Gia: 1/20");
        pthread_create(&megRecvThread1,NULL,&revMsgToServer1,NULL);
    }
    free(buff);
}

GtkWidget *numberUserFrame;
GtkWidget *StartExambtn;
GtkWidget *roomnameFrame;
void draw_Wait_Room_View_1(GtkWidget * window, char *numberUserString){ // Room Master View
    GtkWidget *background;
    
    GtkWidget *detailFrame;
    GtkWidget *CancelRoombtn;
    GtkWidget *Label1;
    char * string;
    string  = (char*)calloc(100,sizeof(char));
    layout5 = gtk_layout_new(NULL, NULL);
    
    background = gtk_image_new_from_file("Pictures/Backgrounds/background3.png");
    gtk_layout_put(GTK_LAYOUT(layout5), background, 0, 0);
    
    sprintf(string,"Ph\u00f2ng Thi: %s",RoomName);
    //sprintf(buff,"Ph\u00f2ng Thi: TOT NGHIEP LAI XE");
    
    roomnameFrame = gtk_button_new_with_label(string);
    gtk_layout_put(GTK_LAYOUT(layout5), roomnameFrame, 20, 30);
    gtk_widget_set_size_request(roomnameFrame, 740, 100);
    gtk_style_context_add_class(gtk_widget_get_style_context(roomnameFrame),"roomNameFrame");
    
    
    numberUserFrame = gtk_button_new_with_label(numberUserString);
    gtk_layout_put(GTK_LAYOUT(layout5), numberUserFrame, 20, 150);
    gtk_widget_set_size_request(numberUserFrame, 740, 450);
    gtk_style_context_add_class(gtk_widget_get_style_context(numberUserFrame),"numberUserFrame");
    
    Label1 = gtk_button_new_with_label("Chờ Phòng Thi Bắt Đầu");
    gtk_layout_put(GTK_LAYOUT(layout5), Label1, 20, 150);
    gtk_widget_set_size_request(Label1, 740, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context( Label1),"Label1");

    CancelRoombtn = gtk_button_new_with_label("H\u1ee7y Ph\u00f2ng");
    gtk_layout_put(GTK_LAYOUT(layout5), CancelRoombtn, 780, 150);
    gtk_widget_set_size_request(CancelRoombtn, 300, 50);
    gtk_style_context_add_class(gtk_widget_get_style_context(CancelRoombtn),"CancelRoombtn");
                
    StartExambtn= gtk_button_new_with_label("B\u1eaft \u0110\u1ea7u Thi");
    gtk_layout_put(GTK_LAYOUT(layout5), StartExambtn, 780, 210);
    gtk_widget_set_size_request(StartExambtn, 300, 50);
    if(roomStart == 1) gtk_style_context_add_class(gtk_widget_get_style_context(StartExambtn),"StartExambtn");
    if (roomStart == 0)gtk_style_context_add_class(gtk_widget_get_style_context(StartExambtn),"NoStartExambtn");
    
    //numberQuestion = 20;
    //maxtime =1200;
    sprintf(string,"S\u1ed1 C\u00e2u H\u1ecfi: %d C\u00e2u \n\nTh\u1eddi Gian Thi: %d Ph\u00fat\n\n Mức Độ Khó: %d",numberQuestion1,maxtime1/60,level1);
    detailFrame = gtk_button_new_with_label(string);
    gtk_layout_put(GTK_LAYOUT(layout5), detailFrame, 780, 270);
    gtk_widget_set_size_request(detailFrame, 300, 330);
    gtk_style_context_add_class(gtk_widget_get_style_context(detailFrame),"detailFrame");
    
    
    gtk_container_add(GTK_CONTAINER (window), layout5);
    gtk_widget_show(layout5);
    g_signal_connect(G_OBJECT(StartExambtn),"clicked",G_CALLBACK(startRoomFunClick),NULL);
    g_signal_connect(G_OBJECT(CancelRoombtn),"clicked",G_CALLBACK(CancelRoom),NULL);
    gtk_widget_show_all(window);
    free(string);
}

void * revMsgToServer1(void *data) // Chủ phòng nhận phản hồi tham gia
{
	pthread_detach(pthread_self());
	int bytes_received;
	char strNumberUser[50];
	joinRoomReponse *joinroomreponse1;
	

	while(1){
		joinroomreponse1 = (joinRoomReponse *)calloc(1,sizeof(joinRoomReponse));
		bytes_received = recv(clientsock_3,(void* ) joinroomreponse1, sizeof(joinRoomReponse), 0); 
			if (bytes_received < 0) perror("\nError: ");

			
		if(joinroomreponse1->type == JOINROOM){
			sprintf(strNumberUser,"S\u1ed1 Ng\u01b0\u1eddi Tham Gia: %d/20",joinroomreponse1->numberUser);
			gtk_button_set_label(GTK_BUTTON(numberUserFrame),strNumberUser);
		}
		else if(joinroomreponse1->type == STARTEXAM||joinroomreponse1->type == CANCELROOM){
		    break;
		}
		free(joinroomreponse1);
	}
	return NULL;
}

void CancelRoom(){
	gtk_container_remove(GTK_CONTAINER (w5), layout5);
	drawLuyenTap(w5,clientsock_3);
	sleep(1);
	RequestForm data_sent;
	int bytes_sent;
	data_sent.type =CANCELROOM;
	data_sent.number1 = roomID;
	bytes_sent = send(clientsock_3, (void *) &data_sent, sizeof(data_sent), 0); 
	    if (bytes_sent < 0) perror("\nError: ");
}

void startRoomFunClick(){
	RequestForm data_sent;
	int bytes_sent;
	data_sent.type =STARTEXAM;
	data_sent.number1 = roomID;
	bytes_sent = send(clientsock_3, (void *) &data_sent, sizeof(data_sent), 0); 
	    if (bytes_sent < 0) perror("\nError: ");
	gtk_container_remove(GTK_CONTAINER (w5), layout5);
	startONLExamFunc(w5,clientsock_3,roomID,UserName);
}







