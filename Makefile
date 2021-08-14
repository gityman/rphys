CC=gcc
PLATFORM := $(shell uname)
OUTPUT = rphys
ifeq  ($(PLATFORM),Darwin)
	LFLAGS=-I/usr/local/include -L/usr/local/lib -lglfw -framework OpenGL
else
	LFLAGS=-I/usr/local/include -L/usr/local/lib -lglfw -lGL
endif

$(OUTPUT): physics.o render.o main.o
	$(CC) -o $(OUTPUT) physics.o render.o main.o $(LFLAGS)
main.o: physics.h render.h main.c
	$(CC) -c -o main.o main.c $(LFLAGS)
render.o: physics.h render.h render.c
	$(CC) -c -o render.o render.c $(LFLAGS)
physics.o: physics.h physics.c
	$(CC) -c -o physics.o physics.c $(LFLAGS)
clean:
	rm *.o $(OUTPUT)
