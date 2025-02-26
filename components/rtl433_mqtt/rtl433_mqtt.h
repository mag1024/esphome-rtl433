#pragma once

#include "esphome.h"
#include "esphome/components/json/json_util.h"
#include "esphome/components/mqtt/mqtt_client.h"

#undef yield
#undef millis
#undef micros
#undef delay
#undef delayMicroseconds

#include "rtl_433_ESP.h"

namespace esphome {
namespace rtl433_mqtt {

const char* TAG = "rtl433_mqtt";

class Gateway : public Component {
 public:
  Gateway(mqtt::MQTTClientComponent* mqtt_client,
          const std::string& topic_prefix, const std::string& source)
      : mqtt_client_(mqtt_client),
        topic_prefix_(topic_prefix),
        source_(source) {
    instance_ = this;
  }

  void setup() override;
  void loop() override { rf_.loop(); }
  void stop() { rf_.disableReceiver(); }

  void status() {
    rf_.getStatus();
    rf_.getModuleStatus();
  }

 private:
  mqtt::MQTTClientComponent* mqtt_client_;
  const std::string topic_prefix_;
  const std::string source_;

  char buffer_[512];
  rtl_433_ESP rf_;
  static Gateway* instance_;

  static void process_dispatch(char* msg) {
    if (instance_ != nullptr) instance_->process(msg);
  }
  void process(char* msg);
  void process_json(JsonObject root);
};

Gateway* Gateway::instance_ = nullptr;

void Gateway::setup() {
  rf_.initReceiver(RF_MODULE_RECEIVER_GPIO, RF_MODULE_FREQUENCY);
  rf_.setCallback(&Gateway::process_dispatch, buffer_, sizeof(buffer_));
  rf_.enableReceiver();
}

void Gateway::process(char* msg) {
  ESP_LOGD(TAG, "Received msg: %s", msg);
  json::parse_json(msg, [this](JsonObject doc) {
    process_json(doc);
    return true;
  });
}

void Gateway::process_json(JsonObject doc) {
  const char* model = doc["model"];
  if (strcmp(model, "status") == 0) return;
  char topic[30];
  snprintf(topic, sizeof(topic), "%s/%s/%d", topic_prefix_.c_str(), model,
           doc["id"].as<int>());
  ESP_LOGD(TAG, "Publishing to topic: %s", topic);
  bool r = mqtt_client_->publish_json(
      topic,
      [this, doc](JsonObject root) {
        root.set(doc);
        if (!source_.empty()) root["source"] = source_;
      },
      1, true);
  if (!r) ESP_LOGW(TAG, "Publishing failed!");
}

template <typename... Ts>
class StatusAction : public Action<Ts...>, public Parented<Gateway> {
 public:
  void play(Ts... x) override { parent_->status(); }
};

template <typename... Ts>
class StopAction : public Action<Ts...>, public Parented<Gateway> {
 public:
  void play(Ts... x) override { parent_->stop(); }
};

}  // namespace rtl433_mqtt
}  // namespace esphome
