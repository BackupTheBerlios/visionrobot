RUTA = /usr
CFLAGS = -I$(RUTA)/include/gtk-2.0 -I$(RUTA)/lib/gtk-2.0/include -I$(RUTA)/include/atk-1.0 -I$(RUTA)/include/pango-1.0 -I$(RUTA)/X11R6/include -I$(RUTA)/include/freetype2 -I$(RUTA)/include/freetype2/config -I$(RUTA)/include/glib-2.0 -I$(RUTA)/lib/glib-2.0/include -I$(RUTA)/include/gnome-xml
LIBS = -Wl,--export-dynamic -lgtk-x11-2.0 -latk-1.0 -lgdk-x11-2.0 -lgdk_pixbuf-2.0 -lm -lpangoxft-1.0 -lpangox-1.0 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 -lxml
OPTS = -Wall -g
GCC = gcc
OUTPUT = pipeline
TAGS = ctags
DOXYGEN = doxygen
INDENT = indent
OBJS = callbacks.o main.o interface.o auxiliar.o support.o pipeline.o

all:	$(OBJS) tags
	$(GCC) $(OBJS) -o $(OUTPUT) $(OPTS) $(LIBS)

clean:
	rm -f *.o
	rm -rf html
	rm -f *~
	rm -f TAGS
	rm -f $(OUTPUT)

full:	indent all tags doxygen

indent:
	$(INDENT) -kr *.h *.c
tags:
	$(TAGS) -e *.c

doxygen:
	$(DOXYGEN)

callbacks.o: callbacks.c
	$(GCC) -c -o callbacks.o callbacks.c $(OPTS) $(CFLAGS)

main.o: main.c
	$(GCC) -c -o main.o main.c $(OPTS) $(CFLAGS)

support.o: support.c
	$(GCC) -c -o support.o support.c $(OPTS) $(CFLAGS)

pipeline.o: pipeline.c
	$(GCC) -c -o pipeline.o pipeline.c $(OPTS) $(CFLAGS)

auxiliar.o: auxiliar.c
	$(GCC) -c -o auxiliar.o auxiliar.c $(OPTS) $(CFLAGS)

interface.o: interface.c
	$(GCC) -c -o interface.o interface.c $(OPTS) $(CFLAGS)

