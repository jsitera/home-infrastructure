// ESP8266 sketch receiving data from MQTT and driving RGB LED (strip)
// wifi connection, receiving from OpenHAB

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "FastLED.h"



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

// =============== configuration of ws2812 LEDs ==============================
#define NUM_LEDS 24
#define DATA_PIN 3
// ===================== config end ==========================================

WiFiClient espClient; // wifi client
PubSubClient client(espClient); // MQTT client
CRGB leds[NUM_LEDS]; // array of leds

void setup() {
  Serial.begin(115200);

  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(1000);

  //FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );

  for(int i = 0; i < NUM_LEDS; i = i + 1) {
    leds[i] = CRGB::Black;
  }
  
  FastLED.show();

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

// callback called when MQTT message arrives / see setCallback()
void callback(char* topic, byte* payload, unsigned int length) {

  payload[length] = '\0';
  String s_payload = String((char *)payload);
  String s_topic = String((char *)topic);

  Serial.print("MQTT message arrived, topic:");
  Serial.print(topic);
  Serial.print(" => ");
  Serial.println(s_payload);
 

  if (s_topic == RGB_topic){
    //topic from OpenHAB is in a form of r;g;b where the individual color 
    //values are 0-100
    //output is inverse - RGB LED example
    int c1 = s_payload.indexOf(';');
    int c2 = s_payload.indexOf(';',c1+1);
    int red = s_payload.toInt();
    int green = s_payload.substring(c1+1,c2).toInt();
    int blue = s_payload.substring(c2+1).toInt();  

    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i].setRGB(red, green, blue);
    }
 
    FastLED.show();

    Serial.print("RGB values written to LEDs: ");
    Serial.print(red);
    Serial.print(";");
    Serial.print(green);
    Serial.print(";");
    Serial.println(blue);
    Serial.flush();
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
