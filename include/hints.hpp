/*
 * MIT License
 *
 * Copyright (c) 2019-2020 Albin Johansson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @brief Provides utilities related to managing hints.
 *
 * @details Provides utilities related to managing hints ("configuration
 * variables" on the SDL2 wiki). Refer to the official SDL2 wiki or the
 * <code>SDL_hints.hpp</code> header for details regarding any specific hint
 * type.
 *
 * @todo `WindowsIntResourceIcon`, `WindowsIntResourceIconSmall`,
 * `X11WindowVisualID` are string hints because the types of their values
 * isn't known. Should be fixed if the type isn't actually string.
 *
 * @todo C++20: Make callback signature depend on the `UserData` and the type
 * of the associated hint, so that the values supplied to the callback aren't
 * always strings.
 *
 * @todo Document all of the hint classes.
 *
 * @todo Remove hint namespace.
 *
 * @file hints.hpp
 * @since 4.1.0
 * @author Albin Johansson
 * @date 2019-2020
 * @copyright MIT License
 */

/**
 * @defgroup configuration Configuration
 *
 * @brief Contains the API related to hints/configuration variables.
 */

#ifndef CENTURION_HINTS_HEADER
#define CENTURION_HINTS_HEADER

#include <SDL_hints.h>

#include <cstring>
#include <type_traits>

#include "centurion_api.hpp"
#include "centurion_exception.hpp"
#include "centurion_utils.hpp"
#include "log.hpp"

#ifdef CENTURION_USE_PRAGMA_ONCE
#pragma once
#endif  // CENTURION_USE_PRAGMA_ONCE

namespace centurion {

/// @cond FALSE

namespace detail {

template <typename Derived, typename Arg>
class crtp_hint {
 public:
  template <typename T>
  [[nodiscard]] static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Arg>;
  }

  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return Derived::name();
  }

  [[nodiscard]] static auto value() noexcept -> std::optional<Arg>
  {
    return Derived::current_value();
  }

  [[nodiscard]] static auto to_string(Arg value) -> std::string
  {
    return std::to_string(value);
  }
};

// A hint class that only accepts booleans
template <typename Hint>
class bool_hint : public crtp_hint<bool_hint<Hint>, bool> {
 public:
  template <typename T>
  [[nodiscard]] static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, bool>;
  }

  [[nodiscard]] static auto current_value() noexcept -> std::optional<bool>
  {
    return static_cast<bool>(SDL_GetHintBoolean(Hint::name(), SDL_FALSE));
  }

  [[nodiscard]] static auto to_string(bool value) -> std::string
  {
    return value ? "1" : "0";
  }
};

// A hint class that only accepts strings
template <typename Hint>
class string_hint : public crtp_hint<string_hint<Hint>, czstring> {
 public:
  template <typename T>
  [[nodiscard]] static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_convertible_v<T, czstring>;
  }

  [[nodiscard]] static auto current_value() noexcept -> std::optional<czstring>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value) {
      return std::nullopt;
    } else {
      return value;
    }
  }

  [[nodiscard]] static auto to_string(czstring value) -> std::string
  {
    return value;
  }
};

// A hint class that only accepts integers
template <typename Hint>
class int_hint : public crtp_hint<int_hint<Hint>, int> {
 public:
  template <typename T>
  [[nodiscard]] static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, int>;
  }

  [[nodiscard]] static auto current_value() noexcept -> std::optional<int>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value) {
      return std::nullopt;
    } else {
      return std::stoi(value);
    }
  }
};

// A hint class that only accepts unsigned integers
template <typename Hint>
class unsigned_int_hint : public crtp_hint<int_hint<Hint>, unsigned int> {
 public:
  template <typename T>
  [[nodiscard]] static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, unsigned int>;
  }

  [[nodiscard]] static auto current_value() noexcept
      -> std::optional<unsigned int>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value) {
      return std::nullopt;
    } else {
      return static_cast<unsigned int>(std::stoul(value));
    }
  }
};

