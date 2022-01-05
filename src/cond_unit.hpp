#ifndef CONDUNIT_HPP
#define CONDUNIT_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <ctime>

using namespace std;

// Fraction class
class CondUnit {

  // private: no access from outside
  private:
    
    
    int id; //index of readbuf
    int array_index; //position in read_buf
    short type; // 0 = chiller | 1 = heater
    short cold_supply;
    short cold_return;
    short hot_supply;
    short hot_return;
    short pressure_low_supply;
    short pressure_low_return;
    short pressure_high_supply;
    short pressure_high_return;
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
    short getCondUnitPressureHighSupply();
    short getCondUnitPressureHighReturn();
    short getCondUnitPressureLowSupply();
    short getCondUnitPressureLowReturn();
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
    void setCondUnitPressureHighSupply(short);
    void setCondUnitPressureHighReturn(short);
    void setCondUnitPressureLowSupply(short);
    void setCondUnitPressureLowReturn(short);
    void setCondUnitName(string);
    void setCondUnitDescription(string); 

};

#endif    /* CONDUNIT_HPP */