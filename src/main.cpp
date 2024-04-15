#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <SimpleDHT.h>
#include <SimpleTimer.h>

#include "MAIN_PAGE.h"


const char* ssid = "#ToCoToCo";
const char* pw = "1900636936";

SimpleTimer timer;
ESP8266WebServer server(80);
WebSocketsServer ws = WebSocketsServer(81);

SimpleDHT11 dht11(D0);

volatile bool led_state = false;
volatile uint8_t temp;
volatile uint8_t humi;
bool toggle_led();
void readDHT();
void sendDHTToClient();

void wsHandler(uint8_t, WStype_t, uint8_t*, size_t);

void setup() {
  // Mo Serial va cho doi ket noi
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  // Ket noi den WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  // Request handler
  server.on("/", HTTP_ANY, []() {
    server.send(200, "text/html", MAIN_PAGE);
  });

  server.on("/toggle", HTTP_GET, []() {
    server.send(200, "text/plain", String(toggle_led()));
  });

  server.on("/getstatus", HTTP_GET, []() {
    server.send(200, "text/html", String(led_state));
  });

  server.onNotFound([]() {
    server.send(404, "text/html", "<h1>Not found</h1>");
  });

  server.begin();
  ws.begin();
  ws.onEvent(wsHandler);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, led_state);

  timer.setInterval(1000L, readDHT);
  timer.setInterval(1000L, sendDHTToClient);
}



void wsHandler(uint8_t client_id, WStype_t type, uint8_t* payload, size_t length)
{
  switch (type)
  {
  
  // Thiết bị mới kết nối
  case WStype_CONNECTED:
    Serial.printf("[%u] Connected! - IP: %s\n", client_id, ws.remoteIP(client_id).toString().c_str());
    break;
  
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", client_id);
    break;
  
  case WStype_TEXT:
    String plstr = String((char*)payload);
    if (plstr.startsWith("#TOGGLE"))
    {
      toggle_led();
      ws.sendTXT(client_id, String(led_state).c_str());
      // ws.broadcastTXT // gửi tới toàn bộ client
    }
  }
}


void loop() {
  server.handleClient();
  ws.loop();
  timer.run();
}

bool toggle_led() {
  led_state = !led_state;
  digitalWrite(LED_BUILTIN, led_state);
  if (ws.connectedClients() > 0) {
    char payload[20];
    sprintf(payload, "LED_STATE:%d", led_state);
    ws.broadcastTXT(payload);
  }
  return led_state;
}

void readDHT() {
  uint8_t t;
  uint8_t h;
  if (dht11.read(&t, &h, NULL) == SimpleDHTErrSuccess) {
    temp = t;
    humi = h;
  }
}

void sendDHTToClient() {
  if (ws.connectedClients() == 0) return;
  char payload[20];
  sprintf(payload, "READ_DHT:%d-%d", temp, humi);
  ws.broadcastTXT(payload);
}