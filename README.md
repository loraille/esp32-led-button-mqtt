# esp32-led-button-mqtt

## Description
Commande l'allumage d'une led sur un esp 32 avec un bouton ou via un serveur MQTT.
## Pré requis!
Ajouter un ficher config.h avec toutes les infos de connexion ou directement dans le code.
### Fichier `config.h`

```cpp
// config.h
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "votre_SSID"
#define WIFI_PASSWORD "votre_mot_de_passe_WiFi"
#define MQTT_SERVER "votre_serveur_MQTT"
#define MQTT_USER "votre_utilisateur_MQTT"
#define MQTT_PASSWORD "votre_mot_de_passe_MQTT"

#endif // CONFIG_H
