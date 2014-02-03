#include "mbed.h"
#include "EthernetInterface.h"
#include "Websocket.h"
#include "string.h"

#include "Teridian.h"
#include "pin_map.h"
#include "data.h"
#include "settings.h"

bool INITIALISE = 1;

int outlets[2];
DigitalOut outlet1(LED1);
DigitalOut outlet2(LED2);    
DigitalOut outlet3(LED3);

char recv[256];
char data_to_send[256];

int long current_time;
char time_alloc[30];
int POLL_DATA;
int poll_timer;
int read_data;
int num_data_polls;
int data_array_size;
float unit_cost;
int reserve;
bool pause;

Websocket ws("ws://192.168.0.18:8888/ws/module/2");
Serial pc(USBTX,USBRX);
Teridian interface(TERIDIAN_TX,TERIDIAN_RX);
DigitalOut led_active(LED4);
Ticker data_poll_ticker;

void _init_();
void initialise(char * decode_init);
void set_data_flag();
void validate();

int main() {
    EthernetInterface eth;
    eth.init();
    eth.connect();
    ws.connect();
    
    _init_();
    
    while(1) {
        if (POLL_DATA != 0) {
            pc.printf("NIGHN\r\n");
            led_active =! led_active; 
            POLL_DATA = 0;
            poll_timer++;
        }
        
        if (!INITIALISE) {
            validate();
        }
        
        if(ws.read(recv)) {
            if (!INITIALISE) initialise(recv);
                pc.printf("%s", recv);
        }
        pc.printf("ENDEND\r\n");
    }
}

void _init_() {
    read_data = 1;
    data_array_size = 20;
    unit_cost = 1.5;
    set_time(1256729737);
    pause = 1;
    
    num_data_polls = 0;
    POLL_DATA = 0;
    poll_timer = 0;
    data_poll_ticker.attach(&set_data_flag, DATA_POLL_PERIOD);
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
    // parse string for start value
    y = 0;
    *buffer = NULL;
    for (int x = (limiter[4] + 1); x < limiter[5]; x++) {
        buffer[y] = recv[x];
        y++;
    }
    
    //DO_NOT_ALLOW = atoi(buffer);
    // end of setting start value
    
    // parse string for assigning outlets
    y = 0;
    *buffer = NULL;
    for (int x = (limiter[5] + 1); x < strlen(recv); x++) {
        buffer[y] = recv[x];
        y++;
    }
    
    for (int x = 0; x < NUM_OUTLETS; x++) {
        container = buffer[x];
        outlets[x] = container;
    }
    //end of assigning outlets
    if (outlets[0] == 0) outlet1 = 1; 
    if (outlets[1] == 0) outlet2 = 1; 
    INITIALISE = 1;
    pc.printf("COMPLETED PROCEDURE: INITIALISED\r\n");
}

void set_data_flag() {
    POLL_DATA++;
}

void validate() {
    char buffer[2];
    sprintf(buffer, "%c", NUM_OUTLETS);
    strcat(data_to_send, "{\"info\":{\"type\":\"validate\",\"numoutlets\":\""); 
    strcat(data_to_send, buffer);
    strcat(data_to_send, "\"}}");
    ws.send(data_to_send);
    *data_to_send = NULL;

    wait(0.1);
}