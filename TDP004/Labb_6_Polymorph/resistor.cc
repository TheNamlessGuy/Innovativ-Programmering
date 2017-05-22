#include <algorithm>

#include "resistor.h"

using namespace std;

Resistor::Resistor(string const& na, double const& r,
                   Connection& p, Connection& n)
  : Component(na, 0.0, 0.0, p, n), resistance(r)
{}

Resistor::~Resistor()
{}

void Resistor::update(double const& time)
{
  voltage = abs(prev->get_charge() - next->get_charge());
  current = voltage / resistance;
  if (max(prev->get_charge(), next->get_charge()) == prev->get_charge())
  {
    prev->set_charge(prev->get_charge() - current * time);
    next->set_charge(next->get_charge() + current * time);
  }
  else
  {
    prev->set_charge(prev->get_charge() + current * time);
    next->set_charge(next->get_charge() - current * time);
  }
}
