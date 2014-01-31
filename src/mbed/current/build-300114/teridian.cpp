//TERIDIAN.CPP

#include "Teridian.h"

// Common data
#define is_temp 1 //t
#define is_freq 1 //f
#define is_volt 1 //v
// Testing Outlet 1
#define is_watt1 1 //w
#define is_cur1 1 //c
#define is_pf1 1 //p
// Testing Socket 2 *** Only Outlet 1 is currently available.
#define is_watt2 1 //w
#define is_cur2 1 //c
#define is_pf2 1 //p

void Teridian::init() {
    baud(38400);
    latest_data = all_data;
    num_data = num_data_polls;
}

void Teridian::get_all_data() { // request + receive data, for now just request data (blocks) UI needs, later request 
//more (ie for processing)
    // increment latest_data ptr
    
    data * set_data = latest_data;
    
    get_temp_data(temp_data.delta_temp,(set_data+num_data)->delta_temp);
    get_temp_data(temp_data.freq,(set_data+num_data)->freq);
    get_temp_data(temp_data.voltage,(set_data+num_data)->voltage);
    
    get_temp_data(temp_data.watts_1,(set_data+num_data)->watts_1);
    get_temp_data(temp_data.current_1,(set_data+num_data)->current_1);
    get_temp_data(temp_data.pf_1,(set_data+num_data)->pf_1);
    get_temp_data(temp_data.cost_1,(set_data+num_data)->cost_1);
    
    get_temp_data(temp_data.watts_2,(set_data+num_data)->watts_2);
    get_temp_data(temp_data.current_2,(set_data+num_data)->current_2);
    get_temp_data(temp_data.pf_2,(set_data+num_data)->pf_2);
    get_temp_data(temp_data.cost_2,(set_data+num_data)->cost_2);
}
/*    
    get_data(DELTA_TEMP_ADR,(set_data+num_data)->delta_temp);
    get_data(FREQ_ADR,(set_data+num_data)->freq);
    get_data(VOLTAGE_ADR,(set_data+num_data)->voltage);
    
    get_data(WATTS_ADR_1,(set_data+num_data)->watts_1);
    get_data(CURRENT_ADR_1,(set_data+num_data)->current_1);
    get_data(PF_ADR_1,(set_data+num_data)->pf_1);

    get_cost((set_data+num_data)->watts_1,(set_data+num_data)->cost_1,unit_cost);

    get_data(WATTS_ADR_2,(set_data+num_data)->watts_2);
    get_data(CURRENT_ADR_2,(set_data+num_data)->current_2);
    get_data(PF_ADR_2,(set_data+num_data)->pf_2);
    
    get_cost((set_data+num_data)->watts_2,(set_data+num_data)->cost_2,unit_cost);
}*/

void Teridian::get_temp_data() { // request + receive data, for now just request data (blocks) UI needs,
// later request more (ie for processing)
    // increment latest_data ptr
    
    int x = 0;
    char *set_alloc_time = temp_data.timestamp;
    while (x <= (strlen(time_alloc))) {
        set_alloc_time[x] = time_alloc[x];
        x++;
    }
    
    get_data(DELTA_TEMP_ADR,temp_data.delta_temp);
    get_data(FREQ_ADR,temp_data.freq);
    get_data(VOLTAGE_ADR,temp_data.voltage);
    
    get_data(WATTS_ADR_1,temp_data.watts_1);
    get_data(CURRENT_ADR_1,temp_data.current_1);
    get_data(PF_ADR_1,temp_data.pf_1);

    get_cost(temp_data.watts_1,temp_data.cost_1,unit_cost);

    get_data(WATTS_ADR_2,temp_data.watts_2);
    get_data(CURRENT_ADR_2,temp_data.current_2);
    get_data(PF_ADR_2,temp_data.pf_2);
    
    get_cost(temp_data.watts_2,temp_data.cost_2,unit_cost);
}

// get specific piece of data - refer to teridian firwmare description doc for formatting info
void Teridian::get_data(const char* adr, double &res) { // args: teridian address, and result
// storage (as string and double for now)                                        
    char temp[20]; // temp data storage string - use [] not char* for proper null termination

    //send request // class is derived from Serial so char auto put to UART
    putc(')'); // MPU_DATA_ACCESS 
    printf(adr); 
    putc('?' ); // Specifies decimal format (no hex used for new, easier for debug_) 
    putc(CR); // Carriage return - submits request
       
    while(getc() != '\n'); // wait for linefeed character (ignore echo) - next char is answer
       
    int i = 0; // temp vars - i to count length
    char check = '0'; // stores read char
    
    while(check != '>') // read chars until we get a '>', which indicates start of data return
    {
        if(readable())
        { 
            check = getc(); // read char and store in temp[] if valid digit
            if( check != 0x0D && check != '\n' && check != '>') temp[i] = check; // carriage return
            // follows answer ***make more robust - make to check for number or '.'
            i++;
        }
    }
    temp[i] = 0;                                                        // string termination   - 
    //                                                                     only for string display
   
   // first char of temp is +/-, then is answer as a float (in chars!!!)
   res = atof(temp);                                                       // convert to double - 
   //                                                     automatically ignores plus/minus sign!!!
}


