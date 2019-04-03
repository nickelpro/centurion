#include "application_path.h"
#include <SDL_filesystem.h>
#include <SDL_stdinc.h>
#include <string>

using std::string;

namespace centurion {
namespace tools {

ApplicationPath::ApplicationPath() {
  path_cstr = SDL_GetBasePath();
  path_cpp = string(path_cstr);
}

ApplicationPath::~ApplicationPath() {
  //path_cpp.~string();
  SDL_free(path_cstr);
}

}  // namespace tools
}  // namespace centurion