#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "component.h"
#include "connection.h"

class Battery : public Component
{
public:
  Battery(std::string const& name, double const& voltage,
          Connection& p, Connection& n);
  ~Battery();
  
  void update(double const& time);
};

#endif
