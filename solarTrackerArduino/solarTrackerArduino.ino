#define voltageSensor A0
#define currentSensor A1
#define sensorSupEsq A2
#define sensorSupDir A3
#define sensorInfEsq A4
#define sensorInfDir A5

unsigned long measureTime = millis();

int dirVer   = 3;
int stepVer  = 4;
int dirHor   = 5;
int stepHor  = 6;

int posVer = 0;
int posHor = 0;
int comparaSensoresLaterais = 85;
int comparaSensoresExtremos = 15;
float voltageMeasure = 0;
float currentMeasure = 0;
float voltRef = 0;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool invertRead = false;

void setup() {

  pinMode(dirVer , OUTPUT);
  pinMode(stepVer, OUTPUT);
  pinMode(dirHor , OUTPUT);
  pinMode(stepHor, OUTPUT);

  digitalWrite(dirVer, LOW);
  digitalWrite(dirHor, LOW);
  
  Serial.begin(9600);

  inputString.reserve(200);
}

void loop() {
 /* if (stringComplete) {
    inputString.trim();
    if (inputString == "S1") {
      solarTrackerON();
    } else if (inputString == "U1") {
      manualMotion(1, 1);
    } else if (inputString == "D1") {
      manualMotion(1, 0);
    } else if (inputString == "R1") {
      manualMotion(0, 1);
    } else if (inputString == "L1") {
      manualMotion(0, 0);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }*/
  solarTrackerON();
  if (millis() - measureTime > 600000) {
    if (invertRead) {
      Serial.println("V" + String(voltageMeasureFUN()));
      invertRead = !invertRead;
    } else {
      Serial.println("C" + String(currentMeasureFUN()));
      invertRead = !invertRead;
    }
    measureTime = millis();
  }
  delay(100);
}

void solarTrackerON()
{
  if (!noite()) {
    
    posVer = comparaVertical();
    posHor = comparaHorizontal();
/////////////////////////////////////////////////////////////////////
/*
* MOVIMENTO TRANSLAÇÃO 
*/
////////////////////////////////////////////////////////////////////
  
    if (posVer < 2) {
      //Define sentido de Rotação
      if (posVer == 1 ) {
        digitalWrite(dirVer, HIGH);
      } else {
        digitalWrite(dirVer, LOW);
      }

      for(int i = 0; i < 2; i++)
      {
        digitalWrite(stepVer, HIGH);
        delay(10);
        digitalWrite(stepVer, LOW);
        delay(10);
      }
    }

/////////////////////////////////////////////////////////////////////
/*
* MOVIMENTO ROTAÇÃO 
*/
////////////////////////////////////////////////////////////////////
    
    if (posHor < 2) {
      //Define sentido de Rotação
      if (posHor == 1 ) {
        digitalWrite(dirHor, HIGH);
      } else {
        digitalWrite(dirHor, LOW);
      }

      for(int i = 0; i < 5; i++)
      { 
        digitalWrite(stepHor, HIGH);
        delay(10);
        digitalWrite(stepHor, LOW);
        delay(10);
      }
    }    
  }
  delay(10);
}

int comparaHorizontal() {
  int valorEsq = analogRead(sensorSupEsq) + analogRead(sensorInfEsq);
  int valorDir = analogRead(sensorSupDir) + analogRead(sensorInfDir);

  if (valorEsq - valorDir > comparaSensoresLaterais) {
    return 1;
  } else if (valorDir - valorEsq > comparaSensoresLaterais) {
    return 0;
  } else return 2;
}

int comparaVertical() {
  int valorSup = analogRead(sensorSupEsq) + analogRead(sensorSupDir);
  int valorInf = analogRead(sensorInfEsq) + analogRead(sensorInfDir);

  if (valorSup - valorInf > comparaSensoresExtremos) {
    return 1;
  } else if (valorInf - valorSup > comparaSensoresExtremos) {
    return 0;
  } else return 2;
}

bool noite() {
  return ((analogRead(sensorSupEsq) > 1000) &&
          (analogRead(sensorSupDir) > 1000) &&
          (analogRead(sensorInfDir) > 1000) &&  
          (analogRead(sensorInfEsq) > 1000));
}

float voltageMeasureFUN(){
  voltageMeasure = (analogRead(voltageSensor)*0.00489)*5;

  return voltageMeasure;
}

float currentMeasureFUN() {
  float rawValue = 0;
  float samples = 0;
  for (int x = 0; x < 10; x++) {
    samples = samples + analogRead(currentSensor);
    rawValue = samples/10;
  }
  voltRef = (5.0 / 1023.0)*rawValue;
  voltRef = voltRef - 2.5 + 0.007;
  currentMeasure = voltRef / 0.100; //5000 é a Vin*1000, 2500 é o offSet de ajuste e 66 é a resolução do sensor 66mV/A

  return currentMeasure;
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void manualMotion(int motor, int dir) {
  if (motor == 1) {
    if (dir == 1 ) {
      digitalWrite(dirVer, HIGH);
    } else {
      digitalWrite(dirVer, LOW);
    }
    for(int i = 0; i < 5; i++)
    {
      digitalWrite(stepVer, HIGH);
      delay(10);
      digitalWrite(stepVer, LOW);
      delay(10);
    }
    Serial.println("UD0");
  }  else {
    if (dir == 1 ) {
      digitalWrite(dirHor, HIGH);
    } else {
      digitalWrite(dirHor, LOW);
    }
    
    for(int i = 0; i < 5; i++)
    {
      digitalWrite(stepHor, HIGH);
      delay(10);
      digitalWrite(stepHor, LOW);
      delay(10);
    }
    Serial.println("RL0");
  }
}
