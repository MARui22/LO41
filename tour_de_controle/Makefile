EXEC=tour_decollage.elf

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

tour_decollage.elf: $(OBJ) ../const.h
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf *.o *.elf
