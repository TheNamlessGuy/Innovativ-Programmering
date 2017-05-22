#include <iostream>
#include <fstream>

using namespace std;

int main()
{
  ifstream file("cool.txt");
  cin.rdbuf(file.rdbuf());
  int i{0};
  string s{""};
  cin >> i;
  if (cin.fail())
    {
      cin.clear();
      cin >> s;
    }
  cout << "s: '" << s << "'" << endl;
  cout << "i: '" << i << "'" << endl;
  return 0;
}
