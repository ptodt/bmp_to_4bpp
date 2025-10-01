CC=gcc
CFLAGS=-Wall -std=c99
LIBS=

SOURCES=bmp_to_xbpp.c bmp_reader.c utils.c options.c bmp_writer.c bmp_palette.c
OBJECTS=$(SOURCES:.c=.o)

all: version.h bmp_to_xbpp

version.h: VERSION
	./scripts/update_version.sh

bmp_to_xbpp: $(OBJECTS)
	${CC} -o $@ ${CFLAGS} $(OBJECTS) ${LIBS}

%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

clean:
	rm -f $(OBJECTS) version.h

.PHONY: all clean
