#include "cloud_light.h"
#include "yellow_addressable_light_effect.h"
#include "cyan_addressable_light_effect.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cloud_light {

static const char *const TAG = "cloud_light";
static const int PORT = 5568;

CloudLightComponent::CloudLightComponent()
    : strip(PixelCount, PixelPin) {}

void CloudLightComponent::setup() {
}

void CloudLightComponent::loop() { }

void CloudLightComponent::add_effect(YellowAddressableLightEffect *yellow_light_effect) {
  if (yellow_light_effects_.count(yellow_light_effect)) {
    return;
  }

  yellow_light_effects_.insert(yellow_light_effect);
}

void CloudLightComponent::add_effect(CyanAddressableLightEffect *cyan_light_effect) {
  if (cyan_light_effects_.count(cyan_light_effect)) {
    return;
  }

  cyan_light_effects_.insert(cyan_light_effect);
}

void CloudLightComponent::remove_effect(YellowAddressableLightEffect *yellow_light_effect) {
  if (!yellow_light_effects_.count(yellow_light_effect)) {
    return;
  }

  yellow_light_effects_.erase(yellow_light_effect);
}

void CloudLightComponent::remove_effect(CyanAddressableLightEffect *cyan_light_effect) {
  if (!cyan_light_effects_.count(cyan_light_effect)) {
    return;
  }

  cyan_light_effects_.erase(cyan_light_effect);
}

}  // namespace cloud_light
}  // namespace esphome
