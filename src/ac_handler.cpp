//============================================================================
// Name        : ac_handler.cpp
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

#include "functional"
#include "algorithm"

#include <mysql/mysql.h>
#include <ctime>

#include "./ac_handler.hpp"
#include "./cond_unit.hpp"
#include "./zone.hpp"
#include "./constants.hpp"


// Functor for deleting pointers in vector.
template<class T> class DeleteVector
{
    public:
    // Overloaded () operator.
    // This will be called by for_each() function.
    bool operator()(T x) const
    {
        // Delete pointer.
        delete x;
        return true;
    }
};

void ACHandler::init( vector<vector<string>> zone_config, vector<vector<string>> cond_unit_config, vector<vector<string>> adj_table_config){
    if(!(zone_config.size() >= 0 && cond_unit_config.size() >= 0)){
        return;
    }

    check_timer = make_shared<Timer>(1, 60);

    //Create ZONES using config
    int sw_con_size = zone_config.size();
    //Reserve saves time on push_back
    zones.reserve(ZONES_MAX_SIZE);
   

    for(int i=0; i<sw_con_size; i++){
         //id, array_index, type, name, description, toggle_timer, move_timer, mode
         int s_id = stoi(zone_config[i][0]); 
         int s_array_index = stoi(zone_config[i][1]);
         string s_name = string(zone_config[i][2]);
         string s_description = string(zone_config[i][3]);
         int s_mode = stoi(zone_config[i][4]);
        
        //Check mode: 0==cold 1==hot
        
        zones.push_back( make_shared<Zone>(s_id, s_array_index, s_mode, 0, 0, 0,0,  s_name, s_description));    
        
    }

    //Create LIGHTS using config
    int cond_unit_con_size = cond_unit_config.size();
    //Reserve saves time on push_back
    cond_units.reserve(COND_UNIT_MAX_SIZE);

    for(int i=0; i<cond_unit_con_size; i++){
        // id, array_index, type, description
        int l_id = stoi(cond_unit_config[i][0]);
        int l_array_index = stoi(cond_unit_config[i][1]);
        int l_type = stoi(cond_unit_config[i][2]);
        string l_description = string(cond_unit_config[i][3]);
        int compressor_on = stoi(cond_unit_config[i][4]);
        int temp_cold_supply = 0;
        int temp_cold_return = 0;
        int temp_hot_supply, temp_hot_return, freon_temp_low,quarts_per_min, pressure_high, pressure_low = 0;



        //id, array_i, type, cold_supply, cold_return, hot_supply, hot_return, pressure_low, pressure_high, name, desc
        shared_ptr<CondUnit> unit = make_shared<CondUnit>(l_id, l_array_index, l_type, temp_cold_supply, temp_cold_return, temp_hot_supply, temp_hot_return, freon_temp_low, quarts_per_min,
            pressure_low, pressure_high, "Cond Unit", l_description);

        cond_units.push_back( unit);  

        if(l_type == int(1)){ //cold
            if(compressor_on == 1){
                compressor_on_queue.push_back(unit);
            }else{
                compressor_off_queue.push_back(unit);
            }
        }
    }

    //Adjustment table
    adj_table  = make_shared<AdjustTable>(adj_table_config);

    cout<<"AC Handler done"<<endl;

    this->initialized = true;

}

//Default Constructor
ACHandler::ACHandler(){


}

// Constructor with int  current_mode passed
ACHandler::ACHandler( vector<vector<string>> zone_config, vector<vector<string>> cond_unit_config, vector<vector<string>> adj_table_config){
    this->initialized = false;
    this->init(zone_config, cond_unit_config, adj_table_config);

}

//Copy Constructor
ACHandler::ACHandler(const ACHandler  &cp)
    : initialized(cp.initialized), cond_units(cp.cond_units),zones(cp.zones)
{cout<<"copy const"<<endl;}

//Copy Constructor Assignment
ACHandler& ACHandler::operator=(const ACHandler& cp){
  
    //shared pointers dont need to be deallocated now
    return *this;
}

ACHandler::~ACHandler(){
    //  shared pointers dont need to be deallocated now

    //Not sure if this is necessary
    cond_units.clear();
    zones.clear();
    cond_units.shrink_to_fit();
    zones.shrink_to_fit();

}




