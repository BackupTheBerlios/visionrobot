pkglib_LTLIBRARIES = libocr.la

AM_CFLAGS=$(GLIB_CFLAGS) -I../libpipeline -I ../filtro_gestos
libocr_la_LIBADD=$(GLIB_LIBS)
libocr_la_LDFLAGS=-module
libocr_la_SOURCES = ocr.c ocr_code.c
dist_pkgdata_DATA = base_datos