// A hint class that only accepts floats
template <typename Hint>
class float_hint : public crtp_hint<float_hint<Hint>, float> {
 public:
  template <typename T>
  [[nodiscard]] static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, float>;
  }

  [[nodiscard]] static auto current_value() noexcept -> std::optional<float>
  {
    const czstring value = SDL_GetHint(Hint::name());
    if (!value) {
      return std::nullopt;
    } else {
      return std::stof(value);
    }
  }
};

}  // namespace detail

/// @endcond

/// @addtogroup configuration
/// @{

/**
 * @namespace centurion::hint
 *
 *
 *
 * @brief Contains all hint types.
 *
 * @since 4.1.0
 */
namespace hint {

/// @name Enum-hints
/// @{

/**
 * @class render_driver
 *
 * @ingroup configuration
 *
 * @brief Used to specify the render driver that will be used.
 *
 * @headerfile hints.hpp
 */
class render_driver final {
 public:
  enum Value { direct_3d, open_gl, open_gles, open_gles2, metal, software };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_DRIVER;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "direct3d")) {
      return direct_3d;
    } else if (equal(hint, "opengl")) {
      return open_gl;
    } else if (equal(hint, "opengles")) {
      return open_gles;
    } else if (equal(hint, "opengles2")) {
      return open_gles2;
    } else if (equal(hint, "metal")) {
      return metal;
    } else {
      return software;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      case direct_3d:
        return "direct3d";
      case open_gl:
        return "opengl";
      case open_gles:
        return "opengles";
      case open_gles2:
        return "opengles2";
      case metal:
        return "metal";
      default:
        /* FALLTHROUGH */
      case software:
        return "software";
    }
  }
};

class audio_resampling_mode final {
 public:
  enum Value { normal = 0, fast = 1, medium = 2, best = 3 };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_AUDIO_RESAMPLING_MODE;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "default")) {
      return normal;
    } else if (equal(hint, "fast")) {
      return fast;
    } else if (equal(hint, "medium")) {
      return medium;
    } else /*if (equal(hint, "best"))*/ {
      return best;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      case normal:
      default:
        return "default";
      case fast:
        return "fast";
      case medium:
        return "medium";
      case best:
        return "best";
    }
  }
};

class scale_quality final {
 public:
  enum Value { nearest = 0, linear = 1, best = 2 };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_SCALE_QUALITY;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "nearest")) {
      return nearest;
    } else if (equal(hint, "linear")) {
      return linear;
    } else /*if (equal(hint, "best"))*/ {
      return best;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
      case nearest:
        return "nearest";
      case linear:
        return "linear";
      case best:
        return "best";
    }
  }
};

class framebuffer_acceleration final {
 public:
  enum Value {
    off,
    on,
    direct_3d,
    open_gl,
    open_gles,
    open_gles2,
    metal,
    software
  };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_FRAMEBUFFER_ACCELERATION;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "0")) {
      return off;
    } else if (equal(hint, "1")) {
      return on;
    } else if (equal(hint, "direct3d")) {
      return direct_3d;
    } else if (equal(hint, "opengl")) {
      return open_gl;
    } else if (equal(hint, "opengles")) {
      return open_gles;
    } else if (equal(hint, "opengles2")) {
      return open_gles2;
    } else if (equal(hint, "metal")) {
      return metal;
    } else {
      return software;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case off:
        return "0";
      case on:
        return "1";
      case direct_3d:
        return "direct3d";
      case open_gl:
        return "opengl";
      case open_gles:
        return "opengles";
      case open_gles2:
        return "opengles2";
      case metal:
        return "metal";
      case software:
        return "software";
    }
  }
};

class audio_category final {
 public:
  enum Value { ambient, playback };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_AUDIO_CATEGORY;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "ambient")) {
      return ambient;
    } else /*if (equal(hint, "playback"))*/ {
      return playback;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case ambient:
        return "ambient";
      case playback:
        return "playback";
    }
  }
};

