#ifndef RUDE_RECEPTIONIST_HPP
#define RUDE_RECEPTIONIST_HPP

#include "hired-receptionist.hpp"

class RudeReceptionist : public HiredReceptionist{
public:
  RudeReceptionist() : HiredReceptionist("Ballsface") {
    std::cout << "He was so rude he didn't even tell his name." << std::endl;
  }

  virtual void greet(std::string client) override {
    say("Hello, scumbag!");
  }
};

#endif
