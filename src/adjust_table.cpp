//============================================================================
// Name        : adjust_table.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : Handles adjust_table related functions
//============================================================================


// C library headers


#include "./adjust_table.hpp"



void AdjustTable::init(vector<vector<string>> table_config){
    this->table_config = table_config;
    this->initialized = true;
}

//Default Constructor
AdjustTable::AdjustTable(){
    
}

// Constructor
AdjustTable::AdjustTable( vector<vector<string>> table_config) {
    init(table_config);
    
}

//Copy Constructor
AdjustTable::AdjustTable( const AdjustTable &cp){
    init(cp.table_config);
}

//Copy Constructor Assignment
AdjustTable& AdjustTable::operator=(const AdjustTable& cp){
    if(this != &cp){
        
        init(cp.table_config);
    }
    return *this;
}

//UPDATE
int AdjustTable::getValueFromTemp(short temp){

    if(avg_temp_cold_return__save <= 0){
        cout<<"avg tmp is 0"<<endl;
        return -1;
    }

    for(int i=0; i<table_config.size(); i++){
        //id, temp_min, temp_max, down_2, down_1, no_change, up_1, up_2
        // int s_id = stoi(zone_config[i][0]); 
        int temp_min = stoi(table_config[i][1]);
        int temp_max = stoi(table_config[i][2]);
        int down_2 = stoi(table_config[i][3]);
        int down_1 = stoi(table_config[i][4]);
        int no_change = stoi(table_config[i][5]);
        int up_1 = stoi(table_config[i][6]);
        int up_2 = stoi(table_config[i][7]);

        int difference = avg_temp_cold_return__save - temp;
        cout<<"Diff: "<<difference<<endl;
        if(temp >= temp_min && temp <= temp_max){
            cout<<"In range"<<endl;
            //compare against saved value
            if(difference >= 2){//down at least 2
                return down_2;
            }
            if(difference >= 1){//down at least 1
                return down_1;
            }
            if(difference <= -2){//up at least 2
                return up_2;
            }
            if(difference <= -1){//up at least 1
                return up_1;
            }
            if(difference == 0){
                return no_change;
            }
        }
        
    }
    cout<<"didnt find a lane"<<endl;
    return -1;
}



//getters


bool AdjustTable::isInitialized(){
    return this->initialized;
}

short AdjustTable::getAvgSavedTempColdReturn(){
    return avg_temp_cold_return__save;
}
short AdjustTable::getAvgSavedTempColdSupply(){
    return avg_temp_cold_supply__save;
}
short AdjustTable::getAvgSavedTempHotReturn(){
    return avg_temp_hot_return__save;
}
short AdjustTable::getAvgSavedTempHotSupply(){
    return avg_temp_hot_supply__save;
}

//setters

void AdjustTable::setAvgSavedTempColdReturn(short value){
    avg_temp_cold_return__save = value;
}
void AdjustTable::setAvgSavedTempColdSupply(short value){
    avg_temp_cold_supply__save = value;
}
void AdjustTable::setAvgSavedTempHotReturn(short value){
    avg_temp_hot_return__save = value;
}
void AdjustTable::setAvgSavedTempHotSupply(short value){
    avg_temp_hot_supply__save = value;
}


