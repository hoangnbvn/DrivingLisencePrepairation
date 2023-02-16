#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include "fields.h"
#include <string.h>
#include <ctype.h>
#include "message.h"
#include "menu.h"

//alyout
Exam * questionList1 =NULL;
GtkWidget *layout2;
GtkWidget *w2;
// Exame
int UserSelect2;
int list[50];
int numberQuestion;
int count;
int results[50];
char buff[1024];
//clock
static int sec_expired = 0;
int maxtime = 10000;
int RUN = FALSE;
int connectserver = 0;
char UserName5[50];
int roomID5;
int clientsocket_5;
GtkWidget *buttonA;
GtkWidget *buttonB;
GtkWidget *buttonC;
GtkWidget *buttonD;
GtkWidget *questionImage;
GtkWidget *questionButton;
GtkWidget *NumberQuestionFrame;


void nextButtonClick();
void backButtonClick();
void resetScreen();
void selectAnswer(int select);
void selectAnswer1();
void selectAnswer2();
void selectAnswer3();
void selectAnswer4();
void submitButtonClick();
void backMenu1();
void drawResultLayout(int score,int time,int numberQuestion);
void drawHint();
void resetScreen2();
void nextButtonClick2();
void backButtonClick2();

static gboolean
_label_update(gpointer data)
{
    GtkLabel *label = (GtkLabel*)data;
    char buf[256];
    int p;
    int g;
    sec_expired++;
    p = sec_expired/60;
    g = sec_expired%60;
    memset(&buf, 0x0, 256);
    if(p<10&&g<10){
    	snprintf(buf, 255, "Th\u1eddi Gian: 0%d:0%d",p,g);
    }
    else if(p>=10&&g<10){
    	snprintf(buf, 255, "Th\u1eddi Gian: %d:0%d",p,g);
    }
    else if(p<10&&g>=10){
    	snprintf(buf, 255, "Th\u1eddi Gian: 0%d:%d",p,g);
    }
    else if(p>=10&&g>=10){
    	snprintf(buf, 255, "Th\u1eddi Gian: %d:%d",p,g);
    }
    if(RUN ==TRUE) gtk_label_set_label(label, buf);
    if(sec_expired>=maxtime) {
    	RUN =  FALSE;
    	submitButtonClick();
    
    }
    return RUN;
}

void writeUserscore(int select,int score){
	
	int score1,score2,score3,score4,score5,score6;
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
	
	FILE*fp;
	char *buff;
	buff = (char*)calloc(50,sizeof(char));
	fp =fopen("diemso.txt","w");
	if(select ==1) sprintf(buff,"%d %d %d %d %d %d",score,score2,score3,score4,score5,score6);
	else if(select == 2) sprintf(buff,"%d %d %d %d %d %d",score1,score,score3,score4,score5,score6);
	else if(select == 3) sprintf(buff,"%d %d %d %d %d %d",score1,score2,score,score4,score5,score6);
	else if(select == 4) sprintf(buff,"%d %d %d %d %d %d",score1,score2,score3,score,score5,score6);
	else if(select == 5) sprintf(buff,"%d %d %d %d %d %d",score1,score2,score3,score4,score,score6);
	else if(select == 6) sprintf(buff,"%d %d %d %d %d %d",score1,score2,score3,score4,score5,score);
	fputs(buff,fp);
	fclose(fp);
	free(buff);
}

