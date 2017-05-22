#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>

using namespace std;

bool in_order(int smallest, int in_between, int largest)
{
  /*
   * Checks if smallest <= in_between <= largest
   */
  return ((smallest <= in_between) &&
          (in_between <= largest));
}

bool check_ranges(vector<int> sides,
                  int smallest, int largest)
{
  /*
   * Checks if all the values in 'sides'
   * are between 'smallest' and 'largest'
   */
  return ((in_order(smallest, sides.at(0), largest)) &&
	  (in_order(smallest, sides.at(1), largest)) &&
	  (in_order(smallest, sides.at(2), largest)));
}

void turn(vector<int>& turner)
{
  /*
   * Puts the first value last in 'turner'
   */
  turner.push_back(turner.at(0));
  turner.erase(turner.begin());
}

bool sides_is_valid(vector<int> sides, int loops)
{
  /*
   * Checks if, at any point, sides[0] <= sides[1] <= sides[2]
   * and if not it turns so that sides[0] becomes sides[2]
   */
  bool is_valid{false};
  while (loops != 0 && !is_valid)
  {
    turn(sides);
    is_valid = in_order(sides.at(0), sides.at(1), sides.at(2));
    --loops;
  }
  return is_valid;
}

vector<int> get_sides_from_line(string line, int max)
{
  /*
   * Gets three ints from 'line' and returns them in 'return_vector'
   * If it can't read three ints, it fills the rest out with 'max + 1'
   */
  vector<int> return_vector;
  istringstream iss(line);
  int number{0};
  int counter{0};
  
  while (counter < 3 && iss >> number)
  {
    return_vector.push_back(number);
    ++counter;
  }
  
  if (return_vector.size() < 3)
  {
    for (int i{counter}; i < 3; ++i)
    {
      return_vector.push_back(max+1);
    }
  }
  return return_vector;
}

void get_int_from_user(int& i, string const& message)
{
  /*
   * Prompts the user with 'message' and reads in 'i'
   */
  cout << message << flush;
  
  if (!(cin >> i))
  {
    cin.clear();
    cin.ignore(10000000, '\n');
    cout << "Talet kunde inte läsas in" << endl;
    get_int_from_user(i, message);
  }
}

void get_min_max(int& min, int& max)
{
  /*
   * Prompts the user to input the min and max values
   */
  get_int_from_user(min, "Skriv in det minsta värdet: ");
  get_int_from_user(max, "Skriv in det största värdet: ");

  if (min >= max)
  {
    cout << "Max has to be at least 1 (one) larger than min" << endl;
    get_min_max(min, max);
  }
}

int main()
{
  ifstream ifs("testfil_up3.txt");
  int min{0};
  int max{0};
  int line_index{0};
  
  if (!ifs.is_open())
  {
    throw runtime_error("File not found");
  }

  get_min_max(min, max);
  
  for (string line{""}; getline(ifs, line);)
  {
    line_index++;
    vector<int> sides{get_sides_from_line(line, max)};
    
    if (check_ranges(sides, min, max))
    {
      if (!sides_is_valid(sides, 3))
      {
        cout << line_index << endl;
      }
    }
    else
    {
      cout << line_index << endl;
    }
  }
  return 0;
}
