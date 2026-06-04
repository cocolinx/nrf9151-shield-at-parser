# CocoLinx nRF9151-Shield AT command parser Library

## Description

A C++ based AT command parser library for CocoLinx nRF9151-Shield.

## Features

- Network attach / detach control
- UDP / TCP / MQTT / secure UDP / secure TCP / secure MQTT / RS485 data transmission
- GNSS positioning support

## Installation

1. Download ZIP file
2. Open Arduino IDE and go to **Sketch -> Include Library -> Add .ZIP Library**
3. Add .ZIP file
4. Click **Sketch -> Include Library -> CocoLinx nRF9151_Shield AT Parser Library**

## Examples

 - FullTest_Example: An all-in-one integration example that exercises most library features, including UDP, TCP, MQTT, and RS-485, for quick sanity and setup verification.
 - GNSS_Example: Read GNSS data
 - MQTT_Example: Connects to LTE, connects to an MQTT broker, subscribes to a topic, and publishes a message.
 - MQTT_TLS_Example: Connects to LTE, connects to an MQTT broker over TLS, subscribes to a topic, and publishes a message.
 - UDP_Example: Connect to LTE, open a UDP socket, and sends/receives data with an echo server.


