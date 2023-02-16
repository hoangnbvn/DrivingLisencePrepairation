#ifndef __LAMBAI_H__
#define __LAMBAI_H__
#include "message.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
void drawQuestion(GtkWidget *window);
static gboolean _label_update(gpointer data);
void startExamFunc(GtkWidget *window,int select);
void nextButtonClick();
void backButtonClick();
void resetScreen();
void selectAnswer(int select);
void selectAnswer1();
void selectAnswer2();
void selectAnswer3();
void selectAnswer4();
void submitButtonClick();
void deleteQuestion();
void deleteResultLayout();
void backMenu1();
void drawResultLayout(int score,int time);
void drawHint();
void resetScreen2();
void nextButtonClick2();
void backButtonClick2();
void startONLExamFunc(GtkWidget *window,int socket,int roomID,char Name[50]);
void writeUserscore(int select,int score);
#endif
