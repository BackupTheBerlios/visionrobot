CPP  = g++ -D__DEBUG__
CC   = gcc -D__DEBUG__
RES  = 
OBJ  = pipeline.o $(RES)
LINKOBJ  = pipeline.o $(RES)
LIBS =  -L"/usr/lib" -g3
INCS =  -I"/usr/include" -I"/usr/include/gnome-xml"
BIN  = libpipeline.a
CFLAGS = $(INCS)   -g3

.PHONY: all all-before all-after clean clean-custom

all: all-before libpipeline.a all-after 


clean: clean-custom
	rm -f $(OBJ) $(BIN)

$(BIN): $(LINKOBJ)
	ar r $(BIN) $(LINKOBJ)
	ranlib $(BIN)

pipeline.o: pipeline.c
	$(CC) -c pipeline.c -o pipeline.o $(CFLAGS)
