#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main()
{
  cout << fixed << setprecision(4);
  ifstream ifs("cool.txt");

  if ( ! ifs )
  {
     cerr << "Kunde inte öppna swag'." << endl;
     return 1; // terminate program with error code 1
  }

  double nr;
  double sum{0.0};
  int count{0};
  while (ifs >> nr)
  {
     cout << nr << endl;
     sum += nr;
     ++count;
  }
  cout << endl;
  double average = sum / count;
  cout << average << endl;
  return 0;
}
