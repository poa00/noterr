#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "note.hxx"
#include "object.hxx"
#include "popup.hxx"
#include "raylib.hxx"
#include "single_line_input.hxx"
#include "theme.hxx"

namespace ui {
  enum class state {
    just_looking,
    popup_menu,
    drawing_new_note,
    focused_on_object,
  };

  class ui {
  public:
    explicit ui(std::filesystem::path file_path);
    ~ui();

    void main_loop();

  private:
    void update_window_size();
    void update_camera();
    void update_ui_state();

    void render();

    void execute_popup_action(popup_actions action);

    void pass_input_events_to_focused_object();

  private:
    std::filesystem::path m_file_path;

    raylib::Camera2D m_camera {};

    std::unique_ptr<popup> m_popup;

    float m_width {640};
    float m_height {480};

    state m_state {state::just_looking};
    bool m_started_drawing {false};

    theme m_theme;

    bool m_should_close {false};

    raylib::Rectangle m_note_placeholder;
    std::vector<std::unique_ptr<object>> m_objects;

    raylib::Shader m_background_texture_shader;
    raylib::RenderTexture2D m_background_texture_for_shader;
    int m_background_shader_screen_resolution_location;
    int m_background_shader_grid_tile_size_as_percentage_location;
  };
};  // namespace ui
