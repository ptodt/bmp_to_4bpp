CC=gcc
CFLAGS=-Wall -std=c99
LIBS=

SOURCES=bmp_to_xbpp.c bmp_reader.c utils.c options.c
OBJECTS=$(SOURCES:.c=.o)

all: bmp_to_xbpp

bmp_to_xbpp: $(OBJECTS)
	${CC} -o $@ ${CFLAGS} $(OBJECTS) ${LIBS}

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

clean:
	rm -f $(OBJECTS)

.PHONY: all clean
