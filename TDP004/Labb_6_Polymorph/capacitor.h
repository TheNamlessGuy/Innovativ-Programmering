#ifndef _CAPACITOR_H_
#define _CAPACITOR_H_

#include <string>
#include "component.h"

class Capacitor: public Component
{
public:
  Capacitor(std::string const& name, double const& capacity,
            Connection& p, Connection& n);
  ~Capacitor();

  void update(double const& time);
private:
  double capacity;
  double load;
};

#endif
