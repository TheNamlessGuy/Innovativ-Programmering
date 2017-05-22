#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;
int main()
{
  float first_price{0.0f};
  float last_price{0.0f};
  float step{0.0f};
  float sales_tax_percentage{0.0f};
  float get_value(string start_string, int min_value, int max_value, string error_string);
  int price_len{12};
  int sales_tax_len{17};
  int sales_tax_price_len{20};
  int number_of_loops{0};
  
  cout << fixed << setprecision(2);
  cout << "INMATNINGSDEL\n" << string(12, '=') << endl;
  
  first_price = get_value("Mata in första pris: ", 0, numeric_limits<int>::max(), "FEL: Första pris måste vara mer än 0 (noll) kronor!");
  last_price = get_value("Mata in sista pris : ", first_price, numeric_limits<int>::max(), "FEL: Sista pris måste vara större än första pris (" + to_string(first_price) + ")!");
  step = get_value("Mata in steglängd  : ", 0, (last_price - first_price),
		    "FEL: Steglängden måste vara minst 0.01 och måste vara mindre eller lika med skillnaden mellan det första och det sista priset (" + to_string(last_price - first_price) + ")!");
  sales_tax_percentage = get_value("Mata in momsprocent: ", 0, 100, "FEL: Momsprocenten måste vara mellan 0 (noll) och 100 (hundra) procent!");
  
  cout << right << setfill(' ');
  cout << "\nMOMSTABELLEN\n" << string(12, '=') << endl
       << setw(price_len) << "Pris"
       << setw(sales_tax_len) << "Moms"
       << setw(sales_tax_price_len) << "Pris med moms" << endl
       << string(49, '-') << endl;
  number_of_loops = (last_price - first_price) / step;
  for (int i = 0; i <= number_of_loops; ++i)
    {
      float current_price = first_price + step * i;
      float sales_tax = (current_price / 100) * sales_tax_percentage;
      cout << setw(price_len) << current_price
	   << setw(sales_tax_len) << sales_tax
	   << setw(sales_tax_price_len) << (current_price + sales_tax) << endl;
    }
  return 0;
}

float get_value(string start_string, int min_value, int max_value, string error_string)
{
  float return_value{0};
  bool check{false};
  do
    {
      cout << start_string << flush;
      cin >> return_value;
      check = (min_value > return_value || return_value > max_value);
      if (check)
	{
	  cout << error_string << endl;
	}
    }while(check);
  return return_value; 
}