void ACHandler::updateUsingData(vector<vector<short>> data_from_read){
    //Update members using params
    if(!(this->initialized == true)){
        cout<<"Not yet initialized"<<endl;
        return;
    }

    //update zones
    auto iter = zones.begin();
    for ( ; iter !=  zones.end(); iter++)
    {
        int array_index = (*iter)->getZoneArrayIndex();
        (*iter)->updateZone(data_from_read[array_index]);
    }

    //update conditioning units
    auto iter2 = cond_units.begin();
    for ( ; iter2 !=  cond_units.end(); iter2++)
    {
        int array_index = (*iter2)->getCondUnitArrayIndex();
        (*iter2)->updateCondUnit(data_from_read[array_index]);
    }

    //update avg temps from zones
    auto iter3 = zones.begin();
    short sum_cold_return = 0;
    short sum_cold_supply = 0;
    short sum_hot_return = 0;
    short sum_hot_supply = 0;
    int zones_cold_size = 0;
    int zones_hot_size = 0;
    for ( ; iter3 !=  zones.end(); iter3++)
    {
        if((*iter3)->getModeValue() == int(1)){ //cold
            sum_cold_return += (*iter3)->getZoneColdReturn();
            sum_cold_supply += (*iter3)->getZoneColdSupply();
            zones_cold_size++;
        }
         
        if((*iter3)->getModeValue() == int(2)){ //hot
            sum_hot_return += (*iter3)->getZoneHotReturn();
            sum_hot_supply += (*iter3)->getZoneHotSupply();
            zones_hot_size++;
        }
    }
    if(zones_cold_size > 0){
        setAvgTempColdReturn(short( sum_cold_return /  zones_cold_size));
        setAvgTempColdSupply(short( sum_cold_supply / zones_cold_size ));
    }else{
        setAvgTempColdReturn(0);
        setAvgTempColdSupply(0);
    }
    if(zones_hot_size > 0){
        setAvgTempHotReturn(short( sum_hot_return / zones_hot_size ));
        setAvgTempHotSupply(short( sum_hot_supply / zones_hot_size ));
    }else{
        setAvgTempHotReturn(0);
        setAvgTempHotSupply(0);
    }

    //check check timer
    if((*check_timer).getIsTimeUp()){
        cout<<"Time is up, checking table"<<endl;
        //refer to "table config"
        int change_value = (*adj_table).getValueFromTemp(avg_temp_cold_return);
        cout<<"change_value: "<<change_value<<endl;
        cout<<"avg_temp_cold_return: "<<avg_temp_cold_return<<endl;
        cout<<"saved avg_temp_cold_return: "<<(*adj_table).getAvgSavedTempColdReturn()<<endl;
        if(change_value != -1){
            if(!updateCompressors(change_value)){
                cout<<"Compressor update failed"<<endl;
            }
        }

        //set saved temps for next time
        (*adj_table).setAvgSavedTempColdReturn(avg_temp_cold_return);
        (*adj_table).setAvgSavedTempColdSupply(avg_temp_cold_supply);
        (*adj_table).setAvgSavedTempHotReturn(avg_temp_hot_return);
        (*adj_table).setAvgSavedTempHotSupply(avg_temp_hot_supply);

        //restart timer
        setCheckTimer(60);
    }
    
}

int ACHandler::updateCompressors(int change_value){
 //make changes based on change value
    if(change_value == 2){ // nothing
        return 1;
    }

    if(change_value == 1){ // 1 off
        //take first compressor from on_queue and add to off_queue
        if(!compressor_on_queue.empty()){
            compressor_off_queue.push_back(compressor_on_queue.front());
            compressor_on_queue.pop_front();
        }else{
            cout<<"On_queue is empty; no compressor to turn off"<<endl;
            return -1;
        }
        return 1;
    }
        
    if(change_value == 3){ // 1 on
        //take first compressor from off_queue and add to on_queue
        if(!compressor_off_queue.empty()){
            compressor_on_queue.push_back(compressor_off_queue.front());
            compressor_off_queue.pop_front();
        }else{
            cout<<"Off_queue is empty; no compressor to turn on"<<endl;
            return -1;
        }
        return 1;
    }
    return -1;
}

void ACHandler::updateCheckTimer(float seconds_passed){
    //cout<<"Timer Updated SH: "<<seconds_passed<<endl;
    (*check_timer).updateTimer(seconds_passed);
    
} 

void ACHandler::setCheckTimer(float seconds){
    cout<<"Timer Updated SH: "<<seconds<<endl;
    (*check_timer).setTimerValue(seconds);
    
} 

//getters


