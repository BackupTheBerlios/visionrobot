#include "funciones.h"
#include <gtk/gtk.h>
#include <stdio.h>

/* This is an internally used function to create pixmaps. */
GdkPixbuf *create_pixbuf(const gchar * filename)
{
    gchar *pathname = NULL;
    GdkPixbuf *pixbuf;
    GError *error = NULL;

    if (!filename || !filename[0])
	return NULL;

    pathname = (gchar*)filename;

    if (!pathname) {
	g_warning("Couldn't find pixmap file: %s", filename);
	return NULL;
    }

    pixbuf = gdk_pixbuf_new_from_file(pathname, &error);
    if (!pixbuf) {
	fprintf(stderr, "Failed to load pixbuf file: %s: %s\n",
		pathname, error->message);
	g_error_free(error);
    }
    //g_free(pathname);
    return pixbuf;
}
