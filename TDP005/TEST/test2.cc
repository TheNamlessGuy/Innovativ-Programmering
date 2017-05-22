#include "test2.h"
#include <iostream>

using namespace std;

Test2::Test2()
{
  thing = "";
}

void Test2::print_thing()
{
  cout << thing << endl;
}

void Test2::set_thing(string s)
{
  thing = s;
}
