//============================================================================
// Name        : com.cpp
// Author      : Ben Newey
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================


// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <jsoncpp/json/json.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/file.h>
#include <sys/socket.h>

#include <vector>
#include <iostream>
#include <iomanip>

#include <ctime>
#include <ratio>
#include <chrono>

#include <pthread.h>

// Socket header
#include <netinet/in.h> 

#include <mysql/mysql.h>

//forward declaration
class ACHandler;
//Functions File
#include "./functions.hpp"


#define PORT 8081 //also defined in functions.h


using namespace std;

//BUFF_SIZE defined in functions.cpp

struct arg_struct {
    vector<string> cond_unit_values;
	vector<string> zone_values;
	MYSQL mysql;
};
struct arg_struct args;

void * sendToDb(void * arguments){
	struct arg_struct *args1 = (struct arg_struct *)arguments;

	MYSQL mysql = (MYSQL) args1->mysql;
	vector<string> cond_unit_data = args1->cond_unit_values;
	vector<string> zone_data = args1->zone_values;

	//Lights sql
	auto iter = cond_unit_data.begin();
	for ( ; iter !=  cond_unit_data.end(); iter++){
		if(!(mysqlQueryNoReturn(mysql, (*iter)))){
			cout<<"Error sending light data to DB"<<endl;
		}
	}

	//Switch sql 
	auto iter2 = zone_data.begin();
	for ( ; iter2 !=  zone_data.end(); iter2++){
		if(!(mysqlQueryNoReturn(mysql, (*iter2)))){
			cout<<"Error sending light data to DB"<<endl;
		}
	}

	return NULL;
}

pthread_mutex_t UI_args_mutex =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t retrying_mutex = PTHREAD_MUTEX_INITIALIZER;
bool retrying_Ui_reconnect = false;

struct arg_struct_UI {
    int new_socket;
	int server_fd;
};
struct arg_struct_UI args_UI;

void * reconnectUiInNewThread(void * arguments){
	struct arg_struct_UI *args1 = (struct arg_struct_UI *)arguments;

	pthread_mutex_lock(&UI_args_mutex);
	int server_fd = (int) args1->server_fd;
	int new_socket = (int) args1->new_socket;
	pthread_mutex_unlock(&UI_args_mutex);
	

	if(new_socket != -1)
			close(new_socket);
		if(server_fd != -1)
			close(server_fd);

		server_fd = -1;
	new_socket = nodeSocket(server_fd);

	while(new_socket == -1){
		if(new_socket != -1)
			close(new_socket);
		if(server_fd != -1)
			close(server_fd);

		server_fd = -1;
		new_socket = nodeSocket(server_fd);
	
		cout<<"Check ip addr to make sure IP ADDRESS is correct. New socket:"<<new_socket<<endl;
	}
	
	//Set our global struct
	pthread_mutex_lock(&UI_args_mutex);
	args_UI.server_fd = server_fd;
	args_UI.new_socket = new_socket;
	pthread_mutex_unlock(&UI_args_mutex);

	pthread_mutex_lock(&retrying_mutex);
	retrying_Ui_reconnect = false;
	pthread_mutex_unlock(&retrying_mutex);

	return NULL;
}

