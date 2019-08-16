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
RGBdriver Driver(CLK,DIO);
int wait_time = 3000;

void setup()  
{ 
  Driver.begin(); // begin
  Driver.SetColor(0, 0, 0); //Blue. second node data
  Driver.SetColor(0, 0, 0); //Blue. second node data
  Driver.SetColor(0, 0, 0); //Blue. second node data
  Driver.end();

} 
void loop()  
{ 
  Driver.begin(); // begin
  Driver.SetColor(70, 0, 0); //Red. first node data
  Driver.SetColor(0, 70, 0); //Red. first node data
  Driver.SetColor(0, 0, 70); //Blue. second node data
  Driver.end();
 
  
  delay(wait_time);
  Driver.begin(); // begin
  Driver.SetColor(0, 70, 0); //Red. first node data
  Driver.SetColor(0, 0, 70); //Blue. second node data
  Driver.SetColor(70, 0, 0); //Red. first node data
  Driver.end();
  delay(wait_time);
  Driver.begin(); // begin
  Driver.SetColor(0, 0, 70); //Blue. second node data
  Driver.SetColor(70, 0, 0); //Red. first node data
  Driver.SetColor(0, 70, 0); //Red. first node data
  Driver.end();
 
  
  delay(wait_time);
}
