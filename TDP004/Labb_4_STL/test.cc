#include <map>
#include <iostream>

using namespace std;

int main()
{
  map<string, int> defult;
  defult["hej"] += 1;
  cout << defult["hej"] << endl;
  return 0;
}
