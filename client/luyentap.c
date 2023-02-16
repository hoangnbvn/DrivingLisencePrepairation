#include "message.h"
#include "luyentap.h"
#include "lambai.h"
#include "menu.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "fields.h"

GtkWidget *layout1;
GtkWidget *w1;
int score1=0,score2=0,score3=0,score4=0,score5=0,score6=0;
int UserLevel = 0;
int s;
void backMenu();
void Practice1();
void Practice2();
void Practice3();
void Practice4();
void Practice5();
void Practice6();

int checkUserLevel(){
	IS is;
	int sum = 0;
	int score;
	is=new_inputstruct("diemso.txt");
	get_line(is);
	for(int i=0;i<6;i++){
		score = atoi(is->fields[i]);
		sum += score;
	}
	jettison_inputstruct(is);
	if(sum<30) return 0;
	else if(sum>=30&&sum<60) return 1;
	else if(sum>=60&&sum<90) return 2;
	else return 3;
}

void readUserScore(){
	IS is;
	is=new_inputstruct("diemso.txt");
	get_line(is);
	score1 = atoi(is->fields[0]);
	score2 = atoi(is->fields[1]);
	score3 = atoi(is->fields[2]);
	score4 = atoi(is->fields[3]);
	score5 = atoi(is->fields[4]);
	score6 = atoi(is->fields[5]);
	jettison_inputstruct(is);
}

void drawListCourse(GtkWidget *window){
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;
    GtkWidget *button5;
    GtkWidget *button6;
    GtkWidget *button7;
     GtkWidget *button8;
    GtkWidget *image;
    char  buff[100];
    //gtk_container_remove(GTK_CONTAINER (window), layout);	
    UserLevel = checkUserLevel();
    readUserScore();
    layout1 = gtk_layout_new(NULL, NULL);
   image = gtk_image_new_from_file("Pictures/Backgrounds/background2.png");
      gtk_layout_put(GTK_LAYOUT(layout1), image, 0, 0);
  if(UserLevel == 0) sprintf(buff,"Trình Độ Hiện Tại: Luyện Tập");
  if(UserLevel == 1) sprintf(buff,"Trình Độ Hiện Tại: Sơ Cấp");
  if(UserLevel == 2) sprintf(buff,"Trình Độ Hiện Tại: Trung Cấp");
  if(UserLevel == 3) sprintf(buff,"Trình Độ Hiện Tại: Cao Cấp");
  
  button8 = gtk_button_new_with_label(buff);
  gtk_layout_put(GTK_LAYOUT(layout1), button8, 300, 50);
    gtk_widget_set_size_request(button8, 500, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button8),"buttonListCourse"); 
       
  sprintf(buff,"Lu\u00e2\u0323t l\u00ea\u0323 giao th\u00f4ng (%d/20)",score1);
  button1 = gtk_button_new_with_label(buff);
     gtk_layout_put(GTK_LAYOUT(layout1), button1, 50, 150);
    gtk_widget_set_size_request(button1, 430, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button1),"buttonListCourse");
    
  sprintf(buff,"\u01af\u0301ng x\u01b0\u0309 khi tham gia giao th\u00f4ng (%d/20)",score2);
  button2 = gtk_button_new_with_label(buff);
      gtk_layout_put(GTK_LAYOUT(layout1), button2, 600, 150);
    gtk_widget_set_size_request(button2, 430, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button2),"buttonListCourse");
    
  sprintf(buff,"Pha\u0309n \u01b0\u0301ng theo \u0111i\u00ea\u0300u lu\u00e2\u0323t (%d/20)",score3);
  button3 = gtk_button_new_with_label(buff);
    gtk_layout_put(GTK_LAYOUT(layout1), button3, 50, 270);
    gtk_widget_set_size_request(button3, 430, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button3),"buttonListCourse");
    
  sprintf(buff,"Bi\u00ea\u0309n ba\u0301o giao th\u00f4ng \u0111\u01b0\u01a1\u0300ng b\u00f4\u0323 (%d/20)",score4);
  button4 = gtk_button_new_with_label(buff);
  gtk_layout_put(GTK_LAYOUT(layout1), button4, 600, 270);
    gtk_widget_set_size_request(button4, 430, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button4),"buttonListCourse");
    
  sprintf(buff,"Bi\u00ea\u0309n ba\u0301o giao th\u00f4ng \u0111\u01b0\u01a1\u0300ng b\u00f4\u0323 (%d/20)",score5);
  button5 = gtk_button_new_with_label(buff);
  gtk_layout_put(GTK_LAYOUT(layout1), button5, 50, 390);
    gtk_widget_set_size_request(button5, 430, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button5),"buttonListCourse");

  sprintf(buff,"Cac\u0301 quy \u0111i\u0323nh khi v\u00ea\u0300 ph\u01b0\u01a1ng ti\u00ea\u0323n tham gia giao th\u00f4ng (%d/20)",score6);
  button6 = gtk_button_new_with_label(buff);
  gtk_layout_put(GTK_LAYOUT(layout1), button6, 600, 390);
    gtk_widget_set_size_request(button6, 430, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button6),"buttonListCourse");

  button7 = gtk_button_new_with_label("Thoa\u0301t");
  gtk_layout_put(GTK_LAYOUT(layout1), button7, 360, 500);
    gtk_widget_set_size_request(button7, 380, 80);
    gtk_style_context_add_class(gtk_widget_get_style_context(button7),"buttonListCourse");


    gtk_container_add(GTK_CONTAINER(window), layout1);
    gtk_widget_show(layout1);
    gtk_widget_show_all(window); 
    g_signal_connect(G_OBJECT(button7),"clicked",G_CALLBACK(backMenu),NULL);
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(Practice1),NULL);
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(Practice2),NULL);
    g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(Practice3),NULL);
    g_signal_connect(G_OBJECT(button4),"clicked",G_CALLBACK(Practice4),NULL);
    g_signal_connect(G_OBJECT(button5),"clicked",G_CALLBACK(Practice5),NULL);
    g_signal_connect(G_OBJECT(button6),"clicked",G_CALLBACK(Practice6),NULL);
    
    
    //g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(Practice1),(NULL);
    //g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(Practice2),NULL);
    //g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(Practice3),NULL);
    //g_signal_connect(G_OBJECT(button4),"clicked",G_CALLBACK(Practice4),NULL);
    //g_signal_connect(G_OBJECT(button5),"clicked",G_CALLBACK(Practice5),NULL);
    //g_signal_connect(G_OBJECT(button6),"clicked",G_CALLBACK(Practice6),NULL);
    //g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(gtk_main_quit),NULL);
    
}
void deleteListCourse(GtkWidget *window){
    gtk_container_remove(GTK_CONTAINER (window), layout1);	
}
//event

void backMenu(){ 
    deleteListCourse(w1);
    drawMenu(w1);
}

void Practice(int select){
	deleteListCourse(w1);
	s = select;
	startExamFunc(w1,select);
}
void Practice1(){
	Practice(1);
}
void Practice2(){
	Practice(2);
}
void Practice3(){
	Practice(3);
}
void Practice4(){
	Practice(4);
}
void Practice5(){
	Practice(5);
}
void Practice6(){
	Practice(6);
}

//event

void ListCourse(GtkWidget *window){
	w1 = window;
	drawListCourse(window);
}



