#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>     // Include the Wi-Fi library

const char* ssid     = "Orange-7EFE";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "39149919";     // The password of the Wi-Fi network

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  Serial.println("...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  delay(2000);
  Serial.print(ss.available()); 
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      // Latitude in degrees (double)
      Serial.print("Latitude= "); 
      double lat = gps.location.lat();
      Serial.print(lat, 6);      
      // Longitude in degrees (double)
      Serial.print(" Longitude= "); 
      double lng = gps.location.lng();
      Serial.println(lng, 6); 

         if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
            
             String contentType = "application/x-www-form-urlencoded";
 
             HTTPClient http;    //Declare object of class HTTPClient
             http.begin("http://pur.przedprojekt.com/gps");      //Specify request destination
             http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header
             String DatatoSend ="";
             DatatoSend += "lng=";
             DatatoSend += (lng);
             DatatoSend += "&ltd=";
             DatatoSend += (lat);
             //char data = "lng=".lng."&ltd=".lat."";
             int httpCode = http.POST(DatatoSend);   //Send the request
             String payload = http.getString();                  //Get the response payload
           
             Serial.println(httpCode);   //Print HTTP return code
             Serial.println(payload);    //Print request response payload
           
             http.end();  //Close connection
           
           }else{
           
              Serial.println("Error in WiFi connection");   
           
           }
           
            delay(15000);  //Send a request every 15 seconds

    }
  }
}
