#include "cyan_addressable_light_effect.h"
#include "cloud_light.h"
#include "esphome/core/log.h"

#include <NeoPixelBus.h>

namespace esphome {
namespace cloud_light {

static const char *const TAG = "cyan_addressable_light_effect";

CyanAddressableLightEffect::CyanAddressableLightEffect(const std::string &name) : AddressableLightEffect(name) {}

void CyanAddressableLightEffect::start() {
  AddressableLightEffect::start();

  ESP_LOGD(TAG, "Start cyan effect");
  this->cyan_->strip.Begin();
  this->cyan_->strip.Show();
  CyanAddressableLightEffect::SetStripColor(RgbColor(0, 15, 15));

  if (this->cyan_) {
    this->cyan_->add_effect(this);
  }
}

void CyanAddressableLightEffect::stop() {
  if (this->cyan_) {
    this->cyan_->remove_effect(this);
  }
  ESP_LOGD(TAG, "Stop cyan effect");
  this->cyan_->strip.ClearTo(RgbColor(0, 0, 0));
  this->cyan_->strip.Show();

  AddressableLightEffect::stop();
}

void CyanAddressableLightEffect::apply(light::AddressableLight &it, const Color &current_color) {
  // ignore, it is run by `CloudLightComponent::update()`
}

void CyanAddressableLightEffect::SetStripColor(RgbColor color) {
  for (uint16_t i = 0; i < this->cyan_->PixelCount; i++) {
    this->cyan_->strip.SetPixelColor(i, color);
  }
  this->cyan_->strip.Show();
}

}  // namespace cloud_light
}  // namespace esphome
