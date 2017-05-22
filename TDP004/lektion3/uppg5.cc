#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
  vector<string> const argument(argv + 1, argv + argc);
  for (string value : argument)
    {
      cout << value << endl;
    }
  return 0;
}
