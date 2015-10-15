PRJ=ifj15
#
PROGS=$(PRJ)
FILES=main.c errors.c ial.c finite.c input.c
CC=gcc
# -Wall -W -Wextra
CFLAGS=-std=c99 -Wall -pedantic -g

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
