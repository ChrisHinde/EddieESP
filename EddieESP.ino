/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "EddieCtrl.h"

// Update these with values suitable for your network.

#define ssid "HeartOfGold"
#define password "F0rthyTw0"
const char* mqtt_server = "192.168.1.65";

#define TOPIC_PREFIX String("eddie/")
#define INFO_TOPIC "eddie/info/#"
#define ALIVE_TOPIC (String("eddie/info/devices/eddie_node_") + String(ESP.getChipId()) + "/status").c_str()

#define TEMP_TOP "eddie/states/room/temperature"
#define TEMP_INT 60000
#define TEMP_CAL -1
#define HUMI_TOP "eddie/states/room/humidity"
#define HUMI_INT 60000
#define HUMI_CAL 0
#define LAMP_TOP "eddie_states/room/bed_lamp"
#define LAMP_INT 60000
#define LAMP_DEF "off"
#define RGB_TOP "eddie/states/room/rgb"
#define RGB_INT 50
#define RGB_DEF "#db78f3"

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

  String top = String(topic);

  if ( top.startsWith( LAMP_TOP ) ) {
    
  } else if ( top.startsWith( RGB_TOP ) ) {
    
  }

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
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
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  ctrl = EddieCtrl();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
