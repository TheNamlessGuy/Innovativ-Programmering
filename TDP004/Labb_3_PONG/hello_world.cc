#include "console.h"

int main()
{
  Console c;
  
  c.setPos(1,1);
  c.put("Hello World");

  while ( c.get() != 'q' )
    ;

  return 0;
}
