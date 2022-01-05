#ifndef DOUBLESWITCH_HPP
#define DOUBLESWITCH_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <ctime>

// #include "./timer.cpp"
// #include "./light.cpp"
#include "./constants.hpp"
#include "./switch.hpp"

using namespace std;


// Fraction class
class DoubleSwitch : public Switch{

  // private: no access from outside
    private:
    


    public:
    // default constructor
    DoubleSwitch();

    // constructor with params
    DoubleSwitch( int, int, short, short,short,  string, string, float, float);

    //Copy constructor
    DoubleSwitch( const DoubleSwitch &cp);

    //Copy assignment
    DoubleSwitch& operator=(const DoubleSwitch& cp);

    //Destructor
    ~DoubleSwitch();

    // update

    void updateSwitch(short); //used

    void updateSwitch(short, short); //not used, has to be here to work on double_switch
    



};

#endif    /* DOUBLESWITCH_HPP */