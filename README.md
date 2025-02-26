A demo of how the [rtl_433_ESP](https://github.com/NorthernMan54/rtl_433_ESP) library can be used with ESPHome.

```
external_components:
  - source: github://mag1024/esphome-rtl433

rtl433_mqtt:
  id: gateway
  mqtt_id: my_mqtt

# improves OTA stability
ota:
  on_begin:  
    - rtl433_mqtt.stop: gateway
```

Configuration variables:

- **mqtt_id** (*Required*): id of a configured [MQTT Client](https://esphome.io/components/mqtt.html) component.
- **topic_prefix** (*Optional*): Defaults to `rtl_433`
- **source** (*Optional*): A value for the `source=` field added to the published JSON.
                           Useful if you have multiple boards publishing to the same topic.

I'm using the 433 Mhz version of the [LoRa32 V2.1_1.6](https://www.lilygo.cc/products/lora3), and running it with settings similar to the example provided in `rtl433-mqtt-espidf.yaml`. An untested non-ESP-IDF (Arduino) version is in `rtl433-mqtt.yaml`
