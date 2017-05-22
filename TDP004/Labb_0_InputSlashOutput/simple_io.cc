#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
  string input_str;
  int input_int;
  float input_float;
  char input_char;
  
  
  
  cout << fixed;
  cout << setprecision(3);
  
  //Integer
  cout << "Skriv in ett heltal: " << flush;
  cin >> input_int;
  cout << "Du skrev in talet: " << input_int << "\n" << endl;
  cin.ignore(1000000, '\n');
  
  //5 integers
  cout << "Skriv in fem heltal: " << flush;
  for (int i = 0; i < 5; i++)
    {
      cin >> input_int;
      input_str += to_string(input_int) + ", ";
    }
  input_str = input_str.substr(0, input_str.size()-2);
  cout << "Du skrev in talen: " << input_str << "\n" << endl;
  cin.ignore(1000000, '\n');
  
  //Float
  cout << "Skriv in ett flyttal: " << flush;
  cin >> input_float;
  cout << "Du skrev in flyttalet: " << input_float << "\n" << endl;
  cin.ignore(1000000, '\n');
  
  //Integer and float
  cout << "Skriv in ett heltal och ett flyttal: " << flush;
  cin >> input_int >> input_float;
  cout << "Du skrev in heltalet: " << input_int << endl;
  cout << "Du skrev in flyttalet: " << input_float << "\n" << endl;
  cin.ignore(1000000, '\n');
  
  //Char
  cout << "Skriv in ett tecken: " << flush;
  cin >> input_char;
  cout << "Du skrev in tecknet: " << input_char << "\n" << endl;
  cin.ignore(1000000, '\n');
  
  //String
  cout << "Skriv in en sträng: " << flush;
  cin >> input_str;
  cout << "Strängen '" << input_str << "' har " << input_str.size() << " tecken.\n" << endl;
  cin.ignore(1000000, '\n');
  
  //String and integer
  cout << "Skriv in ett heltal och en sträng: " << flush;
  cin >> input_int >> input_str;
  cout << "Du skrev in talet |" << input_int << "| och strängen |" << input_str << "|.\n" << endl;
  cin.ignore(1000000, '\n');
  
  //String and float
  cout << "Skriv in en sträng och ett flyttal: " << flush;
  cin >> input_str >> input_float;
  cout << "Du skrev in \"" << input_float << "\" och \"" << input_str << "\".\n" << endl;
  cin.ignore(1000000, '\n');
  
  //Sentence
  cout << "Skriv in en hel rad med text: " << flush;
  getline(cin, input_str, '\n');
  cout << "Du skrev in textraden: '" << input_str << "'\n" << endl;
  
  //Sentence and a negative integer
  cout << "Skriv in en textrad som slutar med ett negativt heltal: " << flush;
  getline(cin, input_str, '-');
  cin >> input_int;
  input_int = -input_int;
  cout << "Du skrev in textraden: '" << input_str << "' och heltalet '" << input_int << "'" << endl;
  return 0;
}
