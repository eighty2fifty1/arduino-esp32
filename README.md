# Arduino core for ESP32 WiFi chip

The following changes have been made from [Original Version](https://github.com/espressif/arduino-esp32), focusing on BLEClient.

- Simultaneous connection to multiple devices
- Consider reconnection after disconnection
- Bug fixes

The following is an example of automatic reconnection after multiple simultaneous connection and disconnection

[libraries/BLE/examples/BLE_clients_catch_notifies](https://github.com/wakwak-koba/arduino-esp32/tree/master/libraries/BLE/examples/BLE_clients_catch_notifies)
