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
#include "createroom.h"
#include "lambai.h"
#include "showresultsofroom.h"
#include "luyentap.h"

pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t megRecvThread2;


// PHAN 7: SHOWROOM LIST
int page;
showRoomListResponse roomlist;
int clientsocket_2;
char str1[50],str2[50];
int roomID1;
char UserName1[50];
char RoomName1[50];
int roomStart1,maxtime2,numberQuestion2,level2;
GtkWidget *w4;
GtkWidget *layout4;
int typeOfRoom;
void RoomListLayout(GtkWidget *window,int select);
void resetScreen3(int select);
void nextButtonClick3();
void backButtonClick3();
void backtoVaoThi();
void inputUserNameView(int select,GtkWidget *window);

void inputUserNameView1();
void inputUserNameView2();
void inputUserNameView3();
void JoinRoomClick();
void draw_Wait_Room_View_2(GtkWidget * window, char *numberUserString);
void * revMsgToServer2(void *data);
void LeaveRoom();
void startRoomclick();
void showResultClick(int select);
void showResultClick1();
void showResultClick2();
void showResultClick3();


void RoomList(GtkWidget *window,int socket,int n){
     RequestForm data_sent;
     clientsocket_2 = socket;
     int bytes_sent;
     w4 = window;
    data_sent.type=SHOWROOMLIST;
    typeOfRoom = n;
    data_sent.number1 = typeOfRoom;
    bytes_sent = send(socket, (void *) &data_sent, sizeof(data_sent), 0); 
        if (bytes_sent < 0) perror("\nError: ");
    bytes_sent = recv(socket, (void *) &roomlist, sizeof(showRoomListResponse), 0); 
        if (bytes_sent < 0) perror("\nError: ");
    RoomListLayout(window,typeOfRoom);
}

