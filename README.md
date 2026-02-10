### Deprecated

I no longer use this.

ESPHome now natively support [sx1278](https://esphome.io/components/sx127x/), and I extracted the decoder for the device I care about as a custom component instead. 

----

A demo of how the [rtl_433_ESP](https://github.com/NorthernMan54/rtl_433_ESP) library can be used with ESPHome.

I'm using the 433 Mhz version of the [LoRa32 V2.1_1.6](https://www.lilygo.cc/products/lora3).

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
