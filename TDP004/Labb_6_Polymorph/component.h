#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>

#include "connection.h"

class Component
{
public:
  Component(std::string const& na, double const& v,
            double const& c, Connection& p, Connection& n);
  virtual ~Component();

  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;

  virtual void update(double const& time) = 0;

  std::string get_name() const;
  double get_voltage() const;
  double get_current() const;
protected:
  std::string name;
  double voltage;
  double current;
  
  Connection* prev;
  Connection* next;
};

#endif
