#ifndef SWITCH_HPP
#define SWITCH_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory>

#include <ctime>

#include "./constants.hpp"
#include "./light.hpp"
#include "./timer.hpp"



using namespace std;

// Fraction class
class Switch {

  // private: no access from outside
  protected:
    
    
    int id; //index of readbuf
    int array_index; //position in read_buf
    short value; // 0==no movement 1=movement 2=toggle
    short mode; // 0==no overrider 1=toggle override 
    short type; // 0==single 1==double switch
    string name;
    string description;

    //Light is weak_ptr because it lives in switch_handler
    vector< weak_ptr<Light> > lights;
    
    vector< shared_ptr<Timer> > move_timers; 
    vector< shared_ptr<Timer> > toggle_timers;  
    vector< shared_ptr<Timer> > delay_timers;

    bool checkLightsInit();

    public:
    // default constructor
    Switch();

    // constructor with params
    Switch( int, int, short, short, short,  string, string, float, float );

    void init(int , int , short, short, short , string , string, float, float );

    //Copy constructor
    Switch( const Switch &cp);

    //Copy assignment
    Switch& operator=(const Switch& cp);

    //Destructor
    ~Switch();

    // update
    virtual void updateSwitch(short) =0;
    //update with two values
    virtual void updateSwitch(short, short) = 0;

    //Timer 
    void updateTimer(float);
    void setMoveTimer(float);
    void setToggleTimer(float);
    void setDelayTimer(float);
    vector<float> getTimerValues();

    //Lights
    void setLight(short );
    void toggleLight();
    void addLightToSwitch( weak_ptr<Light> );
    

    //getters
    int getSwitchId();
    int getSwitchArrayIndex();
    short getSwitchValue();
    short getSwitchType(); //0=single 1=double
    short getModeValue(); //0 =motion 1=toggle
    string getSwitchName();
    string getSwitchDesc();
    vector<float> getToggleTimers();
    vector<float> getMoveTimers();

    //setters 
    void setSwitchId(int);
    void setSwitchArrayIndex(int);
    void setSwitchValue(short);
    void setModeValue(short); //0=motion 1=toggle
    void setSwitchName(string);
    void setSwitchDescription(string); 

};

#endif    /* SWITCH_HPP */