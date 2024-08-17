# Aquasonic

Aquaponic System Monitoring

## Description

This has been designed to monitor several parameters of my home aquaponic system:
- 2 water pumps flow
- air and water temperature
- water level
- 2 air pump pressure

All sensors are plugged to an ESP32 board with Esphome. 
It transmits data to HomeAssistant.
HomeAssistant then trigger alarm when values are out of bounds.

## Sensors Table & Connection

| ESP32 Pin      | Sensor Pin | Sensor Type   | Sensor Name    |
|----------------|------------|---------------|----------------|
| GND            | GND        | Tf Mini Lidar | Water Level    |
| 5V             | VCC        |               |                |
| GPIO16 UART RX | Data Tx    |               |                |
| GND            | GND        | DS18B20       | Water Temp     |
| 3V3            | VCC        |               |                |
| GPIO4          | Data       |               |                |
| GND            | GND        | DS18B20       | Air Temp       |
| 3V3            | VCC        |               |                |
| GPIO4          | Data       |               |                |
| GND            | GND        | Tachometer    | Water Flow 1   |
| 5V             | VCC        |               |                |
| GPIO12         | Data       |               |                |
| GND            | GND        | Tachometer    | Water Flow 2   |
| 5V             | VCC        |               |                |
| GPIO14         | Data       |               |                |
| GND            | GND        | Pressure      | Air Pressure 1 |
| 3V3            | VCC        |               |                |
| GPIO32         | Data       |               |                |
| GND            | GND        | Tachometer    | Air Pressure 2 |
| 3V3            | VCC        |               |                |
| GPIO33         | Data       |               |                |

NOTE: The 2 temperature sensors share the same pins, you need to adapt the sensor address in the configuration YAML.

NOTE2: You need a pull-up resistor on temperature sensor data pin. 4.7kOhm works fine. Internal pull-up is too high (around 50kOhm). So put a resistor between 3.3V and data pin (4).

NOTE3: Some pins can be changed in the YAML configuration, some other are dictated by hardware.

## Build and Run

You need esphome installed locally, or in a Python virtual environment or in a docker image. 
Refer to Esphome documentation. The commands below are given as a reminder only.

    source .env/bin/activate
    esphome run aquasonic.yaml

## Links 

Here are some links about the sensors, not commercial but more documentation.

[Seeed Studio Tf Mini Lidar](https://www.seeedstudio.com/Seeedstudio-Grove-TF-Mini-LiDAR.html?srsltid=AfmBOorn6_GhEOgjtANmWUS2KbA4qk7PrmvXOvGeQQty0rA1bB3meeuv)

[Dallas DS18B20 1 Wire Temperature Sensor](https://randomnerdtutorials.com/guide-for-ds18b20-temperature-sensor-with-arduino/)

[Water Flow Sensor](https://bc-robotics.com/tutorials/using-a-flow-sensor-with-arduino/)

[Air Pressur Sensor](https://images.app.goo.gl/MEBP54qZBxndh18Y7)