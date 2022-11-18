#include <WiFi.h>         
#include <IOXhop_FirebaseESP32.h>                           
#include <ArduinoJson.h>  
#include <ArduinoOTA.h>
#include <WiFiManager.h>        

#define FIREBASE_HOST "https://solarpaneldb-default-rtdb.firebaseio.com/"    
#define FIREBASE_AUTH "EaWCLGnyE7L3UnzpbjLRIdqw2CZkAeLW3BWEKyrM"

const int ledConected = 19;

float voltageMeasure = 0;
float currentMeasure = 0;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
 
//  WiFi.mode(WIFI_STA);
  
  Serial.begin(9600);

  pinMode (ledConected, OUTPUT);

  WiFiManager wm;

  bool res;
  res = wm.autoConnect("SolarTrackerAP","123456789"); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
  } 
  else {  
      Serial.println("connected...yeey :)");
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  String ip = WiFi.localIP().toString();
  Serial.println(ip);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(1000);
  Firebase.setString("ip", ip);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledConected, LOW);
  } else {
    digitalWrite(ledConected, HIGH);
  }

  String statusDB = Firebase.getString("/status/status");
  String motionU = Firebase.getString("/move/UP");
  String motionD = Firebase.getString("/move/DOWN");
  String motionR = Firebase.getString("/move/RIGHT");
  String motionL = Firebase.getString("/move/LEFT");

  if (motionU == "0" && motionD == "0" && motionR == "0" && motionL == "0") {
    Serial.println("S" + statusDB); 
  } else {
    if (motionU == "1") {
      Serial.println("U" + motionU);
    } else if (motionD == "1") {
      Serial.println("D" +motionD);
    } else if (motionR == "1") {
      Serial.println("R" +motionR);
    } else if (motionL == "1") {
      Serial.println("L" +motionL);
    }
  }
  
  if (serialEvent()) {
    inputString.trim();
    char which = inputString[0];
    if (statusDB == "1" && inputString.length() < 6) {
      if (which == 'C'){
        inputString.remove(0, 1);
        Firebase.pushString("current", String(inputString)); 
      } else if (which == 'V') {
        inputString.remove(0, 1);
        Firebase.pushString("voltage", String(inputString));
      }
    } else if (inputString.length() < 6) {
      if (which == 'C'){
        inputString.remove(0, 1);
        Firebase.pushString("current2", String(inputString)); 
      } else if (which == 'V') {
        inputString.remove(0, 1);
        Firebase.pushString("voltage2", String(inputString));
      } else if (inputString == "UD0"){
        Firebase.setString("move/UP", "0");
        delay(1);
        Firebase.setString("move/DOWN", "0");
      } else if (inputString == "RL0"){
        Firebase.setString("move/RIGHT", "0");
        delay(1);
        Firebase.setString("move/LEFT", "0");        
      }
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  delay(100);
}

bool serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  return stringComplete;
}
