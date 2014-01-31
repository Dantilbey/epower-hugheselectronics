#ifndef DATA_H
#define DATA_H

struct data                                         // this is the data we actually care about and hence read from Teridian - data for ALL outlets here
{                                                     // for now store all as doubles - more precision than we actually need
    //record
    char timestamp[20];
    //common data                                   // ** be careful with bytelengths here **
    double delta_temp;                                 // temperature DIFFERENCE from 22C    LSB = 0.1C
    double freq;                                       // line freq in Hz                    LSB = 0.1Hz
    double voltage;                                    // V(rms)                             LSB = mVrms

    //outlet1
    double cost_1;
    double watts_1;                                    // Watts                              LSB=mW
    double current_1;                                  // A(rms)                             LSB=mA(rms)
    double pf_1;                                    // PowerFactor                        ranges from -0.95 to +1 ( + indicates current lagging voltage)

    //outlet2
    double cost_2;
    double watts_2;                                 // Watts                              LSB=mW
    double current_2;                                // A(rms)                             LSB=mA(rms)
    double pf_2;
    
    //* For now just care about these figures, later add more   
};

struct temp_storage {
    char data[256];
};

#endif /* DATA_H */