class win_d3d_compiler final {
 public:
  enum Value { d3d_compiler_46, d3d_compiler_43, none };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_WIN_D3DCOMPILER;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "d3dcompiler_46.dll")) {
      return d3d_compiler_46;
    } else if (equal(hint, "d3dcompiler_43.dll")) {
      return d3d_compiler_43;
    } else {
      return none;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case none:
        return "none";
      case d3d_compiler_43:
        return "d3dcompiler_43.dll";
      case d3d_compiler_46:
        return "d3dcompiler_46.dll";
    }
  }
};

class wave_riff_chunk_size final {
 public:
  enum Value { force, ignore_zero, ignore, maximum };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WAVE_RIFF_CHUNK_SIZE;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "force")) {
      return force;
    } else if (equal(hint, "ignorezero")) {
      return ignore_zero;
    } else if (equal(hint, "ignore")) {
      return ignore;
    } else /* if (equal(hint, "maximum")) */ {
      return maximum;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case ignore_zero:
        return "ignorezero";
      case ignore:
        return "ignore";
      case force:
        return "force";
      case maximum:
        return "maximum";
    }
  }
};

class wave_truncation final {
 public:
  enum Value { very_strict, strict, drop_frame, drop_block };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WAVE_TRUNCATION;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "verystrict")) {
      return very_strict;
    } else if (equal(hint, "strict")) {
      return strict;
    } else if (equal(hint, "dropframe")) {
      return drop_frame;
    } else /* if (equal(hint, "dropblock")) */ {
      return drop_block;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case drop_block:
        return "dropblock";
      case drop_frame:
        return "dropframe";
      case very_strict:
        return "verystrict";
      case strict:
        return "strict";
    }
  }
};

class wave_fact_chunk final {
 public:
  enum Value { truncate, strict, ignore_zero, ignore };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WAVE_FACT_CHUNK;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "truncate")) {
      return truncate;
    } else if (equal(hint, "strict")) {
      return strict;
    } else if (equal(hint, "ignorezero")) {
      return ignore_zero;
    } else /* if (equal(hint, "ignore")) */ {
      return ignore;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case ignore:
        return "ignore";
      case ignore_zero:
        return "ignorezero";
      case truncate:
        return "truncate";
      case strict:
        return "strict";
    }
  }
};

class logical_size_mode final {
 public:
  enum Value { letterbox, overscan };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_LOGICAL_SIZE_MODE;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "0") || equal(hint, "letterbox")) {
      return letterbox;
    } else /*if (equal(hint, "1") || equal(hint, "overscan"))*/ {
      return overscan;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case letterbox:
        return "letterbox";
      case overscan:
        return "overscan";
    }
  }
};

class qt_wayland_content_orientation final {
 public:
  enum Value {
    primary,
    portrait,
    landscape,
    inverted_portrait,
    inverted_landscape
  };

  template <typename T>
  static constexpr auto valid_arg() noexcept -> bool
  {
    return std::is_same_v<T, Value>;
  }

  static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_QTWAYLAND_CONTENT_ORIENTATION;
  }

  static auto current_value() noexcept -> std::optional<Value>
  {
    const czstring hint = SDL_GetHint(name());
    if (!hint) {
      return std::nullopt;
    }

    using detail::equal;
    if (equal(hint, "primary")) {
      return primary;
    } else if (equal(hint, "portrait")) {
      return portrait;
    } else if (equal(hint, "landscape")) {
      return landscape;
    } else if (equal(hint, "inverted-portrait")) {
      return inverted_portrait;
    } else /*if (equal(hint, "inverted-landscape"))*/ {
      return inverted_landscape;
    }
  }

  static auto to_string(Value value) -> std::string
  {
    switch (value) {
      default:
        [[fallthrough]];
      case primary:
        return "primary";
      case portrait:
        return "portrait";
      case landscape:
        return "landscape";
      case inverted_portrait:
        return "inverted-portrait";
      case inverted_landscape:
        return "inverted-landscape";
    }
  }
};

