//  Author:Frankie.Chu 
//  Date:March 9,2012
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/***************************************************************************/
#include "RGBdriver.h"
#define CLK 6//pins definitions for the driver        
#define DIO 7
RGBdriver Driver_pylon(CLK,DIO);
unsigned int rgbColour[3];
  
void setup()  
{ 
} 

void loop()
{
  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
//      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      Driver_pylon.begin(); // begin
      Driver_pylon.SetColor(rgbColour[0], rgbColour[1], rgbColour[2]); //Green. first node data
      Driver_pylon.SetColor(rgbColour[1], rgbColour[2], rgbColour[0]); //Red. second node data
      Driver_pylon.SetColor(rgbColour[2], rgbColour[0], rgbColour[1]); //Red. second node data
      Driver_pylon.end();
      delay(10);
    }
  }
}

void rainbow_update(unsigned int rgb[3])
{
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgb[decColour] -= 1;
      rgb[incColour] += 1;
}