GtkWidget *room1;
GtkWidget *room2;
GtkWidget *room3;
GtkWidget *btn1;
GtkWidget *btn2;
GtkWidget *btn3;
void RoomListLayout(GtkWidget *window,int select){ 

    GtkWidget *topFrame;
    GtkWidget *background;
    GtkWidget *exitButton;
    GtkWidget *BackButton;
    GtkWidget *NextButton;
    
    char *buff;
    buff = (char*)calloc(100,sizeof(char));
    page = 0;
    
    
    if(select == 1){
    	strcpy(str1,"Ph\u00f2ng S\u1eb5n S\u00e0ng");
    	strcpy(str2,"Tham Gia");
    }
    else if(select ==2){
    	strcpy(str1,"Ph\u00f2ng \u0110ang Thi");
    	strcpy(str2,"Xem");
    }
    else if(select ==3){
    	strcpy(str1,"Ph\u00f2ng \u0110\u00e3 K\u1ebft Th\u00fac");
    	strcpy(str2,"Xem");
    }
    
    	//gtk_container_remove(GTK_CONTAINER (window), layout);
	layout4 = gtk_layout_new(NULL, NULL);
	background = gtk_image_new_from_file("Pictures/Backgrounds/background3.png");
	gtk_layout_put(GTK_LAYOUT(layout4), background, 0, 0);
	
	BackButton = gtk_button_new_with_label("L\u00f9i L\u1ea1i");
	gtk_layout_put(GTK_LAYOUT(layout4), BackButton, 125, 457);
	gtk_widget_set_size_request(BackButton, 185, 50);
	gtk_style_context_add_class(gtk_widget_get_style_context(BackButton),"roomListButton");
	    
	exitButton = gtk_button_new_with_label("Tho\u00e1t");
	gtk_layout_put(GTK_LAYOUT(layout4), exitButton, 435, 457);
	gtk_widget_set_size_request(exitButton, 185, 50);
	gtk_style_context_add_class(gtk_widget_get_style_context(exitButton),"roomListButton");
	    
	NextButton = gtk_button_new_with_label("Ti\u1ebfp theo");
	gtk_layout_put(GTK_LAYOUT(layout4), NextButton, 770, 457);
	gtk_widget_set_size_request(NextButton, 185, 50);
	gtk_style_context_add_class(gtk_widget_get_style_context(NextButton),"roomListButton");
	    
	topFrame = gtk_button_new_with_label(str1);
	gtk_layout_put(GTK_LAYOUT(layout4), topFrame, 125, 30);
	gtk_widget_set_size_request(topFrame, 835, 80);
	gtk_style_context_add_class(gtk_widget_get_style_context(topFrame),"roomNameFrame");
	
	if(page*3+1<=roomlist.numberroom){
		if(roomlist.level[page*3]!=0){
		 	if (roomlist.level[page*3]==1) sprintf(buff,"%s - ĐỘ KHÓ: Sơ Cấp",roomlist.roomname[page*3]);
		 	if (roomlist.level[page*3]==2) sprintf(buff,"%s - ĐỘ KHÓ: Trung Cấp",roomlist.roomname[page*3]);
		 	if (roomlist.level[page*3]==3) sprintf(buff,"%s - ĐỘ KHÓ: Cao Cấp",roomlist.roomname[page*3]);
		}
		else sprintf(buff,"%s",roomlist.roomname[page*3]);
		room1 = gtk_button_new_with_label(buff);
		gtk_layout_put(GTK_LAYOUT(layout4), room1, 125, 170);
		gtk_widget_set_size_request(room1, 556, 50);

		btn1 = gtk_button_new_with_label(str2);
		gtk_layout_put(GTK_LAYOUT(layout4), btn1, 750, 170);
		gtk_widget_set_size_request(btn1, 210, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(btn1),"joinButton");
		if(select==1){
			g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(inputUserNameView1),NULL);
		}
		else if (select == 2){
			//g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(inputUserNameView2),NULL);
		}
		else if(select == 3){
			g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(showResultClick1),NULL);
		}
		
	}
	if(page*3+2<=roomlist.numberroom){
		if(roomlist.level[page*3+1]!=0){
		 	if (roomlist.level[page*3+1]==1) sprintf(buff,"%s - ĐỘ KHÓ: Sơ Cấp",roomlist.roomname[page*3+1]);
		 	if (roomlist.level[page*3+1]==2) sprintf(buff,"%s - ĐỘ KHÓ: Trung Cấp",roomlist.roomname[page*3+1]);
		 	if (roomlist.level[page*3+1]==3) sprintf(buff,"%s - ĐỘ KHÓ: Cao Cấp",roomlist.roomname[page*3+1]);
		}
		else sprintf(buff,"%s",roomlist.roomname[page*3+1]);
		room2 = gtk_button_new_with_label(buff);
		gtk_layout_put(GTK_LAYOUT(layout4), room2, 125, 253);
		gtk_widget_set_size_request(room2, 556, 50);

		btn2 = gtk_button_new_with_label(str2);
		gtk_layout_put(GTK_LAYOUT(layout4), btn2, 750, 253);
		gtk_widget_set_size_request(btn2, 210, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(btn2),"joinButton");
		if(select==1){
			g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(inputUserNameView2),NULL);
		}
		else if (select == 2){
			//g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(inputUserNameView),(int)room2ID);
		}
		else if(select == 3){
			g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(showResultClick2),NULL);
		}
	}
	if(page*3+3<=roomlist.numberroom){
		if(roomlist.level[page*3+2]!=0){
		 	if (roomlist.level[page*3+2]==1) sprintf(buff,"%s - ĐỘ KHÓ: Sơ Cấp",roomlist.roomname[page*3+2]);
		 	if (roomlist.level[page*3+2]==2) sprintf(buff,"%s - ĐỘ KHÓ: Trung Cấp",roomlist.roomname[page*3+2]);
		 	if (roomlist.level[page*3+2]==3) sprintf(buff,"%s - ĐỘ KHÓ: Cao Cấp",roomlist.roomname[page*3+2]);
		}
		else sprintf(buff,"%s",roomlist.roomname[page*3+2]);
		room3 = gtk_button_new_with_label(buff);
		gtk_layout_put(GTK_LAYOUT(layout4), room3, 125, 332);
		gtk_widget_set_size_request(room3, 556, 50);

		btn3 = gtk_button_new_with_label(str2);
		gtk_layout_put(GTK_LAYOUT(layout4), btn3, 750, 332);
		gtk_widget_set_size_request(btn3, 210, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(btn3),"joinButton");
		if(select==1){
			g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(inputUserNameView3),NULL);
		}
		else if (select == 2){
			//g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(inputUserNameView),(int)room3ID);
		}
		else if(select == 3){
			g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(showResultClick3),NULL);
		}
	}
	gtk_container_add(GTK_CONTAINER (window), layout4);
    	gtk_widget_show(layout4);
    	free(buff);
    	g_signal_connect(G_OBJECT(NextButton),"clicked",G_CALLBACK(nextButtonClick3),NULL);
    	g_signal_connect(G_OBJECT(BackButton),"clicked",G_CALLBACK(backButtonClick3),NULL);
    	g_signal_connect(G_OBJECT(exitButton),"clicked",G_CALLBACK(backtoVaoThi),NULL);
    	gtk_widget_show_all(window);
}
//--------------------------------------------------------------------------------------------
void resetScreen3(int select){
	char *buff;
    	buff = (char*)calloc(100,sizeof(char));
	if(page*3+1<=roomlist.numberroom) {
		if(roomlist.level[page*3]!=0){
		 	if (roomlist.level[page*3]==1) sprintf(buff,"%s - ĐỘ KHÓ: Sơ Cấp",roomlist.roomname[page*3]);
		 	if (roomlist.level[page*3]==2) sprintf(buff,"%s - ĐỘ KHÓ: Trung Cấp",roomlist.roomname[page*3]);
		 	if (roomlist.level[page*3]==3) sprintf(buff,"%s - ĐỘ KHÓ: Cao Cấp",roomlist.roomname[page*3]);
		}
		else sprintf(buff,"%s",roomlist.roomname[page*3]);
		gtk_button_set_label(GTK_BUTTON(room1),buff);
		gtk_widget_destroy(btn1);
		btn1 = gtk_button_new_with_label(str2);
		gtk_layout_put(GTK_LAYOUT(layout4), btn1, 750, 170);
		gtk_widget_set_size_request(btn1, 210, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(btn1),"joinButton");
		if(select==1){
			g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(inputUserNameView1),NULL);
		}
		else if (select == 2){
			//g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(inputUserNameView),(int)room1ID);
		}
		else if(select == 3){
			g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(showResultClick1),NULL);
		}
	}
	if(page*3+2<=roomlist.numberroom) {
		if(roomlist.level[page*3+1]!=0){
		 	if (roomlist.level[page*3+1]==1) sprintf(buff,"%s - ĐỘ KHÓ: Sơ Cấp",roomlist.roomname[page*3+1]);
		 	if (roomlist.level[page*3+1]==2) sprintf(buff,"%s - ĐỘ KHÓ: Trung Cấp",roomlist.roomname[page*3+1]);
		 	if (roomlist.level[page*3+1]==3) sprintf(buff,"%s - ĐỘ KHÓ: Cao Cấp",roomlist.roomname[page*3+1]);
		}
		else sprintf(buff,"%s",roomlist.roomname[page*3+1]);
		gtk_button_set_label(GTK_BUTTON(room2),buff);
		gtk_widget_destroy(btn2);
		btn2 = gtk_button_new_with_label(str2);
		gtk_layout_put(GTK_LAYOUT(layout4), btn2, 750, 253);
		gtk_widget_set_size_request(btn2, 210, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(btn2),"joinButton");
		if(select==1){
			g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(inputUserNameView2),NULL);
		}
		else if (select == 2){
			//g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(inputUserNameView),(int)room2ID);
		}
		else if(select == 3){
			g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(showResultClick2),NULL);
		}
	}
	if(page*3+3<=roomlist.numberroom) {
		if(roomlist.level[page*3+2]!=0){
		 	if (roomlist.level[page*3+2]==1) sprintf(buff,"%s - ĐỘ KHÓ: Sơ Cấp",roomlist.roomname[page*3+2]);
		 	if (roomlist.level[page*3+2]==2) sprintf(buff,"%s - ĐỘ KHÓ: Trung Cấp",roomlist.roomname[page*3+2]);
		 	if (roomlist.level[page*3+2]==3) sprintf(buff,"%s - ĐỘ KHÓ: Cao Cấp",roomlist.roomname[page*3+2]);
		}
		else sprintf(buff,"%s",roomlist.roomname[page*3+2]);
		gtk_button_set_label(GTK_BUTTON(room3),buff);
		gtk_widget_destroy(btn3);
		btn3 = gtk_button_new_with_label(str2);
		gtk_layout_put(GTK_LAYOUT(layout4), btn3, 750, 332);
		gtk_widget_set_size_request(btn3, 210, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(btn3),"joinButton");
		if(select==1){
			g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(inputUserNameView3),NULL);
		}
		else if (select == 2){
			//g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(inputUserNameView),(int)room3ID);
		}
		else if(select == 3){
			g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(showResultClick3),NULL);
		}
	}
	free(buff);
	gtk_widget_show(layout4);
	gtk_widget_show_all(w4);
}

