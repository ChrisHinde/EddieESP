// Update these with values suitable for your network.

#ifndef _CONF_H_
#define _CONF_H_

#define ssid "HeartOfGold"
#define password "F0rthyTw0"
#define mqtt_server "192.168.1.65"

#define TOPIC_PREFIX String("eddie/")
#define INFO_TOPIC "eddie/info/#"
#define ALIVE_TOPIC (String("eddie/info/devices/eddie_node_") + String(ESP.getChipId()) + "/status").c_str()

#define TEMP_TOP "eddie/states/room/temperature"
#define TEMP_INT 60000
#define TEMP_CAL -1
#define HUMI_TOP "eddie/states/room/humidity"
#define HUMI_INT 60000
#define HUMI_CAL 0
#define LAMP_TOP "eddie/states/room/desk_lamp"
#define LAMP_INT 60000
#define LAMP_DEF 0
#define LAMP_DEF_CMD OFF
#define LAMP_SPEED 10
#define RGB_TOP "eddie/states/room/desk_rgb"
#define RGB_INT 50
#define RGB_DIM_DEF 0
#define RGB_DEF Color((RgbColor){ 0, 0, 0 })
#define RGB_HSV_DEF { 0, 0, 0 }
#define RGB_SPEED 10

#define RGB_R_PIN 1
#define RGB_G_PIN 2
#define RGB_B_PIN 3
//#define RGB_W_PIN 4 // Uncomment this if your RGB strip/lamp/output has a white component

#define RGB_MAX 1023
#define PWM_MAX 1023

#endif
