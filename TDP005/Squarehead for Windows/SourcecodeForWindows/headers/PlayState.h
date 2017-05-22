#ifndef HEADERS_PLAYSTATE_H_
#define HEADERS_PLAYSTATE_H_

#include <SDL2/SDL.h>
#include <vector>

#include "GameState.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Door.h"
#include "HeadUpDisplay.h"

/**
 * The PlayState is the state the game is played in.
 *
 * Contains a collection of all of the objects in the game's input, update, and render functions.
 * It checks the players input and updates the game logic accordingly.
 */

class PlayState: public GameState
{
public:
	static const SDL_Rect PLAYING_FIELD;

	PlayState(SDL_Renderer* const& r, TTF_Font* const& font);
	~PlayState();

	void input(bool KEYS[], bool const& mousedown, SDL_Renderer* const& r);
	int update(long long& score, SDL_Renderer* const& r, bool cheats[]);
	void render(SDL_Renderer* const& r, bool const& debug);

	void reset(SDL_Renderer* const& r);
private:
	double enemy_spawn_timer;
	double karma_tick_timer;

	bool is_dead;

	Player player;

	std::vector<Bullet*> bullets;
	std::vector<Enemy*> enemies;
	std::vector<Pickup*> pickups;
	std::vector<Door*> doors;
	HeadUpDisplay hud;

	Image floor;

	/**
	 * check_powerups updates the powerups that can be updated in a general fashion.
	 */
	void check_powerups();

	void update_doors();
	void update_bullets();
	void update_enemies(long long& score, SDL_Renderer* r);
	void update_pickups(SDL_Renderer* r, long long const& score);

	/**
	 * Picks a random door to spawn an enemy if there are less than 10 enemies on the field,
	 * and if the spawn timer is up.
	 */
	void spawn_enemies(SDL_Renderer* const& r);
};

#endif
