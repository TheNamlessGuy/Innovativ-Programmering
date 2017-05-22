#include <sstream>
#include "console.h"

/**
 * Please look in the header file to get an overview of the functions
 * and how to use them. The gory details here should not be needed.
 */

using namespace std;

bool Console::instantiated = false;
static const string one_instance_error_msg = 
  "only one Console instance allowed, "
  "make sure you only create one Console variable, "
  "and that you always pass it to functions as a reference";
  
Console::Console() :
  _has_colors(false),
  _fg_color(COLOR_WHITE), _bg_color(COLOR_BLACK),
  co(0)
{
  if (instantiated)
    throw ConsoleError("constructor: " + one_instance_error_msg);
  
  instantiated = true;
  
  initscr();
  cbreak();
  //raw();
  keypad(stdscr, TRUE);
  
  halfdelay(1);
  
  if (has_colors() == TRUE)
  {
    start_color();
    _has_colors = true;
    pair_content(0, &_fg_color, &_bg_color);
    // set color offset so default color get pair 0
    co = COLOR_PAIRS - (_fg_color + _bg_color * COLORS);
    // we do not use assume_default_colors to set pair 0,
    // since errors will leave the screen messed up
    //  assume_default_colors(COLOR_BLACK, COLOR_BLACK);
    attron(COLOR_PAIR(0));
  }
  else
  {
    _has_colors = false;
  }

  //  echo();
  noecho();
  curs_set(0);
  //  curs_set(1);
//  setForegroundColor(COLOR_WHITE);
//  setBackgroundColor(COLOR_BLACK);
}

Console::~Console()
{
  endwin();
  instantiated = false;
//  assume_default_colors(-1, -1);
}

// Console::Console(Console&)
// {
//   throw ConsoleError("copy constructor: " + one_instance_error_msg);
// }

// Console& Console::operator=(Console&)
// {
//   throw ConsoleError("assignment operator: " + one_instance_error_msg);
// }


/* get input */
int  Console::get() const
{
  return getch();
}

/* get input */
bool  Console::get(char& c) const
{
  int ch = getch();
  
  if (ch == ERR)
    return false;
  
  c = char(ch);
  
  return true;
}

/* write character at current position, and advance position */
void Console::put(char c)
{
  addch(c);
  refresh();
}

void Console::put(int i)
{
  ostringstream os;
  os << i;
  put(os.str());
}

void Console::put(unsigned int i)
{
  ostringstream os;
  os << i;
  put(os.str());
}

void Console::put(string const& str)
{
  for (unsigned i = 0; i < str.length(); ++i)
  {
    addch(str.at(i));
  }
  refresh();
}

/* set/get current write position */
void Console::setPos(int x, int y)
{
  if ( x < 0 || x >= getWidth() )
    throw ConsoleError("setPos: x(==" + to_string(x) +
                       ") out of bound(==" + to_string(getWidth()) + ")");
  
  if ( y < 0 || y >= getHeight() )
    throw ConsoleError("setPos: y(==" + to_string(y) +
                       ") out of bound(==" + to_string(getWidth()) + ")");

  move(y, x);
}

void Console::getPos(int& x, int& y) const
{
  getyx(stdscr, y, x);
}

int Console::getWidth() const
{
  int top, left, bottom __attribute__((unused)), right;
  
  /* those are macro definitions, no pointer arguments... */
  getbegyx(stdscr, top, left);
  getmaxyx(stdscr, bottom, right);

  if (top != 0 || left != 0)
    throw ConsoleError("getWidth: unexpected top/left screen coordinate");
  
  return right;
}

int Console::getHeight() const
{
  int top, left, bottom, right __attribute__((unused));
  
  /* those are macro definitions, no pointer arguments... */
  getbegyx(stdscr, top, left);
  getmaxyx(stdscr, bottom, right);

  if (top != 0 || left != 0)
    throw ConsoleError("getHeight: unexpected top/left screen coordinate");
  
  return bottom;
}


void Console::setForegroundColor(color c)
{
   if (!_has_colors)
      throw ConsoleError("setForegroundColor: colors not available");

   if (c < 0 || c >= COLORS)
      throw ConsoleError("setForegroundColor: invalid color");
   
   _fg_color = c;
   
   const int pair = (_fg_color + _bg_color * COLORS + co) % COLOR_PAIRS;

   if ( pair >= COLOR_PAIRS )
      throw ConsoleError("setForegroundColor: out of colors");
   
   init_pair(pair, _fg_color, _bg_color);
   attrset(COLOR_PAIR(pair));
}

void Console::setBackgroundColor(color c)
{
   if (!_has_colors)
      throw ConsoleError("setBackgroundColor: colors not available");
   
   if (c < 0 || c >= COLORS)
      throw ConsoleError("setBackgroundColor: invalid color");
   
   _bg_color = c;
   
   const int pair = (_fg_color + _bg_color * COLORS + co) % COLOR_PAIRS;

   if ( pair >= COLOR_PAIRS )
      throw ConsoleError("setBackgroundColor: out of colors");
   
   init_pair(pair, _fg_color, _bg_color);
   attrset(COLOR_PAIR(pair));
}

color Console::getForegroundColor() const
{
   if (!_has_colors)
      throw ConsoleError("getForegroundColor: colors not available");

   return _fg_color;
}

color Console::getBackgroundColor() const
{
   if (!_has_colors)
      throw ConsoleError("getBackgroundColor: colors not available");
   
   return _bg_color;
}
