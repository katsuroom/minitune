LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
FLAGS = -Wl,--subsystem,windows -O3
SRCS = src/*.c src/vis/*.c lib/libwinsane.o

all:
	gcc -Iinclude $(SRCS) -Llib $(LIBS) -o ./main
	
run:
	./main

release:
	gcc -Iinclude $(FLAGS) $(SRCS) -Llib $(LIBS) -o ./minitune
