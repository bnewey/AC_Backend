#ifndef ADJUSTTABLE_HPP
#define ADJUSTTABLE_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <ctime>

using namespace std;

// Fraction class
class AdjustTable {

  // private: no access from outside
  private:
    
    
    vector<vector<string>> table_config;

    bool initialized;

    short avg_temp_cold_return__save;
    short avg_temp_cold_supply__save;
    short avg_temp_hot_return__save;
    short avg_temp_hot_supply__save;

  public:
    // default constructor
    AdjustTable();

    // constructor with params
    AdjustTable( vector<vector<string>> );

    //Init
    void init( vector<vector<string>> );

    //Copy Constructor
    AdjustTable( const AdjustTable &cp);

    //Copy assignment
    AdjustTable& operator=(const AdjustTable& cp);

    //getters
    int getValueFromTemp(short);
    bool isInitialized();

    short getAvgSavedTempColdReturn(); 
    short getAvgSavedTempColdSupply();
    short getAvgSavedTempHotReturn();
    short getAvgSavedTempHotSupply();

    //setters 
    void setAvgSavedTempColdReturn(short); 
    void setAvgSavedTempColdSupply(short);
    void setAvgSavedTempHotReturn(short);
    void setAvgSavedTempHotSupply(short);

};

#endif    /* ADJUSTTABLE_HPP */