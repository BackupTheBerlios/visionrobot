OPTS=-g

all: camara.so salida.so filtro_gestos.so red_neuronal.so pipeline

salida.so: salida.c
	gcc $(OPTS) -shared -o salida.so salida.c

filtro_gestos.so: filtro_gestos.c filtro.c
	gcc $(OPTS) -shared -o filtro_gestos.so filtro.c filtro_gestos.c

red_neuronal.so: red.c red_neuronal.c
	gcc $(OPTS) -shared -o red_neuronal.so red.c red_neuronal.c

camara.so: camara.cpp #Captura.cpp Captura.cpp
	g++ $(OPTS) -shared -o camara.so camara.cpp #  -lstrmiids -luuid -loleaut32 -lole32 -lquartz

pipeline: main.c libpipeline.a
	gcc $(OPTS) main.c -o pipeline -L ./ -lpipeline -lxml2 -ldl

libpipeline.a: pipeline.c
	gcc $(OPTS) -c pipeline.c -o libpipeline.a `xml2-config --cflags`

clean:
	rm -rf pipeline
	rm -rf *.so
	rm -rf *.o
