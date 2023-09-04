CC=gcc -std=gnu11
CFLAGS=-Wall -Wextra -pedantic
LDFLAGS=-flto

DEBUG?=0
ifeq ($(DEBUG), 0)
   CFLAGS+=-O3 -DNDEBUG
else
   CFLAGS+=-g3
endif

FILES=src/watershed_cuts_cli.c watershed_cuts.o
watershed_cuts: $(FILES)
	$(CC) $(CFLAGS) $(FILES) $(LDFLAGS) -o watershed_cuts

watershed_cuts.o:
	$(CC) -c $(CFLAGS) src/watershed_cuts.c

clean:
	rm -f watershed_cuts.o
	rm -f waterhed_cuts
