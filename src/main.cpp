#include <Arduino.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>
#include <config.h>

// Topics MQTT
const char *led_topic = "home/led";
const char *led_state_topic = "home/led/state";

// Pin de la LED et du bouton poussoir
const int ledPin = 27;
const int buttonPin = 4;

WiFiMulti wifiMulti;
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connexion au Wi-Fi...");
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.println("Adresse IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message reçu [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Allumer ou éteindre la LED en fonction du message reçu
  if ((char)payload[0] == '1')
  {
    digitalWrite(ledPin, HIGH);
    client.publish(led_state_topic, "1", true);
  }
  else
  {
    digitalWrite(ledPin, LOW);
    client.publish(led_state_topic, "0", true);
  }
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Tentative de connexion MQTT...");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("connecté");
      client.subscribe(led_topic);
    }
    else
    {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      Serial.println(" essayer à nouveau dans 2 secondes");
      delay(2000);
    }
  }
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  client.setKeepAlive(15); // Activer keepAlive avec un intervalle de 15 secondes
}

void loop()
{
  if (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.println("Déconnecté du Wi-Fi. Reconnexion...");
    setup_wifi();
  }

  if (!client.connected())
  {
    Serial.println("Déconnecté du serveur MQTT. Reconnexion...");
    reconnect();
  }
  client.loop();

  // Vérifier l'état du bouton poussoir
  if (digitalRead(buttonPin) == LOW)
  {
    delay(50); // Debounce
    if (digitalRead(buttonPin) == LOW)
    {
      // Inverser l'état de la LED
      int ledState = digitalRead(ledPin);
      digitalWrite(ledPin, !ledState);
      client.publish(led_state_topic, ledState ? "0" : "1", true);
      Serial.println("Bouton pressé, état de la LED inversé");
      while (digitalRead(buttonPin) == LOW)
        ; // Attendre que le bouton soit relâché
    }
  }
}