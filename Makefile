CC=gcc
CFLAGS=-std=c99
LDFLAGS=
EXEC=main.elf
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
HED= $(SRC:.c=.h)

all: $(EXEC)

main.elf: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# main.o: $(HED)

%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
	
	# gcc -std=c99 -o gui tab.h gui.c tab.c