#include <SDL2/SDL.h>

#include "../headers/Entity.h"
#include "../headers/Sprite.h"
#include "../headers/Weapon.h"

using namespace std;

Entity::Entity(int const& x_i, int const& y_i, int facing_i, SDL_Renderer* const& r, bool const& friendly_i)
	: x(x_i), y(y_i), facing(facing_i), sprite(x_i, y_i, r), weapon(Weapon::PISTOL, friendly_i) {}

Entity::~Entity() {}

SDL_Rect Entity::get_hitbox()
{
	return sprite.get_hitbox(facing);
}

void Entity::shoot(vector<Bullet*>& bullets, int const& shot_direction)
{
	pair<int, int> temp = weapon.get_shot_location(x, y, sprite.get_width(), sprite.get_height(), facing);
	weapon.shoot(bullets, temp.first, temp.second, shot_direction);
}
