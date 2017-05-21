CC=gcc
CFLAGS=-std=c99
LDFLAGS=
EXEC=main.elf
SRC= $(wildcard gui/*.c)
OBJ= $(SRC:.c=.o)
HED= $(SRC:.c=.h)

all: $(EXEC)

main.elf: $(OBJ) main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c $(HED)
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o $(EXEC) $(OBJ)

	
	# gcc -std=c99 -o main.elf gui/tab.h gui/gui.h gui/tab.c gui/gui.c main.c