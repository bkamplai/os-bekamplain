CC = g++
CFLAGS = -std=c++11 -Wall
LDFLAGS = -lpthread

all: bakery.cpp
	$(CC) $(CFLAGS) bakery.cpp $(LDFLAGS) -o bakery

run:
	./bakery

clean:
	$(RM) bakery *.o *~