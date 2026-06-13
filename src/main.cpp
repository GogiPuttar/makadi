#include "makadi/render/overlay_window.hpp"

#include <QApplication>
#include <QStringList>

#include <cstdlib>
#include <iostream>

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

  makadi::render::OverlayWindow window(debug);
  window.show();

  return app.exec();
}