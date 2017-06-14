export CC=gcc
export CFLAGS=-std=c99 -g -lpthread
export LDFLAGS= -lpthread

SRC= $(wildcard gui/*.c)
OBJ= $(SRC:.c=.o)
HED= $(SRC:.c=.h)

DRONE_DIR=drone
TOUR_DIR=tour_de_controle

EXEC=main.elf $(DRONE_DIR)/drone.elf $(TOUR_DIR)/tour_controle.elf

all: $(EXEC)

main.elf: $(OBJ) main.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
$(DRONE_DIR)/drone.elf: $(DRONE_DIR)/drone.o
	@(cd $(DRONE_DIR) && $(MAKE))
  
$(TOUR_DIR)/tour_controle.elf: $(TOUR_DIR)/tour_controle.o
	@(cd $(TOUR_DIR) && $(MAKE))


main.o: main.c $(HED) const.h 
	$(CC) -o $@ -c $< $(CFLAGS) 

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) 

clean:
	rm -rf *.o $(EXEC) $(OBJ) 
	@(cd $(DRONE_DIR) && $(MAKE) $@)

	
	# gcc -std=c99 -o main.elf gui/tab.h gui/gui.h gui/tab.c gui/gui.c main.c
