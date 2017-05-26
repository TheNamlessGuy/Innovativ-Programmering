#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

#include "../headers/GameOverState.h"
#include "../headers/Text.h"
#include "../headers/Button.h"

using namespace std;

GameOverState::GameOverState(SDL_Renderer* const& r, TTF_Font* const& font_i)
	: GameState(GameState::GAMEOVER),
	curr_char(0), private_score(0),
	back_button(350, 450, "src/images/buttons/backButton.png", r),
	input_timer(0), name_str("   "), font(font_i)
{
	for (int i = 0; i < 3; ++i)
	{
		name.push_back(new Text(340 + (50*i), 350, " ", r, font));
	}
}

GameOverState::~GameOverState()
{
	for (Text* text : highscore)
	{
		delete text;
	}
	highscore.clear();

	for (Text* text : name)
	{
		delete text;
	}
	name.clear();

	font = nullptr;
}

void GameOverState::input(bool KEYS[], bool const& mousedown, SDL_Renderer* const& r)
{
	int mouse_x;
	int mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	back_button.set_hover(back_button.is_hover(mouse_x, mouse_y));

	if (mousedown)
	{
		if (back_button.is_hover(mouse_x, mouse_y))
		{
			state = GameState::MAINMENU;
			update_highscore_file();
		}
	}

	if (KEYS[SDLK_RETURN])
	{
		state = GameState::MAINMENU;

		KEYS[SDLK_RETURN] = false;

		update_highscore_file();
	}

	for (int i = 97; i < 122; ++i)
	{
		if (KEYS[i] && input_timer <= 0)
		{
			input_timer = 3;

			name.at(curr_char)->update_char(char(i), r);
			name_str.at(curr_char) = char(i);
			curr_char = (curr_char + 1) % 3;
		}
	}
}

int GameOverState::update(long long& score, SDL_Renderer* const& r, bool cheats[])
{
	input_timer -= 0.1;

	if (private_score == 0)
	{
		private_score = score;
		score = 0;
	}

	if (highscore.empty())
	{
		highscore_values = read_highscore_from_file();
		sort_highscore_values();
		create_highscore_text(r);
	}

	return state;
}

void GameOverState::render(SDL_Renderer* const& r, bool const& debug)
{
	if (debug)
	{
		SDL_SetRenderDrawColor(r, 50, 50, 200, 255);
		SDL_RenderDrawLine(r, 400, 0, 400, 600);
		SDL_RenderDrawLine(r, 0, 300, 800, 300);
	}

	for (Text* text : highscore)
	{
		text->render(r);
	}

	back_button.render(r);

	for (int i = 0; i < 3; ++i)
	{
		if (curr_char == i)
		{
			SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		}

		SDL_RenderDrawLine(r, 335 + (50*i), 400, 360 + (50*i), 400);
		name.at(i)->render(r);
	}
}

void GameOverState::reset(SDL_Renderer* const& r)
{
	for (int i = 0; i < 3; ++i)
	{
		name.at(i)->update_string(" ", r);
	}

	for (Text* text : highscore)
	{
		delete text;
	}
	highscore.clear();

	private_score = 0;
	curr_char = 0;
	state = GameState::GAMEOVER;

	name_str = "   ";
}

vector<pair<long long, std::string>> GameOverState::read_highscore_from_file()
{
	ifstream fs(".highscore.txt");

	if (!fs.is_open())
	{
		fs.close();
		create_highscore_file();
		fs.open(".highscore.txt");
	}

	long long score;
	string nam;

	vector<pair<long long, string>> temp;

	for (string line; getline(fs, line);)
	{
		nam = "";
		score = 0;

		istringstream iss(line);
		iss >> score;

		iss.get();
		nam += iss.get();
		nam += iss.get();
		nam += iss.get();

		temp.push_back(make_pair(score, nam));
	}

	fs.close();

	return temp;
}

void GameOverState::create_highscore_text(SDL_Renderer* r)
{
	for (unsigned int i = 0; i < highscore_values.size(); ++i)
	{
		highscore.push_back(new Text(300, 0 + (50*i), to_string(i + 1) + ". " + highscore_values.at(i).second, r, font));
		highscore.push_back(new Text(400, 0 + (50*i), to_string(highscore_values.at(i).first), r, font));
	}

	highscore.push_back(new Text(275, 275, "Your   score   is:   " + to_string(private_score), r, font));
}

void GameOverState::sort_highscore_values()
{
	sort(highscore_values.begin(), highscore_values.end(),
			[](pair<long long, string> p, pair<long long, string> q){return p.first < q.first;});
}

void GameOverState::update_highscore_file()
{
	highscore_values.push_back(make_pair(private_score, name_str));
	sort_highscore_values();

	ofstream ofs(".highscore.txt");

	for (int i = 0; i < 5; ++i)
	{
		ofs << highscore_values.at(i).first;
		ofs << '|';
		ofs << highscore_values.at(i).second << "\n";
	}

	ofs.close();
}

void GameOverState::create_highscore_file()
{
	ofstream ofs(".highscore.txt");

	for (int i = 0; i < 5; ++i)
	{
		ofs << "0|   " << endl;
	}

	ofs.close();
}
