#pragma once

#include <span>

#include "event.hxx"
#include "raylib.hxx"

namespace ui {
  class entity {
  public:
    virtual void render() const = 0;
    [[nodiscard]] virtual bool can_focus(raylib::Vector2 point) const = 0;
    virtual void send_events(std::span<event> events) = 0;
  };
};  // namespace ui
