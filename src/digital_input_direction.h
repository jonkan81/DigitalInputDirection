#include "sensors/digital_input.h"

class DigitalInputDirection : public DigitalInput, public BooleanProducer {
 public:
  DigitalInputDirection(uint8_t pin1, uint8_t pin2, int pin_mode, int interrupt_type,
                      uint read_delay, String config_path = "");

  void enable() override final;

 private:
  uint8_t pin2_;
  uint read_delay_;
  volatile boolean is_pin2_read_ = false;  
  volatile boolean pin1_first_ = true;
  volatile boolean is_direction_determined = false;
  virtual void get_configuration(JsonObject& doc) override;
  virtual bool set_configuration(const JsonObject& config) override;
  virtual String get_config_schema() override;
};