/// @} // end of enum hints

/// @name Boolean-hints
/// @{

class accelerometer_as_joystick final
    : public detail::bool_hint<accelerometer_as_joystick> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ACCELEROMETER_AS_JOYSTICK;
  }
};

class allow_top_most final : public detail::bool_hint<allow_top_most> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ALLOW_TOPMOST;
  }
};

class android_block_on_pause final
    : public detail::bool_hint<android_block_on_pause> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_BLOCK_ON_PAUSE;
  }
};

class android_trap_back_button final
    : public detail::bool_hint<android_trap_back_button> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_TRAP_BACK_BUTTON;
  }
};

class apple_tv_controller_ui_events final
    : public detail::bool_hint<apple_tv_controller_ui_events> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_APPLE_TV_CONTROLLER_UI_EVENTS;
  }
};

class apple_tv_remote_allow_rotation final
    : public detail::bool_hint<apple_tv_remote_allow_rotation> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_APPLE_TV_REMOTE_ALLOW_ROTATION;
  }
};

class bmp_save_legacy_format final
    : public detail::bool_hint<bmp_save_legacy_format> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_BMP_SAVE_LEGACY_FORMAT;
  }
};

class double_buffer final : public detail::bool_hint<double_buffer> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_DOUBLE_BUFFER;
  }
};

class enable_steam_controllers final
    : public detail::bool_hint<enable_steam_controllers> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ENABLE_STEAM_CONTROLLERS;
  }
};

class grab_keyboard final : public detail::bool_hint<grab_keyboard> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_GRAB_KEYBOARD;
  }
};

class idle_timer_disabled final
    : public detail::bool_hint<idle_timer_disabled> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_IDLE_TIMER_DISABLED;
  }
};

class ime_internal_editing final
    : public detail::bool_hint<ime_internal_editing> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_IME_INTERNAL_EDITING;
  }
};

class mac_background_app final : public detail::bool_hint<mac_background_app> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MAC_BACKGROUND_APP;
  }
};

class mac_ctrl_click_emulate_right_click final
    : public detail::bool_hint<mac_ctrl_click_emulate_right_click> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK;
  }
};

class mouse_focus_clickthrough final
    : public detail::bool_hint<mouse_focus_clickthrough> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH;
  }
};

class mouse_relative_mode_warp final
    : public detail::bool_hint<mouse_relative_mode_warp> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_RELATIVE_MODE_WARP;
  }
};

class no_signal_handlers final : public detail::bool_hint<no_signal_handlers> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_NO_SIGNAL_HANDLERS;
  }
};

class direct_3d_11_debug final : public detail::bool_hint<direct_3d_11_debug> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_DIRECT3D11_DEBUG;
  }
};

class direct_3d_thread_safe final
    : public detail::bool_hint<direct_3d_thread_safe> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_DIRECT3D_THREADSAFE;
  }
};

class opengl_es_driver final : public detail::bool_hint<opengl_es_driver> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_OPENGL_ES_DRIVER;
  }
};

class enable_opengl_shaders final
    : public detail::bool_hint<enable_opengl_shaders> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_OPENGL_SHADERS;
  }
};

class vsync final : public detail::bool_hint<vsync> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_VSYNC;
  }
};

class allow_screensaver final : public detail::bool_hint<allow_screensaver> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_ALLOW_SCREENSAVER;
  }
};

class video_external_context final
    : public detail::bool_hint<video_external_context> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_EXTERNAL_CONTEXT;
  }
};

class disable_high_dpi final : public detail::bool_hint<disable_high_dpi> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_HIGHDPI_DISABLED;
  }
};

