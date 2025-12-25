LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
FLAGS = -Wl,--subsystem,windows -O3

all:
	gcc -Iinclude src/*.c -Llib $(LIBS) -o ./main
	
run:
	./main

release:
	gcc -Iinclude $(FLAGS) src/*.c -Llib $(LIBS) -o ./minitune
