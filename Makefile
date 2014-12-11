CC=gcc
CFLAGS=-c -Wall -std=c99
SOURCES=parqueadero.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=parqueadero

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
