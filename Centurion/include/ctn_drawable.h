#pragma once
#include <memory>
#include "renderer.h"

namespace centurion {
namespace visuals {

/**
\brief The IDrawable class is an interface for objects that may perform
rendering.
\since 1.0.0
*/
class IDrawable {
 protected:
  IDrawable() = default;

 public:
  virtual ~IDrawable() = default;

  /**
  \brief Performs the rendering operations specified by this IDrawable.
  \param renderer - The Renderer that will be used for the rendering.
  \since 1.0.0
  */
  virtual void Draw(centurion::visuals::Renderer& renderer) = 0;
};

typedef std::shared_ptr<IDrawable> IDrawable_sptr;
typedef std::unique_ptr<IDrawable> IDrawable_uptr;
typedef std::weak_ptr<IDrawable> IDrawable_wptr;

}  // namespace visuals
}  // namespace centurion