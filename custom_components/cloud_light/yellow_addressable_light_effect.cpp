#include "yellow_addressable_light_effect.h"
#include "cloud_light.h"
#include "esphome/core/log.h"

#include <NeoPixelBus.h>

namespace esphome {
namespace cloud_light {

static const char *const TAG = "yellow_addressable_light_effect";

YellowAddressableLightEffect::YellowAddressableLightEffect(const std::string &name) : AddressableLightEffect(name) {}

void YellowAddressableLightEffect::start() {
  AddressableLightEffect::start();

  ESP_LOGD(TAG, "Start yellow effect");
  this->yellow_->strip.Begin();
  this->yellow_->strip.Show();
  YellowAddressableLightEffect::SetStripColor(RgbColor(15, 15, 0));

  if (this->yellow_) {
    this->yellow_->add_effect(this);
  }
}

void YellowAddressableLightEffect::stop() {
  if (this->yellow_) {
    this->yellow_->remove_effect(this);
  }
  ESP_LOGD(TAG, "Stop yellow effect");
  this->yellow_->strip.ClearTo(RgbColor(0, 0, 0));
  this->yellow_->strip.Show();

  AddressableLightEffect::stop();
}

void YellowAddressableLightEffect::apply(light::AddressableLight &it, const Color &current_color) {
  // ignore, it is run by `CloudLightComponent::update()`
}

void YellowAddressableLightEffect::SetStripColor(RgbColor color) {
  for (uint16_t i = 0; i < this->yellow_->PixelCount; i++) {
    this->yellow_->strip.SetPixelColor(i, color);
  }
  this->yellow_->strip.Show();
}

}  // namespace cloud_light
}  // namespace esphome
