CC = gcc
CFLAGS = -o3 -g -Wall
 
SRC = main.c enemy.c space.c
OBJ = $(SRC:.c=.o)

nomeDoPrograma = space_invaders
all: $(nomeDoPrograma)

$(nomeDoPrograma): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o
purge:
	rm -rf *.o $(nomeDoPrograma)	
