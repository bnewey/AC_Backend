//============================================================================
// Name        : zone.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles zone related functions
//============================================================================


// C library headers


#include "./zone.hpp"



void Zone::init(int id, int array_i, short mode, short cold_supply, short cold_return, short hot_supply, short hot_return, string name, string desc){
    this->id = id; 
    this->array_index = array_i; 
    this->mode = mode;
    this->cold_supply = cold_supply; 
    this->cold_return = cold_return; 
    this->hot_supply = hot_supply; 
    this->hot_return = hot_return; 
    this->name = name;
    this->description = desc;

}

//Default Constructor
Zone::Zone(){

    //start with stop mode 1

    //(un)provided settings
    //init(1,251,0,0, "defaultLight", "Default constructor was used", );

    //
}

// Constructor with int  current_mode passed
Zone::Zone( int id, int array_i, short mode,  short cold_supply, short cold_return, short hot_supply, short hot_return, string name, string desc) {
    init(id, array_i, mode,cold_supply, cold_return, hot_supply, hot_return,  name, desc);  
}

//Copy constructor
Zone::Zone( const Zone &cp){
    init(cp.id, cp.array_index, cp.mode, cp.cold_supply, cp.cold_return, cp.hot_supply, cp.hot_return, cp.name, cp.description);
}

//Copy Constructor Assignment
Zone& Zone::operator=(const Zone& cp){
    if(this != &cp){   

        init(cp.id, cp.array_index, cp.mode, cp.cold_supply, cp.cold_return, cp.hot_supply, cp.hot_return, cp.name, cp.description);
    }
    return *this;
}

//Deconstructor
Zone::~Zone(){
    cout<<"Destructing"<<endl;

}




// bool Zone::checkLightsInit(){
//     auto iter1 = lights.begin();
//     for ( ; iter1 !=  lights.end(); iter1++){
//         shared_ptr<Light> light_orig = (*iter1).lock();
//         if(!(light_orig->isInitialized())){
//             cout<<"Bad Initialization"<<endl;
//             return false;
//         }
//     }
//     return true;
// }

void Zone::updateZone(vector<short> updateData){
    // [1]Temp 1 high byte
    // [2]Temp 1 low byte
    // [3]Temp 2 high byte
    // [4]Temp 2 low byte
    // [5]Temp 3 high byte
    // [6]Temp 3 low byte
    // [7]quarts per minute
    // [8]Pressure 1 high
    // [9]Pressure 1 high
    // [10]Pressure 2 high
    // [11]Pressure 2 high

    short temp1read = short(updateData[0]*256) + updateData[1];
    short temp2read = short(updateData[2]*256) + updateData[3];
    short temp1, temp2 = 0;
    //short temp3 = short(updateData[4]*256) + updateData[5];

    // if(updateData[11]==1)//below 0 degrees celsius
    //     {
    //         temp1read=4095-temp1read;
    //         ++temp1read;//2's complement for negative temp
    //         if(temp1read<273){
    //             temp1=32-((temp1read*1125)/10000);
    //         }  
    //         else temp1=1;
    //     }
    // else {
        temp1=((temp1read*1125)/10000)+32;
    //}

    // if(updateData[11]==1)//below 0 degrees celsius
    //     {
    //         temp2read=4095-temp2read;
    //         ++temp2read;//2's complement for negative temp
    //         if(temp2read<273){
    //             temp2=32-((temp2read*1125)/10000);
    //         }  
    //         else temp2=1;
    //     }
    // else {
        temp2=((temp2read*1125)/10000)+32;
    //}

    if(mode == 0){ //cold
        this->setZoneColdReturn(temp1);
        this->setZoneColdSupply(temp2);
    }
    if(mode == 1){ //hot
        this->setZoneHotReturn(temp1);
        this->setZoneHotSupply(temp2);
    }
    
    
}


//getters

int Zone::getZoneId(){
    return this->id;
}
int Zone::getZoneArrayIndex(){
    return this->array_index;
} 
short Zone::getModeValue(){
    return this->mode;
}
short Zone::getZoneColdSupply(){
    return this->cold_supply;
}
short Zone::getZoneColdReturn(){
    return this->cold_return;
}
short Zone::getZoneHotSupply(){
    return this->hot_supply;
}
short Zone::getZoneHotReturn(){
    return this->hot_return;
}
string Zone::getZoneName(){
    return this->name;
}
string Zone::getZoneDesc(){
    return this->description;
}


//setters
void Zone::setZoneId(int id){
    if(id < 0){
        return;
    }
    this->id = id;
}
void Zone::setZoneArrayIndex(int array_index){
    if(array_index < 0){
        return;
    }
    this->array_index = array_index;
}
void Zone::setModeValue(short value){
    if(value != 0 || value != 1){
        return;
    }
    this->setModeValue(value);
}
void Zone::setZoneColdSupply(short value){
    this->cold_supply = value;
}
void Zone::setZoneColdReturn(short value){
    this->cold_return = value;
}

void Zone::setZoneHotSupply(short value){
    this->hot_supply = value;
}
void Zone::setZoneHotReturn(short value){
    this->hot_return = value;
}
void Zone::setZoneName(string name){
    this->name = name;
}
void Zone::setZoneDescription(string desc){
    this->description = desc;
}

