pkglib_LTLIBRARIES = libventana_parametros.la

AM_CFLAGS=$(GLIB_CFLAGS) $(GTK_CFLAGS) $(GLADE_CFLAGS) -I../libpipeline -I../filtro_gestos
libventana_parametros_la_LIBADD=$(GLIB_LIBS) $(GTK_LIBS) $(GLADE_LIBS)
libventana_parametros_la_LDFLAGS=-module
libventana_parametros_la_SOURCES = ventana_parametros.c
dist_pkgdata_DATA = ventana_parametros.glade
