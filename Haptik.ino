// Include SPI interface support, and Button library
#include <SPI.h>
#include <Button.h>

// Slave Select, DRDY, and CHANGE pins on digital I/O
// Group 1
const int SS_1 = 49;      //49
const int DRDY_1 = 47;    //47
const int CHANGE_1 = 45;  //45

// Group 2
const int SS_2 = 43;      //43
const int DRDY_2 = 41;    //41
const int CHANGE_2 = 39;  //39

// Group 3
const int SS_3 = 37;      //37
const int DRDY_3 = 35;    //35
const int CHANGE_3 = 34;  //34


// Assign tact switch (buttons) pins as PULLUP
Button button1 = Button(46, PULLUP);    //46
Button button2 = Button(40, PULLUP);    //40
Button button3 = Button(36, PULLUP);    //36

// Prepare three bytes for the data packet we want to write to the QT1106
byte Write1, Write2, Write3;
// Prepare three bytes for the data packet response we expect from the QT1106
byte Read1, Read2, Read3;


void setup() {

  pinMode(SS_1, OUTPUT);
  pinMode(CHANGE_1, INPUT);
  pinMode(DRDY_1, INPUT);
  pinMode(SS_2, OUTPUT);
  pinMode(CHANGE_2, INPUT);
  pinMode(DRDY_2, INPUT);
  pinMode(SS_3, OUTPUT);
  pinMode(CHANGE_3, INPUT);
  pinMode(DRDY_3, INPUT);
  
  // Make Slave Select output pins high (not calling for data)
  digitalWrite(SS_1, HIGH);
  digitalWrite(SS_2, HIGH);
  digitalWrite(SS_3, HIGH);
  
  // Make LED output, turn off
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  
  // Start the SPI library, most significant bit (MSB) first (i.e. bit 7, 6, 5 .... 1, 0)
  // SPI Mode 3 (data valid on rising edge of CLK, clock signal HIGH at rest)
  // SPI clock is oscillator/128 (i.e. quite slow :o)
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setDataMode(SPI_MODE3);
    
  // Start the serial interface
  Serial.begin(9600);
  
 
  // Perfom a calibration of the QT1106 chip
  // Just send the three byte packet and ignore chip response as we are not
  // interested in the return values here during the calibration process.
  
// Calibrate Group 1
  digitalWrite(SS_1, LOW);
      delay(1);
  // Default value
  SPI.transfer(0x01);
  // Default value
  SPI.transfer(0x00);
  // CalW and CalK bits set to calibrate all Keys (Cal Key Num = 0b000) and Wheel
  SPI.transfer(0x18);
      delay(1);
  digitalWrite(SS_1, HIGH);
  
// Calibrate Group 2
  digitalWrite(SS_2, LOW);
      delay(1);
  SPI.transfer(0x01);
  SPI.transfer(0x00);
  SPI.transfer(0x18);
      delay(1);
  digitalWrite(SS_2, HIGH);
  
// Calibrate Group 3
  digitalWrite(SS_3, LOW);
      delay(1);
  SPI.transfer(0x01);
  SPI.transfer(0x00);
  SPI.transfer(0x18);
      delay(1);
  digitalWrite(SS_3, HIGH);

}


void loop() {
  int QT1106_Ready_1 = LOW;
  int QT1106_Change_1 = LOW;
  int QT1106_Ready_2 = LOW;
  int QT1106_Change_2 = LOW;
  int QT1106_Ready_3 = LOW;
  int QT1106_Change_3 = LOW;
  
  /////// Prepare three byte control packet:
  // Byte 1 - AKS Global, and default values
  Write1 = 0x01;
  // Byte 2 - default values
  Write2 = 0x00;
  // Byte 3 - 2-bit wheel resolution, and default values
  Write3 = 0x20;
  
  
  
  // Wait until *both* CHANGE and DRDY for a Group are logic HIGH
  // Only then can we communicate via SPI interface
  do {
    QT1106_Change_1 = digitalRead(CHANGE_1);
    QT1106_Ready_1 = digitalRead(DRDY_1);
    QT1106_Change_2 = digitalRead(CHANGE_2);
    QT1106_Ready_2 = digitalRead(DRDY_2);
    QT1106_Change_3 = digitalRead(CHANGE_3);
    QT1106_Ready_3 = digitalRead(DRDY_3);
    
        // if button is pressed, write the last detected finger position to serial
        if (button1.uniquePress() || button2.uniquePress() || button3.uniquePress()) {
          // Multiply Read2 by 10 - gives a value unique to which button was "pressed"
          Serial.write(10*Read2);
        }
  } while ( (QT1106_Change_1 == LOW || QT1106_Ready_1 == LOW) &&  // OR operator ||
            (QT1106_Change_2 == LOW || QT1106_Ready_2 == LOW) &&  // AND operator &&
            (QT1106_Change_3 == LOW || QT1106_Ready_3 == LOW) );
  
  
  // Once something is detected...
  // Send the three byte packet and receive three byte response
  
  // Group 1
  if ( QT1106_Ready_1 == HIGH && QT1106_Change_1 == HIGH ) {
    digitalWrite(SS_1, LOW);
      delay(1);
    Read1 = SPI.transfer(Write1);
    Read2 = SPI.transfer(Write2);
    Read3 = SPI.transfer(Write3);
      delay(1);
    digitalWrite(SS_1, HIGH);
  }
  
  // Group 2
  else if ( QT1106_Ready_2 == HIGH  && QT1106_Change_2 == HIGH ) {
    digitalWrite(SS_2, LOW);
      delay(1);
    Read1 = SPI.transfer(Write1);
    Read2 = SPI.transfer(Write2);
    Read3 = SPI.transfer(Write3);
      delay(1);
    digitalWrite(SS_2, HIGH);
  }
  
  // Group 3
  else if ( QT1106_Ready_3 == HIGH && QT1106_Change_3 == HIGH ) {
    digitalWrite(SS_3, LOW);
      delay(1);
    Read1 = SPI.transfer(Write1);
    Read2 = SPI.transfer(Write2);
    Read3 = SPI.transfer(Write3);
      delay(1);
    digitalWrite(SS_3, HIGH);
  }


        // Communicate finger position to computer Processing program
        Serial.write(Read2);

}  // void loop()...
