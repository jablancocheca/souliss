/**************************************************************************
    Souliss - Hello World for Expressif ESP8266

    This is the basic example, create a software push-button on Android
    using SoulissApp (get it from Play Store).

    Load this code on ESP8266 board using the porting of the Arduino core
    for this platform.

***************************************************************************/

// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// Configure the framework
#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
#include "conf/SuperNode.h"                   // The main node is the Gateway, we have just one node
#include "conf/usart.h"                      // USART 
#include "conf/IPBroadcast.h"
// **** Define the WiFi name and password ****
#define WIFICONF_INSKETCH
#define WiFi_SSID               "xxxxxxxxxxx"
#define WiFi_Password           "xxxxxxxxxxx"

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>

//*** All configuration includes should be above this line ***/
#include "Souliss.h"

/// This identify the number of the LED logic
#define MYLEDLOGIC          0
#define TEMPERATURE         1
#define HUMIDITY            3
#define MYRELAY             5

// **** Define here the right pin for your ESP module ****
#define OUTPUTPIN     5
#define OUTPUTPIN_RELAY     16

//*********************DHT11***************************
// Include and Configure DHT11 SENSOR
#include "DHT.h"
#define DHTPIN              13       // what pin we're connected to
#define DHTTYPE             DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE, 15);

// This identify the number of the LED logic


#define Debug               Serial  //Change to Serial1 if you want to use the GPIO2 to TX
#define DebugDHT            1       //0 - None      / 1 - Show data on Serial  
#define Celsius             1       //0 - Farenheit / 1 Celsius


void setup()
{
  Initialize();

  //*********************DHT11****************************
  Debug.begin(115200);
  Debug.println("DHTxx test!");
  dht.begin();

  // Connect to the WiFi network and get an address from DHCP
  GetIPAddress();
  //SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp
  // This is the vNet address for this node, used to communicate with other
  // nodes in your Souliss network
  SetAddress(0xAB02, 0xFF00, 0xAB01);
  SetAddress(0xD001, 0xFF00, 0x0000);
  Set_SimpleLight(MYLEDLOGIC);        // Define a simple LED light logic
  pinMode(OUTPUTPIN, OUTPUT);         // Use pin as output
  //****************************RELAY*************************
  Set_T11(MYRELAY);
  digitalWrite(OUTPUTPIN_RELAY, HIGH);
  delay(100);
  pinMode(OUTPUTPIN_RELAY, OUTPUT);
  //****************************DHT11***************************
  Set_Temperature(TEMPERATURE);
  Set_Humidity(HUMIDITY);

  
}

void loop()
{
  // Here we start to play
  EXECUTEFAST() {
    UPDATEFAST();

    FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
      Logic_SimpleLight(MYLEDLOGIC);
      DigOut(OUTPUTPIN, Souliss_T1n_Coil, MYLEDLOGIC);

      //*********************RELAY************************
      Logic_T11(MYRELAY);
      LowDigOut(OUTPUTPIN_RELAY, Souliss_T1n_Coil, MYRELAY);

    }
  
   
    //****************************DHT11*******************
    FAST_2110ms()
    {
      Logic_Temperature(TEMPERATURE);
      Logic_Humidity(HUMIDITY);
    }
     
     
    // Here we handle here the communication with Android
    FAST_PeerComms();
    START_PeerJoin();
    FAST_BridgeComms();
  }

  //***************************DHT11************************
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {
      Souliss_ReadDHT(TEMPERATURE, HUMIDITY);
    }
    SLOW_PeerJoin();
  }
}
//***********************************DHT11*********************
void Souliss_ReadDHT(uint8_t TEMPERATURE_SLOT, uint8_t HUMIDITY_SLOT) {
  // Read temperature and humidity from DHT every 10 seconds
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Debug.println("Failed to read from DHT sensor!");
  }
  if (DebugDHT) {
    Debug.print("Humidity: ");
    Debug.print(h);
    Debug.print(" %\t");
    if (Celsius) {
      Debug.print("Temperature: ");
      Debug.print(t);
      Debug.print(" *C ");
    } else {
      Debug.print("Temperature: ");
      Debug.print(f);
      Debug.print(" *F ");
    }
  }

  if (Celsius) Souliss_ImportAnalog(memory_map, TEMPERATURE_SLOT, &t);
  else Souliss_ImportAnalog(memory_map, TEMPERATURE_SLOT, &f);
  Souliss_ImportAnalog(memory_map, HUMIDITY_SLOT, &h);
}
