#include <QSettings>
#include <interfaces/isettings.h>

namespace conanqt {
struct Settings : public ISettings {
  Settings() = default;

  void write(std::string key, std::string value) override {
    settings.setValue(key.c_str(), QString::fromStdString(value));
    settings.sync();
  };
  std::string read(std::string key, std::string default_value) const override {
    return settings.value(key.c_str(), QString::fromStdString(default_value))
        .toString()
        .toStdString();
  };

 private:
  QSettings settings;
};
}  // namespace conanqt