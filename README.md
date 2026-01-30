# Google Balls (ESP32 Version)

A hardware port of the 2010 interactive doodle. Running natively on an ESP32 and  MPU6050 accelerometer to simulate mouse interaction.

> [!IMPORTANT]
> This project is not affiliated with Alphabet Inc. or Google LLC in any shape or form.

> [!NOTE]
> Check out the main repo
> [https://github.com/weeniemount/googleballs-app](https://github.com/weeniemount/googleballs-app)

---

## Hardware

* **ESP32** (DevKit V1 or similar)
* **SSD1306 OLED Display** (128x64, I2C)
* **MPU6050** (3-axis Accelerometer/Gyro)
* Jumper wires

### Wiring

Both sensors share the same I2C pins:

| Component | ESP32 GPIO |
| --- | --- |
| **VCC** | 3.3V |
| **GND** | GND |
| **SCL** | GPIO 22 |
| **SDA** | GPIO 21 |

---

## Libraries

* `Adafruit SSD1306`
* `Adafruit GFX Library`
* `Adafruit MPU6050`
* `Adafruit Unified Sensor`

---

## License

This project is licensed under the **MIT License**.
