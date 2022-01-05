//============================================================================
// Name        : switch.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles light switch related functions
//============================================================================


// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <ctime>

#include "./tmpSwitch.hpp"


using namespace std;

//Default Constructor
SwitchHandler::SwitchHandler(){

    switch1.value = 0;
    switch1.timer = 0;
    switch1.light_value = 0;
    switch2.value = 0;
    switch2.timer = 0;
    switch2.light_value = 0;
    switch3.value = 0;
    switch3.timer = 0;
    switch3.light_value = 0;
    switch4.value = 0;
    switch4.timer = 0;
    switch4.light_value = 0;
    switch5.value = 0;
    switch5.timer = 0;
    switch5.light_value = 0;
    switch6.value = 0;
    switch6.timer = 0;
    switch6.light_value = 0;
    switch7.value = 0;
    switch7.timer = 0;
    switch7.light_value = 0;
    switch8.value = 0;
    switch8.timer = 0;
    switch8.light_value = 0;
    switch9.value = 0;
    switch9.timer = 0;
    switch9.light_value = 0;
    switch10.value = 0;
    switch10.timer = 0;
    switch10.light_value = 0;

    //(un)provided settings


    //
}

// // Constructor with int  current_mode passed
// SwitchHandler::SwitchHandler( float a, float b, float c, float d, float e, float f, float g,
//   float h, int i, int j, int k, int l, int m, int n) 
//   : TIMER_MODE2_WAIT(a), TIMER_MODE4_WAIT(b), TIMER_MOTOR_RELAY(c), TIMER_START_RELAY(d), TIMER_STOP_RELAY(e),
//     TIMER_SHUT_DOWN_COUNTER(f), TIMER_BLEED_RELAY_M45(g), TIMER_BLEED_RELAY_M1(h), 
//     MAX_HIGH_PRESSURE(i), HIGH_PRESSURE_THRESHOLD(j), MAX_LOW_PRESSURE(k), 
//     LOW_PRESSURE_THRESHOLD(l), MIN_LOW_PRESSURE(m), SHUT_DOWN_COUNTER_MAX(n)
//   {
//     //start with stop mode 1

// }



void SwitchHandler::updateTimer(float seconds_passed){
    cout<<"Timer Updated: "<<seconds_passed<<endl;
    cout<<switch1.timer<<endl;
    cout<<switch2.timer<<endl;
    cout<<switch3.timer<<endl;
    cout<<switch4.timer<<endl;
    cout<<switch5.timer<<endl;
    cout<<switch6.timer<<endl;
    cout<<switch7.timer<<endl;
    cout<<switch8.timer<<endl;
    cout<<switch9.timer<<endl;
    cout<<switch10.timer<<endl;
    if(switch1.timer > 0){
        switch1.timer -=float(seconds_passed);
        if(switch1.timer <= 0 && switch1.value == 0){
            //Turn off switch after timer is up
            switch1.value = 0;
            //Do i update both switch value and light value?
            switch1.light_value = 0;
        }
    }
    if(switch2.timer > 0){
        switch2.timer -=float(seconds_passed);
        if(switch2.timer <= 0 && switch2.value == 0){
            //Turn off switch after timer is up
            switch2.value = 0;
            //Do i update both switch value and light value?
            switch2.light_value = 0;
        }
    }
    if(switch3.timer > 0){
        switch3.timer -=float(seconds_passed);
        if(switch3.timer <= 0 && switch3.value == 0){
            //Turn off switch after timer is up
            switch3.value = 0;
            //Do i update both switch value and light value?
            switch3.light_value = 0;
        }
    }
    if(switch4.timer > 0){
        switch4.timer -=float(seconds_passed);
        if(switch4.timer <= 0 && switch4.value == 0){
            //Turn off switch after timer is up
            switch4.value = 0;
            //Do i update both switch value and light value?
            switch4.light_value = 0;
        }
    }
    if(switch5.timer > 0){
        switch5.timer -=float(seconds_passed);
        if(switch5.timer <= 0 && switch5.value == 0){
            //Turn off switch after timer is up
            switch5.value = 0;
            //Do i update both switch value and light value?
            switch5.light_value = 0;
        }
    }
    if(switch6.timer > 0){
        switch6.timer -=float(seconds_passed);
        if(switch6.timer <= 0 && switch6.value == 0) {
            //Turn off switch after timer is up
            switch6.value = 0;
            //Do i update both switch value and light value?
            switch6.light_value = 0;
        }
    }
    if(switch7.timer > 0){
        switch7.timer -=float(seconds_passed);
        if(switch7.timer <= 0 && switch7.value == 0){
            //Turn off switch after timer is up
            switch7.value = 0;
            //Do i update both switch value and light value?
            switch7.light_value = 0;
        }
    }
    if(switch8.timer > 0){
        switch8.timer -=float(seconds_passed);
        if(switch8.timer <= 0 && switch8.value == 0){
            //Turn off switch after timer is up
            switch8.value = 0;
            //Do i update both switch value and light value?
            switch8.light_value = 0;
        }
    }
    if(switch9.timer > 0){
        switch9.timer -=float(seconds_passed);
        if(switch9.timer <= 0 && switch9.value == 0){
            //Turn off switch after timer is up
            switch9.value = 0;
            //Do i update both switch value and light value?
            switch9.light_value = 0;
        }
    }
    if(switch10.timer > 0){
        switch10.timer -=float(seconds_passed);
        if(switch10.timer <= 0 && switch10.value == 0){
            //Turn off light after timer is up
            switch10.light_value = 0;
        }
    }
    
    
} 

