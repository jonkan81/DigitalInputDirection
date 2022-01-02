#include "digital_input_direction.h"

DigitalInputDirection::DigitalInputDirection(uint8_t pin1, uint8_t pin2, int pin_mode,
                                         int interrupt_type, uint read_delay,
                                         String config_path)
    : DigitalInput{pin1, pin_mode, interrupt_type, config_path},      
      BooleanProducer(),
      pin2_{pin2},
      read_delay_{read_delay} {
  load_configuration();
}

void DigitalInputDirection::enable() {
  app.onInterrupt(pin_, interrupt_type_, [this]() { 
    if(!is_direction_determined) {
      pin1_first_ = !is_pin2_read_;
    } 
    });
  app.onInterrupt(pin2_, interrupt_type_, [this]() { 
    if(!is_direction_determined) {
      is_pin2_read_ = true;
    } 
    });  

  app.onRepeat(read_delay_, [this]() {
    noInterrupts();
    output = pin1_first_;
    is_direction_determined = false;
    interrupts();
    notify();
  });
}

void DigitalInputDirection::get_configuration(JsonObject& root) {
  root["read_delay"] = read_delay_;
}

static const char SCHEMA[] PROGMEM = R"###({
    "type": "object",
    "properties": {
        "read_delay": { "title": "Read delay", "type": "number", "description": "The time, in milliseconds, between each read of the input" }
    }
  })###";

String DigitalInputDirection::get_config_schema() { return FPSTR(SCHEMA); }

bool DigitalInputDirection::set_configuration(const JsonObject& config) {
  String expected[] = {"read_delay"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  read_delay_ = config["read_delay"];
  return true;
}

