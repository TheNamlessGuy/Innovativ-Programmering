#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

class LinkedList
{
public:

  LinkedList();
  ~LinkedList();
  LinkedList(LinkedList const&);
  LinkedList(LinkedList&&);

  void add(int i);
  void remove(int i);
  void print(std::string name) const;
  
  LinkedList& operator= (LinkedList const&);
  LinkedList& operator= (LinkedList&&);

private:

  class Element
  {
  public:

    Element(int i, Element* p = nullptr);
    ~Element();
    Element(Element const&);
    Element(Element&&);

    Element& operator= (Element const&);
    Element& operator= (Element&&);

    int value;
    Element* pointer;
  };

  Element* first;

  void search(int i, Element*& last) const;
  Element* delete_pointer(Element*& del);
};

#endif
