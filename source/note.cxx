#include <iostream>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "note.hxx"
#include "raylib.h"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
  note::note(raylib::Rectangle bounding_box, std::wstring title, std::vector<std::wstring> text, const theme &theme)
      : m_theme(theme),
        m_bounding_box(raylib_helper::subtract_border_from_rectangle(bounding_box, m_theme)),
        m_title_bounding_box(std::make_shared<raylib::Rectangle>(bounding_box.x, bounding_box.y, bounding_box.width, m_theme.font_size)),
        m_title(std::move(title), m_title_bounding_box, m_theme),
        m_text(std::move(text), m_text_bounding_box, theme) {
    compute_element_coordinates();
  }

  note::note(raylib::Rectangle bounding_box, const theme &theme)
      : m_theme(theme),
        m_bounding_box(bounding_box),
        m_border_box(bounding_box),
        m_title_bounding_box(std::make_shared<raylib::Rectangle>(bounding_box.x, bounding_box.y, bounding_box.width, m_theme.font_size)),
        m_title(m_title_bounding_box, m_theme),
        m_text(m_text_bounding_box, theme) {
    compute_element_coordinates();
  }

  void note::compute_element_coordinates() {
    using namespace raylib;

    if (m_title_bounding_box->width > m_bounding_box.width) {
      m_bounding_box.width = m_title_bounding_box->width;
    }

    m_border_box = raylib_helper::add_border_to_rectangle(m_bounding_box, m_theme);

    m_title_delimiter = Rectangle {
      .x = m_bounding_box.x + m_theme.border_size,
      .y = m_title_bounding_box->y + m_title_bounding_box->height,
      .width = m_bounding_box.width - (m_theme.border_size * 2),
      .height = m_theme.border_size,
    };

    // m_text_bounding_box = Rectangle {
    //   .x = m_bounding_box.x + m_theme.glyph_spacing,
    //   .y = m_title_delimiter.y + m_title_delimiter.height + m_theme.glyph_spacing,
    //   .width = m_bounding_box.width - (2 * m_theme.glyph_spacing),
    //   .height = m_bounding_box.height - m_title_bounding_box->height - (4 * m_theme.glyph_spacing) - m_title_delimiter.height,
    // };
  }

  bool note::can_focus(raylib::Vector2 point) const {
    return CheckCollisionPointRec(point, m_bounding_box);
  }

  void note::send_events(std::span<event> events) {
    std::vector<event> title_events;

    for (const auto &boxed_event : events) {
      if (std::holds_alternative<mouse_event>(boxed_event)) {
        auto event = std::get<mouse_event>(boxed_event);

        // TODO: introduce something like `m_title_area` where user can click to focus `m_title`
        // because right now user needs to press directly on the text to be able to focus it
        if (raylib::CheckCollisionPointRec(event.point, *m_title_bounding_box)) {
          m_title_focused = true;
          title_events.push_back(event);
        } else {
          m_title_focused = false;
        }
      } else if (std::holds_alternative<keyboard_event>(boxed_event)) {
        auto event = std::get<keyboard_event>(boxed_event);

        if (m_title_focused) {
          title_events.push_back(event);
        }
      }
    }

    if (!title_events.empty()) {
      m_title.send_events(title_events);
    }

    compute_element_coordinates();
  }

  // TODO: scrolling
  void note::render() const {
    using namespace raylib;
    using namespace raylib_helper;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_bounding_box, m_theme.entity_background);
    DrawRectangleRec(m_title_delimiter, m_theme.border);

    m_title.render();
    m_text.render();
  }
};  // namespace ui
