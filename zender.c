#include <gtk/gtk.h>
#include "data.h"

int main(int argc,char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder =  gtk_builder_new_from_file("zender.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder,"WindowMain"));
    gtk_builder_connect_signals(builder,NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
}

void OnWindowMainDestroy()
{
    gtk_main_quit();
}

void send_btn_clicked (GtkButton *send_btn, GtkFileChooserWidget *FileChoser)
{
    GSList * selection = gtk_file_chooser_get_files((GtkFileChooser *)FileChoser), *tmp;
    if( selection != NULL)
    for(tmp = selection;tmp != NULL; tmp = tmp->next)
    {
            g_print("%s\n",g_file_get_path((GFile *)tmp->data));
    }
    
    g_slist_free(selection);
}