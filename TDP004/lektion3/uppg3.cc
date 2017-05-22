#include <iostream>

using namespace std;

int main()
{
  cout << "Mata in ett flyttal: " << endl;
  double d;
  cin >> d;
  if (cin.fail())
    {
      cin.clear();
      cin.ignore(100000, '\n');
      cout << "Du har fel" << endl;
      cin >> d;
    }
  cout << d << endl;
  return 0;
}
