OPTS=-g -Wall

all: etags salida.so filtro_gestos.so red_neuronal.so imagenes.so ventana_imagen.so pipeline

etags:	
	etags *.c

ventana_imagen.so: ventana_imagen.c
	gcc $(OPTS) -shared -o ventana_imagen.so ventana_imagen.c `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`  `pkg-config libglade-2.0 --cflags --libs`

salida.so: salida.c
	gcc $(OPTS) -shared -o salida.so salida.c `pkg-config glib-2.0 --cflags --libs` `pkg-config libglade-2.0 --cflags --libs`

filtro_gestos.so: filtro_gestos.c filtro.c
	gcc $(OPTS) -shared -o filtro_gestos.so filtro.c filtro_gestos.c `pkg-config glib-2.0 --cflags --libs`

red_neuronal.so: red.c red_neuronal.c
	gcc $(OPTS) -shared -o red_neuronal.so red.c red_neuronal.c `pkg-config glib-2.0 --cflags --libs`

imagenes.so: imagenes.c
	gcc $(OPTS) -shared -o imagenes.so imagenes.c `pkg-config glib-2.0 --cflags --libs`

#camara.so: camara.cpp Captura.cpp
#	g++ $(OPTS) -shared -o camara.so camara.cpp Captura.cpp -lstrmiids -luuid -loleaut32 -lole32 -lquartz

pipeline: main.c libpipeline.a
	gcc $(OPTS) main.c -o pipeline -L ./ -lpipeline `xml2-config --libs` `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` `pkg-config libglade-2.0 --cflags --libs`

libpipeline.a: pipeline.c
	gcc $(OPTS) -c pipeline.c -o libpipeline.a `xml2-config --cflags` `pkg-config --cflags gmodule-2.0`

clean:
	rm -rf pipeline
	rm -rf *.a
	rm -rf *.so
	rm -rf *.o
