#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light_effect.h"

#include <NeoPixelBus.h>

namespace esphome {
namespace cloud_light {

class CloudLightComponent;

enum CyanLightChannels { Cyan_MONO = 1, Cyan_RGB = 3, Cyan_RGBW = 4 };

class CyanAddressableLightEffect : public light::AddressableLightEffect {
 public:
  CyanAddressableLightEffect(const std::string &name);

  void start() override;
  void stop() override;
  void apply(light::AddressableLight &it, const Color &current_color) override;

  void set_cyan(CloudLightComponent *cyan) { this->cyan_ = cyan; }

  void SetStripColor(RgbColor color);

 protected:
  CyanLightChannels channels_{Cyan_RGB};
  CloudLightComponent *cyan_{nullptr};

  friend class CloudLightComponent;
};

}  // namespace cloud_light
}  // namespace esphome