void SwitchHandler::updateSwitches(short s1v , short s2v,short s3v,short s4v,short s5v,short s6v,short s7v,short s8v,short s9v,short s10v){
    if(s1v == 1){
        //(Re)set timer
        switch1.timer = float(10.00);
        switch1.light_value = 1;
    }
    if(s1v == 2){
        //Clear timer and toggle
        switch1.timer = float(0.00);
        switch1.light_value = !switch1.light_value;
    }

    if(s2v == 1){
        //(Re)set timer
        switch2.timer = float(10.00);
        switch2.light_value = 1;
    }
    if(s2v == 2){
        //Clear timer and toggle
        switch2.timer = float(0.00);
        switch2.light_value = !switch2.light_value;
    }

    if(s3v == 1){
        //(Re)set timer
        switch3.timer = float(10.00);
        switch3.light_value = 1;
    }
    if(s3v == 2){
        //Clear timer and toggle
        switch3.timer = float(0.00);
        switch3.light_value = !switch3.light_value;
    }

    if(s4v == 1){
        //(Re)set timer
        switch4.timer = float(10.00);
        switch4.light_value = 1;
    }
    if(s4v == 2){
        //Clear timer and toggle
        switch4.timer = float(0.00);
        switch4.light_value = !switch4.light_value;
    }

    if(s5v == 1){
        //(Re)set timer
        switch5.timer = float(10.00);
        switch5.light_value = 1;
    }
    if(s5v == 2){
        //Clear timer and toggle
        switch5.timer = float(0.00);
        switch5.light_value = !switch5.light_value;
    }

    if(s6v == 1){
        //(Re)set timer
        switch6.timer = float(10.00);
        switch6.light_value = 1;
    }
    if(s6v == 2){
        //Clear timer and toggle
        switch6.timer = float(0.00);
        switch6.light_value = !switch6.light_value;
    }

    if(s7v == 1){
        //(Re)set timer
        switch7.timer = float(10.00);
        switch7.light_value = 1;
    }
    if(s7v == 2){
        //Clear timer and toggle
        switch7.timer = float(0.00);
        switch7.light_value = !switch7.light_value;
    }

    if(s8v == 1){
        //(Re)set timer
        switch8.timer = float(10.00);
        switch8.light_value = 1;
    }
    if(s8v == 2){
        //Clear timer and toggle
        switch8.timer = float(0.00);
        switch8.light_value = !switch8.light_value;
    }

    if(s9v == 1){
        //(Re)set timer
        switch9.timer = float(10.00);
        switch9.light_value = 1;
    }
    if(s9v == 2){
        //Clear timer and toggle
        switch9.timer = float(0.00);
        switch9.light_value = !switch9.light_value;
    }

    if(s10v == 1){
        //(Re)set timer
        switch10.timer = float(10.00);
        switch10.light_value = 1;
    }
    if(s10v == 2){
        //Clear timer and toggle
        switch10.timer = float(0.00);
        switch10.light_value = !switch10.light_value;
    }

    
}

int SwitchHandler::toggleLight(int light_id){
    if(light_id == 01){
        switch1.timer = float(0.00);
        switch1.light_value = !switch1.light_value;
    }
    if(light_id == 02){
        switch2.timer = float(0.00);
        switch2.light_value = !switch2.light_value;
    }
    if(light_id == 03){
        switch3.timer = float(0.00);
        switch3.light_value = !switch3.light_value;
    }
    if(light_id == 04){
        switch4.timer = float(0.00);
        switch4.light_value = !switch4.light_value;
    }
    if(light_id == 05){
        switch5.timer = float(0.00);
        switch5.light_value = !switch5.light_value;
    }
    if(light_id == 6){
        switch6.timer = float(0.00);
        switch6.light_value = !switch6.light_value;
    }
    if(light_id == 7){
        switch7.timer = float(0.00);
        switch7.light_value = !switch7.light_value;
    }
    if(light_id == 8){
        switch8.timer = float(0.00);
        switch8.light_value = !switch8.light_value;
    }
    if(light_id == 9){
        switch9.timer = float(0.00);
        switch9.light_value = !switch9.light_value;
    }
    if(light_id == 10){
        switch10.timer = float(0.00);
        switch10.light_value = !switch10.light_value;
    }
    return 1;
}


//getters

bool SwitchHandler::getLight1(){
    return switch1.light_value;
}

bool SwitchHandler::getLight2(){
    return switch2.light_value;
}

bool SwitchHandler::getLight3(){
    return switch3.light_value;
}

bool SwitchHandler::getLight4(){
    return switch4.light_value;
}

bool SwitchHandler::getLight5(){
    return switch5.light_value;
}

bool SwitchHandler::getLight6(){
    return switch6.light_value;
}

bool SwitchHandler::getLight7(){
    return switch7.light_value;
}

bool SwitchHandler::getLight8(){
    return switch8.light_value;
}

bool SwitchHandler::getLight9(){
    return switch9.light_value;
}

bool SwitchHandler::getLight10(){
    return switch10.light_value;
}


//setters

