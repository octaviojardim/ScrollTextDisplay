/*
  LCD Text Scroling with SD card module
  
  This example shows how get info out of your sd card and display it
  on a 16*2 LCD display. This works with large texts. It can also be used
  to display a string instead of getting the text from the SD card.
  To display only in one line, please remove all "Top Line" comments and
  remove the "2 *" from the main for cicle.
  As it is, the code will display text on the bottom line from the right
  to the left and then jump to the top line, from the right to the left again.

  The circuit:
    SD card and LCD display:
    
  -- LCD Display
  ** GND <---> GND
  ** VCC <---> 5V
  ** SDA <---> A4 on Arduino Nano Every
  ** SCL <---> A5 Arduino Nano Every
  
  -- SD card module
  ** GND <---> GND
  ** MISO <---> D12 Arduino Nano Every
  ** CLK <---> D13 Arduino Nano Every
  ** MOSI <---> D11 Arduino Nano Every
  ** CS <---> D4 Arduino Nano Every
  ** VCC <---> 3.3V
  * 
  created 30 November 2021
  by Octávio Jardim and Francisco Nunes
*/

//----------------------------------
#include <LiquidCrystal_I2C.h>
#include <SD.h>
//----------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
File card;
const int RIGHT_LIMIT = 16;
const int LEFT_LIMIT = 0;
const int SPEED = 500; // 0.5 seconds
const int SCREEN_SIZE = 16;
const int SD_CARD = 4; //CS PIN
char buf[SCREEN_SIZE + 1];           // Top Line
char buf2[SCREEN_SIZE + 1]; 
int k = 0;
//----------------------------------
void (*resetFunc) () = 0;  // reset fuction
//----------------------------------
void setup() {

  while (!SD.begin(SD_CARD)) {} // waits for the SD card module to connect

  buf[SCREEN_SIZE] = NULL;          // Top Line
  buf2[SCREEN_SIZE] = NULL; 

  lcd.init(); //initialize lcd screen

  lcd.backlight();  // turn on the backlight

  lcd.clear(); // clear the lcd screen

  card = SD.open("dummy.txt"); // open the file for reading
}
//----------------------------------
void loop() {

  if (card) {
    while (card.available() and k < SCREEN_SIZE) { //execute while file is available
      buf[k] = ' ';                            // Top Line
      buf2[k] = ' ';
      k++;
    }

    while (card.available()) {

      lcd.setCursor(0, 0);                     // Top Line
      lcd.print(displayFormattedText(buf));   // Top Line

      lcd.setCursor(0, 1);
      lcd.print(displayFormattedText(buf2));

      char letter = card.read(); //read next character from file
      delay(SPEED);

      if (letter == '\n' || letter == '\r' ) { // remove end of line character
        letter = ' ';
      }

      slideLeft(letter);
    }
    
    card.close(); //close file

    for (int i = 0; i < 2 * SCREEN_SIZE; i++) {  //if only one line, remove "2 *" from for cicle
      
      slideLeft(' ');

      lcd.setCursor(0, 0);                    // Top Line
      lcd.print(displayFormattedText(buf));   // Top Line

      lcd.setCursor(0, 1);
      lcd.print(displayFormattedText(buf2));

      delay(SPEED);
    }

  } else {
    // if the file didn't open
  }

  resetFunc(); //call reset
}
//----------------------------------
String displayFormattedText(String StrDisplay) {

  return StrDisplay.substring(LEFT_LIMIT, RIGHT_LIMIT);
}
//----------------------------------
void slideLeft(char nextChar) {
  char c = buf2[0];
  for (int i = 1; i < SCREEN_SIZE; i++) {                    
    buf[i - 1] = buf[i];                    // Top Line
    buf2[i - 1] = buf2[i];    
  }
  buf[SCREEN_SIZE - 1] = c;                // Top Line
  buf2[SCREEN_SIZE - 1] = nextChar;
}
//----------------------------------
