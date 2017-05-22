#include <algorithm>

#include "capacitor.h"
#include "component.h"

using namespace std;

Capacitor::Capacitor(string const& na, double const& c,
                     Connection& p, Connection& n)
  : Component(na, 0.0, 0.0, p, n), capacity(c), load(0)
{}

Capacitor::~Capacitor() 
{}

void Capacitor::update(double const& time)
{
  voltage = abs(prev->get_charge() - next->get_charge());
  current = capacity * (voltage - load);
  load += current * time;
  
  if (max(prev->get_charge(), next->get_charge()) == prev->get_charge())
  {
    prev->set_charge(prev->get_charge() - (current * time));
    next->set_charge(next->get_charge() + (current * time));
  }
  else
  {
    prev->set_charge(prev->get_charge() + (current * time));
    next->set_charge(next->get_charge() - (current * time));
  }
}
