pkglib_LTLIBRARIES = libimagenes.la

AM_CFLAGS=$(GLIB_CFLAGS) $(GTK_CFLAGS) $(XINE_CFLAGS) -I../libpipeline -I../filtro_gestos -I../ventana_imagen
libimagenes_la_LIBADD=-lsane $(XINE_LIBS) $(GLIB_LIBS) $(GTK_LIBS)
libimagenes_la_LDFLAGS=-module
libimagenes_la_SOURCES = imagenes.c
dist_pkgdata_DATA = avanzar.xpm letra.xpm foto.xpm
