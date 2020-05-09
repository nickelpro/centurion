#ifndef CENTURION_MUSIC_SOURCE
#define CENTURION_MUSIC_SOURCE

#include "music.h"

#include "centurion_exception.h"
#include "centurion_utils.h"
#include "error.h"

namespace centurion {
namespace audio {

CENTURION_DEF
Music::Music(const std::string& file)
{
  m_music = Mix_LoadMUS(file.c_str());
  if (!m_music) {
    throw CenturionException{"Failed to create music! " + Error::msg()};
  }
}

CENTURION_DEF
Music::Music(Music&& other) noexcept
{
  Mix_FreeMusic(m_music);

  m_music = other.m_music;
  other.m_music = nullptr;
}

CENTURION_DEF
Music::~Music() noexcept
{
  if (m_music) {
    Mix_FreeMusic(m_music);
  }
}

CENTURION_DEF
Music& Music::operator=(Music&& other) noexcept
{
  Mix_FreeMusic(m_music);

  m_music = other.m_music;
  other.m_music = nullptr;

  return *this;
}

CENTURION_DEF
std::unique_ptr<Music> Music::unique(const std::string& file)
{
  return centurion::detail::make_unique<Music>(file);
}

CENTURION_DEF
std::shared_ptr<Music> Music::shared(const std::string& file)
{
  return std::make_shared<Music>(file);
}

CENTURION_DEF
void Music::play(int nLoops) noexcept
{
  if (nLoops < -1) {
    nLoops = -1;
  }
  Mix_PlayMusic(m_music, nLoops);
}

CENTURION_DEF
void Music::resume() noexcept
{
  Mix_ResumeMusic();
}

CENTURION_DEF
void Music::pause() noexcept
{
  Mix_PauseMusic();
}

CENTURION_DEF
void Music::halt() noexcept
{
  Mix_HaltMusic();
}

CENTURION_DEF
void Music::fade_in(int ms, int nLoops) noexcept
{
  if (ms < 0) {
    ms = 0;
  }
  if (nLoops < -1) {
    nLoops = -1;
  }
  Mix_FadeInMusic(m_music, nLoops, ms);
}

CENTURION_DEF
void Music::fade_out(int ms)
{
  if (fading()) {
    return;
  }
  if (ms < 0) {
    ms = 0;
  }
  Mix_FadeOutMusic(ms);
}

CENTURION_DEF
void Music::set_volume(int volume) noexcept
{
  if (volume > MIX_MAX_VOLUME) {
    volume = MIX_MAX_VOLUME;
  } else if (volume < 0) {
    volume = 0;
  }
  Mix_VolumeMusic(volume);
}

CENTURION_DEF
bool Music::playing() noexcept
{
  return Mix_PlayingMusic();
}

CENTURION_DEF
bool Music::paused() noexcept
{
  return Mix_PausedMusic();
}

CENTURION_DEF
bool Music::fading() noexcept
{
  const auto status = fade_status();
  return status == FadeStatus::In || status == FadeStatus::Out;
}

CENTURION_DEF
int Music::volume() noexcept
{
  return Mix_VolumeMusic(-1);
}

CENTURION_DEF
FadeStatus Music::fade_status() noexcept
{
  return static_cast<FadeStatus>(Mix_FadingMusic());
}

CENTURION_DEF
MusicType Music::music_type() const noexcept
{
  return static_cast<MusicType>(Mix_GetMusicType(m_music));
}

CENTURION_DEF
std::string Music::to_string() const
{
  return "[Music@" + detail::address_of(this) + "]";
}

CENTURION_DEF
bool operator==(FadeStatus lhs, Mix_Fading rhs) noexcept
{
  return static_cast<Mix_Fading>(lhs) == rhs;
}

CENTURION_DEF
bool operator==(Mix_Fading lhs, FadeStatus rhs) noexcept
{
  return lhs == static_cast<Mix_Fading>(rhs);
}

CENTURION_DEF
bool operator==(MusicType lhs, Mix_MusicType rhs) noexcept
{
  return static_cast<Mix_MusicType>(lhs) == rhs;
}

CENTURION_DEF
bool operator==(Mix_MusicType lhs, MusicType rhs) noexcept
{
  return lhs == static_cast<Mix_MusicType>(rhs);
}

}  // namespace audio
}  // namespace centurion

#endif  // CENTURION_MUSIC_SOURCE