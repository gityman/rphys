CC=gcc
PLATFORM := $(shell uname)
OUTPUT = rphys
ifeq  ($(PLATFORM),Darwin)
	LFLAGS=-I/usr/local/include -L/usr/local/lib -lglfw -framework OpenGL
else
	LFLAGS=-I/usr/local/include -L/usr/local/lib -lglfw -lGL
endif

$(OUTPUT): render.o main.o
	$(CC) -o $(OUTPUT) render.o main.o $(LFLAGS)
main.o: main.c
	$(CC) -c -o main.o main.c $(LFLAGS)
render.o: render.h render.c
	$(CC) -c -o render.o render.c $(LFLAGS)
clean:
	rm *.o $(OUTPUT)
