#include <iostream>
#include <cctype>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>
#include <regex>
#include <iomanip>
#include <stdexcept>

using namespace std;
using dict = map<string, int>;

class myfunctionclass
{
  /*Tar in en radlängd och en sträng.
  Ser till att strängen inte blir längre
  än radlängden utan att bryta något ord.*/
public:
  //int counter;
  //int line_len;
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
private:
  int counter;
  int line_len;
};

void string_to_lowercase(string& s)
{
  //'tolower' fungerar endast på ASCII-tecken.
  transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void remove_from_beginning_of_string(string&s, string cv)
{
  //Tar bort alla tecken i 'cv' från början av 's', ett i taget.
  for (char c : cv)
  {
    while (s[0] == c)
    {
      s.erase(0, 1);
    }
  }
  if (cv.find(s[0]) != string::npos)
  {
    remove_from_beginning_of_string(s, cv);
  }
}

bool is_only_valid_chars(string const& s)
{
  //Kollar att alla tecken är alfanumeriska eller bindestreck ('-').
  return all_of(s.begin(), s.end(),
                [](char c){return (c == '-' || isalnum(c));});
}

bool only_legit_hyphens(string const& s)
{
  /*Kollar så att 's' inte startar eller
  slutar med bindestreck, eller har två
  (eller fler) bindestreck i rad.*/
  return !(regex_match (s, regex("-.*")) ||
	   regex_match (s, regex(".*-")) ||
	   regex_match (s, regex(".*--.*")));
}

bool is_size(string const& s)
{
  //Kontrollerar att 's' är minst tre tecken lång.
  return (s.size() >= 3);
}

bool check_validity(string& s)
{
  /*Kollar att 's' är rätt storlek,
  bara har alfanumeriska tecken eller bindestreck,
  och bara har ett bindestreck mellan bokstäver.*/
  return (is_size(s) &&
          is_only_valid_chars(s) &&
          only_legit_hyphens(s));
}

unsigned int get_longest_string_length(dict d)
{
  //Hämtar längden på den största strängen i 'd'.
  dict::iterator s = max_element(d.begin(), d.end(),
                             [](dict::value_type p1, dict::value_type p2)->
                             bool{return p1.first.size() < p2.first.size();});
  return s->first.size();
}

void remove_genitive(string& s)
{
  //Kollar om 's' har substrängen "'s" i sig, och tar bort det.
  if (s.size() > 2)
  {
    if (s.substr(s.size()-2, s.size()).find("'s") != string::npos)
    {
      s = s.substr(0, s.size()-2);
    }
  }
}

bool compare_sort(pair<string, int> p1, pair<string, int> p2)
{
  return (p1.second < p2.second);
}

int main()
{
  unsigned int longest;
  
  string taboo_begin{"\"("};
  string taboo_end{"!?;,:.)\"'"};
  
  vector<string> original_order;
  
  string filename;

  dict list;

  int line_len;

  vector<pair<string, int>> sorted;
  vector<pair<string, int>>::const_reverse_iterator iter;
  
  //Tar in namnet på filen vi vill ha, och en radlängd för utskrift.
  cout << "Mata in ett filnamn: " << flush;
  cin >> filename;

  cout << "Hur många tecken per rad? " << flush;
  cin >> line_len;

  myfunctionclass mfc(line_len);
  
  ifstream ifs(filename);
  istream_iterator<string> begin(ifs);
  istream_iterator<string> end;
  
  if ( !ifs.is_open() )
  {
    throw runtime_error("Filen kunde inte öppnas!");
  }

  for (istream_iterator<string> it = begin; it != end; ++it)
  {
    //'*it' är konstant, så vi måste spara den i en temporär variabel.
    string temp = *it;
    
    /*Tar bort tecknen i 'taboo_begin' från borjan av strängen
    vänder sedan på strängen, och tar bort tecknen från 'taboo_end'.*/
    remove_from_beginning_of_string(temp, taboo_begin);
    temp = string (temp.rbegin(), temp.rend());
    
    remove_from_beginning_of_string(temp, taboo_end);
    temp = string (temp.rbegin(), temp.rend());
    
    remove_genitive(temp);
    if (check_validity(temp))
    {
      /*Lägg in 'temp' i 'original_order',
      sedan gör om den till lowercase och lägg in i 'list'.*/
      original_order.push_back(temp);
      string_to_lowercase(temp);
      list[temp]++;
    }
  }
  
  longest = get_longest_string_length(list);

  /*Kopiera in hela 'list' i 'sorted',
  och sortera den sedan efter värdena i 'list'.*/
  copy(list.begin(), list.end(), back_inserter(sorted));
  stable_sort(sorted.begin(), sorted.end(), compare_sort);

  //Skriver ut 'sorted' i fallande ordning.
  cout << "Vanligaste ordet först:" << endl;
  cout << string(20, '-') << endl;
  
  for (iter = sorted.crbegin(); iter != sorted.crend(); ++iter)
  {
    cout << setw(longest) << iter->first << " : " << iter->second << endl;
  }
  
  cout << endl;
  
  /*Kör 'myfunctionclass' på varje string
  i 'original_order' och skriver sedan ut den.*/
  for_each(original_order.begin(), original_order.end(), mfc);
  
  cout << "Ursprunglig ordning:" << endl;
  cout << string(20, '-') << endl;
  
  for (string s : original_order)
  {
    cout << s << flush;
  }
  
  cout << endl;
  
  return 0;
}
