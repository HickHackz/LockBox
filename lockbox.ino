//This code is used to create WiFi time trigger, which can be used for multiple applications.
//I built a lockbox with a latching solenoid which only opens during certain times throughout the week.
//This helped us to stay focued on school since we could lock the TV remote in the box, and only watch TV late in the evenings when the box was open

//All Wifi Initialization code was referenced from:
// http://man.hubwiz.com/docset/Arduino.docset/Contents/Resources/Documents/www.arduino.cc/en/Tutorial/WiFiNINAConnectWithWPA.html

#include <SPI.h>
#include <WiFiNINA.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <RTCZero.h>
#include <string>
RTCZero rtc;
char ssid[] = ""; //fill in Wifi SSID
char pass[] = ""; //fill in Wifi password
int status = WL_IDLE_STATUS;
const int GMT = 18; //change this to adapt it to your time zone

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
  pinMode(0, INPUT_PULLUP ); //pin 0 is the button
  pinMode(1, OUTPUT); //pin 1 is trigger signal, going to the relay before the solenoid.
   rtc.begin();

  unsigned long epoch;

  int numberOfTries = 0, maxTries = 6;

  do {

    epoch = WiFi.getTime();

    numberOfTries++;

  }

  while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries == maxTries) {

    Serial.print("NTP unreachable!!");

    while (1);

  }

  else {

    Serial.print("Epoch received: ");

    Serial.println(epoch);

    rtc.setEpoch(epoch);

    Serial.println();

  }

}

void loop() {
  

   if((getDayOfWeek() == 0 ||getDayOfWeek() == 1 || (getDayOfWeek() == 6 &&  rtc.getHours() + GMT >= 17) || rtc.getHours() + GMT >= 20 || rtc.getHours() + GMT == 6) && digitalRead(0) == LOW){
      digitalWrite(1, HIGH);
    }
   else {
    digitalWrite(1, LOW);
   }
}
void printTime()
{

  print2digits(rtc.getHours() + GMT);

  Serial.print(":");

  print2digits(rtc.getMinutes());

  Serial.print(":");

  print2digits(rtc.getSeconds());

  Serial.println();
}

void printDate()
{

  Serial.print(rtc.getDay());

  Serial.print("/");

  Serial.print(rtc.getMonth());

  Serial.print("/");

  Serial.print(rtc.getYear());

  Serial.print(" ");
}

void printWiFiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}

void print2digits(int number) {

  if (number < 10) {

    Serial.print("0");

  }

  Serial.print(number);
}
void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print(weekday());
  Serial.print(hour());

  
 // printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }

}

uint8_t getDayOfWeek() {
    uint16_t y = 2000 + rtc.getYear();
    uint16_t m = rtc.getMonth();
    uint16_t d = rtc.getDay();
    // http://stackoverflow.com/a/21235587
    return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
}
