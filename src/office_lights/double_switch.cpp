//============================================================================
// Name        : double_switch.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles light switch related functions
//============================================================================


// C library headers


#include "./double_switch.hpp"



DoubleSwitch::DoubleSwitch(){};

// Constructor with params
DoubleSwitch::DoubleSwitch( int id, int array_i, short value, short mode, short type, string name, string desc, float s_toggle_timer, float s_move_timer )
    : Switch(id, array_i, value,  mode, type, name, desc, s_toggle_timer, s_move_timer  ) {
        //if we call init, it runs twice, because of parent class
    //init(id, array_i, value, mode, type, name, desc);  
}


//Copy constructor
DoubleSwitch::DoubleSwitch( const DoubleSwitch &cp) : Switch(cp) // if we add params to child use ex ', newParam(cp.newParam)'
{}

//Copy Constructor Assignment
DoubleSwitch& DoubleSwitch::operator=(const DoubleSwitch& cp){
    Switch::operator= (cp);

    //Do the rest of assignment operator here, things are arent done in base class
    return *this;
}

//Deconstructor
DoubleSwitch::~DoubleSwitch(){
    cout<<"Destructing"<<endl;
    //Delete all move_timers

    //Not sure if this is necessary
    lights.clear();
    move_timers.clear();
    toggle_timers.clear();
    delay_timers.clear();

    lights.shrink_to_fit();
    move_timers.shrink_to_fit();
    toggle_timers.shrink_to_fit();
    delay_timers.shrink_to_fit();
}

void DoubleSwitch::updateSwitch(short value){
    cout<<"This should not run, use updateSwitch(short value, short value2) "<<endl;
    
}

void DoubleSwitch::updateSwitch(short value, short value2){
    cout<<"Switch"<<this->id<<": "<<value<<" & "<<value2<<endl;
    if(!checkLightsInit()){
        return;
    }

    this->value = value;

    if((value == 1 || value2 == 1) && mode ==0){
        this->setMoveTimer(float(DBL_MOVE_TIMER_TIME));
        this->setLight(short(1));
        cout<<"Updating DoubleSwitch 1 Lights to 1"<<endl;
    }
    if((value  == 1 || value2 == 1)&& mode == 1){
        //Reset Toggle Timer if movement
        this->setToggleTimer(float(DBL_TOGGLE_TIMER_TIME));
    }
    if(value == 2 && this->delay_timers[0]->getIsTimeUp()){
        this->mode = 1; //toggle mode
        //Clear timer and toggle
        this->setMoveTimer(0.00);
        this->setToggleTimer(float(DBL_TOGGLE_TIMER_TIME));
        this->toggleLight();
        this->delay_timers[0]->setTimerValue(float(DELAY_TIMER_TIME));
        cout<<"Updating DoubleSwitch 1 Lights to TOGGLE"<<endl;
    }

    
}

