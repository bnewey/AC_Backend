//============================================================================
// Name        : light.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles light related functions
//============================================================================


// C library headers


#include "./light.hpp"



void Light::init(int id, int array_i, int switch_id,  short value, string name, string desc){
    this->id = id; 
    this->array_index = array_i; 
    this->switch_id = switch_id;
    this->value = value; 
    this->name = name;
    this->description = desc;
    this->initialized = true;
}

//Default Constructor
Light::Light(){
    
}

// Constructor
Light::Light( int id, int array_i, int switch_id,  short value, string name, string desc) {
    init(id, array_i, switch_id, value, name, desc);
    
}

//Copy Constructor
Light::Light( const Light &cp){
    init(cp.id, cp.array_index, cp.switch_id, cp.value, cp.name, cp.description);
}

//Copy Constructor Assignment
Light& Light::operator=(const Light& cp){
    if(this != &cp){
        
        init(cp.id, cp.array_index, cp.switch_id, cp.value, cp.name, cp.description);
    }
    return *this;
}



//getters
int Light::getLightId(){
    return this->id;
}

int Light::getLightArrayIndex(){
    return this->array_index;
}

int Light::getSwitchId(){
    return this->switch_id;
}
short Light::getLightValue(){
    return this->value;
}
string Light::getLightName(){
    return this->name;
}
string Light::getLightDesc(){
    return this->description;
}
bool Light::isInitialized(){
    return this->initialized;
}

//setters
void Light::setLightId(int id){
    if(id < 0){
        return;
    }
    this->id = id;
}
void Light::setLightArrayIndex(int array_index){
    if(array_index < 0){
        return;
    }
    this->array_index = array_index;
}
void Light::setSwitchId(int id){
    if(id < 0){
        return;
    }
    this->switch_id = id;
}
void Light::setLightValue(short value){
    if(value > 2 || value < 0){
        return;
    }
    this->value = value;
}
void Light::setLightName(string name){
    this->name = name;
}
void Light::setLightDescription(string desc){
    this->description = desc;
}

