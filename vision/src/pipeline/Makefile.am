bin_PROGRAMS = pipeline

AM_CFLAGS=$(GLIB_CFLAGS) $(GLADE_CFLAGS) $(GTK_CFLAGS) -I../libpipeline
pipeline_LDADD=$(GLIB_LIBS) $(GLADE_LIBS) $(GTK_LIBS) -L../libpipeline -lpipeline
pipeline_SOURCES = main.c
dist_pkgdata_DATA = ventana_pipeline.glade vision.xml nulo.xml
