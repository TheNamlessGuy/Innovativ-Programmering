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

void remove_genitive(string& s)
{
  if (s.substr(s.size()-2, s.size()).find("'s") != string::npos)
  {
    s = s.substr(0, s.size()-2);
  }
}

int main()
{
  cout << boolalpha;
  unsigned int longest;
  vector<char> taboo_begin{get_taboo_chars("beginchars.txt")};
  vector<char> taboo_end{get_taboo_chars("endchars.txt")};
  string word;
  string filename;
  dict list;
  dict::const_iterator iter;
  
  cout << "Mata in ett filnamn: " << flush;
  cin >> filename;
  
  ifstream ifs(filename);

  if ( !ifs.is_open() )
  {
    throw;
  }
  
  while ( ifs >> word )
  {
    string_to_lowercase(word);
    remove_from_begin_of_string(word, taboo_begin);
    remove_from_end_of_string(word, taboo_end);
    remove_genitive(word);
    if (check_validity(word))
    {
      list[word]++;
    }
  }
  
  longest = get_longest_string_length(list);

  cout << "Orden alfabetiskt:" << endl;
  for (iter = list.begin(); iter != list.end(); ++iter)
  {
    cout << setw(longest) << iter->first << " : " << iter->second << endl;
  }
  
  return 0;
}
