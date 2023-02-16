#ifndef __CREATEROOM_H__
#define __CREATEROOM_H__
#include "message.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
void draw_Wait_Room_View_1(GtkWidget * window, char *numberUserString);
void drawSettingRoomLayout(GtkWidget * window);
void SettingRoom(GtkWidget * window,int socket);
void CreateRoom();
void changeLabel(char *numberUserString);
void BackToVaoThi2();
#endif
