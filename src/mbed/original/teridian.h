/* ==================================================================================================
ePower Socket Program: teridian.h                                          NOV/DEC 2009  Stuart Hadfield
=====================================================================================================
Interface and data management for Teridian 7816612 via UART

Description:
    Currently request and read every peice of data separately. later use block reads for efficiency!!
    However only access fcn gets EVERY peice of data.  Later can add fcns to get specific data ie single outlet
    Everything sent as STRINGS for easy human reading / debugging
    
**Power Data We Retrieve: **    
    Common data:         Temperature, Frequency, Voltage
    Outlet specific:     Real power, Current

Teridian UART Settings:
    Baud = 38400bps, Databits = 8, Parity = None, Stop bits = 1, Flow Control = Xon/Xoff

FORMAT OF DATA STRING: s0o2nf data: T23432.43242 f234234.234 V32.23 W2132.234 I234.234    T23432.43242 f234234.234 V32.23 W2132.234 I234.234    
    header:    s - socket ID, o - number of outlets, n - seq number
    data: - start of data string
    x - indicates end of transmission
    
Notes/todo:
    - ****NEED TO VERIFY 100% whether we want narrowband or wideband figures!!!!!
    - ** currently get_data only works for two outlet devices, need to generalize!!!
    - *** currently only sends to UI data for SINGLE outlet, need to generalize!
    - currently processing not implemented ie check for overvoltage, etc
    - teridian.cpp and .h together - to be separated
    - NO ERROR CHECKING YET -> NEED TO ADD
    - ** need to check that serial read method truly robust
    - later: implement soft reset command if needed
    - **may need to perform calibration of device based on (later) physical testing and results
    - currently store results as both strings and doubles (debugging) - later dont need both, and need less precision
    - initialize data storage array
    - make members private
    
References:  To understand this code, refer to Teridian datasheet and firmware description document
=================================================================================================== */
#ifndef TERIDIAN_H
#define TERIDIAN_H

#include "mbed.h"
#include "power_obj.h"
#include "pin_map.h"
#include "settings.h"

// commands - currently these keywords not fully used - for later optimaztion
#define CARRIAGE_RETURN 0x0D                                              // use '\r' ?
#define SOFT_RESET 'Z'
#define MPU_DATA_ACCESS ')'
#define HEX_FORMAT '$'
#define DECIMAL_FORMAT '?'

// settings
#define SEQ_NUM_START_CHAR '0'                                            // for now limit seq_num to single digit for debugging (general char may confilt with parsing!)
#define SEQ_NUM_END_CHAR '9'

// external vars
extern Serial pc;                                                        // USB terminal connection
extern char data_to_send[];                                                // serialized form of data to send

// TERIDIAN POWER MONITOR OBJECT
class teridian : public Serial                                            // directly extends serial (UART) object
{
    public:
        data all_data[100];     //data all_data[NUM_READINGS];             // storage for ALL data not yet send to UI (later store at outlet for processing?)  **array must be sufficiently long
        data* latest_data;                                                 // ptr to most current reading
    
        char SEQ_NUM[1];        //char SEQ_NUM;                         // packet squence number - if we declare as char*, causes strcat() to freeze
        
        // 'global' comparison values for automatic shutoff
        // int standby;
        // int overvoltage;
        // int overcurrent;

        //constructor
        teridian(PinName tx, PinName rx) : Serial(tx, rx)  { init(); }    // auto calls Serial contstructor  
     
        // member functions
        void get_all_data();                                            // get all our data required from the teridian 
        void serialize();                                                // compile data into correct form for sending to UI    
 
    private:
        void init();                                                      // initialize UART connection and teridian device
        void get_data(const char* adr, double &res);    // get specific piece of data
        //void send_command(char command);                              // send a command to the terridian          
        //void send_read_command(int address);                             // send read command
        //void get_response(double &store_loc);      
        //void send_multiple_read_command(int *address, int num);                  // request read for multiple addresses
        //void send_block_read_command(int start_address, int end_address);     // request read for block of addresses
        //void clear_post_send();                                        // use this to manage vars after dumping data, ie reseting all_data
};

//==============================================================================================================
// teridian.cpp
//==============================================================================================================

/* 'To verify communication between the host and the 78M6612, the host must send a <CR> (carriage return) to the 78M6612.
  Communication is verified when the 78M6612 returns a > (greater than sign) known as the command prompt' */

// initialize UART connection and teridian device
void teridian::init()
{
    // setup parameters in microcontroller
    baud(38400);                                                        // settings 38400 baud, Databits = 8, Parity = None, Stop bits = 1, Flow Control = Xon/Xoff (all others match MUC default)
     
    // setup UART connection
    // send_command(SOFT_RESET);
    
    // calibration commands??
    
    // set vars
    *SEQ_NUM = SEQ_NUM_START_CHAR;                                        // limit char set for now for easy readability  
    latest_data = all_data;                                                // set pointer to first object in array
    
    //initialize data to 0 or Undefined?
}

/* 'To request information, the host sends the MPU data access command, the address (in hex) which is requested, the format in which the data 
is desired (Hex or Decimal) and a carriage return' */

