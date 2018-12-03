#include "SpriteManager.hpp"
#include "opengl/load_image.hpp"

#include <cassert>
#include <utility>

inline Animation loadAnim(char const *path, uint32_t imageCount)
{
  return {opengl::loadTexture(path), imageCount};
}

template<size_t index>
inline Animation getAnimation()
{
  switch (SpriteId(index))
    {
    case SpriteId::SmolWaspIdle:
      return loadAnim("resources/testWasp.bmp", 4);
      break;
    case SpriteId::WaspAbdomen:
      return loadAnim("resources/wasp abdomen mirrored.bmp", 1);
      break;
    case SpriteId::WaspBody:
      return loadAnim("resources/wasp body.bmp", 1);
      break;
    case SpriteId::WaspHead:
      return loadAnim("resources/wasp head.bmp", 1);
      break;
    case SpriteId::WaspLegs:
      return loadAnim("resources/wasp legs.bmp", 1);
      break;
    case SpriteId::Fireball :
      return loadAnim("resources/fireball-spriteSheet.bmp", 5);
      break;
    case SpriteId::Libeflux :
      return loadAnim("resources/libeflux-spriteSheet.bmp", 16);
      break;
    case SpriteId::Monarch :
      return loadAnim("resources/monarch-spriteSheet.bmp", 8);
      break;
    case SpriteId::SmolWasp :
      return loadAnim("resources/smolWasp-spriteSheet.bmp", 6);
      break;
    default:
      assert("missing animation definition");
    }
}

template<size_t... indices>
SpriteManager::SpriteManager(std::index_sequence<indices...>)
  : animations{getAnimation<indices>()...}
{
}

Animation const &SpriteManager::operator[](SpriteId spriteId) const noexcept
{
  return animations[uint32_t(spriteId)];
}

SpriteManager::SpriteManager()
  : SpriteManager(std::make_index_sequence<size_t(SpriteId::SpriteCount)>())
{
}

