// src/platform/x11_overlay_platform.cpp
#include "makadi/platform/overlay_platform.hpp"

#include <QWidget>

#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>

#include <iostream>

namespace makadi::platform {

void makeInputPassthrough(QWidget& window)
{
  window.winId();

  Display* display = XOpenDisplay(nullptr);
  if (!display) {
    std::cerr << "[makadi] Failed to open X11 display; input passthrough disabled\n";
    return;
  }

  const auto x11_window = static_cast<Window>(window.winId());

  XserverRegion empty_region = XFixesCreateRegion(display, nullptr, 0);

  XFixesSetWindowShapeRegion(
    display,
    x11_window,
    ShapeInput,
    0,
    0,
    empty_region);

  XFixesDestroyRegion(display, empty_region);
  XFlush(display);
  XCloseDisplay(display);

  std::cerr << "[makadi] Enabled X11 input passthrough\n";
}

QPointF globalPointerPosition()
{
  Display* display = XOpenDisplay(nullptr);
  if (!display) {
    return {};
  }

  Window root = DefaultRootWindow(display);
  Window returned_root;
  Window returned_child;
  int root_x = 0;
  int root_y = 0;
  int win_x = 0;
  int win_y = 0;
  unsigned int mask = 0;

  const Bool ok = XQueryPointer(
    display,
    root,
    &returned_root,
    &returned_child,
    &root_x,
    &root_y,
    &win_x,
    &win_y,
    &mask);

  XCloseDisplay(display);

  if (!ok) {
    return {};
  }

  return QPointF(root_x, root_y);
}

}  // namespace makadi::platform