void drawQuestion(GtkWidget *window){
    GtkWidget *button3;
    GtkWidget *backButton;
    GtkWidget *submitButton;
    GtkWidget *nextButton;
    GtkWidget *image1;
    GtkWidget *label;
    //gtk_container_remove(GTK_CONTAINER (window), layout2);
    RUN = TRUE;
    
    
    sprintf(buff,"C\u00e2u S\u1ed1:%d/%d",count+1,numberQuestion);
    layout2 = gtk_layout_new(NULL, NULL);
    image1 = gtk_image_new_from_file("Pictures/Backgrounds/background2.png");
        gtk_layout_put(GTK_LAYOUT(layout2), image1, 0, 0);
        gtk_widget_set_size_request(image1, 1103, 620);
        
    questionButton = gtk_button_new_with_label(questionList1->question[list[count]].q);
        gtk_layout_put(GTK_LAYOUT(layout2), questionButton, 160, 40);
        gtk_widget_set_size_request(questionButton, 780, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(questionButton),"question");
        
    button3 = gtk_button_new_with_label("");
        gtk_layout_put(GTK_LAYOUT(layout2), button3, 950, 40);
        gtk_widget_set_size_request(button3, 130, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(button3),"noselect");
    NumberQuestionFrame = gtk_button_new_with_label(buff);
        gtk_layout_put(GTK_LAYOUT(layout2), NumberQuestionFrame, 20, 40);
        gtk_widget_set_size_request(NumberQuestionFrame, 130, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(NumberQuestionFrame),"noselect");


    label = gtk_label_new("Th\u1eddi Gian 00:00");	    
        gtk_layout_put(GTK_LAYOUT(layout2), label, 950, 40);
        gtk_widget_set_size_request(label, 130, 80);
        

    buttonA = gtk_button_new_with_label(questionList1->question[list[count]].answer1);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonA, 620, 160);
        gtk_widget_set_size_request(buttonA, 460, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
        
    buttonB = gtk_button_new_with_label(questionList1->question[list[count]].answer2);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonB, 620, 250);
        gtk_widget_set_size_request(buttonB, 460, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
        
    buttonC = gtk_button_new_with_label(questionList1->question[list[count]].answer3);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonC, 620, 340);
        gtk_widget_set_size_request(buttonC, 460, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
        
    buttonD = gtk_button_new_with_label(questionList1->question[list[count]].answer4);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonD, 620, 430);
        gtk_widget_set_size_request(buttonD, 460, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");

    nextButton = gtk_button_new_with_label("Ti\u00ea\u0301p theo");
        gtk_layout_put(GTK_LAYOUT(layout2), nextButton, 930, 550);
        gtk_widget_set_size_request(nextButton, 150 , 50);
        gtk_style_context_add_class(gtk_widget_get_style_context(nextButton),"noselect");

    backButton = gtk_button_new_with_label("L\u00f9i L\u1ea1i");
        gtk_layout_put(GTK_LAYOUT(layout2), backButton, 10, 550);
        gtk_widget_set_size_request(backButton, 150, 50);
        gtk_style_context_add_class(gtk_widget_get_style_context(backButton),"noselect");

    submitButton = gtk_button_new_with_label("N\u1ed9p B\u00e0i");
        gtk_layout_put(GTK_LAYOUT(layout2), submitButton, 475, 550);
        gtk_widget_set_size_request(submitButton, 150, 50);
        gtk_style_context_add_class(gtk_widget_get_style_context(submitButton),"submitbutton");
	
    questionImage = gtk_image_new_from_file(questionList1->question[list[count]].imagelink);
        gtk_layout_put(GTK_LAYOUT(layout2), questionImage, 10, 160);
        

        gtk_container_add(GTK_CONTAINER(window), layout2);
        gtk_widget_show(layout2);
        gtk_widget_show_all(window);
       
        g_timeout_add_seconds(1, _label_update, label);
        g_signal_connect(G_OBJECT(nextButton),"clicked",G_CALLBACK(nextButtonClick),NULL);   
        g_signal_connect(G_OBJECT(backButton),"clicked",G_CALLBACK(backButtonClick),NULL);
        g_signal_connect(G_OBJECT(submitButton),"clicked",G_CALLBACK(submitButtonClick),NULL);
        
        g_signal_connect(G_OBJECT(buttonA),"clicked",G_CALLBACK(selectAnswer1),NULL);
        g_signal_connect(G_OBJECT(buttonB),"clicked",G_CALLBACK(selectAnswer2),NULL);
        g_signal_connect(G_OBJECT(buttonC),"clicked",G_CALLBACK(selectAnswer3),NULL);
        g_signal_connect(G_OBJECT(buttonD),"clicked",G_CALLBACK(selectAnswer4),NULL);           
}

void deleteQuestion(GtkWidget *window){
    gtk_container_remove(GTK_CONTAINER (window), layout2);	
}
// event

