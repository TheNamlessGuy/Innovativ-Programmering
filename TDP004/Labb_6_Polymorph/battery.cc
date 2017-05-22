#include "battery.h"
#include "connection.h"

using namespace std;

Battery::Battery(string const& na, double const& v,
                 Connection& p, Connection& n)
  : Component(na, v, 0.0, p, n)
{}

Battery::~Battery()
{}

void Battery::update(double const& time)
{
  (void)time;
  prev->set_charge(0);
  next->set_charge(voltage);
}
