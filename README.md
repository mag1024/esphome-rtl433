A demo of how the [rtl_433_ESP](https://github.com/NorthernMan54/rtl_433_ESP)
library can be used with ESPHome.

I'm using the 433 Mhz version of the [LoRa32 V2.1_1.6](https://www.lilygo.cc/products/lora3).

Usage:

```
substitutions:
  name: "rfgw"
  NAME: "RF Gateway"

packages:
  rfgw: !include 433/rfgw.yaml

mqtt:
  ...
```

Or, if you want to use ESP-IDF framework instead of Arduino ([strongly recommended](https://esphome.io/components/bluetooth_proxy.html) for Bluetooth) you can use `rfgw-espidf.yaml` instead.
