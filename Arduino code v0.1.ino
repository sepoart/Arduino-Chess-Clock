#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int button1 = 6;
const int button2 = 7;
const int buzzer = 8;
const int potPin = A0;

unsigned long player1Time = 0;
unsigned long player2Time = 0;
bool player1Turn = false; // will be set by first press
bool gameStarted = false;
bool paused = false;

unsigned long lastMillis = 0;
unsigned long lastBlink = 0;
bool blinkState = true;

bool zeroCycleActive = false;
int zeroCycleCount = 0;

unsigned long bothPressedTime = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  bool btn1 = digitalRead(button1) == LOW;
  bool btn2 = digitalRead(button2) == LOW;

  // --- Pause logic ---
  if(btn1 && btn2) {
    if(bothPressedTime == 0) bothPressedTime = now;
    else if(now - bothPressedTime >= 3000) {
      paused = !paused;
      bothPressedTime = 0;
    }
  } else {
    bothPressedTime = 0;
  }
  if(paused) return; // skip countdown while paused

// --- Before game starts: select time ---
if(!gameStarted) {
    int potValue = analogRead(potPin);       // 0-1023
    int minutes = map(potValue, 0, 1023, 1, 10); // 1–10 minutes
    player1Time = minutes * 60; // convert to seconds
    player2Time = minutes * 60;

    // Display selected time in minutes
    lcd.setCursor(5,0);
    lcd.print(minutes);
    lcd.print(" min  ");

    // Wait for first player to press
    if(btn1) {
        while(digitalRead(button1) == LOW);
        player1Turn = true;
        gameStarted = true;
        lcd.clear(); // <-- clears “in”
    }
    if(btn2) {
        while(digitalRead(button2) == LOW);
        player1Turn = false;
        gameStarted = true;
        lcd.clear(); // <-- clears “in”
    }
    return;
}
  // --- Countdown ---
  if(now - lastMillis >= 1000) {
    lastMillis = now;
    if(!zeroCycleActive) {
      if(player1Turn && player1Time > 0) player1Time--;
      if(!player1Turn && player2Time > 0) player2Time--;
    }
  }

  // --- Turn switching ---
  if(btn1 && player1Turn && !zeroCycleActive) {
    while(digitalRead(button1) == LOW);
    player1Turn = false;
    player1Time += 5;
    tone(buzzer, 1000, 100);
  }
  if(btn2 && !player1Turn && !zeroCycleActive) {
    while(digitalRead(button2) == LOW);
    player1Turn = true;
    player2Time += 5;
    tone(buzzer, 1000, 100);
  }

  // --- Zero-time blink cycles ---
  if(!zeroCycleActive && (player1Time == 0 || player2Time == 0)) {
    zeroCycleActive = true;
    zeroCycleCount = 0;
    lastBlink = now;
    blinkState = true;
  }

  if(zeroCycleActive) {
    if(now - lastBlink >= 500) {
      lastBlink = now;
      blinkState = !blinkState;
      if(!blinkState) zeroCycleCount++;
    }
    if(zeroCycleCount >= 3) {
      zeroCycleActive = false;
      blinkState = true;
      noTone(buzzer);
    }
  } else {
    // normal active timer blink 0.5s on / 0.1s off
    if(blinkState && now - lastBlink >= 500) {
      lastBlink = now;
      blinkState = false;
    } else if(!blinkState && now - lastBlink >= 100) {
      lastBlink = now;
      blinkState = true;
    }
  }

  // --- Format timers ---
  String t1 = formatTime(player1Time);
  String t2 = formatTime(player2Time);

  if(zeroCycleActive) {
    if(!blinkState) t1 = "     ";
    if(!blinkState) t2 = "     ";
  } else {
    if(player1Turn && !blinkState) t1 = "     ";
    if(!player1Turn && !blinkState) t2 = "     ";
  }

  // --- Display ---
  lcd.setCursor(0,0);
  lcd.print(t1);
  lcd.setCursor(7,0);
  lcd.print("|");
  lcd.setCursor(11,0);
  lcd.print(t2);

  // --- Buzzer ---
  if(zeroCycleActive) {
    if(blinkState) tone(buzzer, 1000);
    else noTone(buzzer);
  } else {
    if(player1Time == 10 || player2Time == 10) tone(buzzer, 1000, 200);
    if(player1Time > 0 && player2Time > 0) noTone(buzzer);
  }
}

String formatTime(unsigned long t) {
  int minutes = t / 60;
  int seconds = t % 60;
  char buf[6];
  sprintf(buf, "%02d:%02d", minutes, seconds);
  return String(buf);
}

/*
     ______                                    __                                         
   _/      \_                                 |  \                                        
  /   $$$$$$ \    ______   __   __   __       | $$  ______   __    __   ______   __    __ 
 /  $$$____$$$\  /      \ |  \ |  \ |  \      | $$ /      \ |  \  |  \ |      \ |  \  |  \
|  $$/     \ $$\|  $$$$$$\| $$ | $$ | $$      | $$|  $$$$$$\| $$  | $$  \$$$$$$\| $$  | $$
| $$|  $$$$$| $$| $$    $$| $$ | $$ | $$      | $$| $$  | $$| $$  | $$ /      $$| $$  | $$
| $$| $$| $$| $$| $$$$$$$$| $$_/ $$_/ $$      | $$| $$__/ $$| $$__/ $$|  $$$$$$$| $$__/ $$
| $$ \$$  $$| $$ \$$     \ \$$   $$   $$______| $$ \$$    $$ \$$    $$ \$$    $$ \$$    $$
 \$$\ \$$$$$$$$   \$$$$$$$  \$$$$$\$$$$|      \\$$  \$$$$$$   \$$$$$$   \$$$$$$$ _\$$$$$$$
  \$$\ __/   \                          \$$$$$$                                 |  \__| $$
   \$$$    $$$                                                                   \$$    $$
     \$$$$$$                                                                      \$$$$$$ 

  Created by Louay Dalali

  Please give credit if you reuse this project.
  It helps encourage open-source coding and learning.
*/
