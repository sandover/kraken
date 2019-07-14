


#include <Adafruit_NeoPixel.h>

#define LED_PER_LEG 5
#define NUM_STRIPS 8
#define BYTES_PER_COLOR 6

Adafruit_NeoPixel strips[] =  {
  Adafruit_NeoPixel(LED_PER_LEG, 9, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 10 , NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 7, NEO_GRB + NEO_KHZ800)
};


String inputString;
bool stringComplete = false;

void setup() {
  for (int i = 0; i < NUM_STRIPS; i++)
  {
    strips[i].begin();
  }

  
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.write('X');
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();


    inputString += inChar;


    if (inChar == '\n') {
      stringComplete = true;
      Serial.write('X');
      break;
    }
  }
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    String thisColor = inputString.substring(0, BYTES_PER_COLOR);
    int id = 0;
    for(int leg = 0; leg < NUM_STRIPS; leg++)
    {
      for(int pixel = 0; pixel < LED_PER_LEG; pixel++)
      {
            SetPixel(leg, pixel, thisColor);
      }
    }
    
    /*
    for(int i = 0; i < inputString.length(); i += BYTES_PER_COLOR)
        {
          SetPixel(id / NUM_STRIPS , id % LED_PER_LEG, );
          id++;
        }
      */      

    inputString = "";
    stringComplete = false;

  }
  else
  {

    DrawTestPattern();
  }

  for(int i = 0; i < NUM_STRIPS; i++)
    strips[i].show();
  
  delay(16);
}

void DrawTestPattern()
{
    for(int leg = 0; leg < NUM_STRIPS; leg++)
    {
      for(int pixel = 0; pixel < LED_PER_LEG; pixel++)
      {
        char str[6]; 
        sprintf(str,"%x0%x0%s", leg * 2, pixel*3, leg % 2 == 0 ? "ff" : "00");
        
        String col = str;
        SetPixel(leg, pixel, col);
      }
    }
}

int StrToHex(String str)
{
  char charBuf[2];
  str.toCharArray(charBuf, 2);
  return (int) strtol(charBuf, 0, 16);
}

void SetPixel(int leg, int balloon, String hexValue)
{
  int r = StrToHex(hexValue.substring(0,2));
  int g = StrToHex(hexValue.substring(2,4));
  int b = StrToHex(hexValue.substring(4,6));
  strips[leg].setPixelColor(balloon, r, g, b); 
}
