#include <iostream>
#include <cctype>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>
#include <regex>
#include <iomanip>

using namespace std;
using dict = map<string, int>;

class myfunctionclass
{
public:
  int counter;
  int line_len;
  myfunctionclass(int i): counter(0), line_len(i)
  {}
  void operator() (string& s)
  {
    s += ' ';
    counter += (s.size());
    if (counter >= line_len)
    {
      s += '\n';
      counter = 0;
    }
  };
};

void string_to_lowercase(string& s)
{
  transform(s.begin(), s.end(), s.begin(), ::tolower);
  //funkar inte på å ä ö
}

void remove_from_begin_of_string(string& s, vector<char> cv)
{
  for (char c : cv)
  {
    while (s[0] == c)
    {
      s.erase(0, 1);
    }
  }
}

void remove_from_end_of_string(string& s, vector<char> cv)
{
  for (char c : cv)
  {
    while (s[s.size()-1] == c)
    {
      s.erase(s.size()-1, s.size());
    }
  }
}

vector<char> get_taboo_chars(string filename)
{
  vector<char> return_vector;
  ifstream ifs(filename);
  char c;
  while ( ifs >> c )
  {
    return_vector.push_back(c);
  }
  return return_vector;
}

bool is_only_valid_chars(string& s)
{
  for (char c : s)
  {
    if (!(c == '-' || isalnum(c)))
    {
      return false;
    }
  }
  return true;
}

bool only_legit_hyphens(string& s)
{
  return !(regex_match (s, regex("-.*")) || //Starts with '-'
          regex_match (s, regex(".*-")) || //ends with '-'
          regex_match (s, regex(".*--.*"))); //'--' somewhere
}

bool is_size(string& s)
{
  int counter{0};
  for (char c : s)
  {
    if (isalpha(c))
    {
      counter++;
    }
  }
  return (counter >= 3);
}

bool check_validity(string& s)
{
  return (is_size(s) &&
          is_only_valid_chars(s) &&
          only_legit_hyphens(s));
}

unsigned int get_longest_string_length(dict d)
{
  dict::iterator s = max_element(d.begin(), d.end(),
                                 [](dict::value_type p1, dict::value_type p2)->bool{return p1.first.size() < p2.first.size();});
  return s->first.size();
}

bool compare_sort(pair<string, int> p1, pair<string, int> p2)
{
  return (p1.second < p2.second);
}

int main()
{
  cout << boolalpha;
  vector<char> taboo_begin{get_taboo_chars("beginchars.txt")};
  vector<char> taboo_end{get_taboo_chars("endchars.txt")};
  vector<string> original_order;
  string word;
  string filename;
  int line_len;
  
  cout << "Mata in ett filnamn: " << flush;
  cin >> filename;

  cout << "Hur många tecken per rad? " << flush;
  cin >> line_len;

  myfunctionclass mfs(line_len);
  
  ifstream ifs(filename);
  
  if ( !ifs.is_open() )
  {
    throw;
  }

  while ( ifs >> word )
  {
    remove_from_begin_of_string(word, taboo_begin);
    remove_from_end_of_string(word, taboo_end);
    if (check_validity(word))
    {
      original_order.push_back(word);
    }
  }
  
  cout << "Ursprunglig ordning:" << endl;
  for_each(original_order.begin(), original_order.end(), mfs);
  for (string s : original_order)
  {
    cout << s << flush;
  }
  cout << endl;
  return 0;
}
