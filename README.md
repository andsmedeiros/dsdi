# **D**ead **S**imple **D**ependency **I**njection

**DSDI** aims to implement simple and direct DI in C++14, focusing on ease of use and readability.
It is fully implemented in headers and makes very few assumptions on the surrounding eco-system.
All library memory is allocated statically.

## Installation

Just copy the src/dsdi directory anywhere in your project.

## Basic usage

Include the *provider* header. It is the central piece in DSDI:

```C++
#include <dsdi/provider.hpp>
```

Then instruct the `Provider` of your interface that it `provides` some implementation:

```C++
DSDI::Provider<Iface>::provides<Impl>();
```
Later, when you need an `Iface`, `inject` it:

```C++
auto iface = DSDI::Provider<Iface>::inject();
iface->do_something();
Iface *iface_ptr = iface.get();
```

Here, `iface` is a std::unique_ptr with some *spice*.

## Advanced usage

### Interfaces ans implementations

An interface is an abstract class with a virtual destructor. It is meant to represent abilities of its descendants through virtual deleted methods.

```C++
class Animal{
public:
  virtual void make_noise() = 0;
  virtual ~Animal() = default;
};
```

An implementation is a class derived from an interface that implements its virtual methods.

```C++
class Dog : public Animal{
public:
  virtual void make_noise() override {
    std::cout << "bark bark!" << std::endl;
};

class Rabbit : public Animal{
public:
  virtual void make_noise() override {
    std::cout << "squeak squeak!" << std::endl;
  }
};

class Human : public Animal{
public:
  std::string name;
  Human(std::string name) : name(name) {}

  virtual void make_noise() override {
    std::cout << name << " says: 'blah blah!'" << std::endl;
  }
};
```

### Unparametrized provision

The simplest form of provision is unparametrized. It means the `Provider` doesn't need any more information in order to know what to supply the user when requested a particular interface.

```C++
DSDI::Provider<Animal>::provides<Dog>();
```

This will instruct the `Provider` to create a new `Dog` instance and handle it over when it is asked for an `Animal`.

Injecting from an unparametrized `Provider` is straightforward.

```C++
auto animal = DSDI::Provider<Animal>::inject();
```

### Parametrized provision

Under the hood, provisioning and injection involve creating implementation instances, and that means invoking constructors.

When a `Provider` is created, it can be bound to incoming parameters, so when you request an instance, it will know which constructor in the implementation to call.

```C++
DSDI::Provider<Animal, std::string>::provides<Human>();
```

This will instruct the `Provider`, when requested an `Animal` that needs a `std::string`, to provide a `Human` constructed with such string.

```C++
auto andre = DSDI::Provider<Animal, std::string>::inject("André");
andre->make_noise();  // prints: "André says: 'blah blah!'"
```

If the parameters are unambiguous, they can be ommited when injecting:

```C++
using Color = enum { RED, BLUE, GREEN, YELLOW };
class Bird : public Animal{
public:
  Color color;
  Bird(Color color) : color(color) {}

  virtual void make_noise () override{
    std::cout << "chirp chirp!" << std::endl;
  }
};

DSDI::Provider<Animal, Color>::provides<Bird>();
auto bird = DSDI::Provider<Animal>::inject(Color::BLUE);
```

### Singleton provision

DSDI can be instructed to provide the same instance over and over.

```C++
DSDI::Provider<Animal>::provides_single<Rabbit>();
auto rabbit1 = DSDI::Provider<Animal>::inject();
auto rabbit2 = DSDI::Provider<Animal>::inject();

assert(rabbit1.get() == rabbit2.get());
```

Singletons are eagerly created and therefore any constructor arguments must be present on provision time. Because of this, **parametrized providers cannot provide singleton instances**.

```C++
DSDI::Provider<Animal>::provides_single<Human>(std::string("Gustavo"));
auto human1 = DSDI::Provider<Animal>::inject();
auto human2 = DSDI::Provider<Animal>::inject();

assert(human1->name == human2->name);
```

### The `inject` helper

To allow for better reading, DSDI ships a `inject` helper function, that wraps the `Provider`'s own `inject` function. It is simpler and clearer to use it.

```C++
#include <dsdi/helper.hpp

using namespace DSDI::helper;

DSDI::Provider<Animal>::provides<Dog>();
auto dog = inject<Animal>(); // Same as DSDI::Provider<Animal>::inject();
```

### Injected properties

The return value of the `inject` function and the `inject` helper is an `InjectedProperty<Iface>`. It is actually a `std::unique_ptr` with custom deleters attached. Therefore, all `std::unique_ptr` APIs are available.

**Please don't try to manage the injected property's lifecycle manually!**

Although an `InjectedProperty` is actually a `std::unique_ptr`, it does not always behave as what is expected of it.
Because when we inject a property there is no way to know if it is singleton or not, it is the `InjectedProperty`'s attached deleter that will decide wether should it be actually deleted on smart pointer destruction.

If you **really** need to `release` your pointer, make sure it is not providing a singleton instance.


## Example

There is an example of some injection scenarios in /example. Run it to better understand how to provision and inject, as well as how lifecycle is managed.

    git clone https://github.com/andsmedeiros/dsdi && cd dsdi && make example

## Roadmap
  * Provide existing implementation instance
  * Attach custom factories and decimators to `Providers`
  * Ability to export `Container`s, which are portable, instance-based interfaces to a `Provider`
  * Write unit tests
