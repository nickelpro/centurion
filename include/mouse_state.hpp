/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Albin Johansson
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

#ifndef CENTURION_MOUSE_STATE_HEADER
#define CENTURION_MOUSE_STATE_HEADER

#include "area.hpp"
#include "centurion_cfg.hpp"
#include "detail/max.hpp"
#include "integers.hpp"
#include "point.hpp"

#ifdef CENTURION_USE_PRAGMA_ONCE
#pragma once
#endif  // CENTURION_USE_PRAGMA_ONCE

namespace cen {

/// \addtogroup input
/// \{

/**
 * \class mouse_state
 *
 * \brief Provides information about the mouse state, which is an
 * alternative to dealing with mouse events.
 *
 * \since 3.0.0
 *
 * \headerfile mouse_state.hpp
 */
class mouse_state final
{
 public:
  /**
   * \brief Creates a `mouse_state` instance.
   *
   * \since 3.0.0
   */
  mouse_state() noexcept = default;

  /**
   * \brief Updates the mouse state. The window width and height will be
   * adjusted to be at least 1.
   *
   * \param windowWidth the current window width, set to 1 by default.
   * \param windowHeight the current window height, set to 1 by default.
   *
   * \since 3.0.0
   */
  void update(const int windowWidth = 1, const int windowHeight = 1) noexcept
  {
    m_oldX = m_mouseX;
    m_oldY = m_mouseY;
    m_prevLeftPressed = m_leftPressed;
    m_prevRightPressed = m_rightPressed;

    {
      const u32 mask = SDL_GetMouseState(&m_mouseX, &m_mouseY);
      m_leftPressed = mask & SDL_BUTTON(SDL_BUTTON_LEFT);
      m_rightPressed = mask & SDL_BUTTON(SDL_BUTTON_RIGHT);
    }

    {
      const auto xRatio = static_cast<float>(m_mouseX) /
                          static_cast<float>(detail::max(windowWidth, 1));
      const auto adjustedX = xRatio * static_cast<float>(m_logicalWidth);

      const auto yRatio = static_cast<float>(m_mouseY) /
                          static_cast<float>(detail::max(windowHeight, 1));
      const auto adjustedY = yRatio * static_cast<float>(m_logicalHeight);

      m_mouseX = static_cast<int>(adjustedX);
      m_mouseY = static_cast<int>(adjustedY);
    }
  }

  /**
   * \brief Resets the screen and logical dimensions of the mouse state
   * instance.
   *
   * \since 3.0.0
   */
  void reset() noexcept
  {
    m_logicalWidth = 1;
    m_logicalHeight = 1;
  }

  /**
   * \brief Sets the logical width that will be used to determine the mouse
   * position.
   *
   * \details The supplied value will be adjusted to be at least 1.
   *
   * \param logicalWidth the logical width that will be used to determine the
   * mouse position.
   *
   * \since 3.0.0
   */
  void set_logical_width(const int logicalWidth) noexcept
  {
    m_logicalWidth = detail::max(logicalWidth, 1);
  }

  /**
   * \brief Sets the logical height that will be used to determine the mouse
   * position.
   *
   * \details The supplied value will be adjusted to be at least 1.
   *
   * \param logicalHeight the logical height that will be used to determine the
   * mouse position.
   *
   * \since 3.0.0
   */
  void set_logical_height(const int logicalHeight) noexcept
  {
    this->m_logicalHeight = detail::max(logicalHeight, 1);
  }

  /**
   * \brief Indicates whether or not the left mouse button was released.
   *
   * \return `true` if the left mouse button was released; `false` otherwise.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto was_left_button_released() const noexcept -> bool
  {
    return !m_leftPressed && m_prevLeftPressed;
  }

  /**
   * \brief Indicates whether or not the right mouse button was released.
   *
   * \return `true` if the right mouse button was released; `false` otherwise.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto was_right_button_released() const noexcept -> bool
  {
    return !m_rightPressed && m_prevRightPressed;
  }

  /**
   * \brief Indicates whether or not the mouse was moved.
   *
   * \return `true` if the mouse was moved; `false` otherwise.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto was_mouse_moved() const noexcept -> bool
  {
    return (m_mouseX != m_oldX) || (m_mouseY != m_oldY);
  }

  /**
   * \brief Returns the x-coordinate of the mouse.
   *
   * \return the x-coordinate of the mouse.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto mouse_x() const noexcept -> int
  {
    return m_mouseX;
  }

  /**
   * \brief Returns the y-coordinate of the mouse.
   *
   * \return the y-coordinate of the mouse.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto mouse_y() const noexcept -> int
  {
    return m_mouseY;
  }

  /**
   * \brief Returns the position of the mouse.
   *
   * \return the current position of the mouse cursor.
   *
   * \since 5.0.0
   */
  [[nodiscard]] auto mouse_pos() const noexcept -> ipoint
  {
    return {m_mouseX, m_mouseY};
  }

  /**
   * \brief Returns the logical width used by the mouse state instance.
   *
   * \return the logical width used by the mouse state instance, 1 is used by
   * default.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto logical_width() const noexcept -> int
  {
    return m_logicalWidth;
  }

  /**
   * \brief Returns the logical height used by the mouse state instance.
   *
   * \return the logical height used by the mouse state instance, 1 is used by
   * default.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto logical_height() const noexcept -> int
  {
    return m_logicalHeight;
  }

  /**
   * \brief Returns the logical size used by the mouse state instance.
   *
   * \return the logical size used.
   *
   * \since 5.0.0
   */
  [[nodiscard]] auto logical_size() const noexcept -> iarea
  {
    return {m_logicalWidth, m_logicalHeight};
  }

  /**
   * \brief Indicates whether or not the left mouse button is currently pressed.
   *
   * \return `true` if the left mouse button is pressed; `false` otherwise.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto is_left_button_pressed() const noexcept -> bool
  {
    return m_leftPressed;
  }

  /**
   * \brief Indicates whether or not the right mouse button is currently
   * pressed.
   *
   * \return `true` if the right mouse button is pressed; `false` otherwise.
   *
   * \since 3.0.0
   */
  [[nodiscard]] auto is_right_button_pressed() const noexcept -> bool
  {
    return m_rightPressed;
  }

 private:
  int m_mouseX{0};
  int m_mouseY{0};
  int m_oldX{0};
  int m_oldY{0};
  int m_logicalWidth{1};
  int m_logicalHeight{1};
  bool m_leftPressed{false};
  bool m_rightPressed{false};
  bool m_prevLeftPressed{false};
  bool m_prevRightPressed{false};
};

/// \}

}  // namespace cen

#endif  // CENTURION_MOUSE_STATE_HEADER