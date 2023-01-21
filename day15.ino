#include <Keypad.h>

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
        {'H','I','J','K'},
        {'L','M','N','O'},
        {'P','Q','R','S'},
        {'T','U','V','W'}
        };

// Use of the tone() function will interfere with PWM output on pins 3 and 11
// Since I need three PWM pins for RGB LED, the keypad pins need to dodge around a bit

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {11,7,12,13}; //connect to the column pinouts of the keypad

const int analogMax = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 - 1;

int keyToPitch[ROWS * COLS] = {
  NOTE_D3,
  NOTE_E3,
  NOTE_F3,
  NOTE_G3,
  NOTE_A3,
  NOTE_B3,
  NOTE_C4,
  NOTE_D4,
  NOTE_E4,
  NOTE_F4,
  NOTE_G4,
  NOTE_A4,
  NOTE_B4,
  NOTE_C5,
  NOTE_D5,
  NOTE_E5
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const byte bluePin = 9;
const byte greenPin = 10;
const byte redPin = 6;

const int buzzer = 8;

void printChar(char *text, char value) {
  Serial.print(text);
  Serial.print(": ");
  Serial.println(value);
}

void printInt(char *text, int value) {
  Serial.print(text);
  Serial.print(": ");
  Serial.println(value);
}

void setup(){
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);      // sets the digital pin as output
  analogWrite(redPin, 0); 
  analogWrite(greenPin, 0); 
  analogWrite(bluePin, 0); 
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
}

void loop(){
  (void)keypad.getKey();
}

//take care of some special events
void keypadEvent(KeypadEvent key){
  int note = key - keys[0][0];
  Serial.println(note);
  int pitch = keyToPitch[note];
  Serial.println(pitch);
  int redValue = max(0, 8 - note) * analogMax / 8;
  int greenValue = (8 - abs(note - 8)) * analogMax / 8;
  int blueValue = max(0,note - 7) * analogMax / 8;
  printInt("red", redValue);
  printInt("green", greenValue);
  printInt("blue", blueValue);
  switch (keypad.getState()){
    case PRESSED:
      //printChar("Pressed",key);
      tone(buzzer,pitch);
      analogWrite(redPin, redValue); 
      analogWrite(greenPin, greenValue); 
      analogWrite(bluePin, blueValue); 
      break;
    case RELEASED:
      //printChar("Released",key);
      noTone(buzzer);
      analogWrite(redPin, 0); 
      analogWrite(greenPin, 0); 
      analogWrite(bluePin, 0); 
      break;   
    case HOLD:
      break;
  }
}