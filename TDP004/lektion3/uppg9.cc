#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main()
{
  string s;
  vector<int> v(istream_iterator<int>(cin), istream_iterator<int>());
  cin >> s;
  vector<int> square;
  for (int i{0}; i < 10; ++i)
    {
      square.push_back(i);
    }
  for (int& value : square)
    {
      cout << value << endl;
    }
  cout << "hallå" << endl;
  return 0;
}
