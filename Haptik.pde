import processing.serial.*;

// Declare variables "Zero"..."TabSix" of type PImage
// Main pages:
PImage Zero;
PImage One;
PImage Two;
PImage Three;
PImage Four;
PImage Five;
PImage Six;
// Tabs:
PImage NoTab;
PImage TabOne;
PImage TabTwo;
PImage TabThree;
PImage TabFour;
PImage TabFive;
PImage TabSix;

Serial ArduinoPort;  // Create object from Serial class
int val;      // Data received from the serial port


void setup() 
{
  size(1280, 530);  // ***ADJUST SIZE***  1280,530
  
  // Set pot for Arduino as COM4
  String portName = Serial.list()[2];
  ArduinoPort = new Serial(this, portName, 9600);

  
  // Load the page images into the program
  One = loadImage("1.jpg"); 
  Two = loadImage("2.jpg");
  Three = loadImage("3.jpg");
  Four = loadImage("4.jpg");
  Five = loadImage("5.jpg");
  Six = loadImage("6.jpg");
  // Load the tab images into the program
  NoTab = loadImage("tab0.jpg");
  TabOne = loadImage("tab1.jpg");
  TabTwo = loadImage("tab2.jpg");
  TabThree = loadImage("tab3.jpg");
  TabFour = loadImage("tab4.jpg");
  TabFive = loadImage("tab5.jpg");
  TabSix = loadImage("tab6.jpg");
  
  // Display home images
  image(One, 0, 50);
  image(NoTab, 0, 0);  // ***ADJUST LOCATION***
}



void draw()
{
  if ( ArduinoPort.available() > 0 ) {  // If data is available,
    val = ArduinoPort.read();         // read it and store it in val
  
    // Tab images (finger scanning)
    if (val == 1) {
       image(TabOne, 0, 0);
    }
    else if (val == 2){
       image(TabTwo, 0, 0);
    }
    else if (val == 4) {
      image(TabThree, 0, 0);
    }
    else if (val == 8) {
      image(TabFour, 0, 0);
    }
    else if (val == 16) {
      image(TabFive, 0, 0);
    }
    else if (val == 32) {
      image(TabSix, 0, 0);
    }
    
    // Page selection (presses a button)
    else if (val == 10) {
       image(One, 0, 50);
    }
    else if (val == 20){
       image(Two, 0, 50);
    }
    else if (val == 40) {
      image(Three, 0, 50);
    }
    else if (val == 80) {
      image(Four, 0, 50);
    }
    else if (val == 160) {
      image(Five, 0, 50);
    }
    // This part is "else if (val == 320)" but that didn't work for some reason...
    else if (val!=0 && val!=1 && val!=2 && val!=4 && val !=8 && val!=16 && val!=32 &&
             val!=10 && val!=20 && val!=40 && val !=80 && val!=160) {
      image(Six, 0, 50);
    }


  }  // if ArduinoPort.available()...
   

}  // void draw()

