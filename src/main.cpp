#include "makadi/render/overlay_window.hpp"

#include <QApplication>
#include <QStringList>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  const bool debug = app.arguments().contains("--debug");

  makadi::render::OverlayWindow window(debug);
  window.show();

  return app.exec();
}