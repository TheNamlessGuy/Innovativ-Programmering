#ifndef _RESISTOR_H_
#define _RESISTOR_H_

#include "component.h"
#include "connection.h"

class Resistor : public Component
{
public:
  Resistor(std::string const& na, double const& r,
           Connection& p, Connection& n);
  ~Resistor();

  void update(double const& time);
private:
  double resistance;
};

#endif
