#include "log.h"
#include <cstdarg>

namespace centurion {

void Log::msg(Category category, Priority prio, const char* fmt, ...) noexcept {
  if (!fmt) { return; }
  std::va_list args;
  va_start(args, fmt);
  SDL_LogMessageV(static_cast<int>(category), static_cast<SDL_LogPriority>(prio), fmt, args);
}

void Log::msg(Category category, const char* fmt, ...) noexcept {
  if (!fmt) { return; }
  std::va_list args;
  va_start(args, fmt);

  const auto category_id = static_cast<int>(category);
  const auto prio = SDL_LogGetPriority(category_id);

  SDL_LogMessageV(static_cast<int>(category), prio, fmt, args);
}

void Log::msg(const char* fmt, ...) noexcept {
  if (!fmt) { return; }
  std::va_list args;
  va_start(args, fmt);

  SDL_LogMessageV(static_cast<int>(Category::App),
                  static_cast<SDL_LogPriority>(Priority::Info),
                  fmt,
                  args);
}

void Log::set_priority(Category category, Priority prio) noexcept {
  SDL_LogSetPriority(static_cast<int>(category), static_cast<SDL_LogPriority>(prio));
}

void Log::set_priority(Priority prio) noexcept {
  const auto p = static_cast<SDL_LogPriority>(prio);
  SDL_LogSetAllPriority(p);
  SDL_LogSetPriority(SDL_LOG_CATEGORY_TEST, p); // Apparently not set by SDL
}

Priority Log::get_priority(Category category) noexcept {
  return static_cast<Priority>(SDL_LogGetPriority(static_cast<int>(category)));
}

}