void resetScreen(){
	sprintf(buff,"C\u00e2u S\u1ed1:%d/%d",count+1,numberQuestion);
    	gtk_widget_destroy(questionImage);
    	gtk_widget_destroy(buttonA);
    	gtk_widget_destroy(buttonB);
    	gtk_widget_destroy(buttonC);
    	gtk_widget_destroy(buttonD);
    	gtk_button_set_label(GTK_BUTTON(questionButton),questionList1->question[list[count]].q);
    	gtk_button_set_label(GTK_BUTTON(NumberQuestionFrame),buff);
    	
    	
    	buttonA = gtk_button_new_with_label(questionList1->question[list[count]].answer1);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonA, 620, 160);
        gtk_widget_set_size_request(buttonA, 460, 80);
  
    	buttonB = gtk_button_new_with_label(questionList1->question[list[count]].answer2);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonB, 620, 250);
        gtk_widget_set_size_request(buttonB, 460, 80);
        
    	buttonC = gtk_button_new_with_label(questionList1->question[list[count]].answer3);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonC, 620, 340);
        gtk_widget_set_size_request(buttonC, 460, 80);
        
    	buttonD = gtk_button_new_with_label(questionList1->question[list[count]].answer4);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonD, 620, 430);
        gtk_widget_set_size_request(buttonD, 460, 80);
    	
    	questionImage = gtk_image_new_from_file(questionList1->question[list[count]].imagelink);
        	gtk_layout_put(GTK_LAYOUT(layout2), questionImage, 10, 160);
        
        if(results[count] == 1){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"select");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");
		 
	}
	else if(results[count] == 2){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"select");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");
	}
	else if(results[count] == 3){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"select");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");
	}
	else if(results[count] == 4){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"select");
	}
	else{
		gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
		gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
		gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
		gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");
	}
	g_signal_connect(G_OBJECT(buttonA),"clicked",G_CALLBACK(selectAnswer1),NULL);
        g_signal_connect(G_OBJECT(buttonB),"clicked",G_CALLBACK(selectAnswer2),NULL);
        g_signal_connect(G_OBJECT(buttonC),"clicked",G_CALLBACK(selectAnswer3),NULL);
        g_signal_connect(G_OBJECT(buttonD),"clicked",G_CALLBACK(selectAnswer4),NULL);
	
    	gtk_widget_show_all(w2);

}

void nextButtonClick(){
    if(count<numberQuestion-1){
    	count++;
    	resetScreen();
    	
    }
}
void backButtonClick(){
    if(count>0){
    	count--;
    	resetScreen();
    }
}

void selectAnswer(int select){
	
	results[count] = select;
	
        gtk_widget_destroy(buttonA);
    	gtk_widget_destroy(buttonB);
    	gtk_widget_destroy(buttonC);
    	gtk_widget_destroy(buttonD);
    	
    	buttonA = gtk_button_new_with_label(questionList1->question[list[count]].answer1);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonA, 620, 160);
        gtk_widget_set_size_request(buttonA, 460, 80);
  
    	buttonB = gtk_button_new_with_label(questionList1->question[list[count]].answer2);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonB, 620, 250);
        gtk_widget_set_size_request(buttonB, 460, 80);
        
    	buttonC = gtk_button_new_with_label(questionList1->question[list[count]].answer3);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonC, 620, 340);
        gtk_widget_set_size_request(buttonC, 460, 80);
        
    	buttonD = gtk_button_new_with_label(questionList1->question[list[count]].answer4);
        gtk_layout_put(GTK_LAYOUT(layout2), buttonD, 620, 430);
        gtk_widget_set_size_request(buttonD, 460, 80);
	
	if(select == 1){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"select");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");
		 
	}
	if(select == 2){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"select");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");
	}
	if(select == 3){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"select");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"noselect");
	}
	if(select == 4){
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonA),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonB),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonC),"noselect");
		 gtk_style_context_add_class(gtk_widget_get_style_context(buttonD),"select");
	}
	g_signal_connect(G_OBJECT(buttonA),"clicked",G_CALLBACK(selectAnswer1),NULL);
        g_signal_connect(G_OBJECT(buttonB),"clicked",G_CALLBACK(selectAnswer2),NULL);
        g_signal_connect(G_OBJECT(buttonC),"clicked",G_CALLBACK(selectAnswer3),NULL);
        g_signal_connect(G_OBJECT(buttonD),"clicked",G_CALLBACK(selectAnswer4),NULL);
	gtk_widget_show_all(w2);
}
void selectAnswer1(){
	selectAnswer(1);
}

void selectAnswer2(){
	selectAnswer(2);
}
void selectAnswer3(){
	selectAnswer(3);
}
void selectAnswer4(){
	selectAnswer(4);
}

