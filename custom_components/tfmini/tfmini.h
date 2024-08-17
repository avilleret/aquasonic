#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace tfmini {

class TFMiniSensor : public PollingComponent, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_distance_sensor(sensor::Sensor *distance_sensor) { distance_sensor_ = distance_sensor; }
  void set_strength_sensor(sensor::Sensor *strength_sensor) { strength_sensor_ = strength_sensor; }

  void update() override;
  void dump_config() override;

 protected:
  uint8_t tfmini_checksum_(uint8_t *ptr);

  sensor::Sensor *distance_sensor_{nullptr};
  sensor::Sensor *strength_sensor_{nullptr};

  std::deque<unsigned int> cyclic_distance_buffer;
  std::deque<unsigned int> cyclic_strength_buffer;
  size_t head = 0;
  unsigned int distance_sum = 0;
  unsigned int strength_sum = 0;
};

}  // namespace tfmini
}  // namespace esphome