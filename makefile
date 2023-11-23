CC = gcc
CFLAGS = -o -g -Wall
PKG_CONFIG = pkg-config
ALLEGRO_MODULES = allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5
LIBS = $(shell $(PKG_CONFIG) --libs $(ALLEGRO_MODULES)) -lm 
CFLAGS += $(shell $(PKG_CONFIG) --cflags $(ALLEGRO_MODULES))

SRC = main.c game.c enemy.c player.c Joystick.c Bullet.c Pistol.c obstacle.c hud.c menu.c
OBJ = $(SRC:.c=.o)

nomeDoPrograma = space_invaders

all: $(nomeDoPrograma)

$(nomeDoPrograma): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o

purge: clean
	rm -rf $(nomeDoPrograma)
