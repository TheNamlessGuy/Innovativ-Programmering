#ifndef _TEST2_H_
#define _TEST2_H_
#include "test1.h"
#include <string>
class Test2: public Test1
{
public:
  Test2();
  void print_thing();
  void set_thing(std::string s);
private:
  std::string thing;
};
#endif
