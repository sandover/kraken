

  import processing.serial.*;     // import the Processing serial library
  Serial myPort;                  // The serial port

  color outcolor;
  
  void setup() {
    size(640, 480);

    myPort = new Serial(this, Serial.list()[0], 9600);

    smooth();
  }

  void draw() {
    float ms = second() * 1000 + millis();
    ms /= 1000;
    
    float gColor = 128 + 128 * sin(ms);
    outcolor = color(255, gColor, 0);
    print(hex(outcolor, 6));
    background(outcolor);

  }

  void serialEvent(Serial myPort) {
    // read the serial buffer:
    String myString = myPort.readStringUntil('X');
    // if you got any bytes other than the linefeed:
    print(trim(myString));
    
    print("sending");
    

    myPort.write(hex(outcolor, 6) + "\n");
  }
