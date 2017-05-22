#include <iostream>
#include <sstream>

using namespace std;

int main()
{
  char a;
  char b;
  char c;
  string hej = "Tralalala";
  cout << hej << endl;
  istringstream iss;
  iss.str(hej);
  cout << hej << endl;
  iss >> a >> b >> c;
  cout << a << endl;
  ostringstream oss;
  hej = oss.str();
  cout << hej << endl;
  return 0;
}
