#include <SDL2/SDL.h>
#include <time.h>
#include <cstdlib>

#include "../headers/PlayState.h"
#include "../headers/Door.h"
#include "../headers/Grenade.h"
#include "../headers/SquareHead.h"

using namespace std;

const SDL_Rect PlayState::PLAYING_FIELD {40, 40, 720, 500};

PlayState::PlayState(SDL_Renderer* const& r, TTF_Font* const& font)
	: GameState(GameState::PLAY),
	enemy_spawn_timer(5), karma_tick_timer(0), is_dead(false),
	player(387, 287, r), hud(0, 500, r, font),
	floor(0, 0, "src/images/floor.png", r)
{
	doors.push_back(new Door(0, 200, true, r));
	doors.push_back(new Door(780, 200, true, r));
	doors.push_back(new Door(300, 0, false, r));
}

PlayState::~PlayState()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();

	for (Bullet* bullet : bullets)
	{
		delete bullet;
	}
	bullets.clear();

	for (Pickup* pickup : pickups)
	{
		delete pickup;
	}
	pickups.clear();

	for (Door* door : doors)
	{
		delete door;
	}
	doors.clear();
}

void PlayState::input(bool KEYS[], bool const& mousedown, SDL_Renderer* const& r)
{
	player.input(KEYS, bullets, PLAYING_FIELD);
}

int PlayState::update(long long& score, SDL_Renderer* const& r, bool cheats[])
{
	enemy_spawn_timer -= 0.1;
	karma_tick_timer -= 0.1;

	player.update(score, hud.get_armor_count(), hud.is_rapidfire(), hud.is_slowfire());

	update_doors();
	update_bullets();
	update_enemies(score, r);
	update_pickups(r, score);

	spawn_enemies(r);

	if (player.is_hit(bullets))
	{
		is_dead = hud.remove_armor();
	}

	if (is_dead && !cheats[SquareHead::CHEAT_IMMORTAL])
	{
		state = GameState::GAMEOVER;
	}
	else
	{
		is_dead = false;
	}

	check_powerups();

	if (karma_tick_timer <= 0)
	{
		score++;
		karma_tick_timer = 10;
	}

	hud.update_score(score, r);
	return state;
}

void PlayState::render(SDL_Renderer* const& r, bool const& debug)
{
	floor.render(r, PLAYING_FIELD);

	for (Pickup* pickup : pickups)
	{
		pickup->render(r);
	}

	for (Bullet* bullet : bullets)
	{
		bullet->render(r, debug);
	}

	for (Enemy* enemy : enemies)
	{
		enemy->render(r, debug);
	}

	for (Door* door : doors)
	{
		door->render(r);
	}

	player.render(r, debug);
	hud.render(r);
}

void PlayState::check_powerups()
{
	if (hud.is_slow_enemies() && hud.is_fast_enemies())
	{
		Enemy::enemy_speed = 1;
	}
	else if (hud.is_slow_enemies())
	{
		Enemy::enemy_speed = 0.5;
	}
	else if (hud.is_fast_enemies())
	{
		Enemy::enemy_speed = 1.5;
	}
	else
	{
		Enemy::enemy_speed = 1;
	}
}

void PlayState::reset(SDL_Renderer* const& r)
{
	is_dead = false;
	karma_tick_timer = 0;
	enemy_spawn_timer = 5;
	state = GameState::PLAY;

	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();

	for (Bullet* bullet : bullets)
	{
		delete bullet;
	}
	bullets.clear();

	for (Pickup* pickup : pickups)
	{
		delete pickup;
	}
	pickups.clear();

	for (Door* door : doors)
	{
		door->reset();
	}

	player.reset();
	hud.reset(r);
}

void PlayState::update_doors()
{
	for (Door* d : doors)
	{
		d->update();
	}
}

void PlayState::update_bullets()
{
	vector<Bullet*> grenade_bullets;

	for (vector<Bullet*>::iterator iter = bullets.begin(); iter != bullets.end(); ++iter)
	{
		Grenade* grenade = dynamic_cast<Grenade*>(*iter);

		if (grenade != nullptr)
		{
			grenade_bullets = grenade->update();
		}
		else
		{
			(*iter)->update();
		}

		if ((*iter)->is_out_of_bounds())
		{
			delete *iter;
			bullets.erase(iter);
			iter--;
		}
	}

	bullets.insert(bullets.end(), grenade_bullets.begin(), grenade_bullets.end());
}

void PlayState::update_enemies(long long& score, SDL_Renderer* r)
{
	for (vector<Enemy*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter)
	{
		bool stabbed = false;

		(*iter)->update(bullets, PLAYING_FIELD, player.get_hitbox(), player.get_aura(), stabbed);

		if (stabbed)
		{
			is_dead = hud.remove_armor();
		}

		if ((*iter)->is_hit(bullets, hud.is_penetrating()))
		{
			if ((*iter)->drop_pickup())
			{
				pickups.push_back(new Pickup((*iter)->get_hitbox().x, (*iter)->get_hitbox().y, r));
			}

			delete (*iter);
			enemies.erase(iter);
			iter--;
			score -= 100;
		}
	}
}

void PlayState::update_pickups(SDL_Renderer* r, long long const& score)
{
	for (vector<Pickup*>::iterator iter = pickups.begin(); iter != pickups.end(); ++iter)
	{
		if ((*iter)->update())
		{
			delete *iter;
			pickups.erase(iter);
			iter--;
		}
		else if ((*iter)->is_colliding_with(player.get_hitbox()))
		{
			pair<bool, int> temp = (*iter)->pick_up(score);

			delete *iter;
			pickups.erase(iter);
			iter--;

			if (temp.first)
			{
				player.set_weapon(temp.second);
				hud.change_weapon_image(temp.second, r);
			}
			else
			{
				hud.add_powerup(temp.second);
			}
		}
	}
}

void PlayState::spawn_enemies(SDL_Renderer* const& r)
{
	if (enemies.size() <= 10 && enemy_spawn_timer <= 0)
	{
		//random_device rd;
		//uniform_int_distribution<int> uid(0, 2);

		srand(time(NULL));
		doors.at((rand()%3))->spawn_enemy(enemies, r);
		enemy_spawn_timer = 20;
	}
}
