#include "../headers/HeadUpDisplay.h"

#include <algorithm>
#include <sstream>
#include <SDL2/SDL_ttf.h>

#include "../headers/Image.h"
#include "../headers/Text.h"
#include "../headers/Weapon.h"

using namespace std;

HeadUpDisplay::HeadUpDisplay(int const& x_i, int const& y_i, SDL_Renderer* const& r, TTF_Font* const& font)
	: x(x_i), y(y_i),
	weapon(new Image(0, 0, "src/images/weapons/pistol.png", r))
{
	load_powerup_image("src/images/powerups/armor.png", r);
	load_powerup_image("src/images/powerups/rapid.png", r);
	load_powerup_image("src/images/powerups/slow.png", r);
	load_powerup_image("src/images/powerups/penetrating.png", r);
	load_powerup_image("src/images/powerups/fast.png", r);
	load_powerup_image("src/images/powerups/slowfire.png", r);

	score_text = new Text(10, 550, "Karma: 0", r, font);
	weapon_text = new Text(250, 550, "Weapon: ", r, font);
	powerups_text = new Text(450, 550, "Powerups: ", r, font);
}

HeadUpDisplay::~HeadUpDisplay()
{
	for (Image* i : powerup_images)
	{
		delete i;
	}
	powerup_images.clear();

	delete weapon;

	delete score_text;
	delete weapon_text;
	delete powerups_text;
}

void HeadUpDisplay::render(SDL_Renderer* const& r) const
{
	score_text->render(r);
	weapon_text->render(r);
	powerups_text->render(r);

	for (unsigned int i = 0; i < current_powerups.size(); ++i)
	{
		powerup_images.at(current_powerups.at(i))->render(r, 600 + (40*i), 555);
	}

	weapon->render(r, 375, 545);
}

void HeadUpDisplay::add_powerup(int const& i)
{
	current_powerups.push_back(i);

	if (current_powerups.size() > 5)
	{
		current_powerups.erase(current_powerups.begin());
	}
}

bool HeadUpDisplay::remove_armor()
{
	int armor = HeadUpDisplay::ARMOR;
	vector<int>::iterator iter = find(current_powerups.begin(), current_powerups.end(), armor);

	if (iter == current_powerups.end())
	{
		return true;
	}
	else
	{
		current_powerups.erase(iter);
		return false;
	}
}

void HeadUpDisplay::change_weapon_image(int new_weapon, SDL_Renderer* const& r)
{
	delete weapon;

	switch (new_weapon)
	{
	case Weapon::PISTOL:
		weapon = new Image(0, 0, "src/images/weapons/pistol.png", r);
		break;
	case Weapon::RIFLE:
		weapon = new Image(0, 0, "src/images/weapons/rifle.png", r);
		break;
	case Weapon::SHOTGUN:
		weapon = new Image(0, 0, "src/images/weapons/shotgun.png", r);
		break;
	case Weapon::GRENADE:
		weapon = new Image(0, 0, "src/images/weapons/grenade.png", r);
		break;
	default:
		break;
	}
}

void HeadUpDisplay::update_score(long long const& score, SDL_Renderer* const& r)
{
	ostringstream ss;

	ss << "KARMA:  ";
	ss << score;

	score_text->update_string(ss.str(), r);
}

int HeadUpDisplay::get_count(int powerup) const
{
	return count(current_powerups.begin(), current_powerups.end(), powerup);
}

int HeadUpDisplay::get_armor_count() const
{
	return get_count(HeadUpDisplay::ARMOR);
}

bool HeadUpDisplay::is_slow_enemies() const
{
	return get_count(HeadUpDisplay::SLOW_ENEMIES);
}

bool HeadUpDisplay::is_penetrating() const
{
	return get_count(HeadUpDisplay::PENETRATING);
}

bool HeadUpDisplay::is_rapidfire() const
{
	return get_count(HeadUpDisplay::RAPIDFIRE);
}

bool HeadUpDisplay::is_fast_enemies() const
{
	return get_count(HeadUpDisplay::FAST_ENEMIES);
}

bool HeadUpDisplay::is_slowfire() const
{
	return get_count(HeadUpDisplay::SLOWFIRE);
}

void HeadUpDisplay::load_powerup_image(string const& filelocation, SDL_Renderer* const& r)
{
	Image* temp = new Image(0, 0, filelocation, r);
	temp->get_hitbox().w -= 15;
	temp->get_hitbox().h -= 15;
	powerup_images.push_back(temp);
	temp = nullptr;
}

void HeadUpDisplay::reset(SDL_Renderer* const& r)
{
	current_powerups.clear();
	change_weapon_image(Weapon::PISTOL, r);
}
