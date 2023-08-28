CC=gcc -std=c11
CFLAGS=-g3 -Wall -Wextra
LDFLAGS=

FILES=src/watershed_cuts.c src/watershed_cuts_cli.c
watershed_cuts: $(FILES)
	$(CC) $(CFLAGS) $(FILES) $(LDFLAGS) -o watershed_cuts
