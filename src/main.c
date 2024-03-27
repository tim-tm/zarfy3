#include "screens.h"
#include <gtk/gtk.h>

typedef struct _button_data_ {
    screen *scr;
} button_data;

static gint width = 700;
static gint height = 400;
static screen *screens;
static button_data *current_data;

static GtkWidget *window;
static GtkWidget *parent_box;
static GtkWidget *monitors_box;
static GtkWidget *settings_box_wrapper;
static GtkWidget *settings_box;
static GtkWidget *name_label;
static GtkWidget *resolution_combo_box;
static GtkWidget *apply_btn;

static void button_clicked(GtkWidget *widget, gpointer user_data) {
    button_data *data = (button_data*)user_data;
    if (data == NULL || data->scr == NULL) {
        g_error("Cannot read button data!");
    }

    gtk_label_set_text(GTK_LABEL(name_label), data->scr->name);

    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(resolution_combo_box));
    mode *m_itr = data->scr->modes;
    while (m_itr != NULL) {
        char text[256] = {0};
        snprintf(text, 256, "%s %.02f", m_itr->name, m_itr->refresh_rate);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(resolution_combo_box), text);
        m_itr = m_itr->next;
    }
    current_data = data;
}

static void apply_clicked(GtkWidget *widget, gpointer user_data) {
    char *selected = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(resolution_combo_box));
    if (current_data == NULL || selected == NULL) {
        GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Please select a device and resolution");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    const char *selected_res = strtok(selected, " ");
    if (selected_res == NULL) {
        GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Couldn't find resolution");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    mode *selected_mode = NULL;
    screen *selected_screen = NULL;

    screen *itr = screens;
    while (itr != NULL) {
        mode *m_itr = itr->modes;
        while (m_itr != NULL) {
            if (strncmp(m_itr->name, selected_res, 256) == 0) {
                selected_mode = m_itr;
                selected_screen = itr;
                break;
            }
            m_itr = m_itr->next;
        }
        itr = itr->next;
    }

    if (selected_mode == NULL || selected_screen == NULL) {
        GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Couldn't find resolution");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    // TODO: Apply resolution
    /*
    selected_screen->width = selected_mode->width;
    selected_screen->height = selected_mode->height;
    screen_apply(selected_screen);
    */
}

int main(int argc, char **argv) {
    screens = screens_init();
    if (screens == NULL) {
        fprintf(stderr, "Error finding screens.\n");
        return EXIT_FAILURE;
    }

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "zarfy3");

    parent_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_halign(parent_box, GTK_ALIGN_START);
    gtk_widget_set_valign(parent_box, GTK_ALIGN_START);

    monitors_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(monitors_box, GTK_ALIGN_START);
    gtk_widget_set_valign(monitors_box, GTK_ALIGN_START);

    settings_box_wrapper = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(settings_box_wrapper, GTK_ALIGN_START);
    gtk_widget_set_valign(settings_box_wrapper, GTK_ALIGN_START);

    settings_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_halign(settings_box, GTK_ALIGN_START);
    gtk_widget_set_valign(settings_box, GTK_ALIGN_START);

    name_label = gtk_label_new("Select a device");
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_widget_set_valign(name_label, GTK_ALIGN_START);
    
    resolution_combo_box = gtk_combo_box_text_new();
    gtk_widget_set_halign(resolution_combo_box, GTK_ALIGN_START);
    gtk_widget_set_valign(resolution_combo_box, GTK_ALIGN_START);
    
    apply_btn = gtk_button_new_with_label("Apply");
    gtk_widget_set_halign(apply_btn, GTK_ALIGN_START);
    gtk_widget_set_valign(apply_btn, GTK_ALIGN_START);
    g_signal_connect(apply_btn, "clicked", G_CALLBACK(apply_clicked), NULL);

    screen *itr = screens;
    while (itr != NULL) {
        GtkWidget *monitor_btn = gtk_button_new_with_label(itr->name);
        gtk_widget_set_halign(monitor_btn, GTK_ALIGN_START);
        gtk_widget_set_valign(monitor_btn, GTK_ALIGN_START);

        button_data *data = calloc(sizeof(button_data), 1);
        if (data == NULL) {
            fprintf(stderr, "Failed to allocate memory!\n");
            return EXIT_FAILURE;
        }
        data->scr = itr;
        g_signal_connect(monitor_btn, "clicked", G_CALLBACK(button_clicked), data);

        gtk_box_pack_start(GTK_BOX(monitors_box), monitor_btn, TRUE, TRUE, 15);
        itr = itr->next;
    }

    gtk_box_pack_start(GTK_BOX(parent_box), monitors_box, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(settings_box), name_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(settings_box), resolution_combo_box, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(settings_box), apply_btn, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(settings_box_wrapper), settings_box, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(parent_box), settings_box_wrapper, TRUE, TRUE, 15);
    gtk_container_add(GTK_CONTAINER(window), parent_box);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    screens_destroy(screens);
    return EXIT_SUCCESS;
}