//2 XU ly su kien
void nextButtonClick3(){
	if(page<roomlist.numberroom/3){
		page++;
		resetScreen3(typeOfRoom);
	}
}
void backButtonClick3(){
	if(page>0){
		page--;
		resetScreen3(typeOfRoom);
	}
}
void backtoVaoThi(){
	gtk_container_remove(GTK_CONTAINER (w4), layout4);
	drawLuyenTap(w4,clientsocket_2);	
}


// join room
int clocktime2;
GtkWidget *errorFrame2;
static gboolean delete_error2()
{
    clocktime2++;
    if(clocktime2 <2) return TRUE;
    else{
    	gtk_widget_destroy(errorFrame2);
    	gtk_widget_show(layout4);
    	gtk_widget_show_all(w4);
    	return FALSE;
    } 
}


GtkWidget *NameInput;
int selectroom;
void inputUserNameView(int select,GtkWidget *window){
	selectroom = select;
    	GtkWidget *background;
    	GtkWidget *exitButton;
    	GtkWidget *btn1;
    	int UserLevel;
    	UserLevel = checkUserLevel();
    	
    	if(UserLevel>=roomlist.level[select]){
    		gtk_container_remove(GTK_CONTAINER (window), layout4);
	    	layout4 = gtk_layout_new(NULL, NULL);
	    	
		background = gtk_image_new_from_file("Pictures/Backgrounds/background5.png");
		gtk_layout_put(GTK_LAYOUT(layout4), background, 0, 0);
		
		NameInput = gtk_entry_new();
		gtk_layout_put(GTK_LAYOUT(layout4), NameInput, 119,278);
		gtk_widget_set_size_request(NameInput,445,50);
		
		btn1 = gtk_button_new_with_label("Tham gia");
		gtk_layout_put(GTK_LAYOUT(layout4), btn1, 126, 362);
		gtk_widget_set_size_request(btn1, 160, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(btn1),"roomListButton");
		

		exitButton = gtk_button_new_with_label("Thoat");
		gtk_layout_put(GTK_LAYOUT(layout4), exitButton, 126, 452);
		gtk_widget_set_size_request(exitButton, 160, 50);
		gtk_style_context_add_class(gtk_widget_get_style_context(exitButton),"roomListButton");

		gtk_widget_show(layout4);
		g_signal_connect(G_OBJECT(exitButton),"clicked",G_CALLBACK(backtoVaoThi),NULL);
		g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(JoinRoomClick),NULL);
		gtk_container_add(GTK_CONTAINER (window), layout4);
		gtk_widget_show_all(window);	
    	}
    	else{
    		errorFrame2 = gtk_button_new_with_label("Không Thể Tham Gia");
    		gtk_layout_put(GTK_LAYOUT(layout4), errorFrame2, 200, 10);
    		gtk_widget_set_size_request(errorFrame2, 700, 10);
    		gtk_style_context_add_class(gtk_widget_get_style_context(errorFrame2),"ErrorFrame");
		    	gtk_widget_show(layout4);
		    	gtk_widget_show_all(w4);
		    	clocktime2 = 0;
		    	g_timeout_add_seconds(1, delete_error2, NULL);
    	}
}
void inputUserNameView1(){
	inputUserNameView(page*3,w4);
}
void inputUserNameView2(){
	inputUserNameView(page*3+1,w4);
}
void inputUserNameView3(){
	inputUserNameView(page*3+2,w4);
}
joinRoomReponse *joinroomreponse;
void JoinRoomClick(){
	const gchar* string;
	char *buff;
	string = gtk_entry_get_text(GTK_ENTRY(NameInput));
	sprintf(UserName1,"%s",string);
	RequestForm data_sent;
	int bytes_sent,bytes_received;
	joinroomreponse = (joinRoomReponse *)calloc(1,sizeof(joinRoomReponse));
	buff = (char*)calloc(100,sizeof(char));
	
	data_sent.type =JOINROOM;
	roomID1 = roomlist.roomID[selectroom]-1;
	data_sent.number1 =roomlist.roomID[selectroom];
	strcpy(data_sent.string1,UserName1);
	
	bytes_sent = send(clientsocket_2, (void *) &data_sent, sizeof(data_sent), 0); 
		if (bytes_sent < 0) perror("\nError: ");
	bytes_received = recv(clientsocket_2, (void *) joinroomreponse, sizeof(joinRoomReponse), 0); 
		if (bytes_sent < 0) perror("\nError: ");
	data_sent.type =HELLOSERVER;
	roomStart1 = 0;
	maxtime2 = joinroomreponse->time;
	g_print("%d",maxtime2);
	
	numberQuestion2 = joinroomreponse->numberQuestion;
	level2 = joinroomreponse->level;
	strcpy(RoomName1,joinroomreponse->roomName);
	sprintf(buff,"S\u1ed1 Ng\u01b0\u1eddi Tham Gia: %d/20",joinroomreponse->numberUser);
	gtk_container_remove(GTK_CONTAINER (w4), layout4);
	draw_Wait_Room_View_2(w4,buff);
	
	pthread_create(&megRecvThread2,NULL,&revMsgToServer2,NULL);
	free(buff);
	free(joinroomreponse);
}

