// code for controlling servo motor using nodered dashboard

#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

Servo myservo; // servo onject

// Wifi
const char *ssid = "MAKERINDO2";
const char *password = "makerindo2019";

// MQTT Broker
const char *mqtt_server = "broker.hivemq.com"; // MQTT broker
const char *topic = "/node-red/servo";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char *topic, byte *payload, unsigned int length);
void setup_wifi();

void setup()
{

  Serial.begin(115200);
  myservo.attach(5);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESPClient"))
    {
      Serial.println("connected");
      client.subscribe("/node-red/servo");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String string;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    string += ((char)payload[i]);
  }
  Serial.print(string);
  if (topic = "/node-red/servo") /// esp32 subscribe topic
    Serial.print(" ");
  int status = string.toInt();
  int pos = map(status, 1, 100, 0, 180);
  Serial.println(pos);
  myservo.write(pos);
  delay(15);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  delay(100);
}