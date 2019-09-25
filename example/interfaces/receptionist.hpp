#ifndef RECEPTIONIST_HPP
#define RECEPTIONIST_HPP

#include <string>

class Receptionist{
public:
  virtual void say(std::string speech) = 0;
  virtual void greet(std::string client) = 0;

  virtual ~Receptionist() = default;
};

#endif
