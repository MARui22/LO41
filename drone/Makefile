EXEC=drone.elf

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

drone.elf: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf *.o *.elf
