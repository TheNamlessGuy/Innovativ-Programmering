#include <iostream>
#include <map>

using namespace std;

int main()
{
  map<string, int> my_map;
  my_map["Kaffe"] = 237;
  my_map["Kaffe"] = 555;
  cout << my_map["Kaffe"] << endl;
  return 0;
}
