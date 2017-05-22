#include <exception>
#include <string>

#include "console.h"

int main()
{
  Console c;
  
  const std::string msg = "Hello Colors";
  int idx = 0;
  
  for (int i = 0; i < 8*3; ++i)
  {
    for (int j = 0; j < 8*4; ++j)
    {
      c.setPos(j,i);
      c.setForegroundColor(i/3);
      c.setBackgroundColor(j/4);
      c.put(msg.at(idx));
      idx = (idx + 1) % msg.size();
    }
  }
  
// To see what happen on error
//  c.setBackgroundColor(90);
  
  while ( c.get() != 'q' )
    ;
  
  return 0;
}
