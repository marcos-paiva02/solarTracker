//*******************************************************************************************************************************
//*******************************************************************************************************************************
//                                                  SOLAR TRACKER ARDUINO
//*******************************************************************************************************************************
//*******************************************************************************************************************************
// DESCRIÇÃO DO FUNCIONAMENO: Esse programa tem como objetivo o acionamento dos drivers dos motores, medições de corrente e tensão
// do sistema foto-voltaico e leitura dos LDR'S para o funcionamento do rastreador solar.
//
// PROGRAMADOR: Marcos da Silva Paiva
// DATA CRIAÇÃO: 13/09/2022
// DATA ÚLTIMA REVISÃO: 01/11/2022
// VERSÃO: 1.0
//*******************************************************************************************************************************
//*******************************************************************************************************************************

/////////////////////////////////////////////////////////////////////
/*
* DECLARAÇÃO DE VARIÁVEIS 
*/
////////////////////////////////////////////////////////////////////

// DEFINIÇÃO DAS PORTAS ANALÓGICAS

#define voltageSensor A0 //LEITURA DA TENSÃO
#define currentSensor A1 //LEITURA DA CORRENTE
#define sensorSupEsq A2 //LEITURA DO LDR DO CANTO SUPERIOR ESQUERDO
#define sensorSupDir A3 //LEITURA DO LDR DO CANTO SUPERIOR DIREITO
#define sensorInfEsq A4 //LEITURA DO LDR DO CANTO INFERIOR ESQUERDO
#define sensorInfDir A5 //LEITURA DO LDR DO CANTO INFERIO DIREITO

unsigned long measureTime = millis(); //CLOCK INTERNO DO ARDUINO

int dirVer   = 3; //SAÍDA PARA ACIONAMENTO DA DIREÇÃO DO MOTOR DO MOVIMENTO DE TRANSLAÇÃO
int stepVer  = 4; //SAÍDA DE PWM PARA QUANTIDADE DE PASSOS PARA O MOTOR DE MOVIMENTO DE TRANSLAÇÃO
int dirHor   = 5; //SAÍDA PARA ACIONAMENTO DA DIREÇÃO DO MOTOR DO MOVIMENTO DE ROTAÇÃO
int stepHor  = 6; //SAÍDA DE PWM PARA QUANTIDADE DE PASSOS PARA O MOTOR DE MOVIMENTO DE ROTAÇÃO

int posVer = 0; //VARIÁVEL QUE RECEBE RESULTADO DA FUNÇÃO COMPARATIVA VERTICAL
int posHor = 0; //VARIÁVEL QUE RECEBE RESULTADO DA FUNÇÃO COMPARATIVA HORIZONTAL
int comparaSensoresLaterais = 85; //COEFICIENTE DE SENSIBILIDADE PARA MOVIMENTO DO MOTOR DA ROTAÇÃO
int comparaSensoresExtremos = 15; //COEFICIENTE DE SENSIBILIDADE PARA MOVIMENTO DO MOTOR DA TRANSLAÇÃO
float voltageMeasure = 0; //VARIÁVEL QUE RECEBE A FUNÇÃO PARA CÁLCULO DA TENSÃO
float currentMeasure = 0; //VARIÁVEL QUE RECEBE A FUNÇÃO PARA CÁLCULO DA CORRENTE
float voltRef = 0;

String inputString = ""; //RECEBE OS DADOS ENVIADO PELA PORTA SERIAL
String lastStatus = ""; //ARMAZENA O STATUS DO RASTREADOR
bool stringComplete = false; //BOLLEANA QUE FUNCIONA COMO BIT STOP PARA RECEBIMENTOS DOS DADOS
bool invertRead = false; //BOOLEANA QUE VARIA O ENVIO DOS DAODS PARA O ESP ENTRE CORRENTE E TENSÃO

/////////////////////////////////////////////////////////////////////
/*
* CONFIGURAÇÕES DO ARDUINO
*/
////////////////////////////////////////////////////////////////////

void setup() {

//DECLARAÇÃO DAS ENTRADAS E SAÍDAS
  pinMode(dirVer , OUTPUT);
  pinMode(stepVer, OUTPUT);
  pinMode(dirHor , OUTPUT);
  pinMode(stepHor, OUTPUT);

  digitalWrite(dirVer, LOW);
  digitalWrite(dirHor, LOW);
  
  Serial.begin(9600); //ATIVA PORTA SERIAL COM BAUD RATE DE 9600

  inputString.reserve(200);
}

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO INFINITA 
*/
////////////////////////////////////////////////////////////////////

