/*
 * brute < input.txt
 *
 * Compute and plot all line segments involving 4 points in input.txt using SDL
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <chrono>
#include "SDL/SDL.h"
#include "Point.h"

using namespace std;

typedef vector<pair<int, double>> pointvector;

bool compare_pairs(pair<int, double> a, pair<int, double> b)
{
  return (a.second < b.second);
}

void render_points(SDL_Surface* screen, const vector<Point>& points)
{
  SDL_LockSurface(screen);

  for(const auto& point : points)
  {
    point.draw(screen);
  }

  SDL_FreeSurface(screen);
  SDL_Flip(screen); // display screen
}

void render_line(SDL_Surface* screen, const Point& p1, const Point& p2)
{
  SDL_LockSurface(screen);

  p1.lineTo(screen, p2);

  SDL_FreeSurface(screen);
  SDL_Flip(screen); // display screen
}

int main(int argc, char* argv[])
{
  if (argc != 1)
  {
    cout << "Usage:" << endl << argv[0] << " < input.txt" << endl;

    return 0;
  }

  // we only need SDL video
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }

  // register SDL_Quit to be called at exit
  atexit(SDL_Quit);

  // set window title
  SDL_WM_SetCaption("Pointplot", 0);

  // Set the screen up
  SDL_Surface* screen = SDL_SetVideoMode(512, 512, 32, SDL_SWSURFACE);

  if (screen == nullptr)
  {
    fprintf(stderr, "Unable to set 512x512 video: %s\n", SDL_GetError());
    exit(1);
  }

  // clear the screen with white
  SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

  // the array of points
  vector<Point> points;

  // read points from cin
  int N;
  unsigned int x;
  unsigned int y;

  cin >> N;

  for (int i{0}; i < N; ++i) {
    cin >> x >> y;
    points.push_back(Point(x, y));
  }

  // draw points to screen all at once
  render_points(screen, points);

  // sort points by natural order
  // makes finding endpoints of line segments easy
  sort(points.begin(), points.end());

  auto begin = chrono::high_resolution_clock::now();
  
  /*
   * CUSTOM CODE STARTS HERE
   */
  // Loop through all the points
  for (int p = 0; p < N-1; ++p) {
    pointvector slopes;
    // Add all the points after the current one (p)
    for (int q = p + 1; q < N; ++q)
      slopes.push_back(make_pair(q, points.at(p).slopeTo(points.at(q))));
    
    // Sort the points for the current p based on the slope value
    stable_sort(slopes.begin(), slopes.end(), compare_pairs);
    
    // Count controls how many points share the value of "current" in a row
    int count = 1;
    // The current value to check for
    double current = slopes.at(0).second;
    
    // Loop through all the saved points
    for (int i = 1; i < slopes.size(); ++i) {
      // If your current doesn't match the new node, reset count and current
      if (current != slopes.at(i).second) {
        // If there are 3 or more nodes in a row, render a line between them
        if (count >= 3)
          render_line(screen, points.at(p), points.at(slopes.at(i - 1).first));
        
        count = 1;
        current = slopes.at(i).second;
      } else {
        ++count;
        
        // If there are 3 or more nodes in a row, render a line between them
        if (count >= 3)
          render_line(screen, points.at(p), points.at(slopes.at(i).first));
      }
    }
    
  }

  /*
   * CUSTOM CODE ENDS HERE
   */
    
  auto end = chrono::high_resolution_clock::now();
  cout << "Computing line segments took "
       << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
       << " milliseconds." << endl;

  // wait for user to terminate program
  while (true) 
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
          break;
        case SDL_KEYUP:
          if (event.key.keysym.sym == SDLK_ESCAPE)
          {
            return 0;
          }
          break;
        case SDL_QUIT:
          return(0);
      }
    }
  }

  return 0;
}
