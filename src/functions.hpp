#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

//============================================================================
// Name        : functions.hpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description : All basic functions for port reading / mysql / json sending to UI
//============================================================================


//#include "./switch_handler.hpp"
#include "./ac_handler.hpp"
#include "./constants.hpp"
//#include "./tmpSwitch.cpp"
 
#define PORT 8081 

using namespace std;


void print_buf(char (&read_buf)[BUFF_SIZE], int numIterations, int numReads);

void print_write_buff(char (&write_buf)[WRITE_BUFF_SIZE], int numIterations, int numReads);

void print_ui_buff(char (&ui_buf)[UI_BUFF_SIZE]);

int read_bytes(char  (&read_buf)[BUFF_SIZE],int & serial_port , int & numIterations);

void getDataFromRead(char  (&read_buf)[BUFF_SIZE], vector<vector<short>> & switch_vector);

void editWriteBuf(char (&temp)[WRITE_BUFF_SIZE] , shared_ptr<ACHandler> sh);

void write_bytes(int & serial_port, char (&temp)[WRITE_BUFF_SIZE]);

int usb_port(int & serial_port) ;

void mysqlConnect(MYSQL & mysql);

int mysqlQueryFixed(MYSQL & mysql, const string, vector< vector<string> > & return_array);

int mysqlQueryNoReturn(MYSQL & mysql, const string);

void mysqlCloseConnect(MYSQL &mysql);

int readNodeSocket( int & new_socket, char  (&ui_buf)[UI_BUFF_SIZE] );

void sendNodeSocket(int & new_socket, char const * data, const unsigned short DATA_SIZE);

int nodeSocket(int & server_fd);

string GetCurrentWorkingDir( void );

string createJsonString(string message);



#endif    /* FUNCTIONS_HPP */