/*
 Based on Basic ESP8266 MQTT example

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "conf.h"

#include "EddieCtrl.h"


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

EddieCtrl ctrl;

void setup_wifi() {

  delay(10);
  Serial.setDebugOutput(true);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.setOutputPower(0);
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);    
  
    Serial.print(WiFi.status());
    //WiFi.printDiag(Serial);

    value = 1 - value;
    if ( value )
      digitalWrite(BUILTIN_LED, HIGH);
    else
      digitalWrite(BUILTIN_LED, LOW);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  payload[length] = 0; // Terminate the string

 // Direct incoming topic to the right "property"
  if ( strcmp( topic, LAMP_TOP ) == 0 ) {
    ctrl.IncomingCommand( LAMP, (char*)payload );
  } else if ( strcmp( topic, RGB_TOP ) == 0 ) {
    ctrl.IncomingCommand( RGB_STRIP, (char*)payload );
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(ALIVE_TOPIC, "ALIVE");
      // ... and resubscribe
      client.subscribe(INFO_TOPIC);
      client.subscribe(LAMP_TOP);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  ctrl = EddieCtrl();
  ctrl.RegisterProperty( new RgbStripProperty() );
  ctrl.RegisterProperty( new LampProperty() );
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  ctrl.Loop();
}
