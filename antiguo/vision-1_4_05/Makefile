OPTS=-Wall -g

all: etags libsalida.so libfiltro_gestos.so libred_neuronal.so libimagenes.so libventana_imagen.so libventana_parametros.so pipeline libfiltro_lua.so

etags:	
	etags *.c

libventana_imagen.so: ventana_imagen.c
	gcc $(OPTS) -shared -o libventana_imagen.so ventana_imagen.c `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`  `pkg-config libglade-2.0 --cflags --libs`

libventana_parametros.so: ventana_parametros.c
	gcc $(OPTS) -shared -o libventana_parametros.so ventana_parametros.c `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`  `pkg-config libglade-2.0 --cflags --libs`

libsalida.so: salida.c
	gcc $(OPTS) -shared -o libsalida.so salida.c `pkg-config glib-2.0 --cflags --libs` `pkg-config libglade-2.0 --cflags --libs`

libfiltro_gestos.so: filtro_gestos.c filtro.c
	gcc $(OPTS) -I../include -shared -o libfiltro_gestos.so filtro.c filtro_gestos.c -L../lib -llua -llualib `pkg-config glib-2.0 --cflags --libs` 

#libfiltro_lua.so: filtro_lua.c
#	gcc $(OPTS) -I../include -shared -o libfiltro_lua.so filtro_lua.c -L../lib -llua

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
