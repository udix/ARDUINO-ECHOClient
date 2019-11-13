#include <WiFi101.h>

#include "myWiFiSettings.h"

char ssid[] = SECRET_SSID;            // your network SSID (name)
char pass[] = SECRET_PASS;            // your network password
char server[] = "192.168.1.38";       // Raspberry's IP
int ECHO_Port = 1025;                 // TCP Port used for ECHO protocol
int timeout = 100;                    // tenths of a second 100 * 0,1s = 10s

// Initialize the Ethernet client object
WiFiClient client;

//------------------------------------------------------------------------------
//                                   SETUP
//------------------------------------------------------------------------------
void setup() {
  //Initialize serial
  Serial.begin(9600);
  
  //Set serial timeout to 1 second
  Serial.setTimeout(1000);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network
  Connect2WiFi();

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");

  // Setup finished
  Serial.println("Setup finished!");
  printCurrentNet();
  printWifiData();
}

//--------------------------------------------------------------------------------
//                                   LOOP
//--------------------------------------------------------------------------------
void loop() {

  if (Serial.available() > 0) {
    // Read command to send
    String cmd = Serial.readStringUntil('\n');

    // Check if device is connected to WiFi network, if it isn't try to connect
    if (WiFi.status() != WL_CONNECTED) { Connect2WiFi(); }
    
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(server, ECHO_Port)) {
      Serial.println("connected to server");
      // Send command
      client.println(cmd);

      // Var used to check timeout
      int i = 0;

      //Wait for an answer from server
      Serial.print("Waiting for an answer from server.");
      while (client.available() == 0 && i < timeout) {
        Serial.print(".");
        i++;
        delay(100);
      }

      Serial.println();

      // If waiting is over for timeout report error, otherwise, print answer from server
      if (i < timeout) {
        // Print answer
        Serial.println(client.readString());
        // Disconnect from the server
        client.stop(); 
      } else {
        Serial.println("No answer from server!");
      }

    }
  }
}


void printWifiData() {
  // print device's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

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

//Connect to WiFi network
void Connect2WiFi() {
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    WiFi.begin(ssid, pass);

    // wait 10 seconds
    delay(10000);
  }
}