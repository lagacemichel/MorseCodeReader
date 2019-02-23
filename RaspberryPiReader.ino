// Main RaspberryPiReader sketch
// Program that reads a digital input from a Raspberry Pi
// GPIO pin and outputs its value to an Arduino digital output
// pin. This sketch is used as part of a demonstration to show
// connectivity between a Raspberry Pi and an Arduino UNO.
// It is associated with the Raspberry Pi Speaks Arduino
// blog post on https://lagacemichel.com
// MIT License
// Copyright (c) 2019, Michel Lagace

// Define input and output ports
#define INPORT 8    // Input port connected to Raspberry Pi
#define OUTPORT 13  // Output port to built-in 

// Time to wait in milliseconds between samples
#define SAMPLE_DELAY 10

// Setup the board.
void setup() {
    pinMode(INPORT, INPUT);
    pinMode(OUTPORT,OUTPUT);
    digitalWrite(OUTPORT,LOW);
}

// Repeat forever
void loop() {
    // Read value from Raspberry Pi
    bool value = digitalRead(INPORT);
    
    // Output value to built-in LED
    digitalWrite(OUTPORT, value);
    
    // Wait before next sample
    delay(SAMPLE_DELAY);
}
