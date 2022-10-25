# ESP32 LED Light Controller via Wifi and Alexa

**This library is based on the ESP-IDF library**

It only requires one extra component for controlling the light bitbang intervals: [led_strip](https://components.espressif.com/components/espressif/led_strip)

---

In order to use:
1. Set your target to ESP32 connected via USB
`idf.py set-target esp32`
2. Build project with
`idf.py build`
3. Flash project to ESP32
`idf.py flash`
