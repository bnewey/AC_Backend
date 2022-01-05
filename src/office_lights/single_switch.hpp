#ifndef SINGLESWITCH_HPP
#define SINGLESWITCH_HPP

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
class SingleSwitch : public Switch{

  // private: no access from outside
    private:
    


    public:
    // default constructor
    SingleSwitch();

    // constructor with params
    SingleSwitch( int, int, short, short,short,  string, string, float, float);

    //Copy constructor
    SingleSwitch( const SingleSwitch &cp);

    //Copy assignment
    SingleSwitch& operator=(const SingleSwitch& cp);

    //Destructor
    ~SingleSwitch();

    // update

    void updateSwitch(short); //used

    void updateSwitch(short, short); //not used, has to be here to work on double_switch
    



};

#endif    /* SINGLESWITCH_HPP */