CC=gcc
CFLAGS=-Wall -std=c99
LIBS=

SOURCES=bmp_to_4bpp.c bmp_reader.c utils.c options.c
OBJECTS=$(SOURCES:.c=.o)

all: bmp_to_4bpp

bmp_to_4bpp: $(OBJECTS)
	${CC} -o $@ ${CFLAGS} $(OBJECTS) ${LIBS}

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

clean:
	rm -f $(OBJECTS)

.PHONY: all clean
