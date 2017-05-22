#ifndef CONSOLE_H
#define CONSOLE_H

#include <exception>
#include <string>
#ifdef _SOLARIS_
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

/**
 * This is the color constants you can use where a color is expected:
 *
 * COLOR_BLACK
 * COLOR_RED
 * COLOR_GREEN
 * COLOR_YELLOW
 * COLOR_BLUE
 * COLOR_MAGENTA
 * COLOR_CYAN
 * COLOR_WHITE
 */
typedef short color;

/**
 * Provide "C++" access to elementary ncurses functions.
 *
 * ** Not meant to be efficient, but simple to grasp fast
 *
 * **NOTE** Console restore the screen on exit. If your
 *          program exit without delay you will not have
 *          time to see any of your output!
 *
 * Version 0.3
 */
class Console
{
public:
  Console();
  ~Console();

  /* you are not allowed to copy console objects, you can only have
   * one console, make sure to pass references, not copies */
  Console(Console&) = delete;
  Console& operator=(Console&) = delete;
  
  /* get input from keyboard */
  int  get() const; // returns ERR after 1/10 second
  bool get(char& c) const; // returns false if a character was not
                           // available within 1/10 second
  
  /* write character(s) at current position */
  void put(char c);     // put the character for ascii code c
  void put(int i);      // put the integer i
  void put(unsigned int i);
  void put(std::string const& str); // put a string
  
  /* set/get position where next character will end up
   * must be (0 <= x < width) and (0 <= y < height) */
  void setPos(int x, int y);
  void getPos(int& x, int& y) const;
  
  /* get the size of the screen */
  int getWidth() const;
  int getHeight() const;

  /* set/get the current color for characters and background */
  void setForegroundColor(color c);
  void setBackgroundColor(color c);
  color getForegroundColor() const;
  color getBackgroundColor() const;
      
private:

  bool _has_colors;
  color _fg_color, _bg_color;
  int co; // color offset

  static bool instantiated;
};

/**
 * Thrown when something goes amiss
 */
class ConsoleError : public std::exception
{
public:
  ConsoleError(std::string const& msg)
    : std::exception(), _what(msg) {}
  ~ConsoleError() noexcept {}
  
  const char* what() const noexcept { return _what.c_str(); }
  
private:
  std::string _what;
};

#endif
