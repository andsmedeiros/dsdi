#ifndef POLITE_GREETER_HPP
#define POLITE_GREETER_HPP

#include "hired-receptionist.hpp"

class PoliteReceptionist : public HiredReceptionist{
public:
  PoliteReceptionist(std::string name) : HiredReceptionist(name) {}

  virtual void greet(std::string client) override {
    say("Hello, " + client + "!");
  }

};
#endif
