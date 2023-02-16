#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include "fields.h"  
#include <string.h>  
#include <ctype.h>     
 #include <unistd.h> 
#include <errno.h>  
#include <netdb.h>  
#include <sys/types.h>
#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/wait.h>  
#include<pthread.h>
#include "message.h"   
#include "menu.h"

//GUI   S
    GtkWidget *window;      
// 
// import css file 

static void load_css(void){ 
	GtkCssProvider *provider; 
	GdkDisplay *display;    
	GdkScreen *screen;         
	          
	const gchar *css_style_file = "style.css";   
	GFile *css_fp = g_file_new_for_path(css_style_file);
	GError *error = 0;  
	 
	provider = gtk_css_provider_new(); 
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display); 
	
	gtk_style_context_add_provider_for_screen (screen,GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_css_provider_load_from_file(provider,css_fp,&error);
	
	g_object_unref(provider);
}  
//


  
int main( void)
{
    gtk_init(NULL,NULL);
    load_css();
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_default_size(GTK_WINDOW(window), 1103, 620);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "\u00d4n Thi L\u00e1i Xe");
    
    Menu(window);
    //RoomListLayout();
    //inputUserNameView();
    //draw_Wait_Room_Room_Master_View();

    g_signal_connect_swapped(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();
    return 0;
}
