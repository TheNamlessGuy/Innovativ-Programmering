CC=g++
CFLAGS=-Wall -std=c++11
SDL_FLAGS=`sdl2-config --libs --cflags` -lSDL2_ttf -lSDL2_image

all: SquareHead

SquareHead: main.o Bullet.o Button.o CollisionBox.o Door.o Enemy.o Entity.o GameOverState.o GameState.o Grenade.o HeadUpDisplay.o Image.o MainMenuState.o Pickup.o Player.o PlayState.o Sprite.o SquareHead.o Text.o Weapon.o
	$(CC) $(CFLAGS) main.o Bullet.o Button.o CollisionBox.o Door.o Enemy.o Entity.o GameOverState.o GameState.o Grenade.o HeadUpDisplay.o Image.o MainMenuState.o Pickup.o Player.o PlayState.o Sprite.o SquareHead.o Text.o Weapon.o -o SquareHead $(SDL_FLAGS)

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc

Bullet.o: source/Bullet.cc headers/Bullet.h
	$(CC) $(CFLAGS) -c source/Bullet.cc

Button.o: source/Button.cc headers/Button.h
	$(CC) $(CFLAGS) -c source/Button.cc

CollisionBox.o: source/CollisionBox.cc headers/CollisionBox.h
	$(CC) $(CFLAGS) -c source/CollisionBox.cc

Door.o: source/Door.cc headers/Door.h
	$(CC) $(CFLAGS) -c source/Door.cc

Enemy.o: source/Enemy.cc headers/Enemy.h
	$(CC) $(CFLAGS) -c source/Enemy.cc

Entity.o: source/Entity.cc headers/Entity.h
	$(CC) $(CFLAGS) -c source/Entity.cc

GameOverState.o: source/GameOverState.cc headers/GameOverState.h
	$(CC) $(CFLAGS) -c source/GameOverState.cc

GameState.o: source/GameState.cc headers/GameState.h
	$(CC) $(CFLAGS) -c source/GameState.cc

Grenade.o: source/Grenade.cc headers/Grenade.h
	$(CC) $(CFLAGS) -c source/Grenade.cc

HeadUpDisplay.o: source/HeadUpDisplay.cc headers/HeadUpDisplay.h
	$(CC) $(CFLAGS) -c source/HeadUpDisplay.cc

Image.o: source/Image.cc headers/Image.h
	$(CC) $(CFLAGS) -c source/Image.cc

MainMenuState.o: source/MainMenuState.cc headers/MainMenuState.h
	$(CC) $(CFLAGS) -c source/MainMenuState.cc

Pickup.o: source/Pickup.cc headers/Pickup.h
	$(CC) $(CFLAGS) -c source/Pickup.cc

Player.o: source/Player.cc headers/Player.h
	$(CC) $(CFLAGS) -c source/Player.cc

PlayState.o: source/PlayState.cc headers/PlayState.h
	$(CC) $(CFLAGS) -c source/PlayState.cc

Sprite.o: source/Sprite.cc headers/Sprite.h
	$(CC) $(CFLAGS) -c source/Sprite.cc

SquareHead.o: source/SquareHead.cc headers/SquareHead.h
	$(CC) $(CFLAGS) -c source/SquareHead.cc

Text.o: source/Text.cc headers/Text.h
	$(CC) $(CFLAGS) -c source/Text.cc

Weapon.o: source/Weapon.cc headers/Weapon.h
	$(CC) $(CFLAGS) -c source/Weapon.cc

run: SquareHead
	SquareHead

clean:
	rm -rf *.o *~ SquareHead

zap:
	rm -rf *.o *~ SquareHead .highscore.txt
