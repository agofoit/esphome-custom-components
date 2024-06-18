#include "esphome/core/component.h"
#include "esphome/components/light/light_effect.h"

namespace esphome {
namespace red_green_blink_effect {

class RedGreenBlinkEffect : public AddressableLight::AddressableEffect {
 public:
  RedGreenBlinkEffect() = default;

 protected:
  void apply(AddressableLight &addressable, const Color &current_color) override;
};

}  // namespace red_green_blink_effect
}  // namespace esphome