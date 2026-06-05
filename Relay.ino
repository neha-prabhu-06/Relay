#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Just Breathe";
const char* password = "9845055994";

WebServer server(80);

const int channels[] = {2, 4, 5, 18, 19, 21, 22, 23};
const int numChannels = 8;

bool relayState[numChannels] = {false,false,false,false,false,false,false,false};


void handleRelay() {

  server.sendHeader(
      "Access-Control-Allow-Origin",
      "*"
  );

  int ch = server.arg("ch").toInt();


    if(ch==7){
      digitalWrite(channels[ch], LOW);
      delay(10000);
      digitalWrite(channels[7], HIGH);
      server.send(200,"text/plain","OFF");
    }

    else{
      Serial.print(ch);
      Serial.println(" on");
      digitalWrite(channels[ch], LOW);
      delay(100);
      digitalWrite(channels[7], LOW);
      server.send(200,"text/plain","ON");
      delay(3000);
      digitalWrite(channels[ch], HIGH);
      delay(7000);
      digitalWrite(channels[7], HIGH);
      server.send(200,"text/plain","OFF");
      }
}
void setup() {

  Serial.begin(115200);

  for (int i = 0; i < numChannels; i++) {
    pinMode(channels[i], OUTPUT);
    digitalWrite(channels[i], HIGH);
    
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "ESP32 Web Server Running");
  });

  server.on("/relay", handleRelay);

  server.begin();

  Serial.println("Server Started");
}
void loop() {
  server.handleClient();
  
}