void loop() {
  
//VERIFICA DADOS RECEBIDOS PELA SERIAL E VERIFICA O QUE FAZER
  if (lastStatus == "S1") {
    solarTrackerON();
  }
  if (stringComplete) {
    if (inputString == "U") {
      manualMotion(1, 1);
    } else if (inputString == "D") {
      manualMotion(1, 0);
    } else if (inputString == "R") {
      manualMotion(0, 1);
    } else if (inputString == "L") {
      manualMotion(0, 0);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
//FUNÇÃO TEMPORIZADA QUE ENVIA PARA O ESP32 O VALOR MEDIDO NOS SENSORES DE TENSÃO E CORRENTE
  if (millis() - measureTime > 10000) {
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

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO PARA MOVIMENTO AUTOMÁTICO DO RASTREADOR 
*/
////////////////////////////////////////////////////////////////////

void solarTrackerON()
{
  if (!noite()) {
    
    posVer = comparaVertical(); //RECEBE O RETORNO DA FUNÇÃO VERTICAL
    posHor = comparaHorizontal(); //RECEBE O RETORNO DA FUNÇÃO HORIZONTAL

//MOVIMENTO DE TRANSLAÇÃO
  
    if (posVer < 2) {
      //Define sentido de Rotação
      if (posVer == 1 ) {
        digitalWrite(dirVer, HIGH); //VAI PRA CIMA
      } else {
        digitalWrite(dirVer, LOW); //VAI PRA BAIXO
      }

      for(int i = 0; i < 2; i++) //2 PASSOS PROGRAMADOS
      {
        digitalWrite(stepVer, HIGH);
        delay(10);
        digitalWrite(stepVer, LOW);
        delay(10);
      }
    }

//MOVIMENTO DE ROTAÇÃO
    
    if (posHor < 2) {
      //Define sentido de Rotação
      if (posHor == 1 ) {
        digitalWrite(dirHor, HIGH); //VAI PRA DIREITA
      } else {
        digitalWrite(dirHor, LOW); //VAI PRA ESQUERDA
      }

      for(int i = 0; i < 5; i++) //5 PASSOS PROGRAMADOS
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

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO QUE COMPARA OS LDRS DA DIREITA E ESQUERDA 
*/
////////////////////////////////////////////////////////////////////

int comparaHorizontal() {
  int valorEsq = analogRead(sensorSupEsq) + analogRead(sensorInfEsq);
  int valorDir = analogRead(sensorSupDir) + analogRead(sensorInfDir);

  if (valorEsq - valorDir > comparaSensoresLaterais) {
    return 1;
  } else if (valorDir - valorEsq > comparaSensoresLaterais) {
    return 0;
  } else return 2;
}

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO QUE COMPARA OS LDRS DE CIMA E BAIXO 
*/
////////////////////////////////////////////////////////////////////

int comparaVertical() {
  int valorSup = analogRead(sensorSupEsq) + analogRead(sensorSupDir);
  int valorInf = analogRead(sensorInfEsq) + analogRead(sensorInfDir);

  if (valorSup - valorInf > comparaSensoresExtremos) {
    return 1;
  } else if (valorInf - valorSup > comparaSensoresExtremos) {
    return 0;
  } else return 2;
}

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO PQUE VERIFICA SE ESTÁ DE NOITE
*/
////////////////////////////////////////////////////////////////////

bool noite() {
  return ((analogRead(sensorSupEsq) > 1000) &&
          (analogRead(sensorSupDir) > 1000) &&
          (analogRead(sensorInfDir) > 1000) &&  
          (analogRead(sensorInfEsq) > 1000));
}

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO PARA CÁLCULO DA TENSÃO
*/
////////////////////////////////////////////////////////////////////

float voltageMeasureFUN(){
  voltageMeasure = (analogRead(voltageSensor)*0.00489)*5;
  
  return voltageMeasure;
}

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO PARA CÁLCULO DA CORRENTE
*/
////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO QUE RECEBE OS DADOS DA PORTA SERIAL DO ESP32
*/
////////////////////////////////////////////////////////////////////

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
    inputString.trim();
    if (inputString == "S0" || inputString == "S1") {
      lastStatus = inputString;
    }
  }
}

////////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO QUE FAZ A MOVIMENTAÇÃO DOS MOTORES MANUALMENTE ATRAVÉS DO APP
*/
////////////////////////////////////////////////////////////////////////

void manualMotion(int motor, int dir) {
  if (motor == 1) {
    if (dir == 1 ) {
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
  }
}
