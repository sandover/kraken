import processing.serial.*;
 
Serial port;
String portname = "COM8";  
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
       port.write (0);
       port.write (1);
       port.write (2);
       port.write (3);
       port.write (0);
}
