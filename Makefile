CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lpthread -lqueue
LIBS=
SOURCES=main.c multilistner.c multisender.c udplistner.c udpsender.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=multi

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