void Teridian::get_cost(double &watt, double &cost, float &u_cost) {
    double kw = 0;
    double kwh = 0;
    double time = 1;
    
    kw = watt / 1000;
    kwh = kw * time;
    
    cost = kwh * u_cost;
}

void Teridian::get_temp_data(double &temp, double &res) {
    res = temp;
}

void Teridian::get_temp_data(char &temp, char &res) {
    res = temp;
}

void Teridian::stream_data() {
    char buffer[30];

    strcat(data_to_send, "{");
    strcat(data_to_send, "{\"info\":{\"type\":\"stream\"},\"data\":[");

    strcat(data_to_send, "{\"timestamp\":\"");
    sprintf(buffer, "%s", temp_data.timestamp);
    strcat(data_to_send, buffer);
    strcat(data_to_send, "\"");

    strcat(data_to_send, ",");

    strcat(data_to_send, "\"regular\":{");

    sprintf(buffer,"%f", temp_data.delta_temp);
    strcat(data_to_send, "\"temp\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");

    sprintf(buffer,"%f", temp_data.freq);
    strcat(data_to_send, "\"freq\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");

    sprintf(buffer,"%f", temp_data.voltage);
    strcat(data_to_send, "\"volt\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, "},");
    strcat(data_to_send, "\"outlet1\":{");
    
    sprintf(buffer, "%f", temp_data.cost_1);
    strcat(data_to_send, "\"ct\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");

    sprintf(buffer,"%f", temp_data.watts_1);
    strcat(data_to_send, "\"w\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");

    sprintf(buffer,"%f", temp_data.current_1);
    strcat(data_to_send, "\"c\":");      
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");

    sprintf(buffer,"%f", temp_data.pf_1);
    strcat(data_to_send, "\"p\":");      
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, "},");
    strcat(data_to_send, "\"outlet2\":{");

    sprintf(buffer, "%f", temp_data.cost_2);
    strcat(data_to_send, "\"ct\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");
    
    sprintf(buffer, "%f", temp_data.watts_2);
    strcat(data_to_send, "\"w\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");

    sprintf(buffer, "%f", temp_data.current_2);
    strcat(data_to_send,"\"c\":");
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, ",");

    sprintf(buffer, "%f", temp_data.pf_2);
    strcat(data_to_send,"\"p\":");
    strcat(data_to_send, buffer);

    strcat(data_to_send, "}]}");
}

void Teridian::load_all_data() {
    char buffer[30];

    strcat(data_to_send, "{");
    strcat(data_to_send, "{\"info\":{\"type\":\"stream\",\"arraylen\":");
    sprintf(buffer, "%d", data_array_size);
    strcat(data_to_send, buffer);
    
    strcat(data_to_send, "},\"data\":[");
    for (int x = data_array_size; x <= 0; x--) {
        strcat(data_to_send, "{\"timestamp\":\"");
        sprintf(buffer, "%s", temp_data.timestamp);
        strcat(data_to_send, buffer);
        strcat(data_to_send, "\"");
    
        strcat(data_to_send, ",");
    
        strcat(data_to_send, "\"regular\":{");
    
        sprintf(buffer,"%f", temp_data.delta_temp);
        strcat(data_to_send, "\"temp\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
    
        sprintf(buffer,"%f", temp_data.freq);
        strcat(data_to_send, "\"freq\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
    
        sprintf(buffer,"%f", temp_data.voltage);
        strcat(data_to_send, "\"volt\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, "},");
        strcat(data_to_send, "\"outlet1\":{");
        
        sprintf(buffer, "%f", temp_data.cost_1);
        strcat(data_to_send, "\"ct\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
    
        sprintf(buffer,"%f", temp_data.watts_1);
        strcat(data_to_send, "\"w\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
    
        sprintf(buffer,"%f", temp_data.current_1);
        strcat(data_to_send, "\"c\":");      
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
    
        sprintf(buffer,"%f", temp_data.pf_1);
        strcat(data_to_send, "\"p\":");      
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, "},");
        strcat(data_to_send, "\"outlet2\":{");
    
        sprintf(buffer, "%f", temp_data.cost_2);
        strcat(data_to_send, "\"ct\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
        
        sprintf(buffer, "%f", temp_data.watts_2);
        strcat(data_to_send, "\"w\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
    
        sprintf(buffer, "%f", temp_data.current_2);
        strcat(data_to_send,"\"c\":");
        strcat(data_to_send, buffer);
        
        strcat(data_to_send, ",");
    
        sprintf(buffer, "%f", temp_data.pf_2);
        strcat(data_to_send,"\"p\":");
        strcat(data_to_send, buffer);

        strcat(data_to_send, "}");
        
        if (x != data_array_size) strcat(data_to_send, ",");
    }
    strcat(data_to_send, "]}");
}