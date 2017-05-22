#include <fstream>
#include <iostream>
#include <vector>

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
  return ((is_smallest(zero, one, two)) && //'zero' is smallest
	  (is_inbetween(one, zero, two)) && //'one' is inbetween
	  (is_largest(two, zero, one))); //'two' is largest
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

vector<int> stovi(string convert) //String to vector<int>
{
  vector<int> return_vector;
  return_vector.push_back(convert[0] - '0');
  return_vector.push_back(convert[1] - '0');
  return_vector.push_back(convert[2] - '0');
  return return_vector;
}

bool sides_is_valid(vector<int> sides, int loops)
{
  bool is_valid{false};
  while (loops > 0 && !is_valid)
    {
      turn(sides);
      is_valid = vector_is_valid(sides.at(0), sides.at(1), sides.at(2));
      --loops;
    }
  return is_valid;
}

int main()
{
  vector<string> lines{getlines("testfil_up1.txt")};
  for (unsigned int i{0}; i < lines.size(); ++i)
    {
      vector<int> sides{stovi(lines.at(i))};
      if (check_validity(sides.at(0), sides.at(1), sides.at(2), 0, 5))
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