vector<string> ACHandler::getMySqlSaveStringCondUnits(MYSQL & mysql){
    //update compressor_on
    vector<string> return_vector;
    string sql = "";

    auto iter = cond_units.begin();
    for ( ; iter !=  cond_units.end(); iter++)
    {
        sql = "";
        sql += "UPDATE conditioning_units SET compressor_on = ";
        sql += to_string(getIsCompressorOn( (*iter)->getCondUnitId() ));
        sql += " WHERE id = ";
        sql += to_string((*iter)->getCondUnitId());
        return_vector.push_back(sql);
    }

    return return_vector;
}

vector<string> ACHandler::getMySqlSaveStringCondUnitsHistory(MYSQL & mysql){
    //insert into compressor save 
    vector<string> return_vector;
    string sql = "";

    auto iter = cond_units.begin();
    for ( ; iter !=  cond_units.end(); iter++)
    {
        sql = "";
        sql += "INSERT INTO history__conditioning_units ( temp_cold_return, temp_cold_supply, temp_hot_return, temp_hot_supply, pressure_high, pressure_low, qpm , type ) ";
        sql += " VALUES ( ";
        sql += to_string( (*iter)->getCondUnitColdReturn() );
        sql += " ,";
        sql += to_string( (*iter)->getCondUnitColdSupply() );
        sql += " ,";
        sql += to_string( (*iter)->getCondUnitHotReturn() );
        sql += " ,";
        sql += to_string( (*iter)->getCondUnitHotSupply() );
        sql += " ,";
        sql += to_string( (*iter)->getCondUnitPressureHigh() );
        sql += " ,";
        sql += to_string( (*iter)->getCondUnitPressureLow() );
        sql += " ,";
        sql += to_string( (*iter)->getCondUnitQPM() );
         sql += " ,";
        sql += to_string( (*iter)->getCondUnitType() );
        sql += " ) ";
        return_vector.push_back(sql);
    }

    return return_vector;
}

vector<string> ACHandler::getMySqlSaveStringZones(MYSQL & mysql){
    //update compressor_on
    vector<string> return_vector;
    string sql = "";

    auto iter = zones.begin();
    for ( ; iter !=  zones.end(); iter++)
    {
        sql = "";
        sql += "UPDATE zones SET mode = ";
        sql += to_string( (*iter)->getModeValue() );
        sql += " WHERE id = ";
        sql += to_string((*iter)->getZoneId());
        return_vector.push_back(sql);
    }

    return return_vector;
}

vector<string> ACHandler::getMySqlSaveStringZonesHistory(MYSQL & mysql){
    //insert into compressor save 
    vector<string> return_vector;
    string sql = "";

    auto iter = zones.begin();
    for ( ; iter !=  zones.end(); iter++)
    {
        sql = "";
        sql += "INSERT INTO history__zones ( temp_cold_return, temp_cold_supply, temp_hot_return, temp_hot_supply , mode) ";
        sql += " VALUES ( ";
        sql += to_string( (*iter)->getZoneColdReturn() );
        sql += " ,";
        sql += to_string( (*iter)->getZoneColdSupply() );
        sql += " ,";
        sql += to_string( (*iter)->getZoneHotReturn() );
        sql += " ,";
        sql += to_string( (*iter)->getZoneHotSupply() );
        sql += " ,";
        sql += to_string( (*iter)->getModeValue() );
        sql += " ) ";
        return_vector.push_back(sql);
    }

    return return_vector;
}

