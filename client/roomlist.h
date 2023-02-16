#ifndef __ROOMLIST_H__
#define __ROOMLIST_H__
#include "message.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
void RoomListLayout(GtkWidget *window,int select);
void resetScreen3(int select);
void nextButtonClick3();
void backButtonClick3();
void RoomList(GtkWidget *window,int socket,int n);
void backtoVaoThi();
void inputUserNameView(int select,GtkWidget *window);
void inputUserNameView1();
void inputUserNameView2();
void inputUserNameView3();
void JoinRoomClick();
void draw_Wait_Room_View_2(GtkWidget * window, char *numberUserString);
void showResultClick(int select);
void showResultClick1();
void showResultClick2();
void showResultClick3();
#endif
