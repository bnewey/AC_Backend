#ifndef SWITCHHANDLER_HPP
#define SWITCHHANDLER_HPP

#include <vector>

struct Switch{
    short value; // 0 = no movement 1=movement 2=toggle 
    float timer;
    bool light_value;
};


// Fraction class
class SwitchHandler {

  // private: no access from outside
  private:
    //Timers will be added/removed multiple times
    


    Switch switch1;
    Switch switch2;
    Switch switch3;
    Switch switch4;
    Switch switch5;
    Switch switch6;
    Switch switch7;
    Switch switch8;
    Switch switch9;
    Switch switch10;




  public:
    // default constructor
    SwitchHandler();

    // constructor from int (denominator)
    //SwitchHandler( settings from db );

    // update
    void updateSwitches(short, short,short,short,short,short,short,short,short,short);

    //Timer 
    void updateTimer(float);
    int toggleLight(int);

    //getters
    bool getLight1();
    bool getLight2();
    bool getLight3();
    bool getLight4();
    bool getLight5();
    bool getLight6();
    bool getLight7();
    bool getLight8();
    bool getLight9();
    bool getLight10();

    //setters  

};

#endif    /* SWITCHHANDLER_HPP */