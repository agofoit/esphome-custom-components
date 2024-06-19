#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light_effect.h"

#include <NeoPixelBus.h>

namespace esphome {
namespace cloud_light {

class CloudLightComponent;

enum YellowLightChannels { Yellow_MONO = 1, Yellow_RGB = 3, Yellow_RGBW = 4 };

class YellowAddressableLightEffect : public light::AddressableLightEffect {
 public:
  YellowAddressableLightEffect(const std::string &name);

  void start() override;
  void stop() override;
  void apply(light::AddressableLight &it, const Color &current_color) override;

  void set_yellow(CloudLightComponent *yellow) { this->yellow_ = yellow; }

  void SetStripColor(RgbColor color);

 protected:
  YellowLightChannels channels_{Yellow_RGB};
  CloudLightComponent *yellow_{nullptr};

  friend class CloudLightComponent;
};

}  // namespace cloud_light
}  // namespace esphome
