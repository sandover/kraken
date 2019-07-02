#include <SoftwareSerial.h>

#include <Adafruit_NeoPixel.h>

#define LED_PER_LEG 5
#define NUM_STRIPS 8

Adafruit_NeoPixel strips[] =  {
  Adafruit_NeoPixel(LED_PER_LEG, 0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LED_PER_LEG, 7, NEO_GRB + NEO_KHZ800)
};







class Color { 
public: 
    float r, g, b; 
public: 
    Color(float _r = 0, float _g = 0, float _b = 0)  {r = _r; g = _g; b = _b;} 
 

  void operator *= (double scale)  {  r *= scale, g *= scale, b *= scale;  }
  Color operator * (double scale)  {  return Color( r * scale, g * scale, b * scale);  }

  void operator *= (float scale)  {  r *= scale, g *= scale, b *= scale;  }
  Color operator * (float scale)  {  return Color( r * scale, g * scale, b * scale);  }
  Color operator + (const Color &c2)  {  return Color( r + c2.r, g + c2.g, b + c2.b);  }
}; 

#define TWOPI 6.28318

class Palette {
  
  public: 
    float aa[3], bb[3], cc[3], dd[3] = {0,0,0};

    Color evaluate(float t) 
    {
      float r = aa[0] + bb[0] * cos ( TWOPI * ( cc[0] * t + dd[0] ) );
      float g = aa[1] + bb[1] * cos ( TWOPI * ( cc[1] * t + dd[1] ) );
      float b = aa[2] + bb[2] * cos ( TWOPI * ( cc[2] * t + dd[2] ) );
      return Color(r, g, b);
    }
    
};

Palette pal;
float time;

void SetPalette(int index)
{
  
  if ( index == 1)
  {
    pal.aa[0] = pal.aa[1] = pal.aa[2] = 0.5;
    pal.bb[0] = pal.bb[1] = pal.bb[2] = 0.5;
    pal.cc[0] = pal.cc[1] = pal.cc[2] = 1.0;
    pal.dd[0] = 0.00; 
    pal.dd[1] = 0.33; 
    pal.dd[2] = 0.66;
  }
  else if ( index == 2)
  {
    pal.aa[0] = pal.aa[1] = pal.aa[2] = 0.5;
    pal.bb[0] = pal.bb[1] = pal.bb[2] = 0.5;
    pal.cc[0] = pal.cc[1] = pal.cc[2] = 1.0;
    pal.dd[0] = pal.dd[1] = pal.dd[2] = 0.0;
  }
  else if ( index == 3)
  {
    pal.aa[0] = pal.aa[1] = pal.aa[2] = 0.5;
    pal.bb[0] = pal.bb[1] = pal.bb[2] = 0.5;
    pal.cc[0] = pal.cc[1] = pal.cc[2] = 0.5;
    
    pal.dd[0] = 0.3; 
    pal.dd[1] = 0.2; 
    pal.dd[2] = 0.2;
  }
    else if ( index == 4)
  {
    pal.aa[0] = pal.aa[1] = pal.aa[2] = 0.5;
    pal.bb[0] = pal.bb[1] = pal.bb[2] = 0.5;
    pal.cc[0] = pal.cc[1] = 0.5;
    pal.cc[2] = 1.0;
    
    pal.dd[0] = 0.8; 
    pal.dd[1] = 0.9; 
    pal.dd[2] = 0.2;
  }
}

void setup() {
  for (int i = 0; i < NUM_STRIPS; i++)
  {
    strips[i].begin();
  }

  SetPalette(3); 
  time = 0;
}

float hash (int i , int j)
{
  return 0.5 + 0.5 * sin((float)i* 179.11 + (float)j* 131.11 + time * 0.05 );
}


void loop() {

  float radiusAmount = sin(time * 0.2) * 0.1 + sin(time * 0.9) * 0.04;
  float cardioidAmount = 0;//sin(time * 0.13)  *0.1 ;
  float breatheAmount = 1.0;
  float breathe = breatheAmount * 0.5 * sin(time * 0.1);
  float dropoutAmount = 0.0;
  
  
  float cardioid = 0.0;
  for (int i = 0; i < NUM_STRIPS; i++) {
      float radius = 0.0;
      cardioid += cardioidAmount;
      for (int j = 0; j < LED_PER_LEG; j++) {
        radius += radiusAmount;

        // palette
        Color base = pal.evaluate(time + radius + sin(cardioid * TWOPI) );
        //darken on sine
        base = base + base * breathe;
        //dropout
        base *= constrain(hash(i,j) + dropoutAmount, 0, 1);
        
        
        SetPixel(i, j, base);
        
      }   
  }



  for (int i = 0; i < NUM_STRIPS; i++) 
        strips[i].show();

  time += 0.016;
  
  delay(16);
}




void SetPixel(int leg, int balloon, Color c)
{
  float r = constrain(c.r, 0, 1);
  float g = constrain(c.g, 0, 1);
  float b = constrain(c.b, 0, 1);
  strips[leg].setPixelColor(balloon, r * 255, g* 255, b* 255); 
}
