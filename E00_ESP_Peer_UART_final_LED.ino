// Configure the framework
#include "bconf/StandardArduino.h"          // Use a standard Arduino
#include "conf/usart.h"                     // USART 

//USART definition of serial ports for ArduinoMega (Serial, Serial 1 - 3)
#define USARTDRIVER_INSKETCH
#define USARTDRIVER Serial  // or any other USART to use

// Include framework code and libraries
#include <SPI.h>
#include "Souliss.h"

// This identify the number of the LED logic
#define MYLEDLOGIC          0     

void setup()
{   
    Initialize();

   // Set network parameters
   SetAddress(0xD002, 0xFF00, 0xD001);

    Set_SimpleLight(MYLEDLOGIC);        // Define a simple LED light logic
    
    // We connect a pushbutton between 5V and pin2 with a pulldown resistor 
    // between pin2 and GND, the LED is connected to pin9 with a resistor to
    // limit the current amount
    pinMode(2, INPUT);                  // Hardware pulldown required
    pinMode(13, OUTPUT);                 // Power the LED
}

void loop()
{
      // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 500 milliseconds
            DigIn(2, Souliss_T1n_ToggleCmd, MYLEDLOGIC);            // Use the pin2 as ON/OFF toggle command
            Logic_SimpleLight(MYLEDLOGIC);                          // Drive the LED as per command
            DigOut(13, Souliss_T1n_Coil, MYLEDLOGIC);                // Use the pin13 to give power to the LED according to the logic
                    } 
    FAST_PeerComms();

    START_PeerJoin();
                  }
}
