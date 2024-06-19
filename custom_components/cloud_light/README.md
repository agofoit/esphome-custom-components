```yaml
cloud_light:

light:
  - platform: neopixelbus # custom_neo # neopixelbus
    name: "Cloud Lamp"
    type: GRB
    variant: WS2812X
    pin: GPIO27
    num_leds: 60
    color_correct: [80%, 80%, 80%]
    effects:
      - yellow:
      - cyan:
```