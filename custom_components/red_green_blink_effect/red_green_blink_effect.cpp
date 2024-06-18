#include "red_green_blink_effect.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace red_green_blink_effect {

static const Color RED = RED_COLOR();
static const Color GREEN = GREEN_COLOR();

void RedGreenBlinkEffect::apply(AddressableLight &addressable, const Color &current_color) {
  static bool red = true;
  static uint32_t last_run = 0;

  if (millis() - last_run > 3000) {
    last_run = millis();
    red = !red;
    addressable.set_effect_data(red ? RED : GREEN);
  }
}

}  // namespace red_green_blink_effect
}  // namespace esphome
