/* ==================================================================================================
ePower Socket Program V1.0 - MAIN.CPP                                   NOV/DEC 2009  Stuart Hadfield 
=====================================================================================================
Code for a single Epower Socket Device 

Commands Implemented: Turn On, Turn Off, Identify
Commands Not Yet Implemented: DataRequest, ACKs, etc...

Description:
    -polls teridan every second (DATA_POLL_PERIOD)
    -sends data over ethernet every NUM_READINGS(*DATA_POLL_PERIOD).
    -acts as client only, no ACKs for now.  commands rcvd after data sent to UI on same TCP connection
    
Notes/todo:
    -***NO ERROR CHECKING YET -> NEED TO ADD
    - should add watchdog
    - **need to generalize relays for variable socket number (and move to pin file)
    
Testing Status:    
    
DEBUGGING:
    USB -> indicates USB connection, allows control/debugging via terminal
    
Libraries used: LWIP
    *** Lib only seems to work if I import example program then modify, not just import library!!!! (http://mbed.org/projects/cookbook/svn/EMAC/lwip/examples/simplehttpc)
=================================================================================================== */
// Includes -> to be moved to header file
#include "mbed.h"                                            // MUC library files
#include "pin_map.h"                                        // MUC pin mappings
#include "words.h"                                            // keywords
#include "teridian.h"                                        // teridian device interface
#include "comms.h"                                           // tcp communication interface
//#include "comms_test.h"                                   // using this instead of comms.h speeds up compilation!!!
#include "settings.h"                                       // various settings

// IMPORTANT PARAMETERS
char version[] = "1.1";
char bin_descrip[] = "Reports Teridian readings and receives commands via both ethernet and terminal";

// Global variables
char* command_received;                                        // stores received command e.g TURNON OUT2

char data_to_send[ MAX_PACKET_LENGTH];
Ticker data_poll_ticker;                                      // this object handles data poll interrupt
char command;                                                 // latest command word e.g. TURNON
int command_id;                                               // indicates outlet to be acted on (if applicable)
char command_id_ch;                                            // outlet to be acted on as a character

// set correct relay pins                                     // *** need to carefully check these pin assigns****   -> move to pin file
DigitalOut outlet0relay(LED1);                                // use LEDs for now for testing, assign pins later
DigitalOut outlet0led(LED1);                                // use same LED for relay for testing
DigitalOut outlet1relay(LED2);                              // **need to make this an array
DigitalOut outlet1led(LED2);
DigitalOut test_led(LED4);

// flags
bool COMMAND_RCVD;                                            // command received indicator
int POLL_DATA;                                                // indicates time to poll data (int for now to indicate if we are behind in data polls)
bool CON_OPEN;                                                // indicates we have an open tcp connection we need to poll from

// function prototypes
void init();                                                // initialization routine
void set_data_flag();                                         // sets flag that we need to poll data -> used as buffer b/c we dont wont to interrupt certain things!

void show_menu();

bool REFRESH_MENU;

// testing functions and vars------------------
Serial pc(USBTX, USBRX);

