
/*
Conexiones:
  
  FACTORY RESET
    INTERRUPCION -> GPIO14

  Sensor de Corriente ACS712 -> A0

*/

//LIBRERIAS
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


//OBJETOS
WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;

//VARIABLES
char mqtt_server[40] = "sambrana.com.ar";
char mqtt_port[6] = "1883";
char suscribe_topic[34] = "corriente_down";
char publish_topic[34] = "corriente_up";
long loopTime = 2000;
bool shouldSaveConfig = false;
long lastMsg = 0;
String json;
bool reset = 0;
bool record = false;

void setup() {
  pinMode(14, INPUT);     //GPIO14
  attachInterrupt(digitalPinToInterrupt(14),factoryReset,RISING);
  Serial.begin(115200);
  setup_wifiManager();
  setup_OTA();  
  setup_wifi();
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(callback);
}
void loop() {
  if (reset)
  {
    reset = false;
    WiFi.disconnect();
    delay(5000);
    ESP.reset();
    delay(2000);
  }
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if ((now - lastMsg > loopTime) && (record) {
    lastMsg = now; 
    Serial.print("Publish message: ");
    Serial.println(json_sensores());
    
    char msg[200];
    json_sensores().toCharArray(msg, 200);
    //Publicamos a MQTT server
    client.publish(publish_topic, msg);
  }
}

/*
TODO
  
  · loopTime persistente. Guardar valor en el File System
  · Agregar usuario y contraseña a MQTT
  · Cambiar en la web ESP los campos Publish y Suscribe Topic por User y Pass MQTT (Oculto)

*/
