#include <WiFi.h>
#include <Wire.h>
#include <Firebase_ESP_Client.h>
#include <MPU6050.h>

// ================== CONFIGURATION ==================
#define WIFI_SSID "IFI_NAME"
#define WIFI_PASSWORD "IFI_PASSWORD"
// ================== FIREBASE SETTINGS ==================
#define API_KEY "AIzaSyAiya_I0uYxGE5nZMWSVA9mgey7FoJuMOE"
#define DATABASE_URL "https://control-system- b259a-default-rtdb.europe-west1.firebasedatabase.app/"

// The path in your database based on the UI images
String carPath = "Cars/car_id_001";     

// ================== PINS & THRESHOLDS ===================
#define LIMIT_SWITCH_PIN 14  
#define LED_PIN 23
#define BUZZER_PIN 18
#define G_THRESHOLD 3.0      // G-Force threshold to trigger collision

MPU6050 mpu;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastUpdate = 0;
const unsigned long interval = 2000; // Update every 2 seconds

void setup() {
  Serial.begin(115200);
  
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin(21, 22);
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection FAILED");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi Connected");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  checkAccident();
}

// ================== CORE LOGIC ==================

void checkAccident() {
  if (millis() - lastUpdate < interval) return;
  lastUpdate = millis();

  // 1. Read Accelerometer and convert to G-Force
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // MPU6050 default range +/- 2g. Sensitivity = 16384 LSB/g
  float gX = ax / 16384.0;
  float gY = ay / 16384.0;
  float gZ = az / 16384.0;

  // Calculate Resultant G-Force: G = sqrt(gx^2 + gy^2 + gz^2)
  float totalG = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));
  
  // 2. Determine Collision Status
  bool limitHit = (digitalRead(LIMIT_SWITCH_PIN) == LOW);
  bool collisionDetected = (totalG > G_THRESHOLD || limitHit);

  // 3. Local Alert (Buzzer/LED)
  if (collisionDetected) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("!!! COLLISION DETECTED !!!");
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // 4. Update Firebase
  if (Firebase.ready()) {
    FirebaseJson update;
    
    // Update current_status node
    update.set("current_status/gforce", totalG);
    update.set("current_status/collision", collisionDetected);
    update.set("current_status/last_updated", "2026-01-03 10:00"); // Ideally use NTP time
    update.set("info/is_connected", true);

    // If collision, push to History as well
    if (collisionDetected) {
      FirebaseJson history;
      history.set("date", "2026-01-03 10:00");
      history.set("GForce", totalG);
      history.set("Collision", "Yes");
      Firebase.RTDB.pushJSON(&fbdo, carPath + "/history", &history);
    }

    if (Firebase.RTDB.updateNode(&fbdo, carPath, &update)) {
      Serial.println("Firebase Sync: OK");
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}
