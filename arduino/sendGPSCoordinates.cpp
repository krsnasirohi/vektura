#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3
SoftwareSerial sim800L(rxPin,txPin); 

int NEO6MRXPin = 4;
int NEO6MTXPin = 5;

TinyGPSPlus gps;

SoftwareSerial gpsSerial(NEO6MRXPin, NEO6MTXPin);

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  sim800L.begin(9600);

  //Begin serial communication with Arduino and NEO6M 
  gpsSerial.begin(9600);

  Serial.println("Initializing...");
  //delay(10000);

  //Once the handshake test is successful, it will back to OK
  sendATcommand("AT", "OK", 2000);
  sendATcommand("AT+CMGF=1", "OK", 2000);
  
}

void loop()
{
  while(sim800L.available()){
    Serial.println(sim800L.readString());
  }
  while(Serial.available())  {
    sim800L.println(Serial.readString());
  }

  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      sendLocationToDynamoDB();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}

int sendLocationToDynamoDB() {

  if (gps.location.isValid())
  {

    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);
  
    String time = String(gps.date.year());
    time += "-";
    time += String(gps.date.month());
    time += "-";
    time += String(gps.date.day());
    time += "-";
    if (gps.time.hour() < 10) time += "0";
    time += String(gps.time.hour());
    time += ":";
    if (gps.time.minute() < 10) time += "0";
    time += String(gps.time.minute());
    time += ":";
    if (gps.time.second() < 10) time += "0";
    time += String(gps.time.second());
    time += ".";
    if (gps.time.centisecond() < 10) time += "0";
    time += String(gps.time.centisecond());


    String url;
    url = "http://<PLACEHOLDER FOR CLOUDFRONT DISTRIBUTIONS DOMAIN NAME>.cloudfront.net/prod/log?lat=";
    url += latitude;
    url += "&longitude=";
    url += longitude;
    url += "&time=";
    url += time;
    url += "&routeId=<PLACEHOLDER FOR ROUTE ID>";
    
    Serial.println(url);    
    delay(30000);
          
    sendATcommand("AT+CFUN=1", "OK", 2000);
    //AT+CGATT = 1 Connect modem is attached to GPRS to a network. AT+CGATT = 0, modem is not attached to GPRS to a network
    sendATcommand("AT+CGATT=1", "OK", 2000);
    //Connection type: GPRS - bearer profile 1
    sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
    //sets the APN settings for your network provider.
    sendATcommand("AT+SAPBR=3,1,\"APN\",\"internet\"", "OK", 2000);
    //enable the GPRS - enable bearer 1
    sendATcommand("AT+SAPBR=1,1", "OK", 2000);
    //Init HTTP service
    sendATcommand("AT+HTTPINIT", "OK", 2000); 
    sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 1000);
    sim800L.print("AT+HTTPPARA=\"URL\",\"");
    sim800L.print(url);
    sendATcommand("\"", "OK", 1000);
    //Set up the HTTP action
    sendATcommand("AT+HTTPACTION=0", "0,200", 1000);
    //Terminate the HTTP service
    sendATcommand("AT+HTTPTERM", "OK", 1000);
    //shuts down the GPRS connection. This returns "SHUT OK".
    sendATcommand("AT+CIPSHUT", "SHUT OK", 1000);
  }
}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    //Initialice the string
    memset(response, '\0', 100);
    delay(100);
    
    //Clean the input buffer
    while( sim800L.available() > 0) sim800L.read();
    
    if (ATcommand[0] != '\0'){
      //Send the AT command 
      sim800L.println(ATcommand);
    }

    x = 0;
    previous = millis();

    //this loop waits for the answer with time out
    do{
        //if there are data in the UART input buffer, reads it and checks for the asnwer
        if(sim800L.available() != 0){
            response[x] = sim800L.read();
            //Serial.print(response[x]);
            x++;
            // check if the desired answer (OK) is in the response of the module
            if(strstr(response, expected_answer) != NULL){
                answer = 1;
            }
        }
    }while((answer == 0) && ((millis() - previous) < timeout));

  Serial.println(response);
  return answer;
}
