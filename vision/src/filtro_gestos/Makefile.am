pkglib_LTLIBRARIES = libfiltro_gestos.la

AM_CFLAGS=$(GLIB_CFLAGS) -I../libpipeline
libfiltro_gestos_la_LIBADD=$(GLIB_LIBS) -llua -llualib
libfiltro_gestos_la_LDFLAGS=-module
libfiltro_gestos_la_SOURCES = filtro_gestos.c
dist_pkgdata_DATA = filtro.lua
