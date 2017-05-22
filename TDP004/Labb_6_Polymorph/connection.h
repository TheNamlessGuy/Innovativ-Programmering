#ifndef _CONNECTION_H_
#define _CONNECTION_H_

class Connection
{
public:
  Connection();
  ~Connection();
  
  void set_charge(double const& value);
  double get_charge() const;
private:
  double charge;
};

#endif
