#ifndef HIRED_RECEPTIONIST_HPP
#define HIRED_RECEPTIONIST_HPP

#include <cstdlib>
#include "interfaces/receptionist.hpp"

class HiredReceptionist : public Receptionist {
private:
  std::string name;

public:
  HiredReceptionist(std::string name) :
    name(name) {

    std::cout << "Receptionist " << name << " was hired!" << std::endl;
  }

  virtual ~HiredReceptionist(){
    std::cout << "Receptionist " << name << " was fired." << std::endl;
  }

  virtual void say(std::string speech) override {
    std::cout << "Receptionist " << name << " says: '" << speech << "'" << std::endl;
  }

};

#endif /* end of include guard: HIRED_RECEPTIONIST_HPP */
