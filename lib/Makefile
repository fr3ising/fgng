AR = ar
CC = g++
LD = g++

CC_FLAGS = -Wall -Werror -I/usr/include/SDL2/ --pedantic  -c -O3
LD_FLAGS = -Wall -Werror --pedantic -lm -O3

OBJECTS = fgng.o fgng_dummy.o

libo2020-1.0.a:  $(OBJECTS)
	$(AR) r libfgng-1.0.a $(OBJECTS)

%.o: %.cpp
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	rm -rf *.o
	rm libfgng-1.0.a
