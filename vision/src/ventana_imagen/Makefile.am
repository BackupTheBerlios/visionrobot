pkglib_LTLIBRARIES = libventana_imagen.la

AM_CFLAGS=$(GLIB_CFLAGS) $(GTK_CFLAGS) $(GLADE_CFLAGS) -I../libpipeline -I../filtro_gestos -I../red_neuronal
libventana_imagen_la_LIBADD=$(GLIB_LIBS) $(GTK_LIBS) $(GLADE_LIBS)
libventana_imagen_la_LDFLAGS=-module
libventana_imagen_la_SOURCES = ventana_imagen.c
dist_pkgdata_DATA = ventana_imagen.glade
