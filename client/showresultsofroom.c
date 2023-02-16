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
#include "message.h"
#include "roomlist.h"
roomResultsResponse roomresultsresponse;
GtkWidget *layout6;
GtkWidget *w6;
int socket_6;

void backtoRoomList(){
	gtk_container_remove(GTK_CONTAINER (w6), layout6);
	RoomList(w6,socket_6,3);
}
void ResultsOfRoomLayout(GtkWidget *window,char RoomName[50]){
    GtkWidget *background;
    GtkWidget *numberUserFrame;
    GtkWidget *roomnameFrame;
    GtkWidget *CancelRoombtn;
    GtkWidget *Label1;
    GtkWidget *timeFrame;
    GtkWidget *scoreFrame;
    GtkWidget *nameFrame;
    GtkWidget *indexFrame;
    
    char * string;
    char *buff;
    string  = (char*)calloc(100,sizeof(char));
    buff = (char*) calloc(100,sizeof(char));
    layout6 = gtk_layout_new(NULL, NULL);
    
    background = gtk_image_new_from_file("Pictures/Backgrounds/background3.png");
    gtk_layout_put(GTK_LAYOUT(layout6), background, 0, 0);
    
    sprintf(string,"Ph\u00f2ng Thi: %s",RoomName);
    //sprintf(string,"Ph\u00f2ng Thi: TOT NGHIEP LAI XE");
    
    roomnameFrame = gtk_button_new_with_label(string);
    gtk_layout_put(GTK_LAYOUT(layout6), roomnameFrame, 20, 30);
    gtk_widget_set_size_request(roomnameFrame, 740, 100);
    gtk_style_context_add_class(gtk_widget_get_style_context(roomnameFrame),"roomNameFrame");
    numberUserFrame = gtk_button_new_with_label("");
    gtk_layout_put(GTK_LAYOUT(layout6), numberUserFrame, 20, 150);
    gtk_widget_set_size_request(numberUserFrame, 740, 450);
    gtk_style_context_add_class(gtk_widget_get_style_context(numberUserFrame),"resultsFrame");
    
    for(int i=0;i<roomresultsresponse.numberUser;i++){
    	    sprintf(buff,"%d",i+1);
            indexFrame = gtk_button_new_with_label(buff);
	    gtk_layout_put(GTK_LAYOUT(layout6), indexFrame, 100, 235+ 40*i);
	    gtk_widget_set_size_request(indexFrame, 50, 30);
	    
	    sprintf(buff,"%s",roomresultsresponse.usename[i]);
	    nameFrame = gtk_button_new_with_label(buff);
	    gtk_layout_put(GTK_LAYOUT(layout6), nameFrame, 180, 235+ 40*i);
	    gtk_widget_set_size_request(nameFrame, 200, 30);
	    
	    sprintf(buff,"%d",roomresultsresponse.time[i]);
	    timeFrame = gtk_button_new_with_label(buff);
	    gtk_layout_put(GTK_LAYOUT(layout6), timeFrame, 470, 235+ 40*i);
	    gtk_widget_set_size_request(timeFrame, 100, 30);
	    
	    sprintf(buff,"%d",roomresultsresponse.score[i]);
	    scoreFrame = gtk_button_new_with_label(buff);
	    gtk_layout_put(GTK_LAYOUT(layout6), scoreFrame, 605, 235+ 40*i);
	    gtk_widget_set_size_request(scoreFrame, 100, 30);
    }
    
    sprintf(string,"%-20s%-35s%-20s%-10s","Thứ Tự","Tên Thí Sinh","Thời Gian","Điểm");
    Label1 = gtk_button_new_with_label(string);
    gtk_layout_put(GTK_LAYOUT(layout6), Label1, 20, 150);
    gtk_widget_set_size_request(Label1, 740, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context( Label1),"Label1");

    CancelRoombtn = gtk_button_new_with_label("Thoat");
    gtk_layout_put(GTK_LAYOUT(layout6), CancelRoombtn, 780, 150);
    gtk_widget_set_size_request(CancelRoombtn, 300, 50);
    gtk_style_context_add_class(gtk_widget_get_style_context(CancelRoombtn),"CancelRoombtn");
                
    gtk_container_add(GTK_CONTAINER (window), layout6);
    gtk_widget_show(layout6);
    
    g_signal_connect(G_OBJECT(CancelRoombtn),"clicked",G_CALLBACK(backtoRoomList),NULL);
    gtk_widget_show_all(window);
    free(string);
    free(buff);
}
void ResultsOfRoom(GtkWidget *window,int socket,int roomID,char RoomName[50]){
	w6 = window;
	socket_6 = socket;
     	RequestForm data_sent;
     	int bytes_sent;
	sprintf(data_sent.string1,"%d",roomID);
	data_sent.type = SHOWRESULTSOFROOM;
	bytes_sent = send(socket, (void *) &data_sent, sizeof(data_sent), 0); 
	if (bytes_sent < 0) perror("\nError: ");
	bytes_sent = recv(socket, (void *) &roomresultsresponse, sizeof(roomResultsResponse), 0); 
	if (bytes_sent < 0) perror("\nError: ");
	ResultsOfRoomLayout(window,RoomName);
}

