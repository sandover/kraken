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
#define CLK 2//pins definitions for the driver        
#define DIO 3
RGBdriver Driver(CLK,DIO);
void setup()  
{ 

}  
void loop()  
{ 
  unsigned int i;
  while(1){
    for(i = 0;i < 256;i ++)
    {
    Driver.begin(); // begin
    Driver.SetColor(0,0,i); //Blue. First node data. SetColor(R,G,B)
    Driver.end();
    delay(10);
    }
    for(i = 255;i > 0;i --)
    {
      Driver.begin(); // begin
      Driver.SetColor(0,0,i); //Blue. first node data
      Driver.end();
      delay(10);
    }
  }
}
