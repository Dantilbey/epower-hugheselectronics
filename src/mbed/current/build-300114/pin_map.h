/* ==================================================================================================
ePower v0.01 - PIN_MAP.H                                    Last Update: 06 August 2013 Daniel Tilbey
=====================================================================================================
File containing pin assignments and register mappings
================================================================================================== */

#ifndef PINMAP_H
#define PINMAP_H

/* =============================================================================================== */
// Mbed pin mappings
/* =============================================================================================== */

#define TERIDIAN_TX    p9
#define TERIDIAN_RX    p10

// TERIDAN REGISTER MAPPINGS

// SET THESE AS DESIRED, FOR EXAMPLE TO CHANGE A MEASUREMENT FROM NARROW BAND TO WIDEBAND

//Common data
#define DELTA_TEMP_ADR    "00" // temperature (given as difference from 22degC)
#define FREQ_ADR          "01" // line frequency (Hz)
#define VOLTAGE_ADR       "06" // voltage (Vrms)

//Outlet 1 data
#define WATTS_ADR_1       "07" // real power (W)    
#define CURRENT_ADR_1     "0A" // current (Irms - amps)
#define PF_ADR_1          "0D" // power factor ( -0.95 to +1)

//Outlet 2 data
#define WATTS_ADR_2       "47" // real power (W)
#define CURRENT_ADR_2     "4A" // current (Irms - amps)
#define PF_ADR_2          "4D" // power factor ( -0.95 to +1)

#endif /* PINMAP_H */