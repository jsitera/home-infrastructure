// ESP8266 sketch receiving data from MQTT and driving RGB LED (strip)
// wifi connection, receiving from OpenHAB

#include <ESP8266WiFi.h>
#include <PubSubClient.h>



// =============== configuration of Wifi ====================================
#define wifi_ssid "ssid"
#define wifi_password "password"

// configuration f MQTT server ==============================================
#define mqtt_server "mqtt server name/ip address"
// no MQTT security
// #define mqtt_user "your_username"
// #define mqtt_password "your_password"

// passwords and other sensitive information in a separate file not published
#include "passwords.h"

// =============== configuration of MQTT topics / OpenHAB items setup =======
#define RGB_topic "/PI1/RGB/Color"

// =============== configuration of RGB LED ==============================
#define redPIN 12
#define greenPIN 14
#define bluePIN 16

// ===================== config end ==========================================

// https://community.openhab.org/t/openhab-control-of-led-rgb-strips-using-arduino-and-mqtt/4366
// https://github.com/openhab/openhab/wiki/Samples-Rules#how-to-use-colorpicker-widget-with-knxdali-rgb-led-stripe
// https://openhardwarecoza.wordpress.com/2015/10/15/openhab-mqtt-arduino-and-esp8266-part-8-rgbw-led-strips/



WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println("OpenHAB actor started.");

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// callback called when MQTT message arrives
void callback(char* topic, byte* payload, unsigned int length) {

  payload[length] = '\0';
  String s_payload = String((char *)payload);

  Serial.print("MQTT message arrived, topic:");
  Serial.print(topic);
  Serial.print(" => ");
  Serial.println(s_payload);
 

  if(topic == RGB_topic){
    //topic from OpenHAB is in a form of r;g;b where the individual color 
    //values are 0-100
    int c1 = s_payload.indexOf(';');
    int c2 = s_payload.indexOf(';',c1+1);
    int red = map(s_payload.toInt(),0,100,0,255);
    int green = map(s_payload.substring(c1+1,c2).toInt(),0,100,0,255);
    int blue = map(s_payload.substring(c2+1).toInt(),0,100,0,255);  
 
   analogWrite(redPIN, red);
   analogWrite(greenPIN, green);
   analogWrite(bluePIN, blue);

   Serial.print("RGB values written to pins: ");
   Serial.print(red);
   Serial.print(green);
   Serial.println(blue);
 }
 
}




// lastMsg is time when last measurement was done
long lastMsg = millis();

void loop(){
  //auto reconnect MQTT if needed
  if (!client.connected()) {
    reconnect();
    client.subscribe(RGB_topic);
  }
  //call MQTT library internal loop
  client.loop();

  //long now = millis();
  //if (now - lastMsg > 5000) {
  //  // do measurement and send it
  //  lastMsg = now;
  //}
}
