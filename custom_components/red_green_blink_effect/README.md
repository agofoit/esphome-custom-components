# EXAMPLE CONFIG:

Part of my esphome-web.yaml

```yaml
external_components:
  - source:
      type: local
      path: custom_components/red_green_blink_effect

light:
  - platform: neopixelbus
    name: "Effect Lamp"
    type: GRB
    variant: WS2812X
    pin: GPIO27
    num_leds: 60
    color_correct: [80%, 80%, 80%]
    effects:
      - red_green_blink_effect:
```

My directories:

```
/config
  |  esphome-web.yaml
  |  /custom_components/red_green_blink_effect
       |  light_effect.py
       |  red_green_blink_effect.cpp
       |  red_green_blink_effect.h
```

# HELP NEEDED:
**Current error while running `esphome compile esphome-config.yaml`:**

    Unable to find effect with the name 'red_green_blink_effect'.