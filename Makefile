AR = ar
CC = g++
LD = g++

LIBS = ./lib/libfgng-1.0.a -lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

CC_FLAGS = -Wall -c  -I./lib/ -I/usr/include/SDL2/ -O3 
LD_FLAGS = -Wall -O3 

OBJECTS = 

all: mapDummyDemo

mapDummyDemo: mapDummyDemo.o libfgng-1.0.a $(OBJECTS)
	$(LD) $(LD_FLAGS) -o $@ mapDummyDemo.o $(LIBS) $(OBJECTS)

dummyDemo: dummyDemo.o libfgng-1.0.a $(OBJECTS)
	$(LD) $(LD_FLAGS) -o $@ dummyDemo.o $(LIBS) $(OBJECTS)

libfgng-1.0.a:
	cd lib/ ;	make

%: %.o fgng-1.0.a $(OBJECTS)
	$(LD) $(LD_FLAGS) -o $@ $(OBJECTS) $< $(LIBS)

%.o: %.cpp
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	rm -rf lib/*.o
	rm -rf lib/*.a
	rm -rf  *.o
	rm -rf dummyDemo

