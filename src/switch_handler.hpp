#ifndef SWITCHHANDLER_HPP
#define SWITCHHANDLER_HPP

#include <vector>

// class SingleSwitch;
// class DoubleSwitch;
// #include "./switch.hpp"
// #include "./light.hpp"

#include "./light.hpp"
#include "./switch.hpp"
#include "./single_switch.hpp"
#include "./double_switch.hpp"

// class Switch;
// #include "./single_switch.hpp"
// #include "./double_switch.hpp"
// #include "./light.hpp"

using namespace std;

// Fraction class
class SwitchHandler {

  // private: no access from outside
  private:
    //Timers will be added/removed multiple times
    int test_var;
    vector<Switch *> switches; 
    vector<Light *> lights;

    void init( int );

  public:
    // default constructor
    SwitchHandler();

    // constructor from int (denominator)
    SwitchHandler( int );

    //Copy Constructor
    SwitchHandler(const SwitchHandler  &cp);

    //Copy Constructor Assignment
    SwitchHandler& operator=(const SwitchHandler& cp);

    //deconstructor
    ~SwitchHandler();

    // update
    void updateSwitches(vector<short>);

    //Timer 
    void updateTimers(float);
    
    

    //getters
    vector<short> getLightValues();

    //setters  

};

#endif    /* SWITCHHANDLER_HPP */