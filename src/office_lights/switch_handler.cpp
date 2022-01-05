//============================================================================
// Name        : switch_handler.cpp
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

#include "./switch_handler.hpp"
#include "./light.hpp"
#include "./switch.hpp"
#include "./single_switch.hpp"
#include "./double_switch.hpp"


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


void SwitchHandler::init( vector<vector<string>> switch_config, vector<vector<string>> light_config){
    if(!(switch_config.size() >= 0 && light_config.size() >= 0)){
        return;
    }

    //Create SWITCHES using config
    int sw_con_size = switch_config.size();
    //Reserve saves time on push_back
    switches.reserve(SWITCHES_MAX_SIZE);
   

    for(int i=0; i<sw_con_size; i++){
         //id, array_index, type, name, description, toggle_timer, move_timer, mode
        int s_id = stoi(switch_config[i][0]); 
        int s_array_index = stoi(switch_config[i][1]);
        int s_type = stoi(switch_config[i][2]);
        string s_name = string(switch_config[i][3]);
        string s_description = string(switch_config[i][4]);
        float s_t_timer = stof(switch_config[i][5]);
        float s_m_timer = stof(switch_config[i][6]);
        int s_mode = stoi(switch_config[i][7]);
        //Check mode: 0==single 1==double switch
        if(s_type == 0){
            switches.push_back( make_shared<SingleSwitch>(s_id, s_array_index, 0, s_mode, s_type, s_name, s_description, s_t_timer, s_m_timer));    
        }
        else if(s_type == 1){
            switches.push_back(make_shared<DoubleSwitch>(s_id, s_array_index, 0, s_mode, s_type, s_name, s_description, s_t_timer, s_m_timer));  
        }
    }

    //Create LIGHTS using config
    int light_con_size = light_config.size();
    //Reserve saves time on push_back
    lights.reserve(LIGHTS_MAX_SIZE);

    for(int i=0; i<light_con_size; i++){
        //id, array_index, switch_id, value, name, description
        int l_id = stoi(light_config[i][0]);
        int l_array_index = stoi(light_config[i][1]);
        int l_switch_id = stoi(light_config[i][2]);
        int l_value = stoi(light_config[i][3]);
        string l_name = string(light_config[i][4]);
        string l_description = string(light_config[i][5]);

        lights.push_back( make_shared<Light>(l_id, l_array_index ,l_switch_id , l_value ,l_name , l_description));

        //Add light to its configured switch with switch_id
        int sw_size = switches.size();
        for(int i=0; i<sw_size; i++){
            if(l_switch_id == switches[i]->getSwitchId()){
                switches[i]->addLightToSwitch(weak_ptr<Light>(lights[i]));
            }
        }
    }

    cout<<"Switch Handler done"<<endl;

    this->initialized = true;

}

//Default Constructor
SwitchHandler::SwitchHandler(){


}

// Constructor with int  current_mode passed
SwitchHandler::SwitchHandler( vector<vector<string>> switch_config, vector<vector<string>> light_config){
    this->initialized = false;
    this->init(switch_config, light_config);

}

//Copy Constructor
SwitchHandler::SwitchHandler(const SwitchHandler  &cp)
    : initialized(cp.initialized), switches(cp.switches), lights(cp.lights)
{cout<<"copy const"<<endl;}

//Copy Constructor Assignment
SwitchHandler& SwitchHandler::operator=(const SwitchHandler& cp){
  
    //shared pointers dont need to be deallocated now
    return *this;
}

SwitchHandler::~SwitchHandler(){
    //  shared pointers dont need to be deallocated now

    //Not sure if this is necessary
    lights.clear();
    switches.clear();
    lights.shrink_to_fit();
    switches.shrink_to_fit();

}




