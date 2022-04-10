#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "ssid";
const char* password = "password";
const int ledPin = 4;

WebServer server(80);

String pagina(String state){
  return "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
"<title>Foco</title>"
"</head>"
"<body>"
"<center>"
"<h1>Estatus:"+String(state)+"</h1>"
"<p><a href='/on'><button style='height:50px;width:100px'>ON</button></a></p>"
"<p><a href='/off'><button style='height:50px;width:100px'>OFF</button></a></p>"
"</center>"
"</body>"
"</html>";
};

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(ledPin, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print(String("Connect)"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);Serial.print("-");
  }
  Serial.print(String(">")+String("\n"));
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp32.foco"))Serial.println("MDNS responder started");
  server.on("/", [](){
    server.send(200,"text/HTML",pagina(String(!digitalRead(4)?"ON":"OFF")));
  });
  server.on("/on", [](){
    digitalWrite(4,LOW);
    server.send(200,"text/HTML",pagina(String(!digitalRead(4)?"ON":"OFF")));
  });
  server.on("/off", [](){
    digitalWrite(4,HIGH);
    server.send(200,"text/HTML",pagina(String(!digitalRead(4)?"ON":"OFF")));
  });
  server.begin();
}
unsigned long awaitTime = 0;
void loop() {
  server.handleClient();
  if(digitalRead(23)){
    if (millis() - awaitTime > 1000){
      digitalWrite(4,!digitalRead(4));
      awaitTime = millis();
    }
  }
  delay(2);//allow the cpu to switch to other tasks
}
