pkglib_LTLIBRARIES = librobot.la

AM_CFLAGS=$(GLIB_CFLAGS) -I../libpipeline
librobot_la_LIBADD=$(GLIB_LIBS)
librobot_la_LDFLAGS=-module
librobot_la_SOURCES = robot.c
dist_pkgdata_DATA = robot.lua
