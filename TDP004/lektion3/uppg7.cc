#include <iostream>
#include <vector>
#include <map>

using namespace std;

void print_vector(vector<int> v)
{
  for (int i : v)
    {
      cout << i << ' ';
    }
  cout << endl;
}

int main()
{
  map<string, vector<int>> my_map;
  vector<int> hej;
  hej.push_back(4);
  my_map["tjena"] = hej;
  my_map["hej"] = hej;
  map<string, vector<int>>::iterator iter;
  
  cout << "BEFORE:" << endl;
  for (iter = my_map.begin(); iter != my_map.end(); ++iter)
    {
      print_vector(iter->second);
    }
  
  for (iter = my_map.begin(); iter != my_map.end(); ++iter)
    {
      (iter -> second).push_back(3);
    }
  
  cout << endl << "AFTER:" << endl;
  for (iter = my_map.begin(); iter != my_map.end(); ++iter)
    {
      print_vector(iter->second);
    }
  return 0;
}
