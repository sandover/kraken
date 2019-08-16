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
#define NUM_DRIVERS_PER_PYLON 3
#define DRIVER_CLK1 13//pins definitions for the driver        
#define DRIVER_DATA1 12
#define DRIVER_CLK2 11//pins definitions for the driver        
#define DRIVER_DATA2 10
#define DRIVER_CLK3 9//pins definitions for the driver        
#define DRIVER_DATA3 8
#define DRIVER_CLK4 7//pins definitions for the driver        
#define DRIVER_DATA4 6
#define DRIVER_CLK5 5//pins definitions for the driver        
#define DRIVER_DATA5 4
#define DRIVER_CLK6 3//pins definitions for the driver        
#define DRIVER_DATA6 2
#define DRIVER_CLK7 14//pins definitions for the driver        
#define DRIVER_DATA7 15
#define DRIVER_CLK8 16//pins definitions for the driver        
#define DRIVER_DATA8 17
#define DRIVER_CLK9 18//pins definitions for the driver        
#define DRIVER_DATA9 19

#define COLOR_UPDATE_INTERVAL 10
 
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

byte nodeRGBs[NUM_NODES][NUM_DRIVERS_PER_PYLON][3] = {0};

RGBdriver Driver[NUM_NODES] = { // Sensor object array.
  RGBdriver(DRIVER_CLK1,DRIVER_DATA1),
  RGBdriver(DRIVER_CLK2,DRIVER_DATA2),
  RGBdriver(DRIVER_CLK3,DRIVER_DATA3),
  RGBdriver(DRIVER_CLK4,DRIVER_DATA4),
  RGBdriver(DRIVER_CLK5,DRIVER_DATA5),
  RGBdriver(DRIVER_CLK6,DRIVER_DATA6),
  RGBdriver(DRIVER_CLK7,DRIVER_DATA7),
  RGBdriver(DRIVER_CLK8,DRIVER_DATA8),
  RGBdriver(DRIVER_CLK9,DRIVER_DATA9)  
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


void updateNodeColors()
{
    for(int node = 0; node < NUM_PYLONS; node++)
      for (int driver = 0; driver < NUM_DRIVERS_PER_PYLON; driver++)
        rainbow_update(nodeRGBs[node][driver]);  
    // Update central balloon separately
    rainbow_update(nodeRGBs[NUM_PYLONS][0]);
}

void updateNodeDisplay()
{
    for(int node = 0; node < NUM_PYLONS; node++){
      Driver[node].begin(); // begin
      for (int driver = 0; driver < NUM_DRIVERS_PER_PYLON; driver++)
        Driver[node].SetColor(nodeRGBs[node][driver][0], nodeRGBs[node][driver][1], nodeRGBs[node][driver][2]); 
      Driver[node].end();
//      if (node == 0){
//        Serial.print(nodeRGBs[node][0][0]);
//        Serial.print(", ");
//        Serial.print(nodeRGBs[node][0][1]);
//        Serial.print(", ");
//        Serial.print(nodeRGBs[node][0][2]);
//        Serial.print("\n");
//      }
  }
  // Handle central balloon separately
  Driver[NUM_PYLONS].begin();
  Driver[NUM_PYLONS].SetColor(nodeRGBs[NUM_PYLONS][0][0], nodeRGBs[NUM_PYLONS][0][1], nodeRGBs[NUM_PYLONS][0][2]); 
  Driver[NUM_PYLONS].end();
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
  if(millis() > pingTimer) {
      sonar[currentSensor].timer_stop();
//      if(currentSensor==0){        
//      Serial.print(currentSensor);   
//      Serial.print("=");
//      Serial.print(distance[currentSensor]);
//      Serial.print("in\n");
//      }
      currentSensor = (currentSensor + 1) % SONAR_NUM;
      distance[currentSensor] = MAX_DISTANCE;
      sonar[currentSensor].ping_timer(echoCheck);
      pingTimer = millis() + PING_INTERVAL;
  }
  
  // Color Update
  if (millis() >  colorUpdateTimer) {  
    updateNodeDisplay();
    updateNodeColors();
    colorUpdateTimer = colorUpdateTimer + COLOR_UPDATE_INTERVAL;
  }
}
