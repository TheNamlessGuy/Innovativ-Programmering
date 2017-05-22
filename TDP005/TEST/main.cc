#include "test1.h"
#include "test2.h"
#include <iostream>

using namespace std;

int main()
{
  Test2 t;
  t.set_thing("Hi");
  Test2 y;
  y.set_thing("Hello");
  Test1 tone = t;
  tone.print_thing();
  t.print_thing(); 
  cout << endl;
  tone = y;
  tone.print_thing();
  y.print_thing();
  cout << endl;
  return 0;
}
