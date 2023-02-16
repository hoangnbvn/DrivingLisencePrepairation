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
#include "menu.h"
#include "roomlist.h"
#include "createroom.h"



//client-server

//

GtkWidget * layout3;
GtkWidget * w3;
int clientsock_1;

void backMenu2();
void RoomListLayoutClick(int select);
void RoomListLayout1();
void RoomListLayout2();
void RoomListLayout3();
void createRoomClick();
void drawLuyenTap(GtkWidget *window,int sk){
    GtkWidget *background;
    GtkWidget *btn1;
    GtkWidget *btn2;
    GtkWidget *btn3;
    GtkWidget *btn4;
    GtkWidget *btn5;
    clientsock_1 = sk;
    
    //gtk_container_remove(GTK_CONTAINER (window), layout3);
    layout3 = gtk_layout_new(NULL, NULL);
    w3 =window;
    
	
    background = gtk_image_new_from_file("Pictures/Backgrounds/background3.png");
    gtk_layout_put(GTK_LAYOUT(layout3), background, 0, 0);
    
    btn1 = gtk_button_new_with_label("Ph\u00f2ng S\u1eb5n S\u00e0ng");
    gtk_layout_put(GTK_LAYOUT(layout3), btn1, 200, 60);
    gtk_widget_set_size_request(btn1, 700, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn1),"sansang");
    
    btn2 = gtk_button_new_with_label("Ph\u00f2ng \u0110ang Thi");
    gtk_layout_put(GTK_LAYOUT(layout3), btn2, 200, 160);
    gtk_widget_set_size_request(btn2, 700, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn2),"dangthi");
    
    btn3 = gtk_button_new_with_label("Ph\u00f2ng \u0110\u00e3 K\u1ebft Th\u00fac");
    gtk_layout_put(GTK_LAYOUT(layout3), btn3, 200, 260);
    gtk_widget_set_size_request(btn3, 700, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn3),"ketthuc");
    
    btn4 = gtk_button_new_with_label("T\u1ea1o Ph\u00f2ng Thi M\u1edbi");
    gtk_layout_put(GTK_LAYOUT(layout3), btn4, 200, 360);
    gtk_widget_set_size_request(btn4, 700, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn4),"taophong");
    
    btn5 = gtk_button_new_with_label("Tr\u1edf v\u1ec1 Menu");
    gtk_layout_put(GTK_LAYOUT(layout3), btn5, 200, 460);
    gtk_widget_set_size_request(btn5, 700, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn5),"trovemenu");
    
    
    gtk_container_add(GTK_CONTAINER (window), layout3);
    gtk_widget_show(layout3);
    
    g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(RoomListLayout1),NULL);
    g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(RoomListLayout2),NULL);
    g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(RoomListLayout3),NULL);
    g_signal_connect(G_OBJECT(btn4),"clicked",G_CALLBACK(createRoomClick),NULL);
    g_signal_connect(G_OBJECT(btn5),"clicked",G_CALLBACK(backMenu2),NULL);
    
    gtk_widget_show_all(window);
}
void deleteLuyenTap(){
	gtk_container_remove(GTK_CONTAINER (w3), layout3);
}

void backMenu2(){

	disconnectServer(clientsock_1);
	deleteLuyenTap();
	drawMenu(w3);
}

void RoomListLayoutClick(int select){
	deleteLuyenTap();
	RoomList(w3,clientsock_1,select);
}
void RoomListLayout1(){
	RoomListLayoutClick(1);
}
void RoomListLayout2(){
	RoomListLayoutClick(2);
}
void RoomListLayout3(){
	RoomListLayoutClick(3);
}
void createRoomClick(){
	gtk_container_remove(GTK_CONTAINER (w3), layout3);
	SettingRoom(w3,clientsock_1);
}




