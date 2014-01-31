/* ==================================================================================================
ePower Socket Program: power_obj.h                                       NOV/DEC 2009  Stuart Hadfield 
=====================================================================================================
Implementation of power data and power outlet objects

Description:
    Data object: stores ALL data read from Teridian (for all outlets)
    Outlet object: used to interface with actual outlets
    
Notes/todo:
    - later make each data object a class (and hence serialization/processing member fcns) but for debugging leave as struct
    - make error catching/handling more robust
    - need to porperly generalize for variable number of outlets
    - change outlet to directly derrive from DigitalOut object
    - **implement monitoring for 'bad state'
    - **implement some degree of standby detection/ device recognition here
    - **make identify() non-blocking
    
=================================================================================================== */
#ifndef POWEROBJ_H
#define POWEROBJ_H

#include "settings.h"
#include "words.h"

// external vars
extern Serial pc;                                                        // USB terminal connection

//--------------------------------------------------------------
// DATA OBJECT
struct data                                         // this is the data we actually care about and hence read from Teridian - data for ALL outlets here
{                                                     // for now store all as doubles - more precision than we actually need
    //common data                                   // ** be careful with bytelengths here **
    double delta_temp;                                 // temperature DIFFERENCE from 22C    LSB = 0.1C
    double freq;                                       // line freq in Hz                    LSB = 0.1Hz
    double voltage;                                    // V(rms)                             LSB = mVrms

    //outlet1
    double watts_1;                                    // Watts                              LSB=mW
    double current_1;                                  // A(rms)                             LSB=mA(rms)
    double pf_1;                                    // PowerFactor                        ranges from -0.95 to +1 ( + indicates current lagging voltage)

    //outlet2
    double watts_2;                                 // Watts                              LSB=mW
    double current_2;                                // A(rms)                             LSB=mA(rms)
    double pf_2;
    
    //* For now just care about these figures, later add more   
};
const data NULL_DATA = {UNDEFINED,UNDEFINED,UNDEFINED,UNDEFINED,UNDEFINED,UNDEFINED,UNDEFINED};        // null data for init/error purposes


//-------------------------------------------------------------
// OUTLET OBJECT - used to store data and status
class outlet                                          //: DigitalOut             // later inherit from DigitalOut object - wasnt working so halted this
{
    public:
        outlet(DigitalOut* relay_ptr, DigitalOut* led_ptr, int out_id);        // constructs an outlet object

        // member functions
        void turn_on();                                                        // function call so that status flag is maintained
        void turn_off();                                                       //(later overload these to = operator)
        void identify();
    
        // flags
        bool status;                                                        // on/off
        //  bool change                                                      // indicates a significant change in Power/V/I  -LATER
        
        // variables
        int id;                                                             // need to correlate UIs id tag with actual outlet
        DigitalOut* relay;                                                    // corresponding relay and LED - this method works, later change to direct inheritance?
        DigitalOut* led;
        // int standby;                                                        // comparison values for automatic shutoff
        // int overvoltage;
        // int overcurrent;

    private:
        void init();                                                        // outlet initialization
        //outlet& operator=(int value);                                        // for setting relay - later when we migrate to inheritance version
};

//==============================================================================================================
// power_obj.cpp
//==============================================================================================================

// initialize outlet object
void outlet::init() 
{
    // other vars are set by constructor
    
    turn_on();                                                                // default setting is ON for outlets
}

// turn outlet on
void outlet::turn_on()
{
    if(DEBUG) pc.printf("\r\nTurning ON outlet %i.", id);
    *relay = ON;
    status = ON;
}

// turn outlet off
void outlet::turn_off()
{
    if(DEBUG) pc.printf("\r\nTurning OFF outlet %i.", id);
    *relay = OFF;
    status = OFF;
}

// identify outlet
 //***** need to make this non-blocking -> either use a thread, or use ticker objects to trigger LED..
void outlet::identify()                                                      
{
    if(DEBUG) pc.printf("\r\nIDENTIFYING outlet %i.", id);

    *led = !(*led); wait(0.3);
    *led = !(*led); wait(0.3);
    *led = !(*led); wait(0.3);
    *led = !(*led); wait(0.3);
    *led = !(*led); wait(0.3);
    *led = !(*led); wait(0.3);
    *led = !(*led); wait(0.3);
    *led = !(*led); wait(0.3);
}

// outlet constructor - requires specification of relay, LED, and ID
outlet::outlet(DigitalOut* relay_ptr, DigitalOut* led_ptr, int out_id) 
{
    if(DEBUG) pc.printf("\r\nCreating outlet %i.", out_id);
    
    relay = relay_ptr;                            // associate outlet object with correct relay
    led = led_ptr;
    id = out_id;                                  // set id
    init();                                       // initialize flags and data
}

/*iNHERITANCE VERSION -> IMPLEMENT LATER
outlet::outlet(PinName pin)
{
    init();
    relay = DigitalOut(pin);
}



outlet::outlet(PinName pin) : DigitalOut(pin)
{
    init();
}

outlet& outlet::operator=(int value)
{
    this::DigitalOut = value;

    return this;
}*/

#endif /* POWEROBJ_H */