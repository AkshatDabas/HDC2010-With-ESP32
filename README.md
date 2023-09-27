| Supported Targets | ESP32 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- |

## Overview

This program demonstrates the usage of I2C driver to display real-time temperature and humidity from HDC2010 sensor from Texas Instruments:

If you have a new I2C application to go (for example, read the temperature data from external sensor with I2C interface), try this as a basic template, then add your own code.

### Hardware Required

To run this example, you should have one ESP32, ESP32-S, ESP32-C or ESP32-H based development board as well as a MPU9250. MPU9250 is a inertial measurement unit, which contains a accelerometer, gyroscope as well as a magnetometer, for more information about it, you can read the [datasheet of the HDC2010 sensor](https://www.ti.com/lit/gpn/hdc2010).

#### Pin Assignment:

**Note:** The following pin assignments are used by default, you can change these in the `menuconfig` .

|                  | SDA             | SCL           |
| ---------------- | -------------- | -------------- |
| ESP I2C Master   | I2C_MASTER_SDA | I2C_MASTER_SCL |
| HDC2010 Sensor   | 21             | 22             |




**Note:** There's no need to add an external pull-up resistors for SDA/SCL pin, because the driver will enable the internal pull-up resistors.

### Build and Flash

Enter `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.



## Troubleshooting

(For any technical queries, please open an [issue](https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you as soon as possible.)
