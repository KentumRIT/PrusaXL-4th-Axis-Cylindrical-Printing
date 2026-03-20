#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include <Arduino.h>

#define LED_PIN 2

const char start_marker = '<';  // start of message character
const char end_marker = '>';    // end of message character
const int message_size = 200;   // max size of serial message to read in bytes
char message[message_size];     // data storage for the serial message as it's coming in
bool new_message = false;       // sets to true if we've received a new message and to false after processing the new message

void receiveSerial() {
    /*
       We need static flag variables here because this function will clear the serial buffer before
       we actually get to the end of the message. This way, we can move on and do other things while
       we wait for the rest of the message to come in.
    */
    static bool receive_happening = false;  
    static int message_index = 0;
    char received_character;

    // read from the serial buffer as much as we can so long as we aren't processing a new message
    while (Serial.available() > 0 && new_message == false) {
        received_character = Serial.read();

        if (receive_happening) {
            // save characters if they're not the end marker
            if (received_character != end_marker) {
                message[message_index] = received_character;
                message_index++;
                
                // abandon the message if it gets too big
                if (message_index >= message_size) {
                    receive_happening = false;
                    message_index = 0;
                    Serial.print("ERROR: Message oversize! Current message:"); Serial.println(message);
                }
            }
            // when we reach the end marker, terminate the string and update flag variables
            else {
                message[message_index] = '\0';
                receive_happening = false;
                message_index = 0;
                new_message = true;
            }
        }
        // wait till we hit the start marker to start saving data
        else if (received_character == start_marker) {
            receive_happening = true;
        }
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {;}

    pinMode(LED_PIN, OUTPUT);
}

long prev_time = 0;
bool led_on = false;

void loop() {
    receiveSerial();

    if (new_message) {
        if (strcmp(message,"test") == 0) {
            digitalWrite(LED_PIN, HIGH);
            led_on = true;
            prev_time = millis();
        }
       Serial.print("New message:"); Serial.println(message);
       new_message = false;
    }
    
    if (led_on && ((millis() - prev_time) > 1000)) {
        
        digitalWrite(LED_PIN, LOW);
    }
}
