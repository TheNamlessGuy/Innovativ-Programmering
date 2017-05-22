#include <iostream>
#include <algorithm>

#include "linkedlist.h"

using namespace std;

LinkedList::LinkedList(): first(nullptr) {}

LinkedList::~LinkedList() 
{
  delete first;
}

LinkedList::LinkedList(LinkedList const& rhs): first(nullptr)
{
  if (rhs.first == nullptr)
  {
    return;
  }
  first = new Element(rhs.first->value);
  Element* this_current{first};
  Element* rhs_current{rhs.first};
  //While the list you're copying has an element left to copy
  while (rhs_current->pointer != nullptr)
  {
    /*Set this_current->pointer to a new element
      that has the same value as the element with
      the same index from rhs*/
    this_current->pointer = new Element(rhs_current->pointer->value);
    this_current = this_current->pointer;
    rhs_current = rhs_current->pointer;
  }
}

LinkedList::LinkedList(LinkedList&& rhs): first(rhs.first)
{
  rhs.first = nullptr;
}

void LinkedList::add(int i)
{
  if (first != nullptr)
  {
    //If the int to add is the smallest in the list
    if (first->value > i)
    {
      first = new Element(i, first);
      return;
    }
    //Returns the pointer you should add i to
    Element* current{first};
    search(i, current);
    current->pointer = new Element(i, current->pointer);
    return;
  }
  else
  {
    first = new Element(i);
  }
}
void LinkedList::search(int i, Element*& last) const
{
  /*If what last->pointer points to is nullptr,
  i is the largest value in the list
  If last->pointer->value < i, you should add i at last*/
  if (last->pointer != nullptr && last->pointer->value < i)
  {
    last = last->pointer;
    search(i, last);
  }
}
void LinkedList::remove(int i)
{
  if (first == nullptr)
  {
    return;
  }
  if (first->value == i)
  {
    first = delete_pointer(first);
    return;
  }
  Element* current_pointer{first};
  //Looks for the element that's before the element to remove, or if i is not in the list
  while (current_pointer->pointer != nullptr && current_pointer->pointer->value != i)
  {
    current_pointer = current_pointer->pointer;
  }
  //If you reached the end of the list, abort
  if (current_pointer->pointer != nullptr)
  {
    current_pointer->pointer = delete_pointer(current_pointer->pointer);
  }
}
LinkedList::Element* LinkedList::delete_pointer(Element*& del)
{
  //Removes an element and returns what it pointed at
  Element* pointer_after = del->pointer;
  del->pointer = nullptr;
  delete del;
  return pointer_after;
}
void LinkedList::print(string name) const
{
  string printstr{"["};
  Element* current_pointer{first};
  
  //Add all values in the list to printstr
  while (current_pointer != nullptr)
  {
    printstr += to_string(current_pointer->value) + ", ";
    current_pointer = current_pointer->pointer;
  }
  
  //Remove the last ", "
  printstr = printstr.substr(0, printstr.size()-2);
  printstr += "]";
  
  //If the name you put in is not empty, write it out
  if (name != "")
  {
    cout << name << ": " << printstr << endl;
  }
  else
  {
    cout << printstr << endl;
  }
}

LinkedList& LinkedList::operator= (LinkedList const& rhs)
{
  if (this != &rhs)
  {
    LinkedList copy = rhs;
    std::swap(copy.first, first);
  }
  return *this;
}

LinkedList& LinkedList::operator= (LinkedList&& rhs)
{
  if (this != &rhs)
  {
    std::swap(rhs.first, first);
  }
  return *this;
}

LinkedList::Element::Element(int i, Element* p): value(i), pointer(p)
{}

LinkedList::Element::~Element()
{
  delete pointer;
}
