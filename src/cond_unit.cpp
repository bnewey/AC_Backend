//============================================================================
// Name        : cond_unit.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles cond_unit related functions
//============================================================================


// C library headers


#include "./cond_unit.hpp"



void CondUnit::init(int id, int array_i, short type,  short cold_supply, short cold_return, short hot_supply, short hot_return, short freon_temp_low, short quarts_per_min,
 short pressure_low, short pressure_high, string name, string desc){
    this->id = id; 
    this->array_index = array_i; 
    this->type = type;
    this->cold_supply = cold_supply; 
    this->cold_return = cold_return; 
    this->hot_supply = hot_supply; 
    this->hot_return = hot_return; 
    this->freon_temp_low = freon_temp_low;
    this->quarts_per_min = quarts_per_min;
    this->pressure_low = pressure_low; 
    this->pressure_high = pressure_high; 
    this->name = name;
    this->description = desc;
    this->initialized = true;
}

//Default Constructor
CondUnit::CondUnit(){
    
}

// Constructor
CondUnit::CondUnit( int id, int array_i, short type,   short cold_supply, short cold_return, short hot_supply, short hot_return, short freon_temp_low,short quarts_per_min,
        short pressure_low, short pressure_high, string name, string desc) {
            
    init(id, array_i, type, cold_supply, cold_return, hot_supply, hot_return, freon_temp_low,quarts_per_min,
    pressure_low, pressure_high, name, desc);
    
}

//Copy Constructor
CondUnit::CondUnit( const CondUnit &cp){
    init(cp.id, cp.array_index, cp.type, cp.cold_supply, cp.cold_return, cp.hot_supply, cp.hot_return, cp.freon_temp_low, cp.quarts_per_min,
    cp.pressure_low, cp.pressure_high, cp.name, cp.description);
}

//Copy Constructor Assignment
CondUnit& CondUnit::operator=(const CondUnit& cp){
    if(this != &cp){
        
        init(cp.id, cp.array_index, cp.type, cp.cold_supply, cp.cold_return, cp.hot_supply, cp.hot_return,cp.freon_temp_low, cp.quarts_per_min,
        cp.pressure_low, cp.pressure_high, cp.name, cp.description);
    }
    return *this;
}

void CondUnit::updateCondUnit(vector<short> updateData){
    // [0]Temp 1 high byte
    // [1]Temp 1 low byte
    // [2]Temp 2 high byte
    // [3]Temp 2 low byte
    // [4]Temp 3 high byte
    // [5]Temp 3 low byte
    // [6]quarts per minute
    // [7]Pressure 1 high
    // [8]Pressure 1 high
    // [9]Pressure 2 high
    // [10]Pressure 2 high

    //TEMPS
    short temp1read = short(updateData[0]*256) + updateData[1];
    short temp2read = short(updateData[2]*256) + updateData[3];
    short temp3read = short(updateData[4]*256) + updateData[5];

    short temp1, temp2, temp3 = 0;

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

    // if(updateData[11]==1)//below 0 degrees celsius
    //     {
    //         temp3read=4095-temp3read;
    //         ++temp3read;//2's complement for negative temp
    //         if(temp3read<273){
    //             temp3=32-((temp3read*1125)/10000);
    //         }  
    //         else temp3=1;
    //     }
    // else {
        temp3=((temp3read*1125)/10000)+32;
    //}

    if(type == 1){ //chiller
        this->setCondUnitColdReturn(temp1);
        this->setCondUnitColdSupply(temp2);
    }
    if(type == 2){ //heater
        this->setCondUnitHotReturn(temp1);
        this->setCondUnitHotSupply(temp2);
    }
    if(type == 3){ //special 1 - drain flow - qpm
        
    }
    if(type == 4){ //special 2 - tube temp & pressures
        this->setCondUnitColdSupply(temp1); // cold tube
        this->setCondUnitHotSupply(temp2); //hot tube
    }

    this->setCondUnitFreonTempLow(temp3);
    
    //

    //Quarts per Min
    if(this->type == int(3)){ //special 1
        short qpm = short(((updateData[6]*10)+38)/76);
        this->setCondUnitQPM(qpm);
    }
    //

    //PRESSURES
    short pressure_high = short(updateData[7]*256) + updateData[8];
    short pressure_low = short(updateData[9]*256) + updateData[10];

    if(pressure_high > 0 && pressure_high < PRESSURE_LOOKUP_SIZE){
        this->setCondUnitPressureHigh(PRESSURE_LOOKUP[pressure_high]);
    }else{
        cout<<"ERROR -- Pressure High is not found in table -- bad index"<<endl;
    }

    if(pressure_low > 0 && pressure_low < PRESSURE_LOOKUP_SIZE){
        this->setCondUnitPressureLow(PRESSURE_LOOKUP[pressure_low]);
    }else{
        cout<<"ERROR -- Pressure Low is not found in table -- bad index"<<endl;
    }
    //
    
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
short CondUnit::getCondUnitFreonTempLow(){
    return this->freon_temp_low;
}
short CondUnit::getCondUnitQPM(){
    return this->quarts_per_min;
}

string CondUnit::getCondUnitName(){
    return this->name;
}
string CondUnit::getCondUnitDesc(){
    return this->description;
}
short CondUnit::getCondUnitPressureHigh(){
    return this->pressure_low;
}

short CondUnit::getCondUnitPressureLow(){
    return this->pressure_high;
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
void CondUnit::setCondUnitFreonTempLow(short value){
    this->freon_temp_low = value;
}

void CondUnit::setCondUnitQPM(short value){
    this->quarts_per_min = value;
}

void CondUnit::setCondUnitPressureHigh(short value){
    this->pressure_low = value;
}

void CondUnit::setCondUnitPressureLow(short value){
    this->pressure_high = value;
}

void CondUnit::setCondUnitName(string name){
    this->name = name;
}
void CondUnit::setCondUnitDescription(string desc){
    this->description = desc;
}
