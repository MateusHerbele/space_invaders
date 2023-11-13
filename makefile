CC = gcc
CFLAGS = -o -g -Wall
PKG_CONFIG = pkg-config
ALLEGRO_MODULES = allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5
LIBS = $(shell $(PKG_CONFIG) --libs $(ALLEGRO_MODULES)) -lm  # Adiciona -lm para incluir a biblioteca math.h
CFLAGS += $(shell $(PKG_CONFIG) --cflags $(ALLEGRO_MODULES))

SRC = main.c enemy.c space.c player.c Joystick.c Bullet.c Pistol.c obstacle.c
OBJ = $(SRC:.c=.o)

nomeDoPrograma = space_invaders

all: $(nomeDoPrograma) clean

$(nomeDoPrograma): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o

purge: clean
	rm -rf $(nomeDoPrograma)
