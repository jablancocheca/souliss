// Configure the framework
#include "bconf/MCU_ESP8266.h"     // Load the code directly on the ESP8266
#include "conf/Gateway.h"          // The main node is the Gateway
#include "conf/usart.h"            // USART
#include "conf/IPBroadcast.h"

// **** Define the WiFi name and password ****
#define WIFICONF_INSKETCH
#define WiFi_SSID             "SSID of wifi-network"
#define WiFi_Password         "password of wifi-network"  

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Souliss.h"

// Define the network configuration according
// to your router settings
uint8_t ip_address[4]  = {198, 158, 1, 6}; //fill in according your network parameters
uint8_t subnet_mask[4] = {255, 255, 255, 0};//fill in according your network parameters
uint8_t ip_gateway[4]  = {198, 158, 1, 1};//fill in according your network parameters
#define myvNet_address  ip_address[3] 

void setup()
{  
    Initialize();

     // Set network parameters
    Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
    SetAsGateway(myvNet_address);

    // Set IPBroadcast vNet Address   
    // This is the vNet address for this node, used to communicate with other
    // nodes in your Souliss network
    SetAddress(0xAB01, 0xFF00, 0x0000);
    SetAddress(0xD001, 0xFF00, 0x0000);

    // This node as gateway will get data from the Peer
    SetAsPeerNode(0xD002, 1);    // Peer will be ArduinoMega 2560
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
             
        // Process the communication basic at max speed, this allow smooth handling of color and music synch                                
        ProcessCommunication(); 
            
        // Complete the communication tasks at normal rate
        FAST_GatewayComms();
    }
}   
