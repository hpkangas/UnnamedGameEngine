#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

enum ComponentId { TCID = 0, RCID = 1, CCID = 2, GCID = 3, ACID = 4 };

class IComponent {
public:
  int ID = -1;
  int parentID = -1;
  virtual ~IComponent() {};
};

#endif