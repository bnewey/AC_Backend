//============================================================================
// Name        : cond_unit.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles cond_unit related functions
//============================================================================


// C library headers


#include "./cond_unit.hpp"



void CondUnit::init(int id, int array_i, short type,  short cold_supply, short cold_return, short hot_supply, short hot_return,
 short pressure_low_supply, short pressure_low_return, short pressure_high_supply, short pressure_high_return, string name, string desc){
    this->id = id; 
    this->array_index = array_i; 
    this->type = type;
    this->cold_supply = cold_supply; 
    this->cold_return = cold_return; 
    this->hot_supply = hot_supply; 
    this->hot_return = hot_return; 
    this->pressure_low_supply = pressure_low_supply; 
    this->pressure_low_return = pressure_low_return; 
    this->pressure_high_supply = pressure_high_supply; 
    this->pressure_high_return = pressure_high_return; 
    this->name = name;
    this->description = desc;
    this->initialized = true;
}

//Default Constructor
CondUnit::CondUnit(){
    
}

// Constructor
CondUnit::CondUnit( int id, int array_i, short type,   short cold_supply, short cold_return, short hot_supply, short hot_return,
        short pressure_low_supply, short pressure_low_return, short pressure_high_supply, short pressure_high_return, string name, string desc) {
            
    init(id, array_i, type, cold_supply, cold_return, hot_supply, hot_return,
    pressure_low_supply, pressure_low_return, pressure_high_supply, pressure_high_return, name, desc);
    
}

//Copy Constructor
CondUnit::CondUnit( const CondUnit &cp){
    init(cp.id, cp.array_index, cp.type, cp.cold_supply, cp.cold_return, cp.hot_supply, cp.hot_return, 
    cp.pressure_low_supply, cp.pressure_low_return, cp.pressure_high_supply, cp.pressure_high_return, cp.name, cp.description);
}

//Copy Constructor Assignment
CondUnit& CondUnit::operator=(const CondUnit& cp){
    if(this != &cp){
        
        init(cp.id, cp.array_index, cp.type, cp.cold_supply, cp.cold_return, cp.hot_supply, cp.hot_return,
        cp.pressure_low_supply, cp.pressure_low_return, cp.pressure_high_supply, cp.pressure_high_return, cp.name, cp.description);
    }
    return *this;
}

void CondUnit::updateCondUnit(vector<short> updateData){
    
    this->setCondUnitColdReturn(updateData[0]);
    this->setCondUnitColdSupply(updateData[1]);
    this->setCondUnitHotReturn(updateData[2]);
    this->setCondUnitHotSupply(updateData[3]);
    this->setCondUnitPressureHighSupply(updateData[4]);
    this->setCondUnitPressureHighReturn(updateData[5]);
    this->setCondUnitPressureLowSupply(updateData[6]);
    this->setCondUnitPressureLowReturn(updateData[7]);
}



//getters
int CondUnit::getCondUnitId(){
    return this->id;
}

int CondUnit::getCondUnitArrayIndex(){
    return this->array_index;
}
short CondUnit::getCondUnitType(){
    return this->type;
}
short CondUnit::getCondUnitColdSupply(){
    return this->cold_supply;
}
short CondUnit::getCondUnitColdReturn(){
    return this->cold_return;
}

short CondUnit::getCondUnitHotSupply(){
    return this->hot_supply;
}
short CondUnit::getCondUnitHotReturn(){
    return this->hot_return;
}

string CondUnit::getCondUnitName(){
    return this->name;
}
string CondUnit::getCondUnitDesc(){
    return this->description;
}
short CondUnit::getCondUnitPressureHighSupply(){
    return this->pressure_low_supply;
}
short CondUnit::getCondUnitPressureHighReturn(){
    return this->pressure_low_return;
}

short CondUnit::getCondUnitPressureLowSupply(){
    return this->pressure_high_supply;
}
short CondUnit::getCondUnitPressureLowReturn(){
    return this->pressure_high_return;
}
bool CondUnit::isInitialized(){
    return this->initialized;
}

//setters
void CondUnit::setCondUnitId(int id){
    if(id < 0){
        return;
    }
    this->id = id;
}
void CondUnit::setCondUnitArrayIndex(int array_index){
    if(array_index < 0){
        return;
    }
    this->array_index = array_index;
}
void CondUnit::setCondUnitType(short value){
    this->type = value;
}
void CondUnit::setCondUnitColdSupply(short value){
    this->cold_supply = value;
}
void CondUnit::setCondUnitColdReturn(short value){
    this->cold_return = value;
}

void CondUnit::setCondUnitHotSupply(short value){
    this->hot_supply = value;
}
void CondUnit::setCondUnitHotReturn(short value){
    this->hot_return = value;
}

void CondUnit::setCondUnitPressureHighSupply(short value){
    this->pressure_low_supply = value;
}
void CondUnit::setCondUnitPressureHighReturn(short value){
    this->pressure_low_return = value;
}

void CondUnit::setCondUnitPressureLowSupply(short value){
    this->pressure_high_supply = value;
}
void CondUnit::setCondUnitPressureLowReturn(short value){
    this->pressure_high_return = value;
}

void CondUnit::setCondUnitName(string name){
    this->name = name;
}
void CondUnit::setCondUnitDescription(string desc){
    this->description = desc;
}

