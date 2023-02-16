#ifndef __MENU_H__
#define __MENU_H__
#include "message.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
void drawMenu(GtkWidget *window);
void Menu(GtkWidget *window);
void deleteMenu(GtkWidget *window);
void LuyenTapButtonClick();
Exam * importQuestionList();
void VaoThiButtonClick();


#endif
