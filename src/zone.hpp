#ifndef ZONE_HPP
#define ZONE_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory>

#include <ctime>

#include "./constants.hpp"
// #include "./light.hpp"
//#include "./timer.hpp"



using namespace std;

// Fraction class
class Zone {

  // private: no access from outside
  protected:
    
    
    int id; //index of readbuf
    int array_index; //position in read_buf
    short mode; // 0==cold 1=hot 
    short cold_supply;
    short cold_return;
    short hot_supply;
    short hot_return;
    string name;
    string description;

    //Light is weak_ptr because it lives in switch_handler
    //vector< weak_ptr<Light> > lights;

    bool checkLightsInit();

    public:
    // default constructor
    Zone();

    // constructor with params
    Zone( int, int, short,short, short,short,short,  string, string );

    void init(int , int , short ,short, short,short,short, string , string);

    //Copy constructor
    Zone( const Zone &cp);

    //Copy assignment
    Zone& operator=(const Zone& cp);

    //Destructor
    ~Zone();

    // update
    void updateZone(vector<short>);
    

    //getters
    int getZoneId();
    int getZoneArrayIndex();
    short getModeValue(); //0 =cold 1=hot
    short getZoneColdSupply();
    short getZoneColdReturn();
    short getZoneHotSupply();
    short getZoneHotReturn();
    string getZoneName();
    string getZoneDesc();
    

    //setters 
    void setZoneId(int);
    void setZoneArrayIndex(int);
    void setModeValue(short); //0=cold 1=hot
    void setZoneColdSupply(short);
    void setZoneColdReturn(short);
    void setZoneHotSupply(short);
    void setZoneHotReturn(short);
    void setZoneName(string);
    void setZoneDescription(string); 

};

#endif    /* ZONE_HPP */