GtkWidget *numberUserFrame2;
GtkWidget *StartExambtn2;
GtkWidget *roomnameFrame2;
void draw_Wait_Room_View_2(GtkWidget * window, char *numberUserString){ // Member View
    GtkWidget *background;
    
    GtkWidget *detailFrame;
    GtkWidget *CancelRoombtn;
    GtkWidget *Label1;
    char * string;
    string  = (char*)calloc(100,sizeof(char));
    layout4 = gtk_layout_new(NULL, NULL);
    
    background = gtk_image_new_from_file("Pictures/Backgrounds/background3.png");
    gtk_layout_put(GTK_LAYOUT(layout4), background, 0, 0);
    
    sprintf(string,"Ph\u00f2ng Thi: %s",RoomName1);
    //sprintf(buff,"Ph\u00f2ng Thi: TOT NGHIEP LAI XE");
    
    roomnameFrame2 = gtk_button_new_with_label(string);
    gtk_layout_put(GTK_LAYOUT(layout4), roomnameFrame2, 20, 30);
    gtk_widget_set_size_request(roomnameFrame2, 740, 100);
    gtk_style_context_add_class(gtk_widget_get_style_context(roomnameFrame2),"roomNameFrame");
    
    
    numberUserFrame2 = gtk_button_new_with_label(numberUserString);
    gtk_layout_put(GTK_LAYOUT(layout4), numberUserFrame2, 20, 150);
    gtk_widget_set_size_request(numberUserFrame2, 740, 450);
    gtk_style_context_add_class(gtk_widget_get_style_context(numberUserFrame2),"numberUserFrame");
    
    Label1 = gtk_button_new_with_label("Chờ Phòng Thi Bắt Đầu");
    gtk_layout_put(GTK_LAYOUT(layout4), Label1, 20, 150);
    gtk_widget_set_size_request(Label1, 740, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context( Label1),"Label1");

    CancelRoombtn = gtk_button_new_with_label("H\u1ee7y Ph\u00f2ng");
    gtk_layout_put(GTK_LAYOUT(layout4), CancelRoombtn, 780, 150);
    gtk_widget_set_size_request(CancelRoombtn, 300, 50);
    gtk_style_context_add_class(gtk_widget_get_style_context(CancelRoombtn),"CancelRoombtn");
                
    StartExambtn2= gtk_button_new_with_label("B\u1eaft \u0110\u1ea7u Thi");
    gtk_layout_put(GTK_LAYOUT(layout4), StartExambtn2, 780, 210);
    gtk_widget_set_size_request(StartExambtn2, 300, 50);
    if(roomStart1 == 1) gtk_style_context_add_class(gtk_widget_get_style_context(StartExambtn2),"StartExambtn");
    if (roomStart1 == 0)gtk_style_context_add_class(gtk_widget_get_style_context(StartExambtn2),"NoStartExambtn");
    
    //numberQuestion = 20;
    //maxtime =1200;
    sprintf(string,"S\u1ed1 C\u00e2u H\u1ecfi: %d C\u00e2u \n\nTh\u1eddi Gian Thi: %d Ph\u00fat\n\n Mức Độ Khó: %d",numberQuestion2,maxtime2/60,level2);
    detailFrame = gtk_button_new_with_label(string);
    gtk_layout_put(GTK_LAYOUT(layout4), detailFrame, 780, 270);
    gtk_widget_set_size_request(detailFrame, 300, 330);
    gtk_style_context_add_class(gtk_widget_get_style_context(detailFrame),"detailFrame");
    
    
    gtk_container_add(GTK_CONTAINER (window), layout4);
    gtk_widget_show(layout4);
    g_signal_connect(G_OBJECT(StartExambtn2),"clicked",G_CALLBACK(startRoomclick),NULL);
    g_signal_connect(G_OBJECT(CancelRoombtn),"clicked",G_CALLBACK(LeaveRoom),NULL);
    gtk_widget_show_all(window);
    free(string);
}

void * revMsgToServer2(void *data)// Nhận thông tin người chơi khác tham gia phòng khi
{
	pthread_detach(pthread_self());
	int bytes_received;
	char strNumberUser[50];
	

	while(1){
		joinroomreponse = (joinRoomReponse *)calloc(1,sizeof(joinRoomReponse));
		bytes_received = recv(clientsocket_2,(void* ) joinroomreponse, sizeof(joinRoomReponse), 0); 
			if (bytes_received < 0) perror("\nError: ");
			
		if(joinroomreponse->type == JOINROOM){
			sprintf(strNumberUser,"S\u1ed1 Ng\u01b0\u1eddi Tham Gia: %d/20",joinroomreponse->numberUser);
			gtk_button_set_label(GTK_BUTTON(numberUserFrame2),strNumberUser);
		}
		else if(joinroomreponse->type == STARTEXAM){
		    roomStart1 = 1;
		    gtk_style_context_add_class(gtk_widget_get_style_context(StartExambtn2),"StartExambtn");
		    gtk_widget_destroy(StartExambtn2);
		    StartExambtn2= gtk_button_new_with_label("B\u1eaft \u0110\u1ea7u Thi");
		    gtk_layout_put(GTK_LAYOUT(layout4), StartExambtn2, 780, 210);
		    gtk_widget_set_size_request(StartExambtn2, 300, 50);
		    gtk_style_context_add_class(gtk_widget_get_style_context(StartExambtn2),"StartExambtn");
		    g_signal_connect(G_OBJECT(StartExambtn2),"clicked",G_CALLBACK(startRoomclick),NULL);
		    gtk_widget_show(layout4);
		    gtk_widget_show_all(w4);	
		    break;
		}
		else if(joinroomreponse->type == CANCELROOM){
		   gtk_button_set_label( GTK_BUTTON(roomnameFrame2),"Phòng Thi Đã Hủy. Hãy Rời Phòng");
		   gtk_widget_show(layout4);
		    gtk_widget_show_all(w4);	
		    break;
		}
		else if(joinroomreponse->type == LEFTROOM) break;
		free(joinroomreponse);
	}
	return NULL;
}

void LeaveRoom(){
	backtoVaoThi();
	sleep(1);
	RequestForm data_sent;
	int bytes_sent;
	data_sent.type = LEFTROOM;
	data_sent.number1 = roomID1;
	strcpy(data_sent.string1,UserName1);
	bytes_sent = send(clientsocket_2, (void *) &data_sent, sizeof(data_sent), 0); 
		if (bytes_sent < 0) perror("\nError: ");
	
}
void startRoomclick(){
	if(roomStart1 == 1){
		gtk_container_remove(GTK_CONTAINER (w4), layout4);
		startONLExamFunc(w4,clientsocket_2,roomID1,UserName1);
		roomStart1 = 0;
	}
}


void showResultClick(int select){
	gtk_container_remove(GTK_CONTAINER (w4), layout4);
	ResultsOfRoom(w4,clientsocket_2,roomlist.roomID[select],roomlist.roomname[select]);
}
void showResultClick1(){
	showResultClick(page*3);
}
void showResultClick2(){
	showResultClick(page*3+1);
}
void showResultClick3(){
	showResultClick(page*3+2);
}




