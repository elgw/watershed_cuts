CC=gcc -std=c11
CFLAGS=-g3 -Wall -Wextra
LDFLAGS=-flto

FILES=src/watershed_cuts_cli.c watershed_cuts.o
watershed_cuts: $(FILES)
	$(CC) $(CFLAGS) $(FILES) $(LDFLAGS) -o watershed_cuts

watershed_cuts.o:
	$(CC) -c $(CFLAGS) src/watershed_cuts.c

clean:
	rm -f watershed_cuts.o
	rm -f waterhed_cuts
