#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;
bool is_smallest(int smallest, int check1, int check2)
{
  return ((smallest <= check1) &&
	  (smallest <= check2));
}

bool is_largest(int largest, int check1, int check2)
{
  return ((largest >= check1) &&
	  (largest >= check2));
}

bool is_inbetween(int inbetween, int smaller, int larger)
{
  return ((inbetween >= smaller) &&
	  (inbetween <= larger));
}

bool check_validity(int check1, int check2, int check3,
                    int smallest, int largest)
{
  return ((is_inbetween(check1, smallest, largest)) &&
	  (is_inbetween(check2, smallest, largest)) &&
	  (is_inbetween(check3, smallest, largest)));
}

void turn(vector<int>& turner)
{
  turner.push_back(turner.at(0));
  turner.erase(turner.begin());
}

bool vector_is_valid(int zero, int one, int two)
{
  bool zero_is_smallest{(is_smallest(zero, one, two))};
  bool one_is_inbetween{(is_inbetween(one, zero, two))};
  bool two_is_largest{(is_largest(two, zero, one))};
  return (zero_is_smallest && two_is_largest && one_is_inbetween);
}

bool sides_is_valid(vector<int> sides, int loops)
{
  bool is_valid{false};
  while (loops != 0 && !is_valid)
    {
      turn(sides);
      is_valid = (vector_is_valid(sides.at(0), sides.at(1), sides.at(2)));
      --loops;
    }
  return is_valid;
}

vector<string> getlines(string file_loc)
{
  vector<string> return_vector;
  string line;
  ifstream file(file_loc);
  if (!file.is_open())
    {
      throw("You did it wrong");
    }
  while(getline(file, line))
    {
      return_vector.push_back(line);
    }
  file.close();
  return return_vector;
}

vector<int> get_sides_from_line(string line)
{
  vector<int> return_vector;
  istringstream iss(line);
  int zero;
  int one;
  int two;
  iss >> zero >> one >> two;
  return_vector.push_back(zero);
  return_vector.push_back(one);
  return_vector.push_back(two);
  return return_vector;
}

int main()
{
  string FILE_NAME{"testfil_up2.txt"};
  vector<string> lines{getlines(FILE_NAME)};
  for (unsigned int i{0}; i < lines.size(); ++i)
    {
      vector<int> sides{get_sides_from_line(lines.at(i))};
      if (check_validity(sides.at(0), sides.at(1), sides.at(2), -100, 100))
	{
	  bool is_valid{sides_is_valid(sides, 3)};
	  if (!is_valid)
	    {
	      cout << (i + 1) << endl;
	    }
	}
      else
	{
	  cout << (i + 1) << endl;
	}
    }
  return 0;
}