void submitButtonClick(){
	int score = 0;
	RUN  = FALSE;
	for(int i=0;i<numberQuestion;i++) {if(questionList1->question[list[i]].correctAnswer==results[i]) score++;}
	if(UserSelect2 != 0) writeUserscore(UserSelect2,score);
	if(connectserver==1){
		RequestForm data_sent;
     		int bytes_sent;
		data_sent.number1 = score; // Điểm Số 
                data_sent.number2 = sec_expired; // Thời Gian
                data_sent.number3 = roomID5;
                strcpy(data_sent.string1,UserName5);
                data_sent.type = FINISHEXAM;
                bytes_sent = send(clientsocket_5, (void *) &data_sent, sizeof(data_sent), 0); 
                    if (bytes_sent < 0) perror("\nError: ");
	}
	deleteQuestion(w2);
	drawResultLayout(score,sec_expired,numberQuestion);
}
char str[50];
void drawResultLayout(int score,int time,int numberQuestion){
    GtkWidget *detailButton;
    GtkWidget *backMenuButton;
    GtkWidget *backgroundFrame;
    GtkWidget *scoreFrame;
    GtkWidget *timeFrame;   
    //gtk_container_remove(GTK_CONTAINER (window), layout);// clear screen
    layout2 = gtk_layout_new(NULL, NULL);
    sprintf(str,"Th\u1eddi Gian : %d'%d\" \t\t\t\t\t \u0110i\u1ec3m : %d/%d",time/60,time%60,score,numberQuestion);
    
    backgroundFrame = gtk_image_new_from_file("Pictures/Backgrounds/background2.png");
    	gtk_layout_put(GTK_LAYOUT(layout2), backgroundFrame, 0, 0);
     
    timeFrame = gtk_button_new_with_label("\u0110\u00e3 Ho\u00e0n Th\u00e0nh B\u00e0i Thi");
	gtk_layout_put(GTK_LAYOUT(layout2), timeFrame, 200, 25);
    	gtk_widget_set_size_request(timeFrame, 700, 100);
    	gtk_style_context_add_class(gtk_widget_get_style_context(timeFrame),"Item_Save");

    scoreFrame = gtk_button_new_with_label(str);
        gtk_layout_put(GTK_LAYOUT(layout2), scoreFrame, 50, 150);
        gtk_widget_set_size_request(scoreFrame, 1000, 375);
        gtk_style_context_add_class(gtk_widget_get_style_context(scoreFrame),"resultsFrame");
        
    
    backMenuButton = gtk_button_new_with_label("Tr\u01a1\u0309 la\u0323i menu ");
    	gtk_layout_put(GTK_LAYOUT(layout2), backMenuButton,200, 550);
    	gtk_widget_set_size_request(backMenuButton, 200, 50);
    	gtk_style_context_add_class(gtk_widget_get_style_context(backMenuButton),"noselect");

    detailButton = gtk_button_new_with_label("Xem chi ti\u00ea\u0301t ba\u0300i thi");
  	gtk_layout_put(GTK_LAYOUT(layout2), detailButton, 700, 550);
    	gtk_widget_set_size_request(detailButton, 200, 50);
    	gtk_style_context_add_class(gtk_widget_get_style_context(detailButton),"noselect");

    gtk_container_add(GTK_CONTAINER(w2), layout2);
        gtk_widget_show(layout2);
    g_signal_connect(G_OBJECT(backMenuButton),"clicked",G_CALLBACK(backMenu1),NULL);
    g_signal_connect(G_OBJECT(detailButton),"clicked",G_CALLBACK(drawHint),NULL);
    
    gtk_widget_show_all(w2);
}
void deleteResultLayout(){
	gtk_container_remove(GTK_CONTAINER (w2), layout2);
}

void backMenu1(){
	deleteResultLayout();
	if(connectserver == 1){
		disconnectServer(clientsocket_5);
		connectserver =0;
	}
	drawMenu(w2);
}

void startExamFunc(GtkWidget *window,int select){//offline
	w2 = window;
	UserSelect2 = select;
	sec_expired = 0;
	maxtime = 100000;
	numberQuestion =10;
	questionList1  = importQuestionList();
	for(int i=0;i<numberQuestion;i++) list[i]=i;
	count = 0;
	for(int i=0;i<numberQuestion;i++) results[i]=0;
	drawQuestion(window);
}

