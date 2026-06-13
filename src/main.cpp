#include "makadi/config/app_config.hpp"
#include "makadi/render/overlay_window.hpp"

#include <QApplication>
#include <QStringList>

#include <cstdlib>
#include <iostream>
#include <exception>
#include <string>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  const char* session_type = std::getenv("XDG_SESSION_TYPE");
  const char* wayland_display = std::getenv("WAYLAND_DISPLAY");
  const char* display = std::getenv("DISPLAY");

  std::cerr
    << "[makadi] XDG_SESSION_TYPE=" << (session_type ? session_type : "<unset>") << "\n"
    << "[makadi] WAYLAND_DISPLAY=" << (wayland_display ? wayland_display : "<unset>") << "\n"
    << "[makadi] DISPLAY=" << (display ? display : "<unset>") << "\n"
    << "[makadi] Qt platform=" << app.platformName().toStdString() << "\n";

  if (session_type && std::string(session_type) == "wayland") {
    std::cerr
      << "[makadi] WARNING: GNOME Wayland/XWayland may not support global pointer tracking "
      << "for click-through overlays. Use an Xorg session for now.\n";
  }

  const bool debug = app.arguments().contains("--debug");

  const QStringList args = app.arguments();

  QString config_path = "configs/default.yaml";
  makadi::config::AppConfig config;

  const int config_index = args.indexOf("--config");
  if (config_index >= 0 && config_index + 1 < args.size()) {
    config_path = args.at(config_index + 1);
  }

  try {
    config = makadi::config::loadAppConfig(config_path);
  } catch (const std::exception& e) {
    std::cerr << "[makadi] Failed to load config: " << e.what() << "\n";
    return 1;
  }

  if (debug) {
    std::cerr << "[makadi debug] Loaded config: "
              << config_path.toStdString() << "\n";
  }

  makadi::render::OverlayWindow window(debug, config);
  window.show();

  return app.exec();
}