// get all our data required from the teridian 
void teridian::get_all_data()                                              // request + receive data, for now just request data (blocks) UI needs, later request more (ie for processing)
{
    // increment latest_data ptr
    if(latest_data != (all_data+99) ) latest_data++;                     // circular array (later make PROPER circular array object)
    else latest_data = all_data;                                        // if at end of array reset to start
    
    //*** For now get every field individually.  Later, when we decide exactly what we want, optimize for block reads, repeats, etc.
    get_data(DELTA_TEMP_ADR,latest_data->delta_temp);                    // get temperature
    get_data(FREQ_ADR,latest_data->freq);                                // get frequency
    get_data(VOLTAGE_ADR,latest_data->voltage);                            // get voltage
    get_data(WATTS_ADR_1,latest_data->watts_1);                            // get outlet1 power
    get_data(CURRENT_ADR_1,latest_data->current_1);                        // get outlet1 current
    get_data(PF_ADR_1,latest_data->pf_1);                                // get outlet1 power factor
    get_data(WATTS_ADR_2,latest_data->watts_2);                            // get outlet2 power
    get_data(CURRENT_ADR_2,latest_data->current_2);                        // get outlet2 current  
    get_data(PF_ADR_2,latest_data->pf_2);                                // get outlet2 power factor
}

// get specific piece of data - refer to teridian firwmare description doc for formatting info
void teridian::get_data(const char* adr, double &res)                // args: teridian adress, and result stoarge (as string and double for now)                                        
{
    char temp[20];                                                        // temp data storage string - use [] not char* for proper null termination

    //send request                                                         // class is derived from Serial so char auto put to UART
    putc(')');                                                            // MPU_DATA_ACCESS 
    printf(adr); 
    putc('?' );                                                         // Specifies decimal format (no hex used for new, easier for debug_) 
    putc(CARRIAGE_RETURN);                                                // Carriage return - submits request
       
    while(getc() != '\n');                                                // wait for linefeed character (ignore echo) - next char is answer
       
    int i = 0;                                                            // temp vars - i to count length
    char check = '0';                                                    // stores read char
    
    while(check != '>')                                                    // read chars until we get a '>', which indicates start of data return
    {
        if(readable())
        { 
            check = getc();                                                // read char and store in temp[] if valid digit
            if( check != 0x0D && check != '\n' && check != '>') temp[i] = check;        // carriage return follows answer ***make more robust - make to check for number or '.'
            i++;
        }
    }
    temp[i] = 0;                                                        // string termination   - only for string display
   
   // first char of temp is +/-, then is answer as a float (in chars!!!)
   res = atof(temp);                                                       // convert to double - automatically ignores plus/minus sign!!!
}

// compile data into correct form for sending to UI    - send last NUM_READINGS worth of data
void teridian::serialize()
{
    char buffer[20];

    // increment sequence number
    if(*SEQ_NUM == SEQ_NUM_END_CHAR) *SEQ_NUM = SEQ_NUM_START_CHAR;        // limit to readable characters for now
    else (*SEQ_NUM)++;
   
    // clear data_to_send
    *data_to_send = NULL;
   
    // append a header with socket info, time, COMMAND
    strcat(data_to_send,"s0");                                            // socket ID  - later make these externally defined symbols
    strcat(data_to_send,"o1n");                                            // number of outlets
    if (SUPER_DEBUG) pc.printf("\r\n>>>>SEQ NUM %c <<<<\r\n",*SEQ_NUM);
    strcat(data_to_send, SEQ_NUM);                                        // data send seq num
   
    //append data - (for now) a single temp,I,V,Freq                     // indicate start of data
    strcat(data_to_send,"data: ");  
   // append (for now) a single temp,I,V,Freq 
    
   
    //for() // generalize for multiple outlets
    for(int i = 0; i < NUM_READINGS; i++ )                                 // send NUM_READINGS worth of data
    {  
        sprintf(buffer, " T%f", (latest_data-i)->delta_temp);            // reconvert double back to string. later send original string?
        strcat(data_to_send, buffer);                                    // add to our overall string
   
        sprintf(buffer, " f%f", (latest_data-i)->freq);
        strcat(data_to_send, buffer);
   
        sprintf(buffer, " V%f", (latest_data-i)->voltage);
        strcat(data_to_send,buffer);
   
        sprintf(buffer, " W%f", (latest_data-i)->watts_1);
        strcat(data_to_send, buffer);
   
        sprintf(buffer, " I%f", (latest_data-i)->current_1);      
        strcat(data_to_send,buffer);
        
        sprintf(buffer, " p%f", (latest_data-i)->pf_1);      
        strcat(data_to_send,buffer);
   
        //sprintf(buffer, " W%f", latest_data->watts_2);
        //strcat(data_to_send,buffer);
   
        //sprintf(buffer, " I%f", latest_data->current_2);
        //strcat(data_to_send,buffer);
    }
    strcat(data_to_send,"x");                                            // add end of data char??
}

#endif /* TERIDIAN_H */