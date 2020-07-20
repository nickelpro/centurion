#ifndef CENTURION_CENTURION_SOURCE
#define CENTURION_CENTURION_SOURCE

#include "centurion.hpp"

#include "error.hpp"

namespace centurion {

CENTURION_DEF
void centurion_lib::init_sdl()
{
  const auto result = SDL_Init(m_cfg.coreFlags);
  if (result < 0) {
    throw detail::core_error("Failed to load SDL2!");
  }
}

CENTURION_DEF
void centurion_lib::init_ttf()
{
  const auto result = TTF_Init();
  if (result == -1) {
    throw detail::ttf_error("Failed to load SDL2_ttf!");
  }
}

CENTURION_DEF
void centurion_lib::init_img()
{
  const auto flags = IMG_Init(m_cfg.imageFlags);
  if (!flags) {
    throw detail::img_error("Failed to load SDL2_image!");
  }
}

CENTURION_DEF
void centurion_lib::init_mix()
{
  const auto flags = Mix_Init(m_cfg.mixerFlags);
  if (!flags) {
    throw detail::mix_error("Failed to load SDL2_mixer!");
  }

  if (Mix_OpenAudio(m_cfg.mixerFreq,
                    m_cfg.mixerFormat,
                    m_cfg.mixerChannels,
                    m_cfg.mixerChunkSize) == -1) {
    throw detail::mix_error("Failed to open audio!");
  }
}

CENTURION_DEF
centurion_lib::centurion_lib()
{
  init();
}

CENTURION_DEF
centurion_lib::centurion_lib(const centurion_config& cfg) : m_cfg{cfg}
{
  init();
}

CENTURION_DEF
centurion_lib::~centurion_lib() noexcept
{
  close();
}

CENTURION_DEF
void centurion_lib::init()
{
  if (m_cfg.initCore) {
    init_sdl();
  }

  if (m_cfg.initImage) {
    try {
      init_img();
    } catch (...) {
      SDL_Quit();
    }
  }

  if (m_cfg.initTTF) {
    try {
      init_ttf();
    } catch (...) {
      IMG_Quit();
      SDL_Quit();
    }
  }

  if (m_cfg.initMixer) {
    try {
      init_mix();
    } catch (...) {
      TTF_Quit();
      IMG_Quit();
      SDL_Quit();
    }
  }
}

CENTURION_DEF
void centurion_lib::close() noexcept
{
  if (m_cfg.initImage) {
    IMG_Quit();
  }

  if (m_cfg.initTTF) {
    TTF_Quit();
  }

  if (m_cfg.initMixer) {
    Mix_CloseAudio();
    Mix_Quit();
  }

  if (m_cfg.initCore) {
    SDL_Quit();
  }
}

}  // namespace centurion

#endif  // CENTURION_CENTURION_SOURCE