void startONLExamFunc(GtkWidget *window,int socket,int roomID,char Name[50]){//online
	w2 = window;
	UserSelect2 = 0;
	createRoomResponse *startexamresponse;
	startexamresponse = (createRoomResponse *)calloc(1,sizeof(createRoomResponse));
	RequestForm data_sent;
	int bytes_sent;
	data_sent.type =STARTEXAM;
	data_sent.number1 = roomID;
	bytes_sent = send(socket, (void *) &data_sent, sizeof(data_sent), 0); 
	    if (bytes_sent < 0) perror("\nError: ");
	bytes_sent = recv(socket, (void *) startexamresponse, sizeof(createRoomResponse), 0); 
	    if (bytes_sent < 0) perror("\nError: ");
	w2 = window;
	sec_expired = 0;
	connectserver =1;
	maxtime = startexamresponse->time;
	numberQuestion =startexamresponse->numberQues;
	questionList1  = importQuestionList();
	roomID5 = roomID;
	g_print("roomID-start: %d\n",roomID5);
	clientsocket_5=socket;
	strcpy(UserName5,Name);
	for(int i=0;i<numberQuestion;i++) {
	
		if(startexamresponse->question[i]<0)list[i]=(startexamresponse->question[i]*(-1))%10;
		else list[i]=startexamresponse->question[i]%10;
		g_print("%d-%d\n",startexamresponse->question[i],list[i]);
	}
	count = 0;
	for(int i=0;i<numberQuestion;i++) results[i]=0;
	drawQuestion(window);
}

// Hint

    GtkWidget *corretAnswerFrame;
    GtkWidget *resultFrame;
    GtkWidget *HintFrame;
