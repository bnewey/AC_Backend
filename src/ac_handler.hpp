#ifndef ACHANDLER_HPP
#define ACHANDLER_HPP

#include <vector>
#include <deque>
#include <jsoncpp/json/json.h>
#include <memory>

#include "./constants.hpp"
#include "./cond_unit.hpp"
#include "./zone.hpp"
#include "./timer.hpp"
#include "./adjust_table.hpp"

using namespace std;

// Fraction class
class ACHandler {

  // private: no access from outside
  private:
    //Timers will be added/removed multiple times
    bool initialized;
    vector< shared_ptr<CondUnit> > cond_units; 
    vector< shared_ptr<Zone> > zones;
    deque< shared_ptr<CondUnit>> compressor_on_queue;
    deque< shared_ptr<CondUnit>> compressor_off_queue;
    shared_ptr<Timer> check_timer;
    shared_ptr<AdjustTable> adj_table;

    //zone temps
    short avg_temp_cold_return;
    short avg_temp_cold_supply;
    short avg_temp_hot_return;
    short avg_temp_hot_supply;

    //compressor pressures
    short avg_pressure_high_return;
    short avg_pressure_high_supply;
    short avg_pressure_low_return;
    short avg_pressure_low_supply;

    void init( vector<vector<string>>, vector<vector<string>>, vector<vector<string>> );

  public:
    // default constructor
    ACHandler();

    // constructor from int (denominator)
    ACHandler( vector<vector<string>>, vector<vector<string>>, vector<vector<string>>);

    //Copy Constructor
    ACHandler(const ACHandler  &cp);

    //Copy Constructor Assignment
    ACHandler& operator=(const ACHandler& cp);

    //deconstructor
    ~ACHandler();

    // update
    void updateUsingData(vector<vector<short>>);

    int updateCompressors(int);

    //Timer 
    void updateCheckTimer(float);
    
    bool setZoneToggle(int);

    //Create Json for Ui Send
    string createJsonDataString(long numJsonSends);

    vector<string> getMySqlSaveStringCondUnits(MYSQL & mysql);
    vector<string> getMySqlSaveStringCondUnitsHistory(MYSQL & mysql);
    vector<string> getMySqlSaveStringZones(MYSQL & mysql);
    vector<string> getMySqlSaveStringZonesHistory(MYSQL & mysql);
   

    //getters
    //CondUnit getters
    vector<short> getCondUnitValues();
    vector<short> getCondUnitZoneIds();
    //Zone getters
    vector<short> getZoneValues();
    vector<short> getModeValues();
    vector<vector<float>> getTimerValue();

    //Getters
    short getAvgTempColdReturn(); 
    short getAvgTempColdSupply();
    short getAvgTempHotReturn();
    short getAvgTempHotSupply();
    short getAvgPressureHighReturn(); 
    short getAvgPressureHighSupply();
    short getAvgPressureLowReturn();
    short getAvgPressureLowSupply();
    short getIsCompressorOn(short);
 


    //setters 
    void setCheckTimer(float);
    void setAvgTempColdReturn(short); 
    void setAvgTempColdSupply(short);
    void setAvgTempHotReturn(short);
    void setAvgTempHotSupply(short);
    void setAvgPressureHighReturn(short); 
    void setAvgPressureHighSupply(short);
    void setAvgPressureLowReturn(short);
    void setAvgPressureLowSupply(short);
    

};

#endif    /* ACHANDLER_HPP */