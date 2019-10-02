///////////////////////////////////////////////////////////////////////////////////////
//Terms of use
///////////////////////////////////////////////////////////////////////////////////////
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////// 
//Car GPS Tracker System part 2.
//by Varad Kulkarni <http://www.microcontrollershub.com>
//Created on 12 September 2019
///////////////////////////////////////////////////////////////////////////////////////// 

#include <TinyGPS++.h>
#include "UbidotsMicroESP8266.h"

#define WIFISSID "your_wifi_name" // Put here your Wi-Fi SSID
#define PASSWORD "your_wifi_password" // Put here your Wi-Fi password
#define TOKEN  "your_ubidots_api_tocken"  // Put here your Ubidots TOKEN

Ubidots client(TOKEN);                            // The Ubidots object.
TinyGPSPlus gps;                                  // The TinyGPS++ object.

void setup()
{
  Serial.begin(9600);                             // Start Serial communication with 9600 Baud rate.
  client.wifiConnection(WIFISSID, PASSWORD);      // Connect to the wifi router.
}

void loop()
{
  while (Serial.available() > 0)                  // If data is available on Serial bus.
  {
    if (gps.encode(Serial.read()))                // Decode GPS NMEA sentennces.
    {
      SendGPSData();                              // Print required data.
    }
  }
}

void SendGPSData()
{
  if (gps.location.isValid())
  {
    int TempSensorValue = 0,HumiditySensorValue = 0; ; 
    float lat= 0, lng = 0;
    
    // Read Sensor data
    TempSensorValue = analogRead(0);
    HumiditySensorValue = analogRead(1);
    
    // Extract GPS co-ordinates
    lat = gps.location.lat();
    lng = gps.location.lng();

    // Print GPS co-ordinates on Serial monitor
    Serial.print(F("Location: ")); 
    Serial.print(lat, 5);           // Print Latitude.      
    Serial.print(F(","));
    Serial.println(lng, 5);           // Print Longitude. 

    // Create context text of GPS co-ordinates
    char context[25];    
    sprintf(context, "lat=%.5f$lng=%.5f", lat, lng); //Sends latitude and longitude for watching position in a map
    client.add("Humidity", HumiditySensorValue, context);
    client.add("Temperature", TempSensorValue);

    // Send sensor data to the Ubidots server
    client.sendAll(true);   
    
    // Delay of 2 Seconds.
    delay(2000);
  }
  else
  {
    Serial.print(F("Error in getting GPS coordinates, Please check hardware connection."));
  }  
  Serial.println();
}
