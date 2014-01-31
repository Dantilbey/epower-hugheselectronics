// This program does 2 things
// 1. Send data based on requirements
// 2. Recieve input from web server and carry out methods based on that input

#include "mbed.h"
#include "EthernetInterface.h"
#include "Websocket.h"
#include "String.h"

#include "Teridian.h"
#include "pin_map.h"
#include "data.h"
#include "settings.h"

int outlets[2];
DigitalOut outlet1(LED1);

char recv[256];
char data_to_send[256];

int main() {
	EthernetInterface eth;
	eth.init();
	eth.connect();
	ws.connect();

}

void initialise(char * decode_init) {
    int y;
    int limiter[6] = {0,0,0,0,0,0};
    char buffer[100];
    char container = '\0';
    
    for (int x = 1; x < strlen(decode_init); x++) {
        // find limiter values for initialising module
        if (decode_init[x] == '!') {
            limiter[0] = x;
        }
        else if (recv[x] == '/') {
            limiter[1] = x;
        }
        else if (recv[x] == '$') {
            limiter[2] = x;
        }
        else if (recv[x] == '@') {
            limiter[3] = x;
        }
        else if (recv[x] == '>') {
            limiter[4] = x;
        }
        else if (recv[x] == '&') {
            limiter[5] = x;
        }
        
        // test for end operation
        if (limiter[0] > 0)
            if (limiter[1] > 0)
                if (limiter[2] > 0)
                    if (limiter[3] > 0)
                        if (limiter[4] > 0)
                            if (limiter[5] > 0)
                                break;
    }
        
    // parse string for timestamp
    y = 0;
    for (int x = (limiter[0] + 2); x < limiter[1]; x++) {
        buffer[y] = recv[x];
        y++;
    }
    y = 0;
    //ws.send(buffer);
    // remove floating decimal point
    for (int x = 0; container != '.'; x++) {
        container = buffer[x];
        y++;
    }
    y -= 1;
    for (int x = 0; x < y; x++) {
        time_alloc[x] = buffer[x];
    }
    current_time = atof(time_alloc); // Set current_time to unix time w/o reccuring floating point
    //end of setting time
    
    // parse string for unit cost(KWh)
    y = 0;
    *buffer = NULL;
    for (int x = (limiter[1] + 1); x < limiter[2]; x++) {
        buffer[y] = recv[x];
        y++;
    }
    
    unit_cost = atoi(buffer);
    // end of setting unit cost
    
    // parse string for timer
    y = 0;
    *buffer = NULL;
    for (int x = (limiter[2] + 1); x < limiter[3]; x++) {
        buffer[y] = recv[x];
        y++;
    }
    
    read_data = atoi(buffer);
    // end of setting timer
    
    // parse string for array size
    y = 0;
    *buffer = NULL;
    for (int x = (limiter[3] + 1); x < limiter[4]; x++) {
        buffer[y] = decode_init[x];
        y++;
    }
    
    data_array_size = atoi(buffer); // Increment num_data_polls, when num_data_polls 
                               // equals array_size send data object as string 
                               // to websocket server.
    /* parse string for start value
    y = 0;
    *buffer = NULL;
    for (int x = (limiter[4] + 1); x < limiter[5]; x++) {
        buffer[y] = recv[x];
        y++;
    } */
    
    //DO_NOT_ALLOW = atoi(buffer);
    // end of setting start value
    
    INITIALISE = 1;
    pc.printf("COMPLETED PROCEDURE: INITIALISED\r\n");
}