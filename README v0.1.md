# Arduino Chess Clock
This project is a simple Arduino-based chess clock with a 16×2 I²C LCD display.
The initial time (1–10 minutes) is selected using a potentiometer, 
and each player receives a 5-second increment after ending their turn. 
The clock features sound notifications, a blinking display for the active player, 
and a pause function activated by holding both buttons for three seconds.

# Features:
- Adjustable starting time (1–10 minutes)
- 5-second increment after each move
- Active player's timer blinks
- Audible alerts
- Automatic timeout indication with flashing display and buzzer
- Pause and resume by holding both buttons for 3 seconds

# Components Required:
- 1 × Arduino Uno (or compatible board)
- 1 × 16×2 I²C LCD display
- 2 × Push buttons
- 1 × Passive buzzer
- 1 × 10 kΩ potentiometer (for selecting the starting time)
- Jumper wires
- Breadboard (optional, for prototyping)
- USB cable for programming the Arduino

# How to Build:
- Assemble the circuit according to the provided wiring diagram.
- Open the supplied code in the Arduino IDE.
- Select the correct board and COM port.
- Compile and upload the sketch to the Arduino.
- Use the potentiometer to choose the starting time and press either player's button to begin.

# How to Use:
- Turn the potentiometer to select the starting time (1–10 minutes).
- Press the button of the player who will make the first move to start the game.
- After making a move, press your button to end your turn and start your opponent's timer.
- Each button press adds a 5-second increment to the player who just moved, just like a traditional chess clock.
- The active player's timer will blink to indicate whose turn it is.
- When either player reaches 10 seconds remaining, the buzzer will sound as a warning.
- If a player's time runs out, both timers will flash and the buzzer will beep three times to signal the end of the game.
- To pause or resume the clock, press and hold both buttons simultaneously for 3 seconds.

# Notes
This is the first version of the project, so there may still be some bugs. Unfortunately, the improved versions were not backed up and have been lost.
In addition, this version, which includes the potentiometer for time selection, has not yet been tested.

Feel free to modify and improve the code however you like. If you make any changes or enhancements,
I would love to see them!
You can contact me on Instagram at @ew_louay or by email at l.dalali.edu@gmail.com and share your improvements.
Any feedback, fixes, or contributions are greatly appreciated!