//-------------------------------------------------------------------------------------------------
// 1. Ve Giao Dien
void drawHint(){
    GtkWidget *nextButton;
    GtkWidget *backButton;
    GtkWidget *BackMenuButton;
    GtkWidget *background;
    char  corretAnswer[1024];
    count =0;
    
    deleteResultLayout();
    layout2 = gtk_layout_new(NULL, NULL);

    if(questionList1->question[list[count]].correctAnswer==1) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer1);
    if(questionList1->question[list[count]].correctAnswer==2) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer2);
    if(questionList1->question[list[count]].correctAnswer==3) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer3);
    if(questionList1->question[list[count]].correctAnswer==4) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer4);


    background = gtk_image_new_from_file("Pictures/Backgrounds/background2.png");
        gtk_layout_put(GTK_LAYOUT(layout2), background, 0, 0);
        gtk_widget_set_size_request(background, 1103, 620);
        
    questionButton = gtk_button_new_with_label(questionList1->question[list[count]].q);
        gtk_layout_put(GTK_LAYOUT(layout2), questionButton, 160, 40);
        gtk_widget_set_size_request(questionButton, 780, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(questionButton),"question");

    corretAnswerFrame = gtk_button_new_with_label(corretAnswer);
        gtk_layout_put(GTK_LAYOUT(layout2), corretAnswerFrame, 620, 160);
        gtk_widget_set_size_request(corretAnswerFrame, 460, 40);
        gtk_style_context_add_class(gtk_widget_get_style_context(corretAnswerFrame),"noselect");
        
    sprintf(str,"C\u00e2u S\u1ed1:1/%d",numberQuestion);    
    NumberQuestionFrame = gtk_button_new_with_label(str);
        gtk_layout_put(GTK_LAYOUT(layout2), NumberQuestionFrame, 20, 40);
        gtk_widget_set_size_request(NumberQuestionFrame, 130, 80);
        gtk_style_context_add_class(gtk_widget_get_style_context(NumberQuestionFrame),"noselect");

    if(results[count]==questionList1->question[list[count]].correctAnswer){
        resultFrame = gtk_button_new_with_label("ĐÚNG");
            gtk_layout_put(GTK_LAYOUT(layout2), resultFrame, 950, 40);
            gtk_widget_set_size_request(resultFrame, 130, 80);
            gtk_style_context_add_class(gtk_widget_get_style_context(resultFrame),"true");
    } 
    else{
        resultFrame = gtk_button_new_with_label("SAI");
            gtk_layout_put(GTK_LAYOUT(layout2), resultFrame, 950, 40);
            gtk_widget_set_size_request(resultFrame, 130, 80);
            gtk_style_context_add_class(gtk_widget_get_style_context(resultFrame),"false");
    } 
    HintFrame = gtk_button_new_with_label(questionList1->question[list[count]].hint);
        gtk_layout_put(GTK_LAYOUT(layout2), HintFrame, 620, 210);
        gtk_widget_set_size_request(HintFrame, 460, 300);
        gtk_style_context_add_class(gtk_widget_get_style_context(HintFrame),"noselect");

    nextButton = gtk_button_new_with_label("Ti\u00ea\u0301p theo");
        gtk_layout_put(GTK_LAYOUT(layout2), nextButton, 930, 550);
        gtk_widget_set_size_request(nextButton, 150 , 50);
        gtk_style_context_add_class(gtk_widget_get_style_context(nextButton),"noselect");

    backButton = gtk_button_new_with_label("L\u00f9i L\u1ea1i");
        gtk_layout_put(GTK_LAYOUT(layout2), backButton, 10, 550);
        gtk_widget_set_size_request(backButton, 150, 50);
        gtk_style_context_add_class(gtk_widget_get_style_context(backButton),"noselect");

    BackMenuButton = gtk_button_new_with_label("Tr\u1edf V\u1ec1 Menu");
        gtk_layout_put(GTK_LAYOUT(layout2), BackMenuButton, 475, 550);
        gtk_widget_set_size_request(BackMenuButton, 150, 50);
        gtk_style_context_add_class(gtk_widget_get_style_context(BackMenuButton),"noselect");

    questionImage = gtk_image_new_from_file(questionList1->question[list[count]].imagelink);
        gtk_layout_put(GTK_LAYOUT(layout2), questionImage, 10, 160);

        gtk_container_add(GTK_CONTAINER(w2), layout2);
        gtk_widget_show(layout2);
        g_signal_connect(G_OBJECT(BackMenuButton),"clicked",G_CALLBACK(backMenu1),NULL);
        g_signal_connect(G_OBJECT(nextButton),"clicked",G_CALLBACK(nextButtonClick2),NULL);
        g_signal_connect(G_OBJECT(backButton),"clicked",G_CALLBACK(backButtonClick2),NULL);


        gtk_widget_show_all(w2);
}
//-------------------------------------------------------------------------------------------------
void resetScreen2(){
	char corretAnswer[1024];
	sprintf(buff,"C\u00e2u S\u1ed1:%d/%d",count+1,numberQuestion);
	if(questionList1->question[list[count]].correctAnswer==1) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer1);
        if(questionList1->question[list[count]].correctAnswer==2) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer2);
        if(questionList1->question[list[count]].correctAnswer==3) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer3);
        if(questionList1->question[list[count]].correctAnswer==4) sprintf(corretAnswer,"\u0110\u00e1p \u00c1n \u0110\u00fang: %s",questionList1->question[list[count]].answer4);
        
        
        
        gtk_button_set_label(GTK_BUTTON(questionButton),questionList1->question[list[count]].q);
        gtk_button_set_label(GTK_BUTTON(NumberQuestionFrame),buff);

        gtk_widget_destroy(questionImage);
        questionImage = gtk_image_new_from_file(questionList1->question[list[count]].imagelink);
        	gtk_layout_put(GTK_LAYOUT(layout2), questionImage, 10, 160);
        
        gtk_button_set_label(GTK_BUTTON(HintFrame),questionList1->question[list[count]].hint);
        gtk_button_set_label(GTK_BUTTON(corretAnswerFrame),corretAnswer);
        gtk_widget_destroy(resultFrame);
        if(results[count]==questionList1->question[list[count]].correctAnswer){
            resultFrame = gtk_button_new_with_label("\u0110\u00daNG");
                gtk_layout_put(GTK_LAYOUT(layout2), resultFrame, 950, 40);
                gtk_widget_set_size_request(resultFrame, 130, 80);
                gtk_style_context_add_class(gtk_widget_get_style_context(resultFrame),"true");
        } 
        else{
            resultFrame = gtk_button_new_with_label("SAI");
                gtk_layout_put(GTK_LAYOUT(layout2), resultFrame, 950, 40);
                gtk_widget_set_size_request(resultFrame, 130, 80);
                gtk_style_context_add_class(gtk_widget_get_style_context(resultFrame),"false");
        } 
         gtk_widget_show_all(w2);
}
// 2. Xu ly su kien
void nextButtonClick2(){
    
    if(count<numberQuestion-1){
        count++;
        resetScreen2();
    }

}

void backButtonClick2(){
    
    if(count>0){
        count--;
        resetScreen2();
    }

}
