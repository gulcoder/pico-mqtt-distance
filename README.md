# 📡 Pico W + MQTT + Ultraljudsprojekt

Detta projekt skickar avståndsmätningar från en Raspberry Pi Pico W till en Raspberry Pi Zero W via MQTT. Zero W agerar som MQTT-broker och visar inkommande avståndsvärden i realtid.

---

## ⚙️ Översikt

**📤 Pico W:**
- Ansluter till WiFi.
- Mäter avstånd med HC-SR04.
- Publicerar värden till MQTT-topic `sensor/distance`.

**📥 Zero W:**
- Kör MQTT-broker (Mosquitto).
- Subscribar till `sensor/distance`.
- Visar inkommande värden i terminalen.

---

## 🛠️ Kom igång

### 1. Krav

- Raspberry Pi Pico W
- Raspberry Pi Zero W med Raspberry Pi OS
- HC-SR04 ultraljudssensor
- LCD 16x2 (via I2C)
- Buzzer, LED

---

### 2. MQTT-broker på Zero W

Installera Mosquitto:

```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients
```

Redigera konfigfilen

```bash
sudo nano /etc/mosquitto/mosquitto.conf
```

Lägg till följande rader

```bash
listener 1883
bind_address 0.0.0.0
allow_anonymous true
```

Starta mqtt:

```bash
sudo systemctl restart mosquitto
sudo systemctl enable mosquitto
```

### 3. Starta MQTT-lyssnare på Zero W

```bash
mosquitto_sub -h localhost -t sensor/distance
```

### 4. Dataflöde

```bash
[Ultraljudssensor]
      ↓
   [Pico W]
      ↓  (via MQTT)
[MQTT broker @ Zero W]
      ↓
  [Terminal som visar avståndet]
```

### 5. Exempeloutput från Zero W - terminal

```bash
34.27
34.03
33.89
33.92
34.11
```

