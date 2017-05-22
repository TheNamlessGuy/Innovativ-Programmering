#include <iostream>
#include <fstream>
#include <random>

using namespace std;

int main()
{
  int x{0};
  random_device rd;
  ofstream file("cool.txt");
  
  for (int i{0}; i < 10; ++i)
  {
    for (int i{0}; i < 3; ++i)
    {
      x = rd() % 9;
      
      cout << x << ' ' << flush;
      file << x;
    }
    file << endl;
    cout << endl;
  }
  file.close();
  return 0;
}
