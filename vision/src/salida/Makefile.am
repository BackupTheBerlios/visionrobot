pkglib_LTLIBRARIES = libsalida.la

AM_CFLAGS=$(GLIB_CFLAGS) $(GTK_CFLAGS) $(GLADE_CFLAGS) -I../libpipeline
libsalida_la_LIBADD=$(GLIB_LIBS) $(GTK_LIBS) $(GLADE_LIBS)
libsalida_la_LDFLAGS=-module
libsalida_la_SOURCES = salida.c
dist_pkgdata_DATA = ventana_salida.glade