int main() {
	
	//initialize socket 
	// values are held in struct args_UI above
	args_UI.new_socket = nodeSocket(args_UI.server_fd);
	
 	
	// Allocate memory for read buffer, set size according to your needs
	int serial_port;
	char read_buf [BUFF_SIZE];
	memset(&read_buf, '\0', sizeof(read_buf));

	//Allocate memory for ui_buf buffer
	char ui_buf[UI_BUFF_SIZE];
	memset(&ui_buf, '\0', sizeof(ui_buf));

	//Allocate memory for write_buf command buffer
	char write_buf[WRITE_BUFF_SIZE];
	memset(&write_buf, '\0', sizeof(write_buf));

	//Create vector for our switch values
	vector<vector<short>> data_in_vector((ZONES_MAX_SIZE+COND_UNIT_MAX_SIZE));

    //Set / read in settings for our Port
	usb_port(serial_port);

	//Connect to MySQL Database
	MYSQL mysql;
	mysqlConnect(mysql);
	

	// //Get Switch config variables
	vector< vector<string> > zone_variables;
	if(!(mysqlQueryFixed(mysql,"SELECT id, array_index, name, description, mode  FROM zones ORDER BY id ASC" , zone_variables))){
		cout<<"Query to MySQL did not successfully get zone variables, default variables applied"<<endl;
		return 0;
	}

	//Get Light config variables
	vector<vector<string> > cond_unit_variables;
	if(!(mysqlQueryFixed(mysql,"SELECT id, array_index, type, description, compressor_on FROM conditioning_units ORDER BY id ASC" , cond_unit_variables))){
		cout<<"Query to MySQL did not successfully get cond_unit variables"<<endl;
		return 0;
	}

	//Get Adjustment Table config variables
	vector<vector<string> > adj_table_variables;
	if(!(mysqlQueryFixed(mysql,"SELECT id, temp_min, temp_max, down_2, down_1, no_change, up_1, up_2 FROM adjustment_table ORDER BY id ASC" , adj_table_variables))){
		cout<<"Query to MySQL did not successfully get cond_unit variables"<<endl;
		return 0;
	}

	//Set Start time for Timers
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	//Set Start Time for DB update
	std::chrono::steady_clock::time_point db_timer1 = std::chrono::steady_clock::now();
	
	//Create ACHandler with our config variables
	shared_ptr<ACHandler> sh(make_shared<ACHandler>(zone_variables, cond_unit_variables, adj_table_variables));

	//numReads: num of reads from port
	//n: num of iterations to read exact num of bits | 0 means nothing read this iteration, > 0 means something has been read 
    long numReads = 0;
	long numJsonSends = 0;
	int numIterations = 0;
	int totalReadChars = 0;
	int missedReads =0;

    while(true){
		

		//    the port might have been disconnected, so we will check for reconnection
		if( missedReads > 10){//if the usb port been disconnected
			if(serial_port > 0){
				close(serial_port);
			}

			///
			//python test to reset USB 
			string command = "python " + GetCurrentWorkingDir() + "/reset_usb.py pathpci /sys/bus/pci/drivers/xhci_hcd/0000:00:14.0";
			string command2 = "python "  + GetCurrentWorkingDir() + "/reset_usb.py search \"Future Technology Devices International\"";
			system(command.c_str());
			usleep(5000000);
			///
			
			if(usb_port(serial_port) > 0){
				numReads = 0;
				cout<<"Successfully reconnecting to Port"<<endl;
				missedReads=0;
				totalReadChars = 0;
				numIterations = 0;
				t1 = std::chrono::steady_clock::now();
				//Set stop on reconnect
				
			}else{	
				//Send error to UI
				string error_json = createJsonString("{error: 1}");
				char const * stringified_error_json = error_json.c_str();
				int error_size = strlen(stringified_error_json);
				pthread_mutex_lock(&UI_args_mutex);
					sendNodeSocket(args_UI.new_socket, stringified_error_json , error_size);
				pthread_mutex_unlock(&UI_args_mutex);
				cout<<"Attempting to reconnect to Port in 5 seconds..."<<endl;
				usleep(5000000);
			}
				
		}else{ // if usb not disconnected, poll & read
			totalReadChars = 0;
			numIterations = 0;
			totalReadChars = read_bytes(read_buf, serial_port, numIterations);	
		}

		if( numIterations > 0 && totalReadChars == BUFF_SIZE){ 
			//if something was successfully polled and read from USB, do stuff with this data
			//cout<<"If poll&read ----"<<"WriteResponse: "<<writeResponse<<"  |  ReadResponse: "<<numIterations<<endl;
			//print_buf(read_buf, numIterations ,totalReadChars);
			missedReads = 0;
	
			//Handle Time Updates
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
			t1 = std::chrono::steady_clock::now();
			(*sh).updateCheckTimer(time_span.count());

			//Take read data and fill our data vector for update
			//data_in_vector can be edited from UI commands also
			getDataFromRead(read_buf, data_in_vector);			

			numJsonSends++;
			const string tmp2 = (*sh).createJsonDataString( numJsonSends);
			//convert string to char array
			char const * stringified_json = tmp2.c_str();
			int size = strlen(stringified_json);

			//make sure client is still connected to socket
			pthread_mutex_lock(&UI_args_mutex);
			int stillAlive = readNodeSocket(args_UI.new_socket, ui_buf);
			pthread_mutex_unlock(&UI_args_mutex);
			//Print UI buff 
			//print_ui_buff(ui_buf);

			//read from node js socket here
			//sterilize string here
			//write to port here
			
			if(stillAlive > 0 && !retrying_Ui_reconnect){
				//Send out data to NodeJS
				pthread_mutex_lock(&UI_args_mutex);
				sendNodeSocket(args_UI.new_socket, stringified_json, size);
				pthread_mutex_unlock(&UI_args_mutex);
				
				if(ui_buf[0]=='0' && ui_buf[1]=='5'){
					// //toggle light
					// string switch_to_toggle = "";
					// switch_to_toggle.push_back(ui_buf[2]);
					// switch_to_toggle.push_back(ui_buf[3]);
					// switch_to_toggle.push_back(ui_buf[4]);
					// int switch_id = stoi(switch_to_toggle);
					// //Edit data_in_vector
					// if(!((*sh).setSwitchToggle(switch_id))){
					// 	cout<<"Error: Did not toggle switch with array_index:"<<switch_id<<endl;
					// }
					
					
					
				}
				if(ui_buf[0]=='0' && ui_buf[1]=='6'){
					// change to turn all on(*sh).setStart();					
					
				}
				if(ui_buf[0]=='9' && ui_buf[1]=='9'){
					if(!(mysqlQueryFixed(mysql,"SELECT id, array_index, name, description, mode  FROM zones ORDER BY id ASC" , zone_variables))){
						cout<<"Query to MySQL did not successfully get zone variables, default variables applied"<<endl;
						return 0;
					}

					//Get Light config variables
					if(!(mysqlQueryFixed(mysql,"SELECT id, array_index, type, description, compressor_on  FROM conditioning_units ORDER BY id ASC" , cond_unit_variables))){
						cout<<"Query to MySQL did not successfully get cond_unit variables"<<endl;
						return 0;
					}
					//Get Adjustment Table config variables
					if(!(mysqlQueryFixed(mysql,"SELECT id, temp_min, temp_max, down_2, down_1, no_change, up_1, up_2 FROM adjustment_table ORDER BY id ASC" , adj_table_variables))){
						cout<<"Query to MySQL did not successfully get adj_table variables"<<endl;
						return 0;
					}

					//Reset will destruct old pointers
					sh.reset();
					//Create ACHandler with our config variables
					shared_ptr<ACHandler> sh_tmp(make_shared<ACHandler>(zone_variables, cond_unit_variables, adj_table_variables));
					sh = sh_tmp;
					cout<<"DB Variables Changed"<<endl;
				}

				

			}else{
				//create 'new' socket (should resuse old one) and wait for client to reconnect until timeout
				cout<<"Client disconnected; Waiting for reconnect."<<endl;

				if(!retrying_Ui_reconnect){
					cout<<"Trying reconnect in new thread."<<endl;
					pthread_mutex_lock(&retrying_mutex);
					retrying_Ui_reconnect = true;
					pthread_mutex_unlock(&retrying_mutex);

					//Start new thread to try to connect 
					pthread_t ui_thread;

					if (pthread_create(&ui_thread, NULL, &reconnectUiInNewThread, (void *)&args_UI) != 0) {
						printf("Uh-oh! Reconnect UI Thread Failed \n");
						return -1;
					}
					//Detatch, thread will update retrying_Ui_reconnect bool when it finishes
					pthread_detach(ui_thread);		
					
				}
			}
	
			//Call Update to ACHandler Object
			(*sh).updateUsingData(data_in_vector);

			//Have this write section after we recieve potential override commands from UI
			//Edit write_buf with relay_p pointer to array
			editWriteBuf(write_buf, sh);

			//Write to Serial Port to Update Relays
			write_bytes(serial_port, write_buf);

			//_write_buff(write_buf, 3 ,3 );

			//Send Update to DB ever 30 seconds
			std::chrono::steady_clock::time_point db_timer2 = std::chrono::steady_clock::now();
			std::chrono::duration<double> db_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(db_timer2 - db_timer1);
			
			if( db_time_span.count() > 30){

				pthread_t db_thread;
				
				args.cond_unit_values = (*sh).getMySqlSaveStringCondUnits(mysql);
				args.zone_values = (*sh).getMySqlSaveStringZones(mysql);

				args.mysql = mysql;

				if (pthread_create(&db_thread, NULL, &sendToDb, (void *)&args) != 0) {
					printf("Uh-oh!\n");
					return -1;
				}
				cout<<"Writing to DB"<<endl;
				db_timer1 = std::chrono::steady_clock::now();
				pthread_detach(db_thread);
			}

			//Reset writeResponse & numIterations
			numIterations = 0;
		}else{
			missedReads++;
		}
        numReads++;
    }

	//Clean up
	//mysqlCloseConnect(mysql);
	
	close(serial_port);


}

