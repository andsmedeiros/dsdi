#include <iostream>
#include <cassert>
#include <dsdi/provider.hpp>
#include <dsdi/helper.hpp>
#include "interfaces/receptionist.hpp"
#include "components/polite-receptionist.hpp"
#include "components/rude-receptionist.hpp"

using namespace DSDI::helper;

int main()
{
  DSDI::Provider<Receptionist, std::string>::provides<PoliteReceptionist>();
  auto julia = DSDI::Provider<Receptionist, std::string>::inject("Júlia");
  julia->greet("André");

  DSDI::Provider<Receptionist>::provides<RudeReceptionist>();
  auto rude_receptionist = inject<Receptionist>();
  rude_receptionist->greet("Guilherme");

  {
    auto virginia = inject<Receptionist, std::string>("Virgínia");
    virginia->greet("Joana");
  }

  auto alberto = inject<Receptionist>(std::string("Alberto"));
  alberto->greet("Caio");

  DSDI::Provider<Receptionist>::provides_single<PoliteReceptionist>(std::string("Estagiário"));
  auto receptionist1 = inject<Receptionist>();
  auto receptionist2 = inject<Receptionist>();

  receptionist1->greet("Vinícius");
  receptionist2->greet("Larissa");

  assert(receptionist1.get() == receptionist2.get());

  std::cout << "Exitting" << std::endl;
}
