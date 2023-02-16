#ifndef __SHOWRESULTSOFROOM_H__
#define __SHOWRESULTSOFROOM_H__
#include "message.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
void ResultsOfRoomLayout(GtkWidget *window,char RoomName[50]);
void ResultsOfRoom(GtkWidget *window,int socket,int roomID,char RoomName[50]);
void backtoRoomList();
#endif
