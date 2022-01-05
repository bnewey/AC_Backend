#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <ctime>

using namespace std;

// Fraction class
class Light {

  // private: no access from outside
  private:
    
    
    int id; //index of readbuf
    int array_index; //position in read_buf
    int switch_id;
    short value; // 0 == off | 1 == on
    string name;
    string description;
    bool initialized;

  public:
    // default constructor
    Light();

    // constructor with params
    Light( int, int, int, short, string, string );

    //Init
    void init(int , int ,int,  short , string , string );

    //Copy Constructor
    Light( const Light &cp);

    //Copy assignment
    Light& operator=(const Light& cp);

    // update
    void updateLight();
    

    //getters
    int getLightId();
    int getLightArrayIndex();
    int getSwitchId();
    short getLightValue();
    string getLightName();
    string getLightDesc();
    bool isInitialized();

    //setters 
    void setLightId(int);
    void setLightArrayIndex(int);
    void setSwitchId(int);
    void setLightValue(short);
    void setLightName(string);
    void setLightDescription(string); 

};

#endif    /* LIGHT_HPP */