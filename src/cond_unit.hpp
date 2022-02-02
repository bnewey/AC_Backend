#ifndef CONDUNIT_HPP
#define CONDUNIT_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <ctime>

#include "./constants.hpp"

using namespace std;

// Fraction class
class CondUnit {

  // private: no access from outside
  private:
    
    
    int id; //index of readbuf
    int array_index; //position in read_buf
    short type; // 1 = chiller | 2 = heater
    short cold_supply;
    short cold_return;
    short hot_supply;
    short hot_return;
    short freon_temp_low;
    short quarts_per_min;
    short pressure_low;
    short pressure_high;
    string name;
    string description;
    bool initialized;

  public:
    // default constructor
    CondUnit();

    // constructor with params
    CondUnit( int, int,short, short, short,short,short,short, short,short,short, string, string );

    //Init
    void init(int , int,short,  short ,short,short,short,short, short,short,short, string , string );

    //Copy Constructor
    CondUnit( const CondUnit &cp);

    //Copy assignment
    CondUnit& operator=(const CondUnit& cp);

    // update
    void updateCondUnit(vector<short>);
    

    //getters
    int getCondUnitId();
    int getCondUnitArrayIndex();
    short getCondUnitType();
    short getCondUnitColdSupply();
    short getCondUnitColdReturn();
    short getCondUnitHotSupply();
    short getCondUnitHotReturn();
    short getCondUnitFreonTempLow();
    short getCondUnitQPM();
    short getCondUnitPressureLow();
    short getCondUnitPressureHigh();
    string getCondUnitName();
    string getCondUnitDesc();
    bool isInitialized();

    //setters 
    void setCondUnitId(int);
    void setCondUnitArrayIndex(int);
    void setSwitchId(int);
    void setCondUnitType(short);
    void setCondUnitColdSupply(short);
    void setCondUnitColdReturn(short);
    void setCondUnitHotSupply(short);
    void setCondUnitHotReturn(short);
    void setCondUnitFreonTempLow(short);
    void setCondUnitQPM(short);
    void setCondUnitPressureLow(short);
    void setCondUnitPressureHigh(short);
    void setCondUnitName(string);
    void setCondUnitDescription(string); 

};

#endif    /* CONDUNIT_HPP */