#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 
RF24 radio(7,8);

byte addresses[][6] = {"1Node","2Node"};


// Set up roles to simplify testing 
boolean role;                                    // The main role variable, holds the current role identifier
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.

void setup() {


  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15,15);                // Max delay between retries & number of retries
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void){
    

/****************** Ping Out Role ***************************/
  if (role == role_ping_out)  {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    
    
    printf("Now sending \n\r");

    unsigned long time = micros();                             // Take the time, and send it.  This will block until complete
    if (!radio.write( &time, sizeof(unsigned long) )){  printf("failed.\n\r");  }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        printf("Failed, response timed out.\n\r");
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );

        // Spew it
        printf("Sent %lu, Got response %lu, round-trip delay: %lu microseconds\n\r",time,got_time,micros()-got_time);
    }

    // Try again 1s later
    delay(1000);
  }

/****************** Pong Back Role ***************************/

  if ( role == role_pong_back )
  {
    if( radio.available()){
      unsigned long got_time;                                       // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
      }    
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      printf("Sent response %lu \n\r", got_time);  
   }
 }


/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      role = role_ping_out;                  // Become the primary transmitter (ping out)
      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(1,addresses[1]);
  
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");
      
       role = role_pong_back;                // Become the primary receiver (pong back)
       radio.openWritingPipe(addresses[1]);
       radio.openReadingPipe(1,addresses[0]);
    }
  }
}
