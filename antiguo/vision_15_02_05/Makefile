OPTS=-Wall -g

all: libsalida.so libfiltro_gestos.so libred_neuronal.so libimagenes.so libventana_imagen.so libventana_parametros.so pipeline libprolog.so libgestion.so

etags:	
	etags *.c

libventana_imagen.so: ventana_imagen.c
	gcc $(OPTS) -shared -o libventana_imagen.so ventana_imagen.c `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`  `pkg-config libglade-2.0 --cflags --libs`

libventana_parametros.so: ventana_parametros.c
	gcc $(OPTS) -shared -o libventana_parametros.so ventana_parametros.c `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`  `pkg-config libglade-2.0 --cflags --libs`

libsalida.so: salida.c
	gcc $(OPTS) -shared -o libsalida.so salida.c `pkg-config glib-2.0 --cflags --libs` `pkg-config libglade-2.0 --cflags --libs`

libprolog.so: prolog.c
	gcc $(OPTS) -shared -o libprolog.so prolog.c `pkg-config glib-2.0 --cflags --libs` -I/usr/lib/pl-5.4.7/include/ -L/usr/lib/pl-5.4.7/lib/i686-linux/ -lpl -lcurses

libgestion.so: gestion.c
	gcc $(OPTS) -shared -o libgestion.so gestion.c `pkg-config glib-2.0 --cflags --libs`

libfiltro_gestos.so: filtro_gestos.c
	gcc $(OPTS) -I../include -shared -o libfiltro_gestos.so filtro_gestos.c -llua -llualib `pkg-config glib-2.0 --cflags --libs`

libred_neuronal.so: red.c red_neuronal.c
	gcc $(OPTS) -shared -o libred_neuronal.so red.c red_neuronal.c `pkg-config glib-2.0 --cflags --libs`

libimagenes.so: imagenes.c
	gcc $(OPTS) -shared -o libimagenes.so imagenes.c  `pkg-config glib-2.0 --cflags --libs` `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -lsane

pipeline: main.c libpipeline.a
	gcc $(OPTS) main.c -o pipeline -L ./ -lpipeline `xml2-config --libs` `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` `pkg-config libglade-2.0 --cflags --libs` `pkg-config --libs gmodule-2.0`

libpipeline.a: pipeline.c
	gcc $(OPTS) -c pipeline.c -o libpipeline.a `xml2-config --cflags` `pkg-config --cflags gmodule-2.0`

clean:
	rm -rf pipeline
	rm -rf *.a
	rm -rf *.so
	rm -rf *.o
