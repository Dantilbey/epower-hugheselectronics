/* ==================================================================================================
ePower Socket Program: pin_map.h                                           NOV/DEC 2009  Stuart Hadfield 
=====================================================================================================
File containing pin assignments and register mappings

Notes/todo:
    - put ALL variable pin mappings here
=================================================================================================== */
#ifndef PINMAP_H
#define PINMAP_H

/* ********* MBED PIN MAPPINGS  ********** */
#define TERIDIAN_TX    p9
#define TERIDIAN_RX    p10

//// set correct relay pins                             *** need to carefully check these pin assigns****
//DigitalOut outlet0relay(LED2);                        // use LEDs for now for testing, assign pins later
//DigitalOut outlet1relay(LED3);                        // ***CURRENTLY IN MAIN.CPP -> change there ***


/* ********* TERIDAN REGISTER MAPPINGS (DO NOT CHANGE!!!!) ********** */
// SET THESE AS DESIRED, FOR EXAMPLE TO CHANGE A MEASUREMENT FROM NARROW BAND TO WIDEBAND
// common data
#define DELTA_TEMP_ADR    "00"                            // temperature (given as difference from 22degC)
#define FREQ_ADR          "01"                            // line frequency (Hz)
#define VOLTAGE_ADR       "06"                            // voltage (Vrms)

// outlet 1 data
#define WATTS_ADR_1       "07"                          // real power (W)    
#define CURRENT_ADR_1     "0A"                            // current (Irms - amps)
#define PF_ADR_1          "0D"                            // power factor ( -0.95 to +1)

// outlet 2 data
#define WATTS_ADR_2       "47"                             // real power (W)
#define CURRENT_ADR_2     "4A"                            // current (Irms - amps)
#define PF_ADR_2          "4D"                            // power factor ( -0.95 to +1)

// other data
//CURRENTLY NOT USED - energy, cost, apparent power, etc.



#endif /* PINMAP_H */