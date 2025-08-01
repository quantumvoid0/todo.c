#include <gtk/gtk.h>


typedef struct {
    GtkEntry *entry;
    GtkLabel *text_view;
} Widgets;

void on_clear_clicked(GtkButton *button, gpointer user_data) {
    Widgets *w = (Widgets *)user_data;
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(w->entry);
    const char *text = gtk_entry_buffer_get_text(buffer);

    FILE *fptr = fopen("saved.txt", "w");
    if (fptr) {
        fprintf(fptr, "%s\n", "");
        fclose(fptr);
    }

    gtk_entry_buffer_set_text(buffer, "", 0);

    fptr = fopen("saved.txt", "r");
    if (fptr) {
        fseek(fptr, 0, SEEK_END);
        long size = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);

        char *file_content = g_malloc(size + 1);
        fread(file_content, 1, size, fptr);
        file_content[size] = '\0';
        fclose(fptr);

        gtk_label_set_text(w->text_view, file_content);

        g_free(file_content);
    }
}

void on_add_clicked(GtkButton *button, gpointer user_data) {
    Widgets *w = (Widgets *)user_data;
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(w->entry);
    const char *text = gtk_entry_buffer_get_text(buffer);

    FILE *fptr = fopen("saved.txt", "a");
    if (fptr) {
        fprintf(fptr, "%s\n", text);
        fclose(fptr);
    }

    gtk_entry_buffer_set_text(buffer, "", 0);

    fptr = fopen("saved.txt", "r");
    if (fptr) {
        fseek(fptr, 0, SEEK_END);
        long size = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);

        char *file_content = g_malloc(size + 1);
        fread(file_content, 1, size, fptr);
        file_content[size] = '\0';
        fclose(fptr);

        gtk_label_set_text(w->text_view, file_content);

        g_free(file_content);
    }
}

void update_label_from_file(GtkLabel *label) {
    FILE *fptr = fopen("saved.txt", "r");
    if (fptr) {
        fseek(fptr, 0, SEEK_END);
        long size = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);

        char *file_content = g_malloc(size + 1);
        fread(file_content, 1, size, fptr);
        file_content[size] = '\0';
        fclose(fptr);

        gtk_label_set_text(label, file_content);

        g_free(file_content);
    } else {
        gtk_label_set_text(label, ""); 
    }
}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window, *grid, *button, *entry, *text_view, *c_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "window");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);



	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, "style.css");
	gtk_style_context_add_provider_for_display(
	    gtk_widget_get_display(window),
	    GTK_STYLE_PROVIDER(provider),
	    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_object_unref(provider);



    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);	

    entry = gtk_entry_new();
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 1, 1);

    button = gtk_button_new_with_label("Add");
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

	c_button = gtk_button_new_with_label("Clear");
	gtk_grid_attach(GTK_GRID(grid), c_button, 2, 0, 1, 1);

    text_view = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(text_view), 0.0); 
	gtk_widget_set_hexpand(text_view, TRUE);
    gtk_grid_attach(GTK_GRID(grid), text_view, 0, 1, 1, 1);
    update_label_from_file(GTK_LABEL(text_view));

	Widgets *w = g_malloc(sizeof(Widgets));
    w->entry = GTK_ENTRY(entry);
    w->text_view = GTK_LABEL(text_view);

	g_signal_connect(button, "clicked", G_CALLBACK(on_add_clicked), w);
	g_signal_connect(c_button, "clicked", G_CALLBACK(on_clear_clicked), w);

	gtk_widget_add_css_class(text_view, "todo-label");



    gtk_window_present(GTK_WINDOW(window));
	g_object_weak_ref(G_OBJECT(window), (GWeakNotify)g_free, w);

}


int main (int argc , char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.todo.app", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);

	return status;
}
