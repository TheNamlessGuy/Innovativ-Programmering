#include <iostream>
#include "linkedlist.h"
using namespace std;

LinkedList make(int i, int j)
{
  LinkedList a;
  a.add(i);
  a.add(j);
  return a;
}

int main()
{
  LinkedList list;
  list.add(2);
  list.add(6);
  list.add(8);
  list.add(4);
  list.add(10);
  list.add(1);
  list.add(1);
  list.print("BEFORE"); //[1, 1, 2, 4, 6, 8, 10]
  list.remove(1);
  list.remove(50);
  list.print("AFTER"); //[1, 2, 4, 6, 8, 10]

  LinkedList list2 = list;
  list2.add(500);
  list2.print("LIST 2"); //[1, 2, 4, 6, 8, 10, 500]
  
  list.print("LIST 1"); //[1, 2, 4, 6, 8, 10]

  list = make(1, 2);
  list.print("LIST 1"); //[1, 2]

  LinkedList list3(make(3, 5));
  list3.print("LIST 3"); //[3, 5]

  list = list3;
  list.print("LIST 1"); //[3, 5]
  return 0;
}
