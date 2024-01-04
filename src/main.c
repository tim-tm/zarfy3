#include "screens.h"
#include <gtk/gtk.h>

static gint width = 700;
static gint height = 400;
static screen* screens;
static screen* current_screen;

static void button_clicked(GtkWidget *widget, gpointer user_data) {
    // Get rid of usage warning
    (void)(widget);

    screen* scr = (screen*)user_data;
    if (!scr) {
        g_error("Cannot read button data!");
    }
    current_screen = scr;
    g_message("Configuring: %s", scr->name);
}

int main(int argc, char** argv) {
    screens = screens_init();
    if (screens == NULL) return 1;
    
    gtk_init(&argc, &argv);

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "zarfy3");

    GtkWidget* monitorsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(monitorsBox, GTK_ALIGN_START);
    gtk_widget_set_valign(monitorsBox, GTK_ALIGN_START);
    
    screen* itr = screens;
    while (itr != NULL) {
        GtkWidget* monitorBtn = gtk_button_new_with_label(itr->name);
        gtk_widget_set_halign(monitorBtn, GTK_ALIGN_START);
        gtk_widget_set_valign(monitorBtn, GTK_ALIGN_START);
        g_message("%p", (void*)itr);
        g_signal_connect(monitorBtn, "clicked", G_CALLBACK(button_clicked), itr);
        gtk_box_pack_start(GTK_BOX(monitorsBox), monitorBtn, TRUE, TRUE, 15);
        itr = itr->next;
        current_screen = itr;
    }
    gtk_container_add(GTK_CONTAINER(window), monitorsBox);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    screens_destroy(screens);
    return 0;
}
