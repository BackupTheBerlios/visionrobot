RUTA = /usr
CFLAGS = -I$(RUTA)/include/gtk-2.0 -I$(RUTA)/lib/gtk-2.0/include -I$(RUTA)/include/atk-1.0 -I$(RUTA)/include/pango-1.0 -I$(RUTA)/X11R6/include -I$(RUTA)/include/freetype2 -I$(RUTA)/include/freetype2/config -I$(RUTA)/include/glib-2.0 -I$(RUTA)/lib/glib-2.0/include -I$(RUTA)/include/gnome-xml
LIBS = -Wl,--export-dynamic -lgtk-x11-2.0 -latk-1.0 -lgdk-x11-2.0 -lgdk_pixbuf-2.0 -lm -lpangoxft-1.0 -lpangox-1.0 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 -lxml
OPTS = -Wall -g
GCC = gcc
OUTPUT = pipeline
TAGS = ctags
DOXYGEN = doxygen
INDENT = indent
OBJS = main.o pipeline.o dialogo_conectar.o dialogo_guardar.o propiedades_modulo.o dialogo_abrir.o ventana_principal.o funciones.o auxiliar.o


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

main.o: main.c
	$(GCC) -c -o main.o main.c $(OPTS) $(CFLAGS)

pipeline.o: pipeline.c
	$(GCC) -c -o pipeline.o pipeline.c $(OPTS) $(CFLAGS)

dialogo_conectar.o: dialogo_conectar.c
	$(GCC) -c -o dialogo_conectar.o dialogo_conectar.c $(OPTS) $(CFLAGS)

dialogo_guardar.o: dialogo_guardar.c
	$(GCC) -c -o dialogo_guardar.o dialogo_guardar.c $(OPTS) $(CFLAGS)

propiedades_modulo.o: propiedades_modulo.c
	$(GCC) -c -o propiedades_modulo.o propiedades_modulo.c $(OPTS) $(CFLAGS)

dialogo_abrir.o: dialogo_abrir.c
	$(GCC) -c -o dialogo_abrir.o dialogo_abrir.c $(OPTS) $(CFLAGS)

ventana_principal.o: ventana_principal.c
	$(GCC) -c -o ventana_principal.o ventana_principal.c $(OPTS) $(CFLAGS)

funciones.o: funciones.c
	$(GCC) -c -o funciones.o funciones.c $(OPTS) $(CFLAGS)

auxiliar.o: auxiliar.c
	$(GCC) -c -o auxiliar.o auxiliar.c $(OPTS) $(CFLAGS)
