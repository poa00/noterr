#pragma once

#include <compare>
#include <memory>
#include <string>
#include <vector>

#include "entity.hxx"
#include "event.hxx"
#include "multi_line_input.hxx"
#include "raylib.h"
#include "raylib.hxx"
#include "single_line_input.hxx"
#include "theme.hxx"

namespace ui {
  class note : public entity {
  public:
    void render() const override;
    bool can_focus(raylib::Vector2 point) const override;
    void send_events(std::span<event> events) override;

    note(raylib::Rectangle bounding_box, std::wstring title, std::vector<std::wstring> text, const theme &theme);
    note(raylib::Rectangle bounding_box, const theme &theme);
    ~note() = default;

  private:
    void compute_element_coordinates();
    raylib::Rectangle title_delimiter();
    raylib::Rectangle text_bounding_box();

  private:
    theme m_theme;
    bool m_focused {false};
    bool m_title_focused {false};

    raylib::Rectangle m_bounding_box;
    raylib::Rectangle m_border_box;

    std::shared_ptr<raylib::Rectangle> m_title_bounding_box;
    single_line_input m_title;

    raylib::Rectangle m_title_delimiter;

    std::shared_ptr<raylib::Rectangle> m_text_bounding_box;
    multi_line_input m_text;
  };
};  // namespace ui
