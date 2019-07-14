import processing.serial.*;
 
Serial port;

int baudrate = 9600;
int value = 0;  

void setup()
{
  size(200,200);
  port = new Serial(this, Serial.list()[0], baudrate);
 
  // List all the available serial ports
  printArray(Serial.list());

  println(port);
}

void draw()
{
  color c1 = #00ff00;
  color c2 = #FF00FF;
  noStroke();
  fill(c1);
  rect(0, 0, 25, 100);
  fill(c2);
  rect(25, 0, 25, 100);
  color c3 = get(10, 50);
  fill(c3);
  rect(50, 0, 50, 100);

  SendOverSerial();
}

void SendOverSerial()
{
  String dataString = "";
  
  loadPixels();
  for (int i = 0; i < 4; i++) {
    dataString += hex(pixels[i], 6);
  }
  dataString+="\n";
  
 //  println(dataString);
  port.write (dataString);

}
