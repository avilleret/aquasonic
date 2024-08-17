#include "tfmini.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tfmini {

static const char *const TAG = "tfmini";
static const uint8_t TFMINI_RESPONSE_LENGTH = 9;
static const uint8_t TFMINI_START_BYTE = 0x59;

void TFMiniSensor::update() {
  uint8_t response[TFMINI_RESPONSE_LENGTH];
  uint8_t peeked;

  // this->flush();
  if(this->available() < TFMINI_RESPONSE_LENGTH)
    return;

  while (this->available() > 0 && this->peek_byte(&peeked) && peeked != TFMINI_START_BYTE)
    this->read();

  bool read_success = read_array(response, TFMINI_RESPONSE_LENGTH);

  if (!read_success) {
    ESP_LOGW(TAG, "Reading data from TFMini failed!");
    status_set_warning();
    return;
  }

  if (response[0] != TFMINI_START_BYTE || response[1] != TFMINI_START_BYTE) {
    ESP_LOGW(TAG, "Invalid preamble for TFMini response!");
    this->status_set_warning();
    return;
  }

  uint16_t calculated_checksum = this->tfmini_checksum_(response);
  if (calculated_checksum != response[TFMINI_RESPONSE_LENGTH - 1]) {
    ESP_LOGW(TAG, "TFMini Checksum doesn't match: 0x%02X!=0x%02X", response[TFMINI_RESPONSE_LENGTH - 1],
             calculated_checksum);
    this->status_set_warning();
    return;
  }

  this->status_clear_warning();

  // ESP_LOGD(TAG, "Successfully read TFMini data");

  const unsigned int distance = response[2] + (response[3] << 8);
  const unsigned int strength = response[4] + (response[5] << 8);

  cyclic_distance_buffer.push_back(distance);
  distance_sum += distance;
  if(cyclic_distance_buffer.size() > 100)
  {
    distance_sum -= cyclic_distance_buffer.front();
    cyclic_distance_buffer.pop_front();
  }

  cyclic_strength_buffer.push_back(strength);
  strength_sum += strength;
  if(cyclic_strength_buffer.size() > 100)
  {
    strength_sum -= cyclic_strength_buffer.front();
    cyclic_strength_buffer.pop_front();
  }

  head++;

  if(head >= 6000)
  {
    const float distance = float(distance_sum) / float(cyclic_distance_buffer.size());
    const float strength = float(strength_sum) / float(cyclic_strength_buffer.size());

    ESP_LOGD(TAG, "Received distance: %u cm", distance);
    if (this->distance_sensor_ != nullptr)
      this->distance_sensor_->publish_state(distance);

    ESP_LOGD(TAG, "Received signal strength: %u", strength);
    if (this->strength_sensor_ != nullptr)
      this->strength_sensor_->publish_state(strength);

    head = 0;
  }
}

uint8_t TFMiniSensor::tfmini_checksum_(uint8_t *ptr) {
  unsigned sum = 0;
  for (int i = 0; i < (TFMINI_RESPONSE_LENGTH - 1); i++) {
    sum += *ptr++;
  }
  return uint8_t(sum & 0xFF);
}

void TFMiniSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "TFMini:");
  LOG_SENSOR("  ", "Distance", this->distance_sensor_);
  LOG_SENSOR("  ", "Strength", this->strength_sensor_);
  this->check_uart_settings(115200);
}

}  // namespace tfmini
}  // namespace esphome