//=============================================================================================================
// MAIN FUNCTION BLOCK
//=============================================================================================================
int main() 
{
     //if (DEBUG) 
       //  pc.printf("\r\n\r\n<>>>> Starting socket program version %s <<<<<>\r\n =%s=\r\n", version,bin_descrip);   
         pc.printf("\r\n\r\n<>>>> Starting socket program version %s, debug level %i <<<<<>\r\n =%s=\r\n", version, (DEBUG+SUPER_DEBUG) ,bin_descrip );
        
        //pc.printf("\r\n\r\n TEST %i *** %c **** %i **** %c \r\n\r\n",DEBUG+SUPER_DEBUG,DEBUG+SUPER_DEBUG,(DEBUG+SUPER_DEBUG),(DEBUG+SUPER_DEBUG));
        
    //pre-alpha only init
    int num_data_polls = 0;
    
    // create vars and initialize everything
    outlet* outlets[NUM_OUTLETS];                               // pointers to all outlets on our socket
    outlets[0] = new outlet(&outlet0relay, &outlet0led,0);      // create outlet and assign relay
    outlets[1] = new outlet(&outlet1relay, &outlet1led,1);         // *** need to make RELAYS an array and this a loop
    teridian power_monitor(TERIDIAN_TX,TERIDIAN_RX);            // setup teridian on correct UART pins
    init();                                                     // setup MUC, set up terridain, load default states
   
    if(DEBUG)
    {
        outlets[0]->identify();
        outlets[1]->identify();
    
        if (USB) 
        {
            show_menu();
            REFRESH_MENU = CLEAR;
        }
    
    }
    
    // --------------------------- main loop --------------------------------------
    while(1) 
    {
        if (REFRESH_MENU) 
        {
            show_menu();
            REFRESH_MENU = CLEAR;
        }
        
        // networking
        if(CON_OPEN)NetServer::poll();                          // if we have an open connection, need to poll it
       
        // TESTING -> allow SIMULATION of ETHERNET COMMAND via USB terminal
        if (USB) if( pc.readable() )                            // if "command waiting"
        {
            command = pc.getc();                                // get command to act on
            pc.putc(command);                                   // echo command to terminal
            command_id_ch = pc.getc();                          // get outlet to act on
            pc.putc(command_id_ch);
            if(command_id_ch == '0') command_id = 0;
            else if(command_id_ch == '1') command_id = 1;
            else  command_id = 0;
            COMMAND_RCVD = TRUE;                                // set flag indicating command received
             REFRESH_MENU = SET;
            pc.putc('\n'); pc.putc('\n');
        }
   
        // CHECK IF COMMAND RECEIVED FROM UI 
        if(COMMAND_RCVD == TRUE) 
        {                
            if(DEBUG)                                            // print the command and outlet we have rcvd
            {                                
                pc.printf("\r\ncommand: ");
                pc.putc(command);
                pc.printf("\r\nid: ");
                pc.putc(command_id_ch);    
            }
  
           switch(command)
           {
            case TURN_ON:
                if(DEBUG) pc.printf("\r\nTURN ON Command");        
                outlets[command_id]->turn_on();                    // turn on specified outlet
                //send_ACK(command,command_id);                    // neglect for now
                COMMAND_RCVD = FALSE;                              // clear flag
                break;
            
            case TURN_OFF:
                if(DEBUG) pc.printf("\r\nTURN OFF Command");
                outlets[command_id]->turn_off();                // turn off specified outlet
                //send_ACK(command,command_id);                    // neglect for now
                COMMAND_RCVD = FALSE;                            // clear flag
                break;
                
            case IDENTIFY:
                if(DEBUG) pc.printf("\r\nIdentify Command");
                outlets[command_id]->identify();                // identify specified outlet
                COMMAND_RCVD = FALSE;
                break;   
           
            //case ACK_RCVD: etc...                              // for now ignore other commands
                    
            //case DATA_REQUEST:                                // for now send data by default as response (UI can always ignore)
            default:                        
                if(DEBUG) pc.printf("\r\nDEFAULT Command");
                //send_data();                                     // SEE CODE BELOW - for now act as client and send data automatically (timer routine) after NUM_READINGS data reads
                COMMAND_RCVD = FALSE;
            }
        }    

        // CHECK IF TIMER(s) HAVE EXPIRED                        // Ticker object automatically INTERUPTs main loop and calls correct function periodically after specified interval. We use flag however so code isnt interrupted in undesireable state!              
        if(POLL_DATA != CLEAR)                                    // indicates we have reached time of next data poll - handles int or bool
        {
            test_led = !test_led;                                // toggle LED to indicate data poll period    
            num_data_polls++;                                    // alpha build only
            power_monitor.get_all_data();                        // get data from teridian
            POLL_DATA = CLEAR;                                    // clear flag
        }     

        //alpha build only - send data automatically after a minute - later restore to send on command request
       if(num_data_polls == NUM_READINGS)                        // collected enough data, time to send
       {
           power_monitor.serialize();                            // update data to send (put in sending form)
           send_latest_data();                                            // this fcn (for now) also gets command back from UI and set flag if nec
           num_data_polls = 0;                                    // reset count
       }  
    } // end while(1)
} // end main()
//=============================================================================================================

// initialization routine
void init()
{
    // set all flags to defaults
    COMMAND_RCVD = FALSE;
    POLL_DATA = SET;                                            // set for initial data poll
    CON_OPEN = FALSE;
       
    //setup vars
    command = 0;                                                // NULL
    command_id = INVALID;
    
    //timer setup
   data_poll_ticker.attach(&set_data_flag, DATA_POLL_PERIOD); // args: the address of the function to trigger and the interval (in seconds)

    // setup networking, MUC,Terridain, Outlets....
}

// sets flag that we need to poll data -> used as buffer b/c we dont wont to interrupt certain things!
void set_data_flag()
{
   // if(SUPER_DEBUG) pc.printf("\r\n Timer Function Called >%i< ", POLL_DATA);    
    POLL_DATA++;                                                // POLL_DATA = SET;
}

// terminal menu for debugging
void show_menu()
{
    pc.printf("\r\n\r\n >>>>>INPUT COMMANDS AT ANYTIME: <<<<\r\n");
    pc.printf("commands: TurnON = 2, TurnOff = 3, Identify = 4\r\n");
    pc.printf("outlet ids:  0   1\r\n");
    pc.printf("FORMAT: (command)(outlet_id)\r\n");
}


