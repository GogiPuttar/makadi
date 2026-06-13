#include "makadi/render/overlay_window.hpp"

#include <QApplication>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  makadi::render::OverlayWindow window;
  window.show();

  return app.exec();
}