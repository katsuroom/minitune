LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
FLAGS = -Wl,--subsystem,windows
SRCS = src/*.c src/vis/*.c lib/libwinsane.o

RES = bin/resource.rc.data

all:
	gcc -Iinclude $(SRCS) -Llib $(LIBS) -o ./main
	
run:
	./main

release: bin/resource.rc.data
	gcc -Iinclude $(FLAGS) $(SRCS) $(RES) -Llib $(LIBS) -o ./minitune

%.rc.data:
	mkdir -p bin
	windres src/resource.rc -o $(RES)

clean:
	rm -rf bin *.exe