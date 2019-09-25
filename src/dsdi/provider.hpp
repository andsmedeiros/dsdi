#ifndef DSDI_PROVIDER_HPP
#define DSDI_PROVIDER_HPP

#include <memory>
#include <cassert>
#include <cstdlib>
#include <functional>

namespace DSDI {
  template<typename Iface, typename ... Args>
  class Provider{
  public:
    using InjectedProperty = std::unique_ptr<Iface, std::function<void(Iface *)>>;
    using State = enum { UNSUPPLIED, SUPPLY_SINGLE, SUPPLY_MANY };

    static State state;

  private:
    using Referencer = Iface *(Args ...);
    using Releaser = void(Iface *);

    static Referencer *referencer;
    static Releaser *releaser;
    static InjectedProperty singleton;

    // Releasers
    static void destroy_instance(Iface *instance){
      delete instance;
    }
    
    static void nop(Iface *instance){}

    // Referencers
    template<typename Impl>
    static Iface *make_instance(Args ... args){
      return static_cast<Iface *>(new Impl(std::forward<Args>(args) ...));
    }

    static Iface *get_singleton(Args ... args){
      return singleton.get();
    }

    template<typename Impl>
    static constexpr void provision_assertions(){
      static_assert(
        std::is_same<Iface, Impl>::value || std::is_base_of<Iface, Impl>::value,
        "Cannot provide an interface from a class that does not implement it."
      );
      static_assert(
        std::is_same<Iface, Impl>::value || std::has_virtual_destructor<Iface>::value,
        "When providing an interface from a derived class, the interface destructor must be virtual."
      );
    }


  public:
    static InjectedProperty inject(Args ... args){
      assert(
        referencer != nullptr &&
        "Attempted to inject an interface from an unsupplied provider."
      );
      Iface *instance = (*referencer)(args ...);
      return InjectedProperty{ instance, *releaser };
    }

    template<typename Impl>
    static void provides(){
      provision_assertions<Impl>();
      referencer = &make_instance<Impl>;
      releaser = &destroy_instance;
    }

    template<typename Impl, typename ... CtorArgs>
    static void provides_single(CtorArgs ... args){
      static_assert(
        sizeof...(Args) == 0,
        "Cannot provide singleton instance from a parametrized provider."
      );
      provision_assertions<Impl>();
      singleton.reset(new Impl(args ...));
      referencer = &get_singleton;
      releaser = &nop;
    }

  };

  template<typename Iface, typename ... Args>
  typename Provider<Iface, Args ...>::Referencer *Provider<Iface, Args ...>::referencer = nullptr;
  template<typename Iface, typename ... Args>
  typename Provider<Iface, Args ...>::Releaser *Provider<Iface, Args ...>::releaser = nullptr;
  template<typename Iface, typename ... Args>
  typename Provider<Iface, Args ...>::InjectedProperty Provider<Iface, Args ...>::singleton = Provider<Iface, Args ...>::InjectedProperty{nullptr, destroy_instance};
  template<typename Iface, typename ... Args>
  typename Provider<Iface, Args ...>::State Provider<Iface, Args ...>::state = Provider<Iface, Args ...>::State::UNSUPPLIED;
} /* DSDI */

#endif /* end of include guard: DSDI_PROVIDER_HPP */
