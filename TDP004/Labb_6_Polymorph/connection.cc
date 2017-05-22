#include "connection.h"

Connection::Connection()
  : charge(0)
{}

Connection::~Connection()
{}

void Connection::set_charge(double const& value)
{
  charge = value;
}

double Connection::get_charge() const
{
  return charge;
}
