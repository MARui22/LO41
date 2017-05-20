CC=gcc
CFLAGS=-std=c99
LDFLAGS=
EXEC=gui
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

gui: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

gui.o: tab.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
	
	# gcc -std=c99 -o gui tab.h gui.c tab.c