string ACHandler::createJsonDataString( long numJsonSends){
	
	//CondUnitData - 
	//     parse into json values into vectors
	vector<Json::Value> condUnitJsonVec;
	Json::Value root;
	Json::Value condUnitJson = root["cond_unitsData"];

    auto c_iter = cond_units.begin();
    for ( ; c_iter !=  cond_units.end(); c_iter++)
    {
        short c_type = (*c_iter)->getCondUnitType();
        
        condUnitJson["id"] = (*c_iter)->getCondUnitId();
        condUnitJson["array_index"] =  ((*c_iter)->getCondUnitArrayIndex());

        if(c_type == 0 || c_type == 1 ){
            condUnitJson["temp_cold_return"] = (*c_iter)->getCondUnitColdReturn();
            condUnitJson["temp_cold_supply"] = (*c_iter)->getCondUnitColdSupply();
            condUnitJson["temp_hot_return"] = (*c_iter)->getCondUnitHotReturn();
            condUnitJson["temp_hot_supply"] = (*c_iter)->getCondUnitHotSupply();
            condUnitJson["pressure_high"] = (*c_iter)->getCondUnitPressureHigh();
            condUnitJson["pressure_low"] = (*c_iter)->getCondUnitPressureLow();
        }
        if(c_type == 2){ //special1 
            condUnitJson["water_drain_flow"] = (*c_iter)->getCondUnitFreonTempLow();
            //or
            condUnitJson["qpm"] = (*c_iter)->getCondUnitQPM();
        }
        if(c_type == 3){ //special2
            condUnitJson["temp_cold_return"] = (*c_iter)->getCondUnitColdReturn();
            condUnitJson["temp_cold_supply"] = (*c_iter)->getCondUnitColdSupply();
            condUnitJson["temp_hot_return"] = (*c_iter)->getCondUnitHotReturn();
            condUnitJson["temp_hot_supply"] = (*c_iter)->getCondUnitHotSupply();
            condUnitJson["pressure_high"] = (*c_iter)->getCondUnitPressureHigh();
            condUnitJson["pressure_low"] = (*c_iter)->getCondUnitPressureLow();
        }
        
        
        condUnitJsonVec.push_back(condUnitJson);
    }
	// End of CondUnit Data

	//ZoneData - 
	//     parse into json values into vectors
	vector<Json::Value> zoneJsonVec;
	Json::Value content(Json::arrayValue);
	Json::Value zoneJson = root["zoneData"];

	auto z_iter = zones.begin();
    for ( ; z_iter !=  zones.end(); z_iter++)
    {
        zoneJson["id"] = (*z_iter)->getZoneId();
        zoneJson["array_index"] =  ((*z_iter)->getZoneArrayIndex());
        zoneJson["temp_cold_return"] = (*z_iter)->getZoneColdReturn();
        zoneJson["temp_cold_supply"] = (*z_iter)->getZoneColdSupply();
        zoneJson["temp_hot_return"] = (*z_iter)->getZoneHotReturn();
        zoneJson["temp_hot_supply"] = (*z_iter)->getZoneHotSupply();
        zoneJsonVec.push_back(zoneJson);
    }
	//End of ZoneData

	//Write JSON to string using Json Vectors
	Json::FastWriter fastWriter;
	string output = "{ \"cond_unitsData\": [";
	
	for(int i=0;i<cond_units.size(); i++){
		if(i!=0){
			output+=",";
		}
		output += fastWriter.write(condUnitJsonVec[i]); //lightData
	}
	output += "] ,";
	output += " \"zonesData\": [";
	for(int i=0;i<zones.size(); i++){
		if(i!=0){
			output+=",";
		}
		output += fastWriter.write(zoneJsonVec[i]); //zonesData
	}
    output += "]";
	output += " }";
	
	//cout<<"OUTPUT"<<output<<endl;
	return(output);
}

void ACHandler::editWriteBuf(char (&temp)[WRITE_BUFF_SIZE]){
	temp[0] = 0x02;

	//Fill the zone section
	auto iter = zones.begin();
    for ( ; iter !=  zones.end(); iter++){ 
        short z_index = (*iter)->getZoneArrayIndex();
        // we use z_index*8 through z_index + 8
        temp[ z_index*8 ] =  0x00; //cold supply
        temp[ z_index*8 +1] = 0x00; //hot supply
        temp[ z_index*8 +2] = 0x00; //cold return
        temp[ z_index*8 +3] = 0x00; //hot return
        temp[ z_index*8 +4] = 0x00; //pump
        temp[ z_index*8 +5] = 0x00; //heater
        temp[ z_index*8 +6] = 0x00;
        temp[ z_index*8 +7] = 0x00;
    }

	//Fill the CondUnit section  -- Special1 & 2 included
	auto iter2 = cond_units.begin();
    for ( ; iter2 !=  cond_units.end(); iter2++){ 
        short cu_index = (*iter2)->getCondUnitArrayIndex();
        short cu_type = (*iter2)->getCondUnitType();
        if(cu_type == 0 || cu_type == 1){
            // we use cu_index*8 through cu_index + 8
            temp[ cu_index*8 ] =  0x00; //compressor bypass
            temp[ cu_index*8 +1] = 0x00; //compressor bypass
            temp[ cu_index*8 +2] = 0x00; //
            temp[ cu_index*8 +3] = 0x00; //
            temp[ cu_index*8 +4] = 0x00; //compressor fan
            temp[ cu_index*8 +5] = 0x00; //compressor
            temp[ cu_index*8 +6] = 0x00;
            temp[ cu_index*8 +7] = 0x00;
        }
        if(cu_type == 2){
            temp[ cu_index*8 ] =  0x00;  //water fill high pressure cold
            temp[ cu_index*8 +1] = 0x00; //water fill high pressure hot
            temp[ cu_index*8 +2] = 0x00; //water fill low pressure cold
            temp[ cu_index*8 +3] = 0x00; //water fill low pressure hot
            temp[ cu_index*8 +4] = 0x00;
            temp[ cu_index*8 +5] = 0x00;
            temp[ cu_index*8 +6] = 0x00;
            temp[ cu_index*8 +7] = 0x00;
        }
        if(cu_type == 3){
            temp[ cu_index*8 ] =  0x00;  //cold bypass
            temp[ cu_index*8 +1] = 0x00; //cold drain
            temp[ cu_index*8 +2] = 0x00; //hot bypass
            temp[ cu_index*8 +3] = 0x00; //hot drain
            temp[ cu_index*8 +4] = 0x00;
            temp[ cu_index*8 +5] = 0x00;
            temp[ cu_index*8 +6] = 0x00;
            temp[ cu_index*8 +7] = 0x00;
        }
    }


	//Give the rest 0's
    short z_size = zones.size();
    short cu_size = cond_units.size();
	for(int i=0; i<((ZONES_MAX_SIZE + COND_UNIT_MAX_SIZE) - ( z_size + cu_size ));i++) { 
		temp[i + ( z_size + cu_size )] =  0x00;
	}

	temp[WRITE_BUFF_SIZE-1] = 0xaa;

}



//Switch Getters
// vector<short> ACHandler::getSwitchValues(){
//     vector<short> return_vector(ZONES_MAX_SIZE,0);

//     auto iter = zones.begin();
//     for ( ; iter !=  zones.end(); iter++)
//     {
//         int tmp = ((*iter)->getSwitchArrayIndex());
//         return_vector[tmp] = (*iter)->getSwitchValue();
//     }
    
//     return return_vector;
// }

// vector<short> ACHandler::getModeValues(){
//     vector<short> return_vector(ZONES_MAX_SIZE,0);

//     auto iter = zones.begin();
//     for ( ; iter !=  zones.end(); iter++)
//     {
//         int tmp = ((*iter)->getSwitchArrayIndex());
//         return_vector[tmp] = (*iter)->getModeValue();
//     }
    
//     return return_vector;
// }

// vector<vector<float>> ACHandler::getTimerValues(){
//     vector<vector<float>> return_vector(ZONES_MAX_SIZE,vector<float>(3));
    
//     auto iter = zones.begin();
//     for ( ; iter !=  zones.end(); iter++)
//     {
//         int tmp = ((*iter)->getSwitchArrayIndex());
//         return_vector[tmp] = (*iter)->getTimerValues();
//     }
    
//     return return_vector;
// } 

//getters
short ACHandler::getAvgTempColdReturn(){
    return avg_temp_cold_return;
}
short ACHandler::getAvgTempColdSupply(){
    return avg_temp_cold_supply;
}
short ACHandler::getAvgTempHotReturn(){
    return avg_temp_hot_return;
}
short ACHandler::getAvgTempHotSupply(){
    return avg_temp_hot_supply;
}
short ACHandler::getAvgPressureHighReturn(){
    return avg_pressure_high_return;
}
short ACHandler::getAvgPressureHighSupply(){
    return avg_pressure_high_supply;
}
short ACHandler::getAvgPressureLowReturn(){
    return avg_pressure_low_return;
}
short ACHandler::getAvgPressureLowSupply(){
    return avg_pressure_low_supply;
}
short ACHandler::getIsCompressorOn(short unit_id){
    for (auto it = compressor_on_queue.cbegin(); it != compressor_on_queue.cend(); ++it) {

        short id = (*it)->getCondUnitId();
        if(id == unit_id){
            return 1;
        }

    }
    return 0;
}


//setters
void ACHandler::setAvgTempColdReturn(short value){
    avg_temp_cold_return = value;
}
void ACHandler::setAvgTempColdSupply(short value){
    avg_temp_cold_supply = value;
}
void ACHandler::setAvgTempHotReturn(short value){
    avg_temp_hot_return = value;
}
void ACHandler::setAvgTempHotSupply(short value){
    avg_temp_hot_supply = value;
}
void ACHandler::setAvgPressureHighReturn(short value){
    avg_pressure_high_return = value;
}
void ACHandler::setAvgPressureHighSupply(short value){
    avg_pressure_high_supply = value;
}
void ACHandler::setAvgPressureLowReturn(short value){
    avg_pressure_low_return = value;
}
void ACHandler::setAvgPressureLowSupply(short value){
    avg_pressure_low_supply = value;
}


