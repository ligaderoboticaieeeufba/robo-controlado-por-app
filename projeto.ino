#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "AndroidAP";
const char* password = "teste123";
String command ="";

int rightMotor2 = 13; // D7 - right Motor -
int rightMotor1 = 15; // D8 - right Motor +
int leftMotor2 = 0; // D3 - left Motor - 
int leftMotor1 = 2; // D4 - left Motor +
int eneLeftMotor = 12; // D6 - enable Motor Left
int eneRightMotor = 14; // D5 - enable Motor Right

void setup() 
{
  Serial.begin(115200);
  
  pinMode(eneLeftMotor, OUTPUT);
  pinMode(eneRightMotor, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  
  digitalWrite(eneLeftMotor, LOW);
  digitalWrite(eneRightMotor, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  
  connectWiFi();
  server.begin();
}
// 192.168.43.112
void loop() 
{

  client = server.available();
  if(!client)
    return;
   
  command = checkClient();
  
  if(command == "frente")
  {
    Serial.println("FRENTE"); 
    forwardMotor();
    
    
  }
  else if(command == "volta")
  {
   Serial.println("VOLTA"); 
    reverseMotor();
  }
  else if(command == "direita")
  {
    Serial.println("DIREITA");
    turnOnRight();
   
  }
  else if(command == "esquerda")
  {
    Serial.println("ESQUERDA");
    turnOnLeft();

  }
  else if(command == "pare")
  {
     Serial.println("PARE");
     stopMotor();

  }

  sendBackEcho(command); // envia o comando de retorn de volta para o dispositivo android.
  
  

}

/* connecting WiFi */

void forwardMotor(void) 
{

  digitalWrite(eneLeftMotor,HIGH);
  digitalWrite(eneRightMotor,HIGH);
  digitalWrite(leftMotor1,HIGH);
  digitalWrite(leftMotor2,LOW);
  digitalWrite(rightMotor1,HIGH);
  digitalWrite(rightMotor2,LOW);

  
}

void reverseMotor(void)
{
  digitalWrite(eneLeftMotor, HIGH);
  digitalWrite(eneRightMotor, HIGH);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH); 
}

void stopMotor(void)
{
  digitalWrite(eneLeftMotor, LOW);
  digitalWrite(eneRightMotor, LOW);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
  command = "";

}

void turnOnLeft(void)
{
   digitalWrite(eneLeftMotor,HIGH);
   digitalWrite(eneRightMotor,HIGH);
   digitalWrite(leftMotor1, LOW);
   digitalWrite(leftMotor2, HIGH);
   digitalWrite(rightMotor1,HIGH);
   digitalWrite(rightMotor2,LOW);
  
}

void turnOnRight(void)
{
  digitalWrite(eneLeftMotor,HIGH);
  digitalWrite(eneRightMotor,HIGH);
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1,LOW);
  digitalWrite(rightMotor2,HIGH);
  
}


void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  
  while ((!(WiFi.status() == WL_CONNECTED))) {  // enquanto não estiver conectado faça:
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("Conectado ao WiFi");
  Serial.print("Rede : ");
  Serial.println(ssid);
  Serial.print("IP obtido : ");
  Serial.print((WiFi.localIP()));
}

String checkClient(void)
{
  while(!client.available())  delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9,9);
  return request;
}

void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: txt/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}

