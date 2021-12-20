#ifndef CENTURION_EVENTS_WINDOW_EVENT_HPP_
#define CENTURION_EVENTS_WINDOW_EVENT_HPP_

#include <SDL.h>

#include "event_base.hpp"
#include "window_event_id.hpp"

namespace cen {

/// \addtogroup event
/// \{

/**
 * \class window_event
 *
 * \brief Represents an event that is associated with an action related to a
 * window.
 *
 * \see `SDL_WindowEvent`
 *
 * \since 4.0.0
 */
class window_event final : public EventBase<SDL_WindowEvent> {
 public:
  /**
   * \brief Creates a window event.
   *
   * \since 4.0.0
   */
  window_event() noexcept : EventBase{EventType::Window} {}

  /**
   * \brief Creates a window event based on the supplied SDL window event.
   *
   * \param event the SDL window event that will be copied.
   *
   * \since 4.0.0
   */
  explicit window_event(const SDL_WindowEvent& event) noexcept : EventBase{event} {}

  /**
   * \brief Returns the event ID of this window event.
   *
   * \details There are many different kinds of window events, use this function to check
   * what kind of action that triggered this event.
   *
   * \return the event ID of this window event.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto event_id() const noexcept -> window_event_id
  {
    return static_cast<window_event_id>(m_event.event);
  }

  /**
   * \brief Returns the value of the first data value.
   *
   * \details The meaning of this value is dependent on the window event ID of this window
   * event.
   *
   * For instance, if the event ID is `window_event_id::size_changed`, then data1 and
   * data2 represent the new width and height of the window respectively. See the
   * `window_event_id` documentation for more details about whether the value returned
   * from this function is meaningful in regard to the window event ID.
   *
   * \return the value of the first data value.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto data_1() const noexcept -> Sint32 { return m_event.data1; }

  /**
   * \brief Returns the value of the second data value.
   *
   * \details The meaning of this value is dependent on the window event ID of this window
   * event.
   *
   * For instance, if the event ID is `window_event_id::size_changed`, then data1 and
   * data2 represent the new width and height of the window respectively. See the
   * `window_event_id` documentation for more details about whether the value returned
   * from this function is meaningful in regard to the window event ID.
   *
   * \return the value of the second data value.
   *
   * \since 4.0.0
   */
  [[nodiscard]] auto data_2() const noexcept -> Sint32 { return m_event.data2; }
};

/// \name SDL event conversions
/// \{

template <>
inline auto AsSDLEvent(const EventBase<SDL_WindowEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.window = event.get();
  return e;
}

/// \} End of SDL event conversions

/// \} End of group event

}  // namespace cen

#endif  // CENTURION_EVENTS_WINDOW_EVENT_HPP_
