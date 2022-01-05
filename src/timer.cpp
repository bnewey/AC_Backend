//============================================================================
// Name        : timer.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles Timer related functions
//============================================================================


// C library headers


#include "./timer.hpp"



//Default Constructor
Timer::Timer(){

    //(un)provided settings

    //
}

// Constructor with int  current_mode passed
Timer::Timer( int id, float value) 
  : id(id) , value(value)
  {
    this->isTimeUp = true;
}

//Copy constructor
Timer::Timer(const Timer &cp){
    this->id = cp.id; 
    this->value = cp.value; 
    this->isTimeUp = cp.isTimeUp;
}


bool Timer::updateTimer(float seconds_passed){
    if(value > 0){
        value -=float(seconds_passed);
        if(value <= 0){
            // TODO return value later
            this->isTimeUp = true;
            return true;
        }
        //cout<<"Timer: "<<value<<endl;
    }
    return false;
}


//getters
int Timer::getTimerId(){
    return this->id;
}

short Timer::getTimerValue(){
    return this->value;
}

bool Timer::getIsTimeUp(){
    return this->isTimeUp;
}

//setters
void Timer::setTimerId(int id){
    if(id < 0){
        return;
    }
    this->id = id;
}
void Timer::setTimerValue(short value){
    if(value < 0){
        return;
    }
    this->isTimeUp = false;
    this->value = value;
}

