// ESP8266 sketch sending data from sensor to MQTT
// sensor: Multiple DS18B20 temperature sensors on one 1-wire bus
// wifi connection, sending to OpenHAB
// TEST / read sensor only

// ---- 1-wire config --------------------------------------------------
// http://arduino-info.wikispaces.com/Brick-Temperature-DS18B20
// connection - connect power and create the one-wire bus (with pull-up resistor 4K7)
// if you have sealed sensor, colors are: black GND, red 3,3V, yellow 1-wire bus
#define ONE_WIRE_BUS 2
// Read your sensors adresses by running appropriate program and ether it here