class mac_fullscreen_spaces final
    : public detail::bool_hint<mac_fullscreen_spaces> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES;
  }
};

class minimize_on_focus_loss final
    : public detail::bool_hint<minimize_on_focus_loss> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS;
  }
};

class window_frame_usable_while_cursor_hidden final
    : public detail::bool_hint<window_frame_usable_while_cursor_hidden> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN;
  }
};

class xinput_enabled final : public detail::bool_hint<xinput_enabled> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_XINPUT_ENABLED;
  }
};

class xinput_use_old_joystick_mapping final
    : public detail::bool_hint<xinput_use_old_joystick_mapping> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_XINPUT_USE_OLD_JOYSTICK_MAPPING;
  }
};

class mouse_touch_events final : public detail::bool_hint<mouse_touch_events> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_TOUCH_EVENTS;
  }
};

class render_batching final : public detail::bool_hint<render_batching> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RENDER_BATCHING;
  }
};

class return_key_hides_ime final
    : public detail::bool_hint<return_key_hides_ime> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RETURN_KEY_HIDES_IME;
  }
};

class touch_mouse_events final : public detail::bool_hint<touch_mouse_events> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_TOUCH_MOUSE_EVENTS;
  }
};

class tv_remote_as_joystick final
    : public detail::bool_hint<tv_remote_as_joystick> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_TV_REMOTE_AS_JOYSTICK;
  }
};

class display_usable_bounds final
    : public detail::string_hint<display_usable_bounds> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_DISPLAY_USABLE_BOUNDS;
  }
};

class emscripten_keyboard_element final
    : public detail::string_hint<emscripten_keyboard_element> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT;
  }
};

class orientations final : public detail::string_hint<orientations> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ORIENTATIONS;
  }
};

class window_share_pixel_format final
    : public detail::string_hint<window_share_pixel_format> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT;
  }
};

class qt_wayland_window_flags final
    : public detail::string_hint<qt_wayland_window_flags> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_QTWAYLAND_WINDOW_FLAGS;
  }
};

class android_apk_expansion_main_file_version final
    : public detail::int_hint<android_apk_expansion_main_file_version> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_APK_EXPANSION_MAIN_FILE_VERSION;
  }
};

class android_apk_expansion_patch_file_version final
    : public detail::int_hint<android_apk_expansion_patch_file_version> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_ANDROID_APK_EXPANSION_PATCH_FILE_VERSION;
  }
};

class event_logging final : public detail::int_hint<event_logging> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_EVENT_LOGGING;
  }
};

class mouse_double_click_time final
    : public detail::int_hint<mouse_double_click_time> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_DOUBLE_CLICK_TIME;
  }
};

class mouse_double_click_radius final
    : public detail::int_hint<mouse_double_click_radius> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_DOUBLE_CLICK_RADIUS;
  }
};

class raspberry_pi_video_layer final
    : public detail::int_hint<raspberry_pi_video_layer> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_RPI_VIDEO_LAYER;
  }
};

class thread_stack_size final
    : public detail::unsigned_int_hint<thread_stack_size> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_THREAD_STACK_SIZE;
  }
};

class timer_resolution final
    : public detail::unsigned_int_hint<timer_resolution> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_TIMER_RESOLUTION;
  }
};

class mouse_normal_speed_scale final
    : public detail::float_hint<mouse_normal_speed_scale> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_NORMAL_SPEED_SCALE;
  }
};

class mouse_relative_speed_scale final
    : public detail::float_hint<mouse_relative_speed_scale> {
 public:
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_MOUSE_RELATIVE_SPEED_SCALE;
  }
};

namespace gamecontroller {

struct use_button_labels final : detail::bool_hint<use_button_labels> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS;
  }
};

struct type final : detail::string_hint<type> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLERTYPE;
  }
};

struct config final : detail::string_hint<config> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLERCONFIG;
  }
};

struct config_file final : detail::string_hint<config_file> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLERCONFIG_FILE;
  }
};

struct ignore_devices final : detail::string_hint<ignore_devices> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLER_IGNORE_DEVICES;
  }
};

struct ignore_devices_except final
    : detail::string_hint<ignore_devices_except> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT;
  }
};

}  // namespace gamecontroller

namespace winrt {

struct privacy_policy_label final : detail::string_hint<privacy_policy_label> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_PRIVACY_POLICY_LABEL;
  }
};

struct privacy_policy_url final : detail::string_hint<privacy_policy_url> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_PRIVACY_POLICY_URL;
  }
};

struct handle_back_button final : detail::bool_hint<handle_back_button> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINRT_HANDLE_BACK_BUTTON;
  }
};

}  // namespace winrt

namespace windows {

struct no_thread_naming final : detail::bool_hint<no_thread_naming> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING;
  }
};

struct enable_message_loop final : detail::bool_hint<enable_message_loop> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_ENABLE_MESSAGELOOP;
  }
};

struct no_close_on_alt_f4 final : detail::bool_hint<no_close_on_alt_f4> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_NO_CLOSE_ON_ALT_F4;
  }
};

struct int_resource_icon final : detail::string_hint<int_resource_icon> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_INTRESOURCE_ICON;
  }
};

struct int_resource_icon_small final
    : detail::string_hint<int_resource_icon_small> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_WINDOWS_INTRESOURCE_ICON_SMALL;
  }
};

}  // namespace windows

namespace joystick {

struct allow_background_events final
    : detail::bool_hint<allow_background_events> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS;
  }
};

struct use_hidapi final : detail::bool_hint<use_hidapi> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI;
  }
};

struct use_hidapi_ps4 final : detail::bool_hint<use_hidapi_ps4> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_PS4;
  }
};

struct use_hidapi_ps4_rumble final : detail::bool_hint<use_hidapi_ps4_rumble> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE;
  }
};

struct use_hidapi_steam final : detail::bool_hint<use_hidapi_steam> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_STEAM;
  }
};

struct use_hidapi_switch final : detail::bool_hint<use_hidapi_switch> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_SWITCH;
  }
};

struct use_hidapi_xbox final : detail::bool_hint<use_hidapi_xbox> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_XBOX;
  }
};

struct use_hidapi_game_cube final : detail::bool_hint<use_hidapi_game_cube> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE;
  }
};

}  // namespace joystick

namespace x11 {

struct net_wm_ping final : detail::bool_hint<net_wm_ping> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_NET_WM_PING;
  }
};

struct net_wm_bypass_compositor final
    : detail::bool_hint<net_wm_bypass_compositor> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR;
  }
};

struct force_egl final : detail::bool_hint<force_egl> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_FORCE_EGL;
  }
};

struct xinerama final : detail::bool_hint<xinerama> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_XINERAMA;
  }
};

struct xrandr final : detail::bool_hint<xrandr> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_XRANDR;
  }
};

struct xvidmode final : detail::bool_hint<xvidmode> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_XVIDMODE;
  }
};

struct window_visual_id final : detail::string_hint<window_visual_id> {
  [[nodiscard]] static constexpr auto name() noexcept -> czstring
  {
    return SDL_HINT_VIDEO_X11_WINDOW_VISUALID;
  }
};

}  // namespace x11

}  // namespace hint

/**
 * @enum hint_priority
 *
 * @brief Provides three different priorities that can be specified when
 * setting the value of a hint.
 *
 * @since 4.1.0
 *
 * @see `SDL_HintPriority`
 *
 * @headerfile hints.hpp
 */
enum class hint_priority {
  low = SDL_HINT_DEFAULT,   /**< The lowest priority, used for default values.*/
  normal = SDL_HINT_NORMAL, /**< The priority used by default by
                             * <code>set_hint</code>.*/
  override = SDL_HINT_OVERRIDE /**< The highest priority.*/
};

