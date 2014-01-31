/* ==================================================================================================
ePower Socket Program: comms.h                                           NOV/DEC 2009  Stuart Hadfield 
=====================================================================================================
Code for sending/receiving data over ethernet/powerline

Description:
    For now acts only as client: sends data and receives command back from UI on same connection
    All data sent via TCP
    **Socket IP semi-static, retrieved from DHCP server running on GUI box
    
Notes/todo:
    -make error catching/handling more robust (catch all callback err_t errors!!!)
    -eventually add listening functionality for receipt of asynchronous commands
    -add more comments to this file
    -need to improve command parsing, allow for arbitrary number of outlets
    -make send() part a member fcn of teridian object
    
Library: LWIP
    *** Lib only seems to work if I import example program then modify, not just import library!!!!
    (http://mbed.org/projects/cookbook/svn/EMAC/lwip/examples/simplehttpc)
=================================================================================================== */
#ifndef COMMS_H
#define COMMS_H

#include "mbed.h"
#include "TCPCallbackConnection.h"
#include "NetServer.h"
#include "iputil.h"
#include "settings.h"

// SET SERVER ADDRESS
struct ip_addr server_ipa = ipv4addr( SERVER_IP );
int server_port= SERVER_PORT;

// external flags                                                
extern bool CON_OPEN;                                            // indicates open TCP connection
extern bool COMMAND_RCVD;                                        // indicates we have recieved a command from the UI

// external vars - maybe later use return values
extern char* command_received;                                    // stores ENTIRE received command e.g TURNON OUT2    
extern char command;                                             // stores command word
extern char command_id_ch;                                        // stores associated id (as char) 
extern int command_id;                                            // stores associated id (as int) 
extern Serial pc;                                                // usb terminal connection

//data to send - keep external for now, need to pass into callback function later
extern char data_to_send[];            // serialized form of latest data

// 'public' function prototypes
void send_ACK(char command, char command_id);                     // send ACK to UI, neglect for now
void send_latest_data();                                                 // send data to UI       

//==============================================================================================================
// comms.cpp
//==============================================================================================================

// This fcn called everytime a packet is received for a TCPConnection which registerd this callback
// This is where we receive data (commands) from the UI
err_t recv_callback(TCPCallbackConnection *arg, struct pbuf *p, err_t err) 
{
    if(DEBUG) pc.printf("\r\nrecv_callback");
  
    if(p==NULL) 
    {
        if (DEBUG) pc.printf("\r\nConnection closed by server!");
        delete arg;
        return ERR_OK;
    }

    while(p) 
    {
        command_received = (char*)p->payload;
        if (DEBUG) 
        {
            pc.printf("\r\n>>>>>>>>>>>> ");
            for(int i=0; i < p->len; i++) pc.printf("%c", command_received[i]);
            pc.printf("<<<<<<<<<<<\r\n<");
        }
        p = p->next;
    }
    arg->recved(p->tot_len);
    pbuf_free(p);
      
    // parse rcvd command                                            // sloppy temp parsing char-> int for now    
    command = command_received[0];                                    // assume first char is command word
    command_id_ch = command_received[1];                             // assume second char is command id
    if(command_id_ch == '0') command_id = 0;                        // convert to int
    else if(command_id_ch == '1') command_id = 1;
    else command_id = 0;
    COMMAND_RCVD = TRUE;                                            // indicate we have received command
    
    // ** FOR NOW Just close connection after we have received data
    arg->close();                                                      // this works, successfully closes connection
    if (DEBUG) pc.printf("\r\nWe have closed connection!");
    CON_OPEN=0;                                                        // set flag too (b/c we dont need to poll anymore)
  
    return ERR_OK;
}

// Connection CALLBACK FUNCTION  -- Connection etablished, lets send the data 
err_t connected_callback(TCPCallbackConnection *arg, err_t err) 
{
    if (DEBUG) pc.printf("\r\nConnected Callback! writing: %s\r\n",data_to_send);
    
    if(arg->write(data_to_send, strlen(data_to_send)) != ERR_OK)     // write data_to_send to our socket, make sure it succeeds
    {
        if (DEBUG) pc.printf("error - Could not write");
        error("Could not write", 0);
    }
    return ERR_OK;                                                    
}

//send all data - data must be serialized before sending to this fcn
void send_latest_data()                                             // should return something to indicate success
 {
    if (DEBUG) pc.printf("\r\nAttempting to send data: %s\r\n",data_to_send);
  
 if(ETHERNET)    // only do anything if ethernet actually conencted
 { 
    TCPCallbackConnection *con = new TCPCallbackConnection(        // create our TCP object
        server_ipa, server_port, NO_SENT_FNC, &recv_callback, 
        NO_POLL_FNC, &connected_callback, NO_ERR_FNC);
    
    if (SUPER_DEBUG) pc. printf("\r\nconnecting to server");
    con->connect();                                                    // open TCP connection -> using DHCP on windows box -> later should use static IP
    if (SUPER_DEBUG) pc. printf("\r\npast connection");

    CON_OPEN=1;                                                        // set flag b/c WE NEED TO POLL SOCKET
  }
  
 else printf("\r\nNO ETHERNET -> ABORTING SEND\r\n");  
}

// igonore for now
void send_ACK(char command, char command_id)     //send ACK to UI
{
    pc.printf("ACK");
    pc.putc(ACK);
    pc.putc(command);
    pc.putc(command_id);
    pc.putc('\n');

}
#endif /* COMMS_H */