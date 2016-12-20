
#include <Arduino.h>
#include <RCSwitch.h>
#include <IRremote.h>

/* Logic :
Code for control microlab solo 6-c by rf315
Receive from rf315 => send to IR
D3 - irda + (with 330Ω Resistor)
D2 - rf315 data
*/

RCSwitch mySwitch = RCSwitch();

unsigned long last;
char *controller;
int ledPin = 13; //internal led
IRsend irsend; // Receiver on interrupt 0 => that is pin #2

void setup()
  {
    // Serial.begin(9600); // Debugging only
    mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

    digitalWrite(ledPin, 0);
    pinMode(ledPin, OUTPUT);
  }

void loop() {
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    int proto = mySwitch.getReceivedProtocol();
    /* For debug
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }
    */

    // Just for filter purpose
    if (proto == 6) {
      if (millis() - last > 200) {
         switch(value) {
            case 1: irsend.sendNEC(0x807F8877, 32); break;  //v+
            case 2: irsend.sendNEC(0x807F08F7, 32); break;  //v-
            case 3: irsend.sendNEC(0x807F906F, 32); break;  //t+
            case 4: irsend.sendNEC(0x807FB04F, 32); break;  //t-
            case 5: irsend.sendNEC(0x807F50AF, 32); break;  //b+
            case 6: irsend.sendNEC(0x807F708F, 32); break;  //b-
            case 7: irsend.sendNEC(0x807F10EF, 32); break;  //reset
            case 8: irsend.sendNEC(0x807F20DF, 32); break;  //input
            case 9: irsend.sendNEC(0x807F40BF, 32); break;  //mute
            default: ;
            }
            // Blink
            digitalWrite(ledPin, 1);
            delay(50);
            digitalWrite(ledPin, 0);
            last = millis();
     }
    mySwitch.resetAvailable();
    }
  }
}
