// TERIDIAN.H //

#ifndef TERIDIAN_H
#define TERIDIAN_H

#include "mbed.h"
#include "pin_map.h"
#include "settings.h"
#include "data.h"

#define CR 0x0D
#define SOFT_RESET 'Z'
#define MPU_DATA_ACCESS ')'
#define HEX_FORMAT '$'
#define DECIMAL_FORMAT '?'

extern Serial pc;
extern char data_to_send[];
extern char time_alloc[];
extern int data_array_size;
extern float unit_cost;
extern int num_data_polls;

class Teridian : public Serial {
public:
    //Constructor
    Teridian(PinName tx, PinName rx) : Serial(tx, rx) { init(); }

    //Data types
    data all_data[100]; // temporary storage
    data* latest_data;
    data temp_data;

    //Functions
    void get_all_data();
    void get_temp_data();
    void stream_data();
    void load_all_data();

private:
    void init();
    void get_data(const char* adr, double &res);
    void get_cost(double &watt, double &cost, float &u_cost);
    void get_time(char &set_alloc_time,char *c_time);
    void get_temp_data(double &temp, double &res);
    void get_temp_data(char &temp, char &res);
    
    int num_data;
};

#endif /* TERIDIAN_H */