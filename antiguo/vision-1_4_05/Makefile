OPTS=-g
LIBS=# -ldl

all: camara.dll salida.dll filtro_gestos.dll red_neuronal.dll camara.dll pipeline.exe

salida.dll: salida.c
	gcc $(OPTS) -shared -o salida.dll salida.c

filtro_gestos.dll: filtro_gestos.c filtro.c
	gcc $(OPTS) -shared -o filtro_gestos.dll filtro.c filtro_gestos.c

red_neuronal.dll: red.c red_neuronal.c
	gcc $(OPTS) -shared -o red_neuronal.dll red.c red_neuronal.c

camara.dll: camara.cpp Captura.cpp
	g++ $(OPTS) -shared -o camara.dll camara.cpp Captura.cpp -lstrmiids -luuid -loleaut32 -lole32 -lquartz

pipeline.exe: main.c libpipeline.a
	gcc $(OPTS) main.c -o pipeline -L ./ -lpipeline -lxml2 

libpipeline.a: pipeline.c
	gcc $(OPTS) -c pipeline.c -o libpipeline.a `xml2-config --cflags`

clean:
	rm -rf pipeline.exe
	rm -rf *.dll
	rm -rf *.o
