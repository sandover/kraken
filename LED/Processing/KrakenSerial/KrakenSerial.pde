

  import processing.serial.*;     // import the Processing serial library
  Serial myPort;                  // The serial port

  color outcolor;
  
  void setup() {
    size(640, 480);

    myPort = new Serial(this, Serial.list()[0], 9600);

    smooth();
  }

  void draw() {
    float ms =  ((float)millis())/ 1000;

    
    float sat = 10 + 50 * sin(ms);
 
    
    colorMode(HSB, 100);   
    outcolor = color((ms * 10) % 100, 80, 100);  

    background(outcolor);
    
  }

  void serialEvent(Serial myPort) {
    // read the serial buffer:
   // String myString = myPort.readStringUntil('X');
   // if ( myString)
    myPort.write(hex(outcolor, 6) + "\n");
  }
