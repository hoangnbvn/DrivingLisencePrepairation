#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include "fields.h"
#include <string.h>
#include <ctype.h>
#include "message.h"
#include "luyentap.h"
#include "vaothi.h"

#define BUFF_SIZE 1024
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"

int client_sock;
struct sockaddr_in server_addr;  

// read file

Exam * questionList =NULL;
    
char * deleteUnderscore(char * string,int n){
	int lenOfString = strlen(string);
	int i = 0;
	int count = 0;
	for(i=0;i<lenOfString;i++){
		if(string[i]=='_'){
			if(count == n){
				count =0;
				string[i] = '\n';
			}
			else{
				string[i] = ' ';
				count ++;
			} 
		}
	}
return string;
}

Exam * readQuestionFile(char* filename){
    IS is;
    int numberQuestion = 0;
    Exam * questionList;
    questionList =(Exam*) calloc(1,sizeof(Exam));
    is=new_inputstruct(filename);
    if(is->f!=NULL){
        while(1){
            get_line(is);
            if(is->NF<1) break;
            
            strcpy(questionList->question[numberQuestion].q,deleteUnderscore(is->fields[0],18));
            strcpy(questionList->question[numberQuestion].imagelink,is->fields[1]);
            strcpy(questionList->question[numberQuestion].hint,deleteUnderscore(is->fields[2],11));
            questionList->question[numberQuestion].correctAnswer = atoi(is->fields[3]);
            strcpy(questionList->question[numberQuestion].answer1,deleteUnderscore(is->fields[4],10));
            strcpy(questionList->question[numberQuestion].answer2,deleteUnderscore(is->fields[5],10));
            strcpy(questionList->question[numberQuestion].answer3,deleteUnderscore(is->fields[6],10));
            if(is->NF == 7 ) strcpy(questionList->question[numberQuestion].answer4,"");
            else  strcpy(questionList->question[numberQuestion].answer4,deleteUnderscore(is->fields[7],10));     
            numberQuestion++;  
        }
        jettison_inputstruct(is);
    }
    questionList->numberQuestion = numberQuestion;
    return questionList;
}

//

//event-func
void LuyenTapButtonClick();
void VaoThiButtonClick();
//
GtkWidget *layout;
GtkWidget *w;
void drawMenu(GtkWidget *window){
    GtkWidget *image;
    GtkWidget *btn1;
    GtkWidget *btn2;
    GtkWidget *btn3;
    GtkWidget *buttonImage;
    //gtk_container_remove(GTK_CONTAINER (window), layout);
    layout = gtk_layout_new(NULL, NULL);
    
    image = gtk_image_new_from_file("Pictures/Backgrounds/background.png");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
    
    btn1 = gtk_button_new_with_label("Luy\u1ec7n t\u1eadp");
    gtk_layout_put(GTK_LAYOUT(layout), btn1, 651, 351);
    gtk_widget_set_size_request(btn1, 218, 47);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn1),"buttonMenu");
    
    btn2 = gtk_button_new_with_label("V\u00e0o thi");
    gtk_layout_put(GTK_LAYOUT(layout), btn2, 700, 420);
    gtk_widget_set_size_request(btn2, 220, 50);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn2),"buttonMenu");
    
    btn3 = gtk_button_new_with_label("Tho\u00e1t");
    gtk_layout_put(GTK_LAYOUT(layout), btn3, 630, 490);
    gtk_widget_set_size_request(btn3, 220, 50);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn3),"buttonMenu");

    
    gtk_container_add(GTK_CONTAINER (window), layout);
    gtk_widget_show(layout);
    g_signal_connect(G_OBJECT(btn1),"clicked",G_CALLBACK(LuyenTapButtonClick),NULL);
    g_signal_connect(G_OBJECT(btn2),"clicked",G_CALLBACK(VaoThiButtonClick),NULL);
    g_signal_connect(G_OBJECT(btn3),"clicked",G_CALLBACK(gtk_main_quit),NULL);
    gtk_widget_show_all(window);
}
void deleteMenu(GtkWidget *window){
	gtk_container_remove(GTK_CONTAINER (window), layout);
}

// event
void LuyenTapButtonClick(){
	deleteMenu(w);
	ListCourse(w);
}

void VaoThiButtonClick(){
    //Step 1: Construct socket
    client_sock = socket(AF_INET,SOCK_STREAM,0);

    //Step 2: Specify server address 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    
    
    if(connectServer(client_sock,server_addr)){
    deleteMenu(w);
    drawLuyenTap(w,client_sock);
    
    }
    else g_print("Error!Can not connect to sever! Client exit imediately!");
}

// event


void Menu(GtkWidget *window){
    w = window;
    questionList = readQuestionFile("question.txt");
    drawMenu(window);
}

Exam * importQuestionList(){
	return questionList;
}


