/* A program to test the AABB class.
 * As such it is not written for efficiency.
 * Compilation example:
 *  g++ -Wall -Wextra -pedantic -std=c++11 unit_test.cc aabb.cc -lSDL
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <SDL/SDL.h>
#include "aabb.h"

using namespace std;

bool blank_string(string const& s)
{
  for (unsigned int i = 0; i < s.size(); ++i)
  {
    if ( ! isblank(s.at(i)) )
      return false;
  }
  return true;
}

void handle_testcase(SDL_Surface* screen, string const& test);

int main(int argc, char *argv[])
{
  if ( argc != 2 )
  {
    cerr << "Usage: " << argv[0] << " TESTCASEFILE" << endl; 
    return 1;
  }

  ifstream testcases(argv[1]);

  if ( ! testcases )
  {
    cerr << "Unable to open test file '" << argv[1] << "'." << endl;
    return 1;    
  }
  
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    cerr << "Unable to init SDL: " << SDL_GetError() << endl;
    return 1;
  }
  atexit( SDL_Quit );
  
  SDL_Surface *screen;
  screen = SDL_SetVideoMode(
    640, /* width */
    480, /* height */
    32, /* bits per pixel */
    SDL_SWSURFACE);
  
  if ( screen == NULL )
  {
    cerr << "Unable to set 64*8x64 video: " << SDL_GetError() << endl;
    return 1;
  }
//  SDL_SetWindowTitle(screen, "Collision Detection Unit Tests");
  
  bool exit_request{ false };
  bool next_test{ true };

  while ( ! exit_request )
  {
    SDL_Event event;
    string line;

    if ( next_test )
    {
      do
      {
        if ( ! getline( testcases, line ) )
        {
          return 0; // End of file or bad testfile format
        }
      }
      while ( blank_string(line) );
      
      handle_testcase(screen, line);
      next_test = false;
    }
    
    if ( SDL_WaitEvent(&event) )
    {
      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
        {          
          if (event.button.button == 1)
          {
            next_test = true;
            break;
          }
          // Other buttons will fall through and exit program
        }
        case SDL_QUIT:
        {
          exit_request = true;
        }
      }
    }
  }
  return 0;
}


static int red(SDL_Surface* s)
{
  return SDL_MapRGB(s->format, 255, 0, 0);
}

static int green(SDL_Surface* s)
{
  return SDL_MapRGB(s->format, 0, 255, 0);
}

static int blue(SDL_Surface* s)
{
  return SDL_MapRGB(s->format, 0, 0, 255);
}

static int grey(SDL_Surface* s)
{
  return SDL_MapRGB(s->format, 128, 128, 128);
}

static int white(SDL_Surface* s)
{
  return SDL_MapRGB(s->format, 255, 255, 255);
}


static void get_rect(istream& is, SDL_Rect& r)
{
  is >> r.x >> r.y >> r.w >> r.h;
}

static void get_point(istream& is, SDL_Rect& r)
{
  is >> r.x >> r.y;
  r.w = 1;
  r.h = 1;
}

static void outline_rect(SDL_Surface* s, SDL_Rect* r, int c)
{
  SDL_Rect   left{ r->x, r->y, 2, r->h };
  SDL_Rect  right{ short(r->x + r->w - 2), r->y, 2, r->h };
  SDL_Rect    top{ r->x, r->y, r->w, 2 };
  SDL_Rect bottom{ r->x, short(r->y + r->h - 2), r->w, 2 };
  SDL_FillRect(s, &left, c);
  SDL_FillRect(s, &right, c);
  SDL_FillRect(s, &top, c);
  SDL_FillRect(s, &bottom, c);
}

static void pass_or_fail(SDL_Surface* s, bool passed)
{
  SDL_Rect status_bar{0,460,640,20};
  if ( passed )
  {
    SDL_FillRect(s, &status_bar, green(s));
  }
  else
  {
    SDL_FillRect(s, &status_bar, red(s));
  }
}


#define SDL_Rect_To_AABB(sdl, aabb) AABB aabb(sdl.y, sdl.x, sdl.y + sdl.h - 1, sdl.x + sdl.w - 1)

void handle_testcase(SDL_Surface* screen, string const& test)
{
  istringstream iss(test);
  string fn;
  char expected;

  if ( iss >> fn )
  {
    SDL_FillRect(screen, NULL, white(screen));
        
    if ( fn == "intersect" )
    {
      SDL_Rect a, b;
      get_rect(iss, a);
      get_rect(iss, b);
          
      outline_rect(screen, &a, red(screen));
      outline_rect(screen, &b, blue(screen));

      SDL_Rect_To_AABB(a, box_a);
      SDL_Rect_To_AABB(b, box_b);
          
      bool r = box_a.intersect(box_b);
          
      pass_or_fail( screen, iss >> expected && (expected == 't') == r );
    }
    else if ( fn == "collision_point")
    {
      SDL_Rect to, fr, ob;
      get_rect(iss, ob);
      get_rect(iss, fr);
      get_rect(iss, to);
          
      SDL_FillRect(screen, &ob, grey(screen));
      outline_rect(screen, &to, red(screen));
      outline_rect(screen, &fr, blue(screen));
          
      SDL_Rect_To_AABB(ob, box_ob);
      SDL_Rect_To_AABB(to, box_to);
      SDL_Rect_To_AABB(fr, box_fr);
      Point pt;
          
      bool collided = box_ob.collision_point(box_fr, box_to, pt);

      if ( collided )
      {
        SDL_Rect c{short(pt.x), short(pt.y), fr.w, fr.h};
        outline_rect(screen, &c, SDL_MapRGB(screen->format, 0, 128, 0));
      }
          
      pass_or_fail( screen, iss >> expected && (expected == 't') == collided );
    }
    else if ( fn == "may_collide")
    {
      SDL_Rect to, fr, ob;
      get_rect(iss, ob);
      get_rect(iss, fr);
      get_rect(iss, to);
          
      SDL_FillRect(screen, &ob, grey(screen));
      outline_rect(screen, &to, red(screen));
      outline_rect(screen, &fr, blue(screen));
          
      SDL_Rect_To_AABB(ob, box_ob);
      SDL_Rect_To_AABB(to, box_to);
      SDL_Rect_To_AABB(fr, box_fr);
          
      bool collided = box_ob.may_collide(box_fr, box_to);
          
      pass_or_fail( screen, iss >> expected && (expected == 't') == collided );
    }
    else if ( fn == "contain")
    {
      SDL_Rect ob, pt;
      get_rect(iss, ob);
      get_point(iss, pt);
          
      outline_rect(screen, &ob, red(screen));
      outline_rect(screen, &pt, blue(screen));
          
      SDL_Rect_To_AABB(ob, box_ob);
      Point p{pt.x, pt.y};
          
      bool r = box_ob.contain(p);
          
      pass_or_fail( screen, iss >> expected && (expected == 't') == r );
    }
    
    SDL_Rect all_screen{0,0,640,480};
    SDL_UpdateRects(screen, 1, &all_screen);
  }
}
