#include <iostream>
#include <vector>

using namespace std;

int main()
{
  vector<int> v;
  for (int i = 0; i < 5; ++i)
    {
      for (int j = 1; j <= 10; ++j)
	{
	  v.push_back(j);
	}
    }
  for (int k : v)
    {
      cout << k << ' ';
    }
  cout << endl << endl;
  vector<int>::iterator i;
  for (i = v.begin(); i != v.end(); ++i)
    {
      if (*i == 7)
	{
	  v.erase(i);
	}
    }
  for (int k : v)
    {
      cout << k << ' ';
    }
  cout << endl;
  return 0;
}
