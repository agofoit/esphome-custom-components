#pragma once

#include "esphome/components/socket/socket.h"
#include "esphome/core/component.h"

#include <cinttypes>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <NeoPixelBus.h>

namespace esphome {
namespace cloud_light {

class YellowAddressableLightEffect;
class CyanAddressableLightEffect;

enum CloudLightListenMethod { CloudLight_MULTICAST, CloudLight_UNICAST };

class CloudLightComponent : public esphome::Component {
 public:
  CloudLightComponent();
  ~CloudLightComponent();

  static const uint16_t PixelCount = 60;  // Set to the number of LEDs in your strip
  static const uint8_t PixelPin = 27;     // Set to the pin you are using
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip;

  void setup() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  void add_effect(YellowAddressableLightEffect *light_effect);
  void add_effect(CyanAddressableLightEffect *light_effect);
  void remove_effect(YellowAddressableLightEffect *light_effect);
  void remove_effect(CyanAddressableLightEffect *light_effect);

 protected:
  // CloudLightListenMethod listen_method_{CloudLight_MULTICAST};
  std::unique_ptr<socket::Socket> socket_;
  std::set<YellowAddressableLightEffect *> yellow_light_effects_;
  std::set<CyanAddressableLightEffect *> cyan_light_effects_;
};

}  // namespace cloud_light
}  // namespace esphome
