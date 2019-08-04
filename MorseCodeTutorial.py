# Morse Code Generator Python Program that translates a
# text string to Morse Code and outputs it to a GPIO pin.
# This program is used as part of the Morse Code Reader
# demonstration. It generates Morse code sent to a digital
# I/O pin of an Arduino board. The Morse code signal is
# decoded back to text on the Arduino. This code is
# associated with the Morse Code Reader blog post on
# https://lagacemichel.com
# MIT License
# Copyright (c) 2019, Michel Lagace

import RPi.GPIO as IO
import time
UNIT_TIME = 0.05
BOARD_PIN = 7
 
# Characters to be encoded
characters = "abcdefghijklmnopqrstuvwxyz0123456789"

# Morse code sequences for each character
codedCharacters = [
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
"..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
"--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
"-.--", "--..", "-----", ".----", "..---", "...--",
"....-", ".....", "-....", "--...", "---..", "----."]

# Setup the board. Digital port LED_BUILTIN in output mode
def setup():
    IO.setwarnings(False)
    IO.setmode (IO.BOARD)
    IO.setup(BOARD_PIN,IO.OUT)

# Function to output a dot: one unit on, one unit off
def outputDot():
    IO.output(BOARD_PIN,1)
    time.sleep(UNIT_TIME)
    IO.output(BOARD_PIN,0)
    time.sleep(UNIT_TIME)

# Function to output a dash: three units on, one unit off
def outputDash():
    IO.output(BOARD_PIN,1)
    time.sleep(UNIT_TIME*3)
    IO.output(BOARD_PIN,0)
    time.sleep(UNIT_TIME)

# Function to output a single character
def outputCharacter(c):
    # Find index of character to encode
    index = characters.find(c)
    # Ignore unencodable characters
    if (index >= 0):
        # Encode morse codeand output it
        code = codedCharacters[index]
        for ch in code:
            if ch == '-':
                outputDash()
            else: # if not '-', must be '.'
                outputDot()
        # Wait 3 units at the end of the letter
        # (2 units assuming previous dot or dash)
        time.sleep(UNIT_TIME*2)

# Function to encode a whole sring
def sentence (text):
    # Output each character in turn
    for ch in text:
        # Only lower case characters are encoded
        if ch != ' ':
            outputCharacter(ch.lower())
        # Spaces are encoded as 7 units,
        # (4 units assuming a previous character)
        else:
            time.sleep(UNIT_TIME*4)

# Main program. Setup board then loop forever
setup()
loopValue = 0
while(True):
    loopValue = loopValue + 1
    sentence("this is a test " + str(loopValue))
    time.sleep(UNIT_TIME*53) # Wait 8 spaces at the end
