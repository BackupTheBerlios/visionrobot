GTK_CFLAGS = -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/pango-1.0 -I/usr/X11R6/include -I/usr/include/freetype2 -I/usr/include/freetype2/config -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include  
GTK_LIBS = -Wl,--export-dynamic -lgtk-x11-2.0 -latk-1.0 -lgdk-x11-2.0 -lgdk_pixbuf-2.0 -lm -lpangoxft-1.0 -lpangox-1.0 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0  
XML_CFLAGS = -I/usr/include/gnome-xml
XML_LIBS = -lxml
FLAGS = $(GTK_CFLAGS) $(XML_CFLAGS)
LIBS = $(GTK_LIBS) $(XML_LIBS)
OPTS = -Wall -g
GCC = gcc
OUTPUT = pipeline
TAGS = ctags
DOXYGEN = doxygen
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
	indent -kr *.h *.c
tags:
	$(TAGS) -e *.c

doxygen:
	$(DOXYGEN)

callbacks.o: callbacks.c
	$(GCC) -c -o callbacks.o callbacks.c $(OPTS) $(FLAGS)

main.o: main.c
	$(GCC) -c -o main.o main.c $(OPTS) $(FLAGS)

support.o: support.c
	$(GCC) -c -o support.o support.c $(OPTS) $(FLAGS)

pipeline.o: pipeline.c
	$(GCC) -c -o pipeline.o pipeline.c $(OPTS) $(FLAGS)

auxiliar.o: auxiliar.c
	$(GCC) -c -o auxiliar.o auxiliar.c $(OPTS) $(FLAGS)

interface.o: interface.c
	$(GCC) -c -o interface.o interface.c $(OPTS) $(FLAGS)

