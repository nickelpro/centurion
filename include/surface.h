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

#ifndef CENTURION_SURFACE_HEADER
#define CENTURION_SURFACE_HEADER

#include <SDL.h>

#include <cstdint>
#include <gsl-lite.hpp>
#include <type_traits>

#include "blend_mode.h"
#include "centurion_api.h"
#include "color.h"

namespace centurion {
namespace video {

class Texture;

/**
 * The <code>Surface</code> class represents a non-accelerated image. In most
 * cases, it's preferable to use the <code>Image</code> class instead.
 *
 * @since 4.0.0
 */
class Surface final {  // TODO rename Image to Texture
 private:
  SDL_Surface* surface = nullptr;

  /**
   * Destroys the associated SDL_Surface. This method has no effect if the
   * associated surface is null.
   *
   * @since 4.0.0
   */
  void destroy() noexcept;

  /**
   * Creates a copy of the internal SDL_Surface, and returns a pointer to it.
   *
   * @return a copy of the internal SDL_Surface, the returned pointer won't be
   * null.
   * @throws CenturionException if the copy couldn't be created.
   * @since 4.0.0
   */
  SDL_Surface* copy_surface() const;

 public:
  /**
   * @param file the file path of the image file that will be loaded.
   * @throws CenturionException if the surface cannot be created.
   * @since 4.0.0
   */
  CENTURION_API
  explicit Surface(const char* file);

  /**
   * @param surface a pointer to the SDL_Surface that will be used to create the
   * surface. Mustn't be null.
   * @throws CenturionException if the supplied pointer is null.
   * @since 4.0.0
   */
  CENTURION_API
  explicit Surface(gsl::owner<SDL_Surface*> surface);

  /**
   * Creates a copy of the supplied surface.
   *
   * @param other the surface that will be copied.
   * @throws CenturionException if the supplied surface couldn't be copied.
   * @since 4.0.0
   */
  CENTURION_API
  Surface(const Surface& other);

  /**
   * Creates a surface by moving the supplied surface.
   *
   * @param other the surface that will be moved.
   * @since 4.0.0
   */
  CENTURION_API
  Surface(Surface&& other) noexcept;

  /**
   * Copies the supplied surface and saves the result in the invoked surface.
   *
   * @param other the surface that will be copied.
   * @throws CenturionException if the supplied surface couldn't be copied.
   * @since 4.0.0
   */
  CENTURION_API
  Surface& operator=(const Surface& other);

  /**
   * Moves the supplied surface into this surface.
   *
   * @param other the surface that will be moved.
   * @since 4.0.0
   */
  CENTURION_API
  Surface& operator=(Surface&& other) noexcept;

  CENTURION_API
  ~Surface() noexcept;

  /**
   * Sets the alpha component modulation value.
   *
   * @param alpha the new alpha component value, in the range [0, 255].
   * @since 4.0.0
   */
  CENTURION_API void set_alpha(uint8_t alpha) noexcept;

  /**
   * Sets the color modulation that will be used by the surface.
   *
   * @param color the color that represents the color modulation that will be
   * used.
   * @since 4.0.0
   */
  CENTURION_API void set_color_mod(const Color& color) noexcept;

  /**
   * Sets the blend mode that will be used by the surface.
   *
   * @param mode the blend mode that will be used.
   * @since 4.0.0
   */
  CENTURION_API void set_blend_mode(BlendMode mode) noexcept;

  /**
   * Returns the alpha component modulation of the surface.
   *
   * @return the alpha modulation value, in the range [0, 255].
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  uint8_t get_alpha() const noexcept;

  /**
   * Returns the color modulation of the surface.
   *
   * @return a color that represents the color modulation of the surface.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  Color get_color_mod() const noexcept;

  /**
   * Returns the blend mode that is being used by the surface.
   *
   * @return the blend mode that the surface uses.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  BlendMode get_blend_mode() const noexcept;

  /**
   * Returns the width of the surface.
   *
   * @return the width of the surface.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  int get_width() const noexcept;

  /**
   * Returns the height of the surface.
   *
   * @return the height of the surface.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  int get_height() const noexcept;

  /**
   * Returns the pitch of the surface.
   *
   * @return the pitch of the surface.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  int get_pitch() const noexcept;

  /**
   * Creates and returns a copy of this surface.
   *
   * @return a copy of the surface.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  Surface duplicate() const;

  /**
   * Converts the surface into its texture equivalent.
   *
   * @param renderer a pointer to the renderer that will be used to create the
   * texture, shouldn't be null.
   * @return a texture that is equivalent to the surface.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  Texture to_texture(SDL_Renderer* renderer) const noexcept;

  /**
   * Returns a pointer to the internal SDL_Surface instance. Don't take
   * ownership of the returned pointer, or bad things will happen.
   *
   * @return a pointer to the internal SDL_Surface instance.
   * @since 4.0.0
   */
  CENTURION_NODISCARD
  CENTURION_API
  SDL_Surface* get_internal() const noexcept;
};

static_assert(!std::is_nothrow_copy_constructible<Surface>::value,
              "Surface isn't nothrow copy constructible!");

static_assert(!std::is_nothrow_copy_assignable<Surface>::value,
              "Surface isn't nothrow copy assignable!");

static_assert(std::is_nothrow_move_constructible<Surface>::value,
              "Surface isn't nothrow move constructible!");

static_assert(std::is_nothrow_move_assignable<Surface>::value,
              "Surface isn't nothrow move assignable!");

}  // namespace video
}  // namespace centurion

#ifdef CENTURION_HEADER_ONLY
#include "surface.cpp"
#endif

#endif  // CENTURION_SURFACE_HEADER