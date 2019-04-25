#pragma once
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <memory>
#include <string>
#include "ctn_color.h"
#include "ctn_dimensioned.h"
#include "ctn_font.h"
#include "ctn_point.h"
#include "ctn_positionable.h"
#include "ctn_rectangle.h"
#include "ctn_texture_interface.h"

namespace centurion {
namespace visuals {

class Renderer;
typedef std::shared_ptr<Renderer> Renderer_sptr;
typedef std::unique_ptr<Renderer> Renderer_uptr;
typedef std::weak_ptr<Renderer> Renderer_wptr;

/**
\brief The Renderer class provides rendering functionality for a Window.
\since 1.0.0
*/
class Renderer {
 private:
  SDL_Renderer* sdlRenderer;
  Font_sptr font;
  Color color;

  void CheckRenderDimensions(int width, int height);

  void UpdateColor() noexcept;

  SDL_Texture* CreateSDLTextureFromString(const std::string& str, int* width,
                                          int* height);

 public:
  /**
  \param renderer - a pointer to the SDL_Renderer that the Renderer will be
  based upon.
  */
  explicit Renderer(SDL_Renderer* renderer);

  ~Renderer();

  /**
  \brief Updates the state of this Renderer. This method has to be called in
  order to apply any previous rendering operations.
  \since 1.0.0
  */
  void Update() noexcept;

  /**
  \brief Clears the rendering target with the currently selected color.
  \since 1.0.0
  */
  void Clear() noexcept;

  /**
  \brief Renders an SDL_Texture.
  \param texture - a pointer to the SDL_Texture to render.
  \param x - the x-coordinate of the rendered SDL_Texture.
  \param y - the y-coordinate of the rendered SDL_Texture.
  \param w - the width of the rendered SDL_Texture.
  \param h - the height of the rendered SDL_Texture.
  \since 2.0.0
  */
  void Render(SDL_Texture* texture, int x, int y, int w, int h);

  /**
  \brief Renders a texture to the rendering target.
  \param texture - the texture that will be rendered.
  \param x - the desired x-coordinate.
  \param y - the desired y-coordinate.
  \param w - the desired width of the image.
  \param h - the desired height of the image.
  \since 1.0.0
  */
  void Render(ITexture& texture, int x, int y, int w, int h);

  /**
  \brief Renders a texture to the rendering target.
  \param texture - the texture that will be rendered.
  \param rect - the Rectangle that provides the dimensions for the rendered
  image.
  \since 1.0.0
  */
  void Render(ITexture& texture, centurion::geo::Rectangle rect);

  /**
  \brief Renders a texture to the rendering target.
  \param texture - the texture that will be rendered.
  \param x - the desired x-coordinate.
  \param y - the desired y-coordinate.
  \since 1.0.0
  */
  void Render(ITexture& texture, int x, int y);

  /**
  \brief Renders a filled rectangle with the currently selected color.
  \param x - the desired x-coordinate.
  \param y - the desired y-coordinate.
  \param w - the desired width of the rectangle.
  \param h - the desired height of the rectangle.
  \since 1.0.0
  */
  void RenderFilledRect(int x, int y, int w, int h);

  /**
  \brief Renders a filled rectangle with the currently selected color.
  \param rect - the rectangle that will be rendered.
  \since 1.0.0
  */
  void RenderFilledRect(centurion::geo::Rectangle rect);

  /**
  \brief Renders an outlined rectangle with the currently selected color.
  \param x - the desired x-coordinate.
  \param y - the desired y-coordinate.
  \param w - the desired width of the rectangle.
  \param h - the desired height of the rectangle.
  \since 1.0.0
  */
  void RenderOutlinedRect(int x, int y, int w, int h);

  /**
  \brief Renders an outlined rectangle with the currently selected color.
  \param rect - the rectangle that will be rendered.
  \since 1.0.0
  */
  void RenderOutlinedRect(centurion::geo::Rectangle rect);

  /**
  \brief Renders a line between two points with the currently selected
  color.
  \param x1 - the x-coordinate of the first point.
  \param y1 - the y-coordinate of the first point.
  \param x2 - the x-coordinate of the second point.
  \param y2 - the y-coordinate of the second point.
  \since 1.0.0
  */
  void RenderLine(int x1, int y1, int x2, int y2);

  /**
  \brief Renders a line between two points with the currently selected color.
  \param p1 - the first point.
  \param p2 - the second point.
  \since 1.0.0
  */
  void RenderLine(centurion::geo::Point p1, centurion::geo::Point p2);

  /**
  \brief Renders the supplied string to the rendering target. This function is
  quite expensive, it's preferable to use the CreateTextureFromString()-method
  instead.
  \param str - the string that will be rendered.
  \param x - the x-coordinate of the rendered text.
  \param y - the y-coordinate of the rendered text.
  \since 1.0.0
  */
  void RenderString(const std::string& str, int x, int y);

  /**
  \brief Assigns the currently active font.
  \param font - a pointer to the font instance that will be used.
  \since 1.0.0
  */
  void SetFont(Font_sptr font);

  /**
  \brief Assigns the active color.
  \param color - the color that will be used.
  \since 1.0.0
  */
  void SetColor(Color color) noexcept;

  /**
  \brief Assigns a Texture instance as the rendering target.
  \param texture - the texture that will be the new rendering target, nullptr
  resets the rendering target.
  \since 1.2.0
  */
  void SetRenderTarget(ITexture_sptr texture) noexcept;

  /**
  \brief Creates a texture of the supplied string, using the currently selected
  font.
  \param str - the string that the created texture will represent.
  \since 1.0.0
  */
  ITexture_sptr CreateTextureFromString(const std::string& str);

  /**
  \brief Creates and returns a subtexture from the supplied texture.
  \param base - a pointer to the texture that the subtexture will be based on.
  \param src - the source rectangle.
  \param dst - the destination rectangle.
  \param pixelFormat - the pixel format used for the subtexture.
  \since 1.2.0
  */
  ITexture_sptr CreateSubtexture(ITexture_sptr base,
                                 centurion::geo::Rectangle src,
                                 centurion::geo::Rectangle dst,
                                 Uint32 pixelFormat);

  /**
  \brief Creates and returns an empty texture.
  \param width - the width of the created texture.
  \param height - the height of the created texture.
  \param access - the access of the created texture.
  \since 1.2.0
  */
  ITexture_sptr CreateRawTexture(int width, int height, Uint32 pixelFormat,
                                 SDL_TextureAccess access);

  /**
  \brief Returns the internal representation of this Renderer. DO NOT use the
  returned pointer to call SDL_DestroyRenderer().
  \since 1.0.0
  */
  inline SDL_Renderer* GetSDLVersion() const { return sdlRenderer; }

  /**
  \brief Returns a shared pointer to a Renderer instance.
  \param renderer - a pointer to the SDL_Renderer that the Renderer will be
  based upon.
  \since 1.1.0
  */
  static Renderer_sptr CreateShared(SDL_Renderer* renderer);

  /**
  \brief Returns a unique pointer to a Renderer instance.
  \param renderer - a pointer to the SDL_Renderer that the Renderer will be
  based upon.
  \since 1.1.0
  */
  static Renderer_uptr CreateUnique(SDL_Renderer* renderer);

  /**
  \brief Returns a weak pointer to a Renderer instance.
  \param renderer - a pointer to the SDL_Renderer that the Renderer will be
  based upon.
  \since 1.1.0
  */
  static Renderer_wptr CreateWeak(SDL_Renderer* renderer);
};

}  // namespace visuals
}  // namespace centurion