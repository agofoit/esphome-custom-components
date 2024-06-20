#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light_effect.h"

namespace esphome {
namespace cloud_light {

class CloudLightComponent : public esphome::Component { };

inline static int16_t sin16_c(uint16_t theta) {
  static const uint16_t BASE[] = {0, 6393, 12539, 18204, 23170, 27245, 30273, 32137};
  static const uint8_t SLOPE[] = {49, 48, 44, 38, 31, 23, 14, 4};
  uint16_t offset = (theta & 0x3FFF) >> 3;  // 0..2047
  if (theta & 0x4000)
    offset = 2047 - offset;
  uint8_t section = offset / 256;  // 0..7
  uint16_t b = BASE[section];
  uint8_t m = SLOPE[section];
  uint8_t secoffset8 = uint8_t(offset) / 2;
  uint16_t mx = m * secoffset8;
  int16_t y = mx + b;
  if (theta & 0x8000)
    return -y;
  return y;
}
inline static uint8_t half_sin8(uint8_t v) { return sin16_c(uint16_t(v) * 128u) >> 8; }

class YellowAddressableLightEffect : public light::AddressableLightEffect {
 public:
  explicit YellowAddressableLightEffect(const std::string &name) : light::AddressableLightEffect(name) {}
  void apply(light::AddressableLight &addressable, const Color &current_color) override {
    const Color yellow_color(150, 150, 0);

    const uint32_t now = millis();
    uint8_t pos_add = 0;
    if (now - this->last_progress_ > this->progress_interval_) {
      const uint32_t pos_add32 = (now - this->last_progress_) / this->progress_interval_;
      pos_add = pos_add32;
      this->last_progress_ += pos_add32 * this->progress_interval_;
    }
    for (auto view : addressable) {
      if (view.get_effect_data() != 0) {
        const uint8_t sine = half_sin8(view.get_effect_data());
        view = yellow_color * sine;
        const uint8_t new_pos = view.get_effect_data() + pos_add;
        if (new_pos < view.get_effect_data())
          view.set_effect_data(0);
        else
          view.set_effect_data(new_pos);
      } else {
        view = Color::BLACK;
      }
    }
    while (random_float() < this->twinkle_probability_) {
      const size_t pos = random_uint32() % addressable.size();
      if (addressable[pos].get_effect_data() != 0)
        continue;
      addressable[pos].set_effect_data(1);
    }
    addressable.schedule_show();
  }
  void set_twinkle_probability(float twinkle_probability) { this->twinkle_probability_ = twinkle_probability; }
  void set_progress_interval(uint32_t progress_interval) { this->progress_interval_ = progress_interval; }

 protected:
  float twinkle_probability_{0.05f};
  uint32_t progress_interval_{4};
  uint32_t last_progress_{0};
};

class CyanAddressableLightEffect : public light::AddressableLightEffect {
 public:
  explicit CyanAddressableLightEffect(const std::string &name) : light::AddressableLightEffect(name) {}
  void apply(light::AddressableLight &addressable, const Color &current_color) override {
    const Color cyan_color(0, 150, 150);

    const uint32_t now = millis();
    uint8_t pos_add = 0;
    if (now - this->last_progress_ > this->progress_interval_) {
      const uint32_t pos_add32 = (now - this->last_progress_) / this->progress_interval_;
      pos_add = pos_add32;
      this->last_progress_ += pos_add32 * this->progress_interval_;
    }
    for (auto view : addressable) {
      if (view.get_effect_data() != 0) {
        const uint8_t sine = half_sin8(view.get_effect_data());
        view = cyan_color * sine;
        const uint8_t new_pos = view.get_effect_data() + pos_add;
        if (new_pos < view.get_effect_data())
          view.set_effect_data(0);
        else
          view.set_effect_data(new_pos);
      } else {
        view = Color::BLACK;
      }
    }
    while (random_float() < this->twinkle_probability_) {
      const size_t pos = random_uint32() % addressable.size();
      if (addressable[pos].get_effect_data() != 0)
        continue;
      addressable[pos].set_effect_data(1);
    }
    addressable.schedule_show();
  }
  void set_twinkle_probability(float twinkle_probability) { this->twinkle_probability_ = twinkle_probability; }
  void set_progress_interval(uint32_t progress_interval) { this->progress_interval_ = progress_interval; }

 protected:
  float twinkle_probability_{0.05f};
  uint32_t progress_interval_{4};
  uint32_t last_progress_{0};
};

}  // namespace cloud_light
}  // namespace esphome
