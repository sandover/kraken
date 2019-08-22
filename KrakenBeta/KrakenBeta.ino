// ---------------------------------------------------------
// Author: Gourav Khadge
// Date: 8/16/2019
// Arduino file to develop Kraken code 
//
// Must Install NewPing and RGBdriver for this to work
// https://playground.arduino.cc/Code/NewPing/
// http://wiki.seeedstudio.com/Grove-LED_Strip_Driver/
// ---------------------------------------------------------
#include <NewPing.h>
#include "RGBdriver.h"
#define NUM_PYLONS 8
#define NUM_NODES 9
#define NUM_STRANDS 3
#define NUM_NODES_LEFT 4
#define NUM_NODES_RIGHT 4
#define NUM_NODES_B 1
#define STRAND_LEFT 0
#define STRAND_RIGHT 1
#define STRAND_B 2
#define NUM_DRIVERS_PER_PYLON 3
#define DRIVER_CLK_L 13//pins definitions for the driver        
#define DRIVER_DATA_L 12
#define DRIVER_CLK_R 11//pins definitions for the driver        
#define DRIVER_DATA_R 10
#define DRIVER_CLK_B 9//pins definitions for the driver        
#define DRIVER_DATA_B 8

#define COLOR_UPDATE_INTERVAL 109
 
#define SONAR_NUM     16 // Number or sensors.
#define MAX_DISTANCE 180 // Max distance in inches.
#define PING_INTERVAL 33 // Milliseconds between pings.


#define TRIGGER_PIN_L  38  // Arduino pin tied to trigger pin on ping sensor.
#define TRIGGER_PIN_R  39  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN_1A     22  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_1B     23  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_2A     24  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_2B     25  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_3A     26  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_3B     27  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_4A     28  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_4B     29  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_5A     30  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_5B     31  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_6A     32  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_6B     33  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_7A     34  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_7B     35  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_8A     36  // Arduino pin tied to echo pin on ping sensor.
#define ECHO_PIN_8B     37  // Arduino pin tied to echo pin on ping sensor.
 
//RGBdriver Driver_pylon(CLK,DIO);
//unsigned int rgbColour1[3];
//unsigned int rgbColour2[3];
//unsigned int rgbColour3[3];

int NODE_MAP[NUM_NODES] = {0,1,2,3,7,6,5,4,8};

byte nodeRGBs[NUM_NODES][NUM_DRIVERS_PER_PYLON][3] = {0};

RGBdriver Driver[NUM_STRANDS] = { // Sensor object array.
  RGBdriver(DRIVER_CLK_L,DRIVER_DATA_L),
  RGBdriver(DRIVER_CLK_R,DRIVER_DATA_R),
  RGBdriver(DRIVER_CLK_B,DRIVER_DATA_B)
};


const byte dim_curve[] = {
    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};


unsigned long pingTimer; // When each pings.
unsigned long colorUpdateTimer;
unsigned int distance[SONAR_NUM]; // Store ping distances.
uint8_t currentSensor = 0; // Which sensor is active.
 
NewPing sonar[SONAR_NUM] = { // Sensor object array.
  NewPing(TRIGGER_PIN_L, ECHO_PIN_1A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_5A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_L, ECHO_PIN_1B, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_5B, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_L, ECHO_PIN_2A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_6A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_L, ECHO_PIN_2B, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_6B, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_L, ECHO_PIN_3A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_7A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_L, ECHO_PIN_3B, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_7B, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_L, ECHO_PIN_4A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_8A, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_L, ECHO_PIN_4B, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_R, ECHO_PIN_8B, MAX_DISTANCE)
};
 
void echoCheck() { // If ping echo, set distance to array.
  if (sonar[currentSensor].check_timer())
    distance[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_IN;
}
 
void initializeNodeRGBs()
{
  for(int node = 0; node < NUM_PYLONS; node++){
    nodeRGBs[node][0][0] = 255;
    nodeRGBs[node][1][1] = 255;
    nodeRGBs[node][2][2] = 255;
  }
}
// Assume rgb[0]+rgb[1]+rgb[2] = 255
void rainbow_update(byte rgb[3])
{
  int decColor;
  // quick logic to find inc and dec colors
  if (rgb[0]) {
    if(rgb[2])
      decColor = 2;
    else
      decColor = 0;
  } else {
    if (rgb[1])
      decColor = 1;
    else
      decColor = 2;
  }
    
  int incColor = (decColor + 1) % 3;
  // cross-fade the two colors.
  rgb[decColor] -= 1;
  rgb[incColor] += 1;
}

void getRGB(int hue, int sat, int val, byte colors[3]) { 
  /* convert hue, saturation and brightness ( HSB/HSV ) to RGB
     The dim_curve is used only on brightness/value and on saturation (inverted).
     This looks the most natural.      
  */

  val = dim_curve[val];
  sat = 255-dim_curve[255-sat];

  int r;
  int g;
  int b;
  int base;

  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;  
  } else  { 

    base = ((255 - sat) * val)>>8;

    switch(hue/60) {
    case 0:
        r = val;
        g = (((val-base)*hue)/60)+base;
        b = base;
    break;

    case 1:
        r = (((val-base)*(60-(hue%60)))/60)+base;
        g = val;
        b = base;
    break;

    case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%60))/60)+base;
    break;

    case 3:
        r = base;
        g = (((val-base)*(60-(hue%60)))/60)+base;
        b = val;
    break;

    case 4:
        r = (((val-base)*(hue%60))/60)+base;
        g = base;
        b = val;
    break;

    case 5:
        r = val;
        g = base;
        b = (((val-base)*(60-(hue%60)))/60)+base;
    break;
    }

    colors[0]=r;
    colors[1]=g;
    colors[2]=b; 
  }   
}

// 358, 88, 93 reddish
//  51, 90, 93 yellow
// 308, 53, 67 purplish

// 135, 78, 70 //green  L
// 229, 65, 62, blue  B
// 279, 71, 94, purple R
//

void setNodeHSB(int h, float s, float b, byte rgb_out[3])
{
  getRGB(h,(int) (255.99*s),(int) (255.99*b),rgb_out); 
}

void transition_pylon_HSB(int hsb1[NUM_DRIVERS_PER_PYLON][3],int hsb2[NUM_DRIVERS_PER_PYLON][3], float ratio, byte rgb_out[NUM_DRIVERS_PER_PYLON][3])
{
  for (int i = 0; i < NUM_DRIVERS_PER_PYLON; ++i)
  {
    transition_color_HSB(hsb1[i][0], (int) (hsb1[i][1]/255.0), (int) (hsb1[i][2]/255.0), hsb2[i][0], (int) (hsb2[i][1]/255.0), (int) (hsb2[i][2]/255.0), ratio, rgb_out[i]);    
  }
}

void transition_color_HSB(int h, float s, float b,int h2, float s2, float b2, float ratio, byte rgb_out[3])
{
  int h_out;// = (int) (((1-ratio)*h + ratio*h2) + 0.5);
  float s_out = (1-ratio)*s + ratio*s2 ;
  float b_out = (1-ratio)*b + ratio*b2;

  if ((h2-h) > 180)
  {
    h_out = (int) (((1-ratio)*(h+360) + ratio*h2) + 0.5) % 360;
  } else if ((h-h2) > 180) {
    h_out = (int) (((1-ratio)*h + ratio*(h+360)) + 0.5) % 360;
  } else {
    h_out = (int) (((1-ratio)*h + ratio*h2) + 0.5);    
  }
  
  setNodeHSB(h_out,s_out,b_out,rgb_out); //L
}

int state = 0;
int percent = 0;
int hsb1[3][3] = {{97, (int) (0.65*255.99), (int) (0.65*255.99)},
                 {44, (int) (0.87*255.99), (int) (0.86*255.99)},
                 {10, (int) (0.81*255.99), (int) (0.79*255.99)}};
int hsb2[3][3] = {{22, (int) (0.82*255.99), (int) (0.84*0.8*255.99)}, 
                 {56, (int) (0.96*255.99), (int) (0.94*0.8*255.99)},
                 {319, (int) (0.85*0.8*255.99), (int) (0.76*0.8*255.99)}};
                 
                 
void updateNodeColors()
{
//  transition_pylon_HSB(hsb1,hsb2,percent/100.0, nodeRGBs[0]);
//  if (state == 0)
//  {
//    transition_pylon_HSB(hsb1,hsb2,percent/100.0, nodeRGBs[0]);
//    percent++;
//    if (percent == 100)
//      state = 1;
//  } else {
//    transition_pylon_HSB(hsb1,hsb2,percent/100.0, nodeRGBs[0]);
//    percent--;
//    if (percent == 0)
//      state = 0;    
//  }
  
  
//  getRGB(358,(int) (255*0.88),(int) (255*0.93),nodeRGBs[0][2]);
//  getRGB(51,(int) (255*0.90),(int) (255*0.93),nodeRGBs[0][1]);
//  getRGB(308,(int) (255*0.53),(int) (255*0.67),nodeRGBs[0][0]);

  setNodeHSB(229,0.65,0.62,nodeRGBs[0][2]); //L
  setNodeHSB(279,0.71,0.84,nodeRGBs[0][1]); //B
  setNodeHSB(135,0.78,0.70,nodeRGBs[0][0]); //R

  
//  setNodeHSB(44,0.87,0.76,nodeRGBs[0][2]); //L yellow
//  setNodeHSB(97,0.65,0.75,nodeRGBs[0][1]); //B green
//  setNodeHSB(12,0.83,0.88,nodeRGBs[0][0]); //R orange
//  
//  setNodeHSB(97,0.65,0.65,nodeRGBs[0][2]); //L green
//  setNodeHSB(44,0.87,0.86,nodeRGBs[0][1]); //B yellow
//  setNodeHSB(10,0.81,0.79,nodeRGBs[0][0]); //R orange
  
//  setNodeHSB(186,1,0.75*0.7,nodeRGBs[0][2]); //L blue
//  setNodeHSB(97,0.65,0.75*0.7,nodeRGBs[0][1]); //B green
////  setNodeHSB(36,0.89,0.98*0.7,nodeRGBs[0][1]); //B yellow
//  setNodeHSB(323,0.98,0.88*0.7,nodeRGBs[0][0]); //R pink
  
//  setNodeHSB(17,0.86,1,nodeRGBs[0][2]); //L kinda orange
//  setNodeHSB(225,0.64,0.85,nodeRGBs[0][1]); //B blue
//  setNodeHSB(316,0.43,0.77,nodeRGBs[0][0]); //R purple
  
//  setNodeHSB(22,0.82,0.84*0.8,nodeRGBs[0][2]); //L orange
//  setNodeHSB(56,0.96,0.94*0.8,nodeRGBs[0][1]); //B yellow
//  setNodeHSB(319,0.85,0.76*0.8,nodeRGBs[0][0]); //R pink

  
  
//  getRGB(229,(int) (255*0.65),(int) (255*0.62),nodeRGBs[0][2]); // L
//  getRGB(279,(int) (255*0.71),(int) (255*0.94),nodeRGBs[0][1]); // B
//  getRGB(103,(int) (255*0.63),(int) (255*0.74),nodeRGBs[0][0]); // R
  
//  for(int node = 0; node < NUM_PYLONS; node++)
//  {
//      for (int driver = 0; driver < NUM_DRIVERS_PER_PYLON; driver++)
//      {
//        getRGB(hues[driver],255,100,nodeRGBs[node][driver]);
//      }
//  }
//  hues[0] = (hues[0]+1) % 360;
//  hues[1] = (hues[1]+1) % 360;
//  hues[2] = (hues[2]+1) % 360;
    
//    for(int node = 0; node < NUM_PYLONS; node++)
//      for (int driver = 0; driver < NUM_DRIVERS_PER_PYLON; driver++)
//      {
//        rainbow_update(nodeRGBs[node][driver]);  
//      }
//    // Update central balloon separately
//    rainbow_update(nodeRGBs[NUM_PYLONS][0]);
}




void updateNodeDisplay()
{
  int cur_node = 0;
  Driver[STRAND_LEFT].begin(); // begin
  for (int node = 0; node < NUM_NODES_LEFT; node++)
  {
    for (int driver = 0; driver < NUM_DRIVERS_PER_PYLON; driver++)
      Driver[STRAND_LEFT].SetColor(nodeRGBs[NODE_MAP[cur_node]][driver][0], nodeRGBs[NODE_MAP[cur_node]][driver][1], nodeRGBs[NODE_MAP[cur_node]][driver][2]); 
    ++cur_node;
  }
  Driver[STRAND_LEFT].end();

  
  Driver[STRAND_RIGHT].begin(); // begin
  for (int node = 0; node < NUM_NODES_RIGHT; node++)
  {
    for (int driver = 0; driver < NUM_DRIVERS_PER_PYLON; driver++)
      Driver[STRAND_RIGHT].SetColor(nodeRGBs[NODE_MAP[cur_node]][driver][0], nodeRGBs[NODE_MAP[cur_node]][driver][1], nodeRGBs[NODE_MAP[cur_node]][driver][2]); 
    ++cur_node;
  }
  Driver[STRAND_RIGHT].end();

  
  Driver[STRAND_B].begin(); // begin
  for (int node = 0; node < NUM_NODES_B; node++)
  {
    Driver[STRAND_B].SetColor(nodeRGBs[NODE_MAP[cur_node]][0][0], nodeRGBs[NODE_MAP[cur_node]][0][1], nodeRGBs[NODE_MAP[cur_node]][0][2]); 
    ++cur_node;
  }
  Driver[STRAND_B].end();
}

void setup() {
  Serial.begin(9600);

  initializeNodeRGBs();
  
  currentSensor = 0;
  distance[currentSensor] = 0;
  sonar[currentSensor].ping_timer(echoCheck);
  pingTimer = millis() + PING_INTERVAL;
  colorUpdateTimer = millis();
}
 
void loop() {
  // Sensor Update
//  if(millis() > pingTimer) {
//      sonar[currentSensor].timer_stop();
////      if(currentSensor==0){        
////      Serial.print(currentSensor);   
////      Serial.print("=");
////      Serial.print(distance[currentSensor]);
////      Serial.print("in\n");
////      }
//      currentSensor = (currentSensor + 1) % SONAR_NUM;
//      distance[currentSensor] = MAX_DISTANCE;
//      sonar[currentSensor].ping_timer(echoCheck);
//      pingTimer = millis() + PING_INTERVAL;
//  }
  
  // Color Update
  if (millis() >  colorUpdateTimer) {  
    updateNodeDisplay();
    updateNodeColors();
    colorUpdateTimer = colorUpdateTimer + COLOR_UPDATE_INTERVAL;
  }
}
