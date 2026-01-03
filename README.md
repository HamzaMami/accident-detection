# 🚗 Système de Détection d'Accident Intelligent (ESP32 & Firebase)

Ce projet consiste en un système embarqué basé sur l'**ESP32** capable de détecter automatiquement les accidents de la route grâce à des capteurs de force G et de collision physique. Les données sont transmises en temps réel à une base de données **Firebase** et visualisées via une application **Flutter**.

---

## 📑 Sommaire

- [🛠 Composants](#-composants)
- [🔌 Câblage (Pinout)](#-câblage-pinout)
- [🧠 Logique de Détection](#-logique-de-détection)
- [💻 Installation et Configuration](#-installation-et-configuration)
  - [1. Prérequis](#1-prérequis)
  - [2. Fichier de Configuration (`config.h`)](#2-fichier-de-configuration-configh)
  - [3. Structure Firebase Realtime Database](#3-structure-firebase-realtime-database)
- [📱 Application Mobile (Flutter)](#-application-mobile-flutter)
- [📝 Note](#-note)
- [📄 Licence](#-licence)

---

## 🛠 Composants

| Composant             | Description                                  |
|-----------------------|----------------------------------------------|
| **Microcontrôleur**   | ESP32 (DevKit V1)                             |
| **Capteur Inertiel**  | MPU6050 (Accéléromètre + Gyroscope)          |
| **Capteur Physique**  | Capteur de fin de course (Limit Switch)      |
| **Indicateurs**       | Buzzer actif et LED 5mm                      |

---

## 🔌 Câblage (Pinout)

| Composant        | Broche ESP32 | Type               |
|------------------|--------------|--------------------|
| MPU6050 SDA      | GPIO 21      | I2C                |
| MPU6050 SCL      | GPIO 22      | I2C                |
| Limit Switch     | GPIO 14      | Entrée (Pull-up)   |
| LED Alerte       | GPIO 23      | Sortie             |
| Buzzer           | GPIO 18      | Sortie             |

---

## 🧠 Logique de Détection

Le système utilise une **double vérification** pour minimiser les erreurs :

### 1. Force G

Calcul de la magnitude vectorielle. Si **G > 3.5**, un impact est enregistré.

```math
G_{total} = \sqrt{g_x^2 + g_y^2 + g_z^2}
```

### 2. Contact Physique

Si le pare-choc (équipé du limit switch) est enfoncé.

---

## 💻 Installation et Configuration

### 1. Prérequis

Installer les bibliothèques suivantes dans l'IDE Arduino :

| Bibliothèque         | Auteur        |
|----------------------|---------------|
| `Firebase ESP Client`| Mobizt        |
| `MPU6050`            | Jeff Rowberg  |

---

### 2. Fichier de Configuration (`config.h`)

Créez un fichier appelé `config.h` à côté de votre script principal avec le contenu suivant :

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "VOTRE_WIFI"
#define WIFI_PASSWORD "VOTRE_MOT_DE_PASSE"
#define API_KEY "VOTRE_CLE_API_FIREBASE"
#define DATABASE_URL "https://votre-projet.firebasedatabase.app/"

#endif
```

---

### 3. Structure Firebase Realtime Database

L’organisation des données permet un suivi historique clair :

```json
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
```

---

## 📱 Application Mobile (Flutter)

L'application mobile Flutter permet les fonctionnalités suivantes :

| Fonctionnalité           | Description                                              |
|--------------------------|----------------------------------------------------------|
| 🔔 Notifications Push    | Réception d’une alerte instantanée en cas d’accident     |
| ❌ Annulation d’alerte   | Bouton "False Alarm" pour invalider une fausse détection |
| 🗺️ Géolocalisation       | Affichage de la position du véhicule sur Google Maps     |

---

## 📝 Note

> Ce projet a été développé dans le cadre d'un système de sécurité routière intelligent visant à améliorer la réactivité en cas d'accident.

---

## 📄 Licence

Ce projet est libre et peut être utilisé à des fins **éducatives et expérimentales**. Pour un usage commercial, merci de contacter l’auteur.

---
