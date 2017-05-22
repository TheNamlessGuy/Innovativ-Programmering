#include <iostream>

#include "component.h"

using namespace std;

Component::Component(string const& na, double const& v, double const& c,
                     Connection& p, Connection& n)
  : name(na), voltage(v), current(c), prev(&p), next(&n)
{}

Component::~Component()
{
  prev = nullptr;
  next = nullptr;
}

string Component::get_name() const
{
  return name;
}

double Component::get_voltage() const
{
  return voltage;
}

double Component::get_current() const
{
  return current;
}
