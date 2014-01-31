/* ==================================================================================================
ePower Socket Program: settings.h                                       NOV/DEC 2009  Stuart Hadfield 
=====================================================================================================
File containing 'EXTERNAL' settings - things which change frequently, or per build, and things which must be matched with UI program

Notes/todo:
    - put ALL settings here - eventually use SAME file to build both socket program and UI program
    - separate USB and DEBUG options more thoroughly
=================================================================================================== */
#ifndef SETTINGS_H
#define SETTINGS_H

// DEBUGGING
#define DEBUG                    1                                // trigger Debugging output
#define SUPER_DEBUG              1                               // use for extremely verbose output

#define ETHERNET                 0            // set to 0 if no ethernet connected (for testing)
#define USB                      1                                  // use if USB terminal connected

// SERVER IP ADDRESS - FIXED FOR NOW
#define SERVER_IP                 192,168,0,2                        // server IP adress - MUST MATCH!!!
#define SERVER_PORT             27015                            // server port

///****** CHANGING ANY SETTING BELOW HERE MAY CAUSE CRASH, NOT FULLY TESTED!!!

// PHYSICAL SETTINGS
#define NUM_OUTLETS             2                               // number of outlets on our socket (for now this is 1 or 2, later a variable)
    // at the moment we only send data for 1, but setting this to 2 allows us to turn both on/off

// DATA SETTINGS
#define MAX_PACKET_LENGTH 400                                // max length of our transmission in bytes (must be GREATER than actual length)
                    //***This MUST be long enough or program will compile but freeze on run.... PUT FORMULA HERE ***
#define NUM_READINGS             5  // 60                           // number of seconds before we transmit data (small for testing)
#define DATA_POLL_PERIOD         1.0                                //(seconds) currently poll data every second from Terridian


#endif /* SETTINGS_H */