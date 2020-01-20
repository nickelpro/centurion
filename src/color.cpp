#include "color.h"
#include <type_traits>

namespace centurion {

static_assert(std::is_convertible_v<Color, SDL_Color>);
static_assert(std::is_convertible_v<Color, SDL_MessageBoxColor>);

static_assert(std::is_default_constructible_v<Color>);

static_assert(std::is_nothrow_copy_constructible_v<Color>);
static_assert(std::is_nothrow_copy_assignable_v<Color>);

static_assert(std::is_nothrow_move_constructible_v<Color>);
static_assert(std::is_nothrow_move_assignable_v<Color>);

const uint8_t Color::max = 0xFF;

Color::Color(Color&& other) noexcept
    : red{other.red}, green{other.green}, blue{other.blue}, alpha{other.alpha} {}

Color::Color(const SDL_Color& color) noexcept
    : red{color.r}, green{color.g}, blue{color.b}, alpha{color.a} {}

Color::Color(SDL_Color&& color) noexcept
    : red{color.r}, green{color.g}, blue{color.b}, alpha{color.a} {}

Color::Color(const SDL_MessageBoxColor& color) noexcept
    : red{color.r}, green{color.g}, blue{color.b}, alpha{max} {}

Color::Color(SDL_MessageBoxColor&& color) noexcept
    : red{color.r}, green{color.g}, blue{color.b}, alpha{max} {}

Color& Color::operator=(const Color& other) noexcept {
  this->red = other.red;
  this->green = other.green;
  this->blue = other.blue;
  this->alpha = other.alpha;
  return *this;
}

Color& Color::operator=(Color&& other) noexcept {
  this->red = other.red;
  this->green = other.green;
  this->blue = other.blue;
  this->alpha = other.alpha;
  return *this;
}

void Color::set_red(uint8_t r) noexcept {
  this->red = r;
}

void Color::set_green(uint8_t g) noexcept {
  this->green = g;
}

void Color::set_blue(uint8_t b) noexcept {
  this->blue = b;
}

void Color::set_alpha(uint8_t a) noexcept {
  this->alpha = a;
}

std::ostream& operator<<(std::ostream& ostream, const Color& color) noexcept {
  ostream << "(Color | R: " << std::to_string(color.get_red())
          << ", G: " << std::to_string(color.get_green())
          << ", B: " << std::to_string(color.get_blue())
          << ", A: " << std::to_string(color.get_alpha())
          << ")";
  return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const SDL_Color& sdlColor) noexcept {
  ostream << "(SDL_Color | R: " << std::to_string(sdlColor.r)
          << ", G: " << std::to_string(sdlColor.g)
          << ", B: " << std::to_string(sdlColor.b)
          << ", A: " << std::to_string(sdlColor.a)
          << ")";
  return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const SDL_MessageBoxColor& msgColor) noexcept {
  ostream << "(SDL_MessageBoxColor | R: " << std::to_string(msgColor.r)
          << ", G: " << std::to_string(msgColor.g)
          << ", B: " << std::to_string(msgColor.b)
          << ")";
  return ostream;
}

}