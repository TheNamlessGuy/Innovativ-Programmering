#ifndef HEADERS_HEADUPDISPLAY_H_
#define HEADERS_HEADUPDISPLAY_H_

#include <vector>
#include <SDL2/SDL.h>

#include "Image.h"
#include "Text.h"

/**
 * The HeadUpDisplay (HUD) class represents all the stats seen at the bottom of the screen in the PlayState.
 *
 * For example, the current weapon, powerups and score.
 * It also contains a list with all the powerups currently held by the player.
 */

class HeadUpDisplay
{
public:
	static const int ARMOR{0};
	static const int RAPIDFIRE{1};
	static const int SLOW_ENEMIES{2};
	static const int PENETRATING{3};
	static const int FAST_ENEMIES{4};
	static const int SLOWFIRE{5};

	HeadUpDisplay(int const& x_i, int const& y_i, SDL_Renderer* const& r, TTF_Font* const& font_i);
	~HeadUpDisplay();

	void add_powerup(int const& p);
	void render(SDL_Renderer* const& r) const;

	void reset(SDL_Renderer* const& r);

	void change_weapon_image(int new_weapon, SDL_Renderer* const& r);
	void update_score(long long const& score, SDL_Renderer* const& r);

	int get_armor_count() const;
	/**
	 * Returns true if called when the player has no armor upgrades.
	 * Otherwise it removes one armor.
	 */
	bool remove_armor();

	bool is_rapidfire() const;
	bool is_slow_enemies() const;
	bool is_penetrating() const;
	bool is_fast_enemies() const;
	bool is_slowfire() const;
private:
	const int x;
	const int y;

	Image* weapon;

	std::vector<int> current_powerups;
	std::vector<Image*> powerup_images;

	Text* score_text;
	Text* weapon_text;
	Text* powerups_text;

	/**
	 * Returns the current amount held of the powerup-type "powerup" (parameter).
	 */
	int get_count(int powerup) const;

	void load_powerup_image(std::string const& filelocation, SDL_Renderer* const& r);
};

#endif
