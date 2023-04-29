#include <algorithm>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>

#include "raylib.hxx"
#include "raylib_helper.hxx"
#include "single_line_input.hxx"

namespace ui {
  single_line_input::single_line_input(raylib::Rectangle bounding_box, const theme &theme, bool center_text)
      : m_border_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text) {
    m_bounding_box = raylib_helper::subtract_border_from_rectangle(m_border_box, m_theme);
  }

  single_line_input::single_line_input(std::wstring text, raylib::Rectangle bounding_box, const theme &theme, bool center_text)
      : m_bounding_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text),
        m_text(std::move(text)) {
    m_text_dimensions = raylib_helper::get_line_dimensions(m_text, m_theme);
  }

  bool single_line_input::can_focus(raylib::Vector2 point) const {
    return CheckCollisionPointRec(point, m_bounding_box);
  }

  void single_line_input::render() const {
    using namespace raylib;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_bounding_box, m_theme.entity_background);
  }

  void single_line_input::send_events(std::span<event> events) {
    for (const auto &boxed_event : events) {
      if (std::holds_alternative<mouse_event>(boxed_event)) {
        auto event = std::get<mouse_event>(boxed_event);
        std::wcout << L"mouse click event!!!!! (" << event.point.x << L", " << event.point.y << L")" << std::endl;
      } else if (std::holds_alternative<keyboard_event>(boxed_event)) {
        auto event = std::get<keyboard_event>(boxed_event);
        std::wcout << L"key: " << event.codepoint << L"; " << event.key << std::endl;
      }
    }
  }
}  // namespace ui
