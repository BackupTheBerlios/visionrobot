all:
	gcc -g -shared -o salida.dll salida.c
	gcc -g -shared -o filtro_gestos.dll filtro.c filtro_gestos.c
	gcc -g -shared -o red_neuronal.dll red.c red_neuronal.c
	gcc -g -shared -o camara.dll camara.cpp Captura.cpp
	gcc -g -c pipeline.c -o libpipeline.a
	gcc -g main.c -o pipeline.exe -L ./ -lpipeline -lxml2
