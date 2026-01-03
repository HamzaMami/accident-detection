# Système de Détection d'Accident Intelligent (ESP32 & Firebase)

Ce projet consiste en un système embarqué basé sur l'**ESP32** capable de détecter automatiquement les accidents de la route grâce à des capteurs de force G et de collision physique. Les données sont transmises en temps réel à une base de données **Firebase** et visualisées via une application **Flutter**.

## 📑 Sommaire
- [Composants](#composants)
- [Architecture logicielle](#architecture-logicielle)
- [Configuration](#configuration)
- [Logique de Détection](#logique-de-détection)
- [Aperçu de la base de données](#aperçu-de-la-base-de-données)

## 🛠 Composants
* **Microcontrôleur :** ESP32 (DevKit V1)
* **Capteur Inertiel :** MPU6050 (Accéléromètre + Gyroscope)
* **Capteur Physique :** Capteur de fin de course (Limit Switch)
* **Indicateurs :** Buzzer actif et LED 5mm

## 🔌 Câblage (Pinout)
| Composant | Broche ESP32 | Type |
| :--- | :--- | :--- |
| MPU6050 SDA | GPIO 21 | I2C |
| MPU6050 SCL | GPIO 22 | I2C |
| Limit Switch | GPIO 14 | Entrée (Pull-up) |
| LED Alerte | GPIO 23 | Sortie |
| Buzzer | GPIO 18 | Sortie |



## 🧠 Logique de Détection

Le système utilise une double vérification pour minimiser les erreurs :
1. **Force G :** Calcul de la magnitude vectorielle. Si $G > 3.5$, un impact est enregistré.
   $$G_{total} = \sqrt{g_x^2 + g_y^2 + g_z^2}$$
2. **Contact Physique :** Si le pare-choc (équipé du limit switch) est enfoncé.



## 💻 Installation et Configuration

### 1. Prérequis
Installer les bibliothèques suivantes dans l'IDE Arduino :
- `Firebase ESP Client`
- `MPU6050` (Jeff Rowberg)

### 2. Fichier de Configuration (`config.h`)
Créez un fichier `config.h` à côté de votre script principal pour vos identifiants :

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "VOTRE_WIFI"
#define WIFI_PASSWORD "VOTRE_MOT_DE_PASSE"
#define API_KEY "VOTRE_CLE_API_FIREBASE"
#define DATABASE_URL "[https://votre-projet.firebasedatabase.app/](https://votre-projet.firebasedatabase.app/)"

#endif

###  3.Structure Firebase Realtime Database
Le système organise les données de la manière suivante pour permettre un suivi historique :

{
  "Cars": {
    "car_id_001": {
      "current_status": {
        "collision": true,
        "gforce": 4.12,
        "false_alarm": false
      },
      "history": {
        "-Nxyz123": {
          "date": "2026-01-03 10:00",
          "GForce": 4.12
        }
      }
    }
  }
}

### 4. Application Mobile (Flutter)
L'application permet :

La réception de notifications push lors d'un accident.

L'annulation d'une alerte via le bouton "False Alarm".

La géolocalisation précise du véhicule accidenté sur Google Maps.

Note : Ce projet a été développé dans le cadre d'un système de sécurité routière intelligent.
