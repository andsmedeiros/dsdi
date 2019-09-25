#ifndef DSDI_HELPER_HPP
#define DSDI_HELPER_HPP

#include "provider.hpp"

namespace DSDI{
  namespace helper {
    template<typename Iface, typename ... Args>
    auto inject(Args ... args){
      return Provider<Iface, Args ...>::inject(args ...);
    }
  } /* helper */
}


#endif /* DSDI_HELPER_HPP */