/**
 * @brief Sets the value of the specified hint.
 *
 * @details This method will only accept values that are related to the
 * specified hint, supplying the wrong kind of value causes a compile-time
 * error. See the related hint class for more details about the associated
 * value type. However, whilst this method is type-safe, it doesn't ensure
 * that *correct* values are specified for the hints.
 *
 * @par Examples
 * The following is an example of how you could use this method to
 * specify the render driver that SDL should use, and the value is specified
 * with an enum value associated with the `render_driver` class.
 * @code{.cpp}
 *   set_hint<render_driver>(render_driver::open_gl);
 * @endcode
 * Most hints only accept boolean or integer values. As in the following
 * example.
 * @code{.cpp}
 *   set_hint<enable_vsync>(true);
 *   set_hint<event_logging>(2);
 * @endcode
 * Sometimes, it's useful to know whether or not the hint was actually set
 * to the desired value. This can be done according to the following example.
 * @code{.cpp}
 *   const bool success = set_hint<double_buffer>(true);
 *   if (success) {
 *     // the hint was actually set!
 *   } else {
 *     // something went wrong!
 *   }
 * @endcode
 *
 * @tparam Hint the type of the hint that will be modified.
 * @tparam priority the priority that will be used, defaults to `normal`.
 * @tparam Value the type of the hint value.
 *
 * @param value the new value that will be set for the specified hint.
 *
 * @return `true` if the hint was successfully set; `false` otherwise.
 *
 * @since 4.1.0
 */
template <typename Hint,
          hint_priority priority = hint_priority::normal,
          typename Value,
          typename = std::enable_if_t<Hint::template valid_arg<Value>()>>
auto set_hint(const Value& value) -> bool
{
  return static_cast<bool>(
      SDL_SetHintWithPriority(Hint::name(),
                              Hint::to_string(value).c_str(),
                              static_cast<SDL_HintPriority>(priority)));
}

/**
 * @brief Returns the current value of the specified hint.
 *
 * @note The returned value is a `std::optional` of the hint value type.
 *
 * @par Examples
 * Many hints aren't actually set by default, so if the specified hint
 * doesn't have a set value, then this method will return a null optional.
 * @code{.cpp}
 *  if (const auto value = get_hint<render_driver>(); value) {
 *    // the hint was set!
 *  } else {
 *    // the hint had no set value!
 *  }
 * @endcode
 *
 * @tparam Hint the type of the Hint to obtain the value of.
 *
 * @return the current value of the specified hint; `std::nullopt` if there is
 * no value set for the hint.
 *
 * @since 4.1.0
 */
template <typename Hint>
[[nodiscard]] auto get_hint() noexcept
{
  return Hint::current_value();
}

/**
 * @class hint_callback
 *
 * @brief Represents a handle for dealing with hint callbacks.
 *
 * @details This class provides methods for easily connecting and disconnecting
 * a callback from receiving updates.
 *
 * @tparam Hint the type of the associated hint. Should be one of the hint
 * types defined in the `hints.hpp` header.
 *
 * @tparam UserData the type of the user data, defaults to `void`.
 *
 * @since 4.1.0
 *
 * @headerfile hints.hpp
 */
template <typename Hint, typename UserData = void>
class hint_callback final {
 public:
  /**
   * @brief Creates a `HintCallback`.
   *
   * @param callback the function object that will be called whenever the
   * associated hint is updated. The signature should be `void(void*,
   * czstring, czstring, czstring)`.
   * @param userData a pointer to some user data. Defaults to `nullptr`.
   *
   * @throws centurion_exception if the supplied function pointer is null.
   *
   * @since 4.1.0
   */
  hint_callback(SDL_HintCallback callback, UserData* userData = nullptr)
      : m_callback{callback}, m_userData{userData}
  {
    if (!callback) {
      throw centurion_exception{"Failed to create hint callback"};
    }
  }

  /**
   * @brief Registers the callback to be invoked whenever the associated hint is
   * updated.
   *
   * @see `SDL_AddHintCallback`
   *
   * @since 4.1.0
   */
  void connect() noexcept
  {
    SDL_AddHintCallback(
        Hint::name(), m_callback, static_cast<void*>(m_userData));
  }

  /**
   * @brief Unregisters the callback from being updated whenever the associated
   * hint is updated.
   *
   * @see `SDL_DelHintCallback`
   *
   * @since 4.1.0
   */
  void disconnect() noexcept
  {
    SDL_DelHintCallback(
        Hint::name(), m_callback, static_cast<void*>(m_userData));
  }

  /**
   * @brief Returns a pointer to the function that is invoked when the
   * associated hint is updated.
   *
   * @return a pointer to the function that is invoked when the associated
   * hint is updated.
   *
   * @since 4.1.0
   */
  [[nodiscard]] auto get() noexcept -> SDL_HintCallback { return m_callback; }

  /**
   * @brief Returns a pointer to the user data associated with the callback.
   *
   * @return a pointer to the user data associated with the callback, can be
   * null.
   *
   * @since 4.1.0
   */
  [[nodiscard]] auto user_data() const noexcept -> UserData*
  {
    return m_userData;
  }

 private:
  SDL_HintCallback m_callback;
  UserData* m_userData;
};

/**
 * @brief Adds a callback to observe changes of the value of the specified
 * hint is updated.
 *
 * @details A callback handle object is returned, which can be used to easily
 * disconnect the callback later.
 *
 * @note The callback will be immediately invoked with the current value of
 * the hint.
 *
 * @note In a future version of centurion (that supports C++20), the
 * signature of the function object will be dependent on the `UserData` type.
 * Unfortunately, this isn't really doable with C++17. Since it requires
 * default-constructible stateless lambdas.
 *
 * @par Examples
 * This method can be used with any function object that is stateless, such
 * as traditional function pointers and lambdas. The simplest way to add a
 * callback is with a lambda and no explicit user data.
 * @code{.cpp}
 *   auto handle = add_hint_callback([](void* userData,
 *                                      czstring hint,
 *                                      czstring oldValue,
 *                                      czstring newValue) {
 *    // code that handles the update
 *   });
 * @endcode
 * It's also possible to supply a pointer to some data that you want to
 * associate with the callback. As always, beware of the lifetime of the data!
 * @code{.cpp}
 *   int data = 8; // shouldn't be local in real code
 *   auto handle = add_hint_callback([](void* userData,
 *                                      czstring hint,
 *                                      czstring oldValue,
 *                                      czstring newValue) {
 *     // code that handles the update
 *   },
 *   &data);
 * @endcode
 *
 * @tparam Hint should one of the many hint types defined in this header.
 * However, all it requires is that the type provides a static method that
 * returns a `czstring`.
 * @tparam UserData the type of the user data, defaults to void.
 *
 * @param fun the function object that will be invoked when the hint is
 * updated. The signature should be `void(void*, czstring, czstring, czstring)`.
 * @param userData the user data to associate with the callback, defaults to
 * `nullptr`.
 *
 * @return a handle to the added callback.
 *
 * @since 4.1.0
 */
template <typename Hint, typename UserData = void>
auto add_hint_callback(SDL_HintCallback fun,
                       UserData* userData = nullptr) noexcept
    -> hint_callback<Hint, UserData>
{
  hint_callback<Hint, UserData> hintCallback{fun, userData};
  hintCallback.connect();
  return hintCallback;
}

/**
 * @brief Clears all stored hints.
 *
 * @see `SDL_ClearHints`
 *
 * @since 4.1.0
 */
inline void clear_hints() noexcept
{
  SDL_ClearHints();
}

/// @}

}  // namespace centurion

#undef CENTURION_BOOL_HINT
#endif  // CENTURION_HINTS_HEADER
