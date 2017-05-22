#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "connection.h"
#include "component.h"
#include "battery.h"
#include "resistor.h"
#include "capacitor.h"

using namespace std;

void clear_net(vector<Component*>& net)
{
  for (Component* c: net)
  {
    delete c;
    c = nullptr;
  }
  net.clear();
}

void reset_connections(Connection& n, Connection& p,
                       Connection& l, Connection& r)
{
  n.set_charge(0.0);
  p.set_charge(0.0);
  l.set_charge(0.0);
  r.set_charge(0.0);
}

void simulate(vector<Component*> net, int iterations,
              int number_of_prints, double time)
{
  cout << fixed << setprecision(2);
  for (Component* c: net)
  {
    cout << setw(11) << c->get_name() << ' ' << flush;
  }
  cout << endl;
  
  for (unsigned int i = 0; i < net.size(); ++i)
  {
    cout << setw(11) << "Volt  Curr" << ' ' << flush;
  }
  cout << endl;

  //int i = 1 because 0 % anything is 0
  for (int i = 1; i < iterations + 1; ++i)
  {
    for (Component* c: net)
    {
      c->update(time);
      if (i % (iterations / number_of_prints) == 0)
      {
        cout << setw(5) << c->get_voltage()
             << setw(6) << c->get_current()
             << ' ' << flush;
      }
    }
    if (i % (iterations / number_of_prints) == 0)
    {
      cout << endl;
    }
  }
}

int main(int argc, char* argv[])
{
  int iterations{0};
  int number_of_prints{0};
  double time{0};
  double battery_voltage{0};
  
  vector<string> arguments(argv + 1, argv + argc);
  
  try
  {
    iterations = stoi(arguments.at(0));
    number_of_prints = stoi(arguments.at(1));
    time = stod(arguments.at(2));
    battery_voltage = stod(arguments.at(3));
  }
  catch (exception& e)
  {
    cout << "Ett av de inmatade talen var fel" << endl
         << "(Iterations, number of prints, time, battery voltage)" << endl;
    cout << e.what() << endl;
    return 1;
  }
  
  Connection p, n, l, r;
  vector<Component*> net;

  //Testfall 1
  net.push_back(new Battery("Bat", battery_voltage, n, p));
  net.push_back(new Resistor("R1", 6.0, r, p));
  net.push_back(new Resistor("R2", 4.0, l, r));
  net.push_back(new Resistor("R3", 8.0, n, l));
  net.push_back(new Resistor("R4", 12.0, n, r));

  simulate(net, iterations, number_of_prints, time);
  clear_net(net);
  reset_connections(p, n, l, r);
  cout << endl << endl;

  //Testfall 2
  net.push_back(new Battery("Bat", battery_voltage, n, p));
  net.push_back(new Resistor("R1", 150.0, p, l));
  net.push_back(new Resistor("R2", 50.0, p, r));
  net.push_back(new Resistor("R3", 100, r, l));
  net.push_back(new Resistor("R4", 300, l, n));
  net.push_back(new Resistor("R5", 250, r, n)); 

  simulate(net, iterations, number_of_prints, time);
  clear_net(net);
  reset_connections(p, n, l, r);
  cout << endl << endl;

  //Testfall 3
  net.push_back(new Battery("Bat", battery_voltage, n, p));
  net.push_back(new Resistor("R1", 150.0, p, l));
  net.push_back(new Resistor("R2", 50.0, p, r));
  net.push_back(new Capacitor("C3", 1.0, r, l));
  net.push_back(new Resistor("R4", 300, l, n));
  net.push_back(new Capacitor("C5", 0.75, r, n));

  simulate(net, iterations, number_of_prints, time);
  clear_net(net);
  reset_connections(p, n, l, r);

  return 0;
}