void SwitchHandler::updateSwitches(vector<short> switch_values){
    //Update members using params
    if(!(this->initialized == true)){
        cout<<"Not yet initialized"<<endl;
        return;
    }

    auto iter = switches.begin();
    for ( ; iter !=  switches.end(); iter++)
    {
        int switch_type = (*iter)->getSwitchType();
        if(switch_type == 0){
            int array_index = (*iter)->getSwitchArrayIndex();
            (*iter)->updateSwitch(switch_values[array_index]);
        }
        //Hacky but works for now
        //works only if double switches are back to back in array_index until i add array_index2 into double_switch
        if(switch_type == 1){
            int array_index = (*iter)->getSwitchArrayIndex();
            (*iter)->updateSwitch(switch_values[array_index], switch_values[array_index + 1]);
        }       
    }
    
    
}

void SwitchHandler::updateTimers(float seconds_passed){
    cout<<"Timer Updated SH: "<<seconds_passed<<endl;
    auto iter = switches.begin();
    for ( ; iter !=  switches.end(); iter++)
    {
        (*iter)->updateTimer(float(seconds_passed));
    }
} 

bool SwitchHandler::setSwitchToggle(int idToToggle){
    //Find switch with id
    auto iter = switches.begin();
    for ( ; iter !=  switches.end(); iter++ )
    {   
        if((*iter)->getSwitchId() == idToToggle){
            (*iter)->toggleLight();
            //Set Mode to 0 because this is coming from UI and shouldnt 
            (*iter)->setModeValue(0);
            //(*iter)->setMoveTimer(SINGLE_MOVE_TIMER_TIME); //REMOVE THIS
            return true;
        } 
    }
    return false;
}


//getters
//Light Getters
vector<short> SwitchHandler::getLightValues(){
   //Create and fill return vector
   //More efficient to delcare size in vector and read/set data using [ ]
    vector<short> return_vector(LIGHTS_MAX_SIZE, 0);
    
    auto iter = lights.begin();
    for ( ; iter !=  lights.end(); iter++)
    {
        int tmp = ((*iter)->getLightArrayIndex()-(LIGHTS_MAX_SIZE+1));
        return_vector[tmp] = (*iter)->getLightValue();
    }

    return return_vector;
} 

vector<short> SwitchHandler::getLightSwitchIds(){ 
    //Create and fill return vector
    vector<short> return_vector(LIGHTS_MAX_SIZE, 0);

    auto iter = lights.begin();
    for ( ; iter !=  lights.end(); iter++)
    {
        int tmp = ((*iter)->getLightArrayIndex()-(LIGHTS_MAX_SIZE+1));
        return_vector[tmp] = (*iter)->getSwitchId();
    }

    return return_vector;
}
vector<string> SwitchHandler::getMySqlSaveStringSwitches(MYSQL & mysql){
    //update toggle_timer, move_timer, mode
    vector<string> return_vector;
    string sql = "";

    auto iter = switches.begin();
    for ( ; iter !=  switches.end(); iter++)
    {
        sql = "";
        sql += "UPDATE switches SET toggle_timer = ";
        sql += to_string((*iter)->getToggleTimers()[0]);
        sql += " , move_timer = ";
        sql += to_string((*iter)->getMoveTimers()[0]);
        sql += " , mode = ";
        sql += to_string((*iter)->getModeValue());
        sql += " WHERE id = ";
        sql += to_string((*iter)->getSwitchId());
        return_vector.push_back(sql);
    }

    return return_vector;
}

vector<string> SwitchHandler::getMySqlSaveStringLights(MYSQL & mysql){
    //update value
    vector<string> return_vector;
    string sql = "";

    auto iter = lights.begin();
    for ( ; iter !=  lights.end(); iter++)
    {
        sql = "";
        sql += "UPDATE lights SET value = ";
        sql += to_string((*iter)->getLightValue());
        sql += " WHERE id = ";
        sql += to_string((*iter)->getLightId());
        return_vector.push_back(sql);
    }

    return return_vector;
}

string SwitchHandler::createJsonDataString( long numJsonSends){

	//Recieve vectors from SwitchHandler
	vector<short> lightValues = this->getLightValues();
	vector<short> lightSwitchIdValues = this->getLightSwitchIds();
	vector<short> switchValues = this->getSwitchValues();
	vector<short> modeValues = this->getModeValues();
	vector<vector<float>> timerValues = this->getTimerValues();

	
	//LightData - 
	//     parse into json values into vectors
	vector<Json::Value> lightJsonVec;
	Json::Value root;
	Json::Value lightJson = root["lightsData"];

	int l_size = lightValues.size();

	for(int p= 0;p < l_size; p++){
		lightJson["array_index"] = Json::Value::Int(p);
		lightJson["switch_id"] = Json::Value::Int( int(lightSwitchIdValues[p]) );
		lightJson["value"] = Json::Value::Int( int(lightValues[p]) );

		lightJsonVec.push_back(lightJson);
	}
	// End of Light Data

	//SwitchData - 
	//     parse into json values into vectors
	vector<Json::Value> switchJsonVec;
	Json::Value content(Json::arrayValue);
	Json::Value switchJson = root["switchData"];

	int mv_size = modeValues.size();
	int tv_size = timerValues.size();
	if(mv_size != tv_size){
		cout<<"UH OH modevariables and timer variables are not same size vectors"<<endl;
	}
	for(int p= 0;p < mv_size; p++){
		switchJson["array_index"] = Json::Value::Int(p);
		switchJson["mode"] = Json::Value::Int( int(modeValues[p]) );
		switchJson["move_timer"] = Json::Value::Int( int(timerValues[p][0]) );
		switchJson["toggle_timer"] = Json::Value::Int( int(timerValues[p][1]) );
		switchJson["delay_timer"] = Json::Value::Int( int(timerValues[p][2]) );
		switchJson["switch_value"] = Json::Value::Int( int(switchValues[p]));

		switchJsonVec.push_back(switchJson);
	}
	//End of SwitchData

	//Write JSON to string using Json Vectors
	Json::FastWriter fastWriter;
	string output = "{ \"lightsData\": [";
	
	for(int i=0;i<l_size; i++){
		if(i!=0){
			output+=",";
		}
		output += fastWriter.write(lightJsonVec[i]); //lightData
	}
	output += "] ,";
	output += " \"switchData\": [";
	for(int i=0;i<mv_size; i++){
		if(i!=0){
			output+=",";
		}
		output += fastWriter.write(switchJsonVec[i]); //switchData
	}	
	output += "]";
	output += " }";
	
	//cout<<"OUTPUT"<<output<<endl;
	return(output);
}





//Switch Getters
vector<short> SwitchHandler::getSwitchValues(){
    vector<short> return_vector(SWITCHES_MAX_SIZE,0);

    auto iter = switches.begin();
    for ( ; iter !=  switches.end(); iter++)
    {
        int tmp = ((*iter)->getSwitchArrayIndex());
        return_vector[tmp] = (*iter)->getSwitchValue();
    }
    
    return return_vector;
}

vector<short> SwitchHandler::getModeValues(){
    vector<short> return_vector(SWITCHES_MAX_SIZE,0);

    auto iter = switches.begin();
    for ( ; iter !=  switches.end(); iter++)
    {
        int tmp = ((*iter)->getSwitchArrayIndex());
        return_vector[tmp] = (*iter)->getModeValue();
    }
    
    return return_vector;
}

vector<vector<float>> SwitchHandler::getTimerValues(){
    vector<vector<float>> return_vector(SWITCHES_MAX_SIZE,vector<float>(3));
    
    auto iter = switches.begin();
    for ( ; iter !=  switches.end(); iter++)
    {
        int tmp = ((*iter)->getSwitchArrayIndex());
        return_vector[tmp] = (*iter)->getTimerValues();
    }
    
    return return_vector;
} 


//setters

