/*
Morse Code Reader
 
Program that reads Morse code and outputs decoded information to the Serial
interface. It is associated with the Morse Code decoder blog post on
https://lagacemichel.com

MIT License

Copyright (c) 2019, Michel Lagace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Morse code will be read from pin 8
#define INPORT 8

// Morse code time unit and timeout in milliseconds
#define TIME_UNIT 50
#define READ_TIMEOUT 10.5*TIME_UNIT

// Current Morse code, decoded line, and signal state
String currentCode;
String currentLine;
bool currentState;

// Function detectEdge.
// inputValue inputState(int port, int timeout)
// port -    Arduino input port to read from
// timeout - Timeout time in milliseconds
// startingState - Current state of the signal
// Wait for an edge or timeout and return the 
// current state.
bool detectEdge(int port, int timeout, bool startingState) {

  // Start timing
  unsigned long timerStart = millis();

  // Wait until value changes or until timeout
  bool signalState = false;
  while (true) {
    // Stop on timeout
    if ((millis() - timerStart) > timeout) {
      break;
    }

    // Stop on value change. Set input state
    signalState = digitalRead(INPORT);
    if (signalState != startingState) {
      break;
    }
  }

  // Return last value read
  return signalState;
}

// 
// Converts time in milliseconds to Morse code time units
int timeUnits(int elapsedTime, int timeUnit, int timeout) {
  int value = 15;
  if (elapsedTime < 0.5*timeUnit) {
    value = 0;
  }
  else if (elapsedTime < 1.5*timeUnit) {
    value = 1;
  }
  else if (elapsedTime < 4*timeUnit) {
    value = 3;
  }
  else if (elapsedTime < timeout) {
    value = 7;
  }
  return value;
}

// List of valid alphanumeric characters
static String characters = "abcdefghijklmnopqrstuvwxyz0123456789";

// Morse code sequences for each alphanumeric character
static String codedCharacters[] = { 
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
  "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
  "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
  "-.--", "--..", "-----", ".----", "..---", "...--",
  "....-", ".....", "-....", "--...", "---..", "----."};

// Convert Morse code signal to a character
char decodeSequence(String sequence) {
  int i;
  char character = ' ';
  for (i = 0; i < sizeof(codedCharacters)/sizeof(String); i++) {
    if (codedCharacters[i] == sequence) {
      character = characters[i];
      break;
    }
  }
  return character;
}

// Setup the board.
void setup() {
  pinMode(INPORT, INPUT);
  currentState = false;
  currentCode = "";
  currentLine = "";
  Serial.begin(9600);
}

// This is the maim loop. It monitors pin 8 for Morse code,
// decodes the signal and returns decoded sentences.
void loop() {
  // Wait for a rising edge, falling edge, or time out
  unsigned long startTime = millis();
  bool previousState = currentState;
  currentState = detectEdge(INPORT, READ_TIMEOUT, previousState);
  int elapsedTimeUnits =
    timeUnits(millis() - startTime, TIME_UNIT, READ_TIMEOUT);

  // On a falling edge, append dot or dash to character
  if (previousState && !currentState) {
      if (elapsedTimeUnits == 1) {
        currentCode += ".";
      }
      else if (elapsedTimeUnits == 3) {
        currentCode += "-";
      }
  }

  // On a rising edge or time out, handle end of
  // character, word, or end of transmission. 
  else if (currentCode != "") {
    if (elapsedTimeUnits > 1) {
       currentLine += decodeSequence(currentCode);
      currentCode = "";
    }
    if (elapsedTimeUnits > 3) {
      currentLine += ' ';
    }
    if (elapsedTimeUnits > 7) {
      Serial.println(currentLine);
      currentLine = "";
    }
  }
}
