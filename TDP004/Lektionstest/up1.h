class Element
{
 public:
  Element(int i, Element* e)
    {
      data = i;
      next = e;
      //next = this; //works
    }
  ~Element()
    {
      delete next;
    }
  int data;
  Element* next;
};
