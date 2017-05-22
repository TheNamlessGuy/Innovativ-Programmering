#include <iostream>
#include "up1.h"
using namespace std;

void insert(Element* e, int i)
{
  e = new Element(i, e->next);
  cout << e->data << endl;
  cout << e->next->data << endl;
  cout << e->next->next->data << endl;
}

int main()
{
  Element* first = new Element(5, new Element(8, new Element(9, nullptr)));
  insert(first, 2);
  cout << endl;
  cout << first->data << endl;
  cout << first->next->data << endl;
  cout << first->next->next->data << endl;
}
