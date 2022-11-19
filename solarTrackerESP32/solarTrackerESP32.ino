//*******************************************************************************************************************************
//*******************************************************************************************************************************
//                                                  SOLAR TRACKER ESP32
//*******************************************************************************************************************************
//*******************************************************************************************************************************
// DESCRIÇÃO DO FUNCIONAMENO: Esse programa tem como objetivo a conexão com uma rede sem fio (wifi), para leitura e envio de
// de dados para o Firebase e assim enviar os dados para o Arduino.
//
// PROGRAMADOR: Marcos da Silva Paiva
// DATA CRIAÇÃO: 13/09/2022
// DATA ÚLTIMA REVISÃO: 01/11/2022
// VERSÃO: 1.0
//*******************************************************************************************************************************
//*******************************************************************************************************************************

/////////////////////////////////////////////////////////////////////
/*
* DECLARAÇÃO DAS BIBLIOTECAS 
*/
////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <WebServer.h>         
#include <IOXhop_FirebaseESP32.h>                           
#include <ArduinoJson.h>  
#include <ArduinoOTA.h>
#include <WiFiManager.h>        

/////////////////////////////////////////////////////////////////////
/*
* DECLARAÇÃO DAS VARIÁVEIS 
*/
////////////////////////////////////////////////////////////////////

#define FIREBASE_HOST "https://solarpaneldb-default-rtdb.firebaseio.com/" //URL PARA COMUNICAÇÃO COM O BANCO DE DADOS DO FIREBASE
#define FIREBASE_AUTH "EaWCLGnyE7L3UnzpbjLRIdqw2CZkAeLW3BWEKyrM" //CHAVE DE AUTENTICAÇÃO DO FIREBASE

const int ledConected = 19; //LED QUE INDICA QUANDO O DISPOSITIVO EDTÁ CONECTADO COM UMA REDE SEM FIO

float voltageMeasure = 0; //RECEBE O VALOR DA TENSÃO MEDIDA
float currentMeasure = 0; //RECEBE O VALOR DA CORRENTE MEDIDA

String inputString = ""; //RECEBE OS DADOS ATRAVÉS DA PORTA SERIAL RX, TX
String lastStatus = ""; //ARMAZENA O ÚLTIMO VALOR DO STATUS DO RASTREADOR AUTOMÁTICO VINDO DO FIREBASE
bool stringComplete = false; //BOOLEANA QUE VERIFICA SE A COMUNICAÇÃO PELA SERIAL FOI FINALIZADA, STOP BIT

WebServer server(80); //ABRE UM SERVER NA PORTA 80 PARA ACESSO AO ESP32

/////////////////////////////////////////////////////////////////////
/*
* CONFIGURAÇÕES DE HARDWARE 
*/
////////////////////////////////////////////////////////////////////

void setup() {
 
//  WiFi.mode(WIFI_STA);
  
  Serial.begin(9600); //INICIA A COMUNICAÇÃO COM A PORTA SERIAL BAUD RATE DE 9600

//DECLARAÇÃO DE IO'S
  pinMode (ledConected, OUTPUT);
  digitalWrite(ledConected, LOW);

  WiFiManager wm; //BIBLIOTECA QUE UM ACCESS POINT PARA ENVIO DE LOGIN E SENHA DO WIFI QUE SE DESEJA CONECTAR

  bool res;
  res = wm.autoConnect("SolarTrackerAP","123456789"); //DECLARAÇÃO DE SSID E SENHA DO AP CRIADO
  
  String ip = WiFi.localIP().toString(); //ARMAZENA O ENDEREÇA DE IP NA REDE CONECTADA
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //INICIA A COMUNICAÇÃO COM O FIREBASE ENVIANDO A URL E AUTENTICAÇÃO
  delay(1000);
  Firebase.setString("ip", ip); //ENVIA O ENDEREÇO DE IP PARA O FIREBASE
  
  server.on("/up", up); //http://192.168.xxx.xxx/up - ACESSANDO O ROOT DO DISPOSITIVO E ENTRANDO NA ROTA MOVE-SE O MOTOR
  server.on("/down", down); //http://192.168.xxx.xxx/down - ACESSANDO O ROOT DO DISPOSITIVO E ENTRANDO NA ROTA MOVE-SE O MOTOR
  server.on("/right", right); //http://192.168.xxx.xxx/right - ACESSANDO O ROOT DO DISPOSITIVO E ENTRANDO NA ROTA MOVE-SE O MOTOR
  server.on("/left", left); //http://192.168.xxx.xxx/left - ACESSANDO O ROOT DO DISPOSITIVO E ENTRANDO NA ROTA MOVE-SE O MOTOR

  server.begin(); //INICIA O SERVER NA INTERNET PARA CONEXÃO COM OUTROS CLIENTES
}

//FUNÇÃO QUE RECEBE UM GET NO IP DO DISPOSITIVO DO CLIENTE NA INTERNET PARA MOVER MOTOR PARA CIMA
void up(){
  Serial.println("U");

  server.send(200, "text/html", "ok");
}
//FUNÇÃO QUE RECEBE UM GET NO IP DO DISPOSITIVO DO CLIENTE NA INTERNET PARA MOVER MOTOR PARA BAIXO
void down(){
  Serial.println("D");

  server.send(200, "text/html", "ok");
}
//FUNÇÃO QUE RECEBE UM GET NO IP DO DISPOSITIVO DO CLIENTE NA INTERNET PARA MOVER MOTOR PARA DIREITA
void right(){
  Serial.println("R");

  server.send(200, "text/html", "ok");
}
//FUNÇÃO QUE RECEBE UM GET NO IP DO DISPOSITIVO DO CLIENTE NA INTERNET PARA MOVER MOTOR PARA ESQUERDA
void left(){
  Serial.println("L");

  server.send(200, "text/html", "ok");
}

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO INFINITA 
*/
////////////////////////////////////////////////////////////////////

void loop() {

//CONDIÇÃO QUE VERIFICA SE ESTÁ CONECTADO A UMA REDE PAR ACENDER O LED DE CONEXÃO
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledConected, LOW);
  } else {
    digitalWrite(ledConected, HIGH);
  }

  String statusDB = Firebase.getString("/status/status"); //FAZ A LEITURA DO STATUS DO RASTREADOR NO FIREBASE

//SE O STATUS FOI ALTERADO PELO APP OU SITE ENTÃO ATUALIZA A VARIÁVEL
  if (statusDB != lastStatus) {
    Serial.println("S" + statusDB);
    lastStatus = statusDB;
  }

//CONDIÇÃO QUE VERIFICA OS DADOS RECEBIDOS PELO ARDUINO DE MEDIÇÕES PARA ENVIAR AO FIREBSE
//SE RASTREADOR LIGADO ENVIADO PARA VOLTAGE E CURRENT, SE DESLIGADO ENVIADO PARA VOLTAGE2 E CURRENT2  
  if (serialEvent()) {
    inputString.trim();
    char which = inputString[0];
    if (statusDB == "1" && inputString.length() < 7) {
      if (which == 'C'){
        inputString.remove(0, 1);
        Firebase.pushString("current", String(inputString)); 
      } else if (which == 'V') {
        inputString.remove(0, 1);
        Firebase.pushString("voltage", String(inputString));
      }
    } else if (inputString.length() < 7) {
      if (which == 'C'){
        inputString.remove(0, 1);
        Firebase.pushString("current2", String(inputString)); 
      } else if (which == 'V') {
        inputString.remove(0, 1);
        Firebase.pushString("voltage2", String(inputString));
      }
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  server.handleClient(); //MANTÉM O SERVIDOR DISPONÍVEL
  delay(100);
}

/////////////////////////////////////////////////////////////////////
/*
* FUNÇÃO BOOLEANA QUE RECEBE OS DADOS PELA PORTA SERIAL DO ESP32 
*/
////////////////////////////////////////////////////////////////////

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
