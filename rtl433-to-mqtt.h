#include "esphome.h"

#undef yield
#undef millis
#undef micros
#undef delay
#undef delayMicroseconds

#include "rtl_433_ESP.h"

class Rtl433ToMqtt : public esphome::Component, public esphome::mqtt::CustomMQTTDevice {
 public:
  Rtl433ToMqtt(const char* source) : source_(source) {
    instance_ = this;
  }
  static Rtl433ToMqtt* get(
      const custom_component::CustomComponentConstructor& c) {
    return static_cast<Rtl433ToMqtt*>(c.get_component(0));
  }

  void setup() override;
  void loop() override { rf_.loop(); }

  void status() {
    rf_.getStatus();
    rf_.getModuleStatus();
  }

 private:
  const char* source_;

  char buffer_[512];
  rtl_433_ESP rf_;
  static Rtl433ToMqtt* instance_;

  static void process_dispatch(char* msg) {
    if (instance_ != nullptr) instance_->process(msg);
  }
  void process(char* msg);
  void process_json(JsonObject root);
};

Rtl433ToMqtt* Rtl433ToMqtt::instance_ = nullptr;

void Rtl433ToMqtt::setup() {
  rf_.initReceiver(RF_MODULE_RECEIVER_GPIO, RF_MODULE_FREQUENCY);
  rf_.setCallback(&Rtl433ToMqtt::process_dispatch, buffer_, sizeof(buffer_));
  rf_.enableReceiver();
}

void Rtl433ToMqtt::process(char* msg) {
  ESP_LOGD("custom", "Received msg: %s", msg);
  parse_json(msg, [this](JsonObject doc) {
    process_json(doc);
  });
}

void Rtl433ToMqtt::process_json(JsonObject doc) {
  const char* model = doc["model"];
  if (strcmp(model, "status") == 0) return;
  char topic[30];
  snprintf(topic, sizeof(topic), "433/%s/%d", model, doc["id"].as<int>());
  publish_json(topic, [this, doc](JsonObject root) {
    root.set(doc);
    root["source"] = source_;
  }, 1, true);
}


