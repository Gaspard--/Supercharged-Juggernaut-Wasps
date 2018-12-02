#include <memory>
#include "SoundHandler.hpp"

std::unique_ptr<SoundHandler> SoundHandler::_instance(nullptr);

SoundHandler::SoundHandler() {}

void SoundHandler::initSoundHandler()
{
  _instance.reset(new SoundHandler());
  if (!_instance->mainMusic.openFromFile("resources/Wasp in the frost.wav"))
    throw ("Music not charged");
  _instance->addSoundBuffer(waspTakeHit, "resources/wasp.ogg");
  _instance->addSoundBuffer(mobTakeHit, "resources/mobKill.ogg");
}

SoundHandler& SoundHandler::getInstance()
{
  return *_instance;
}

void SoundHandler::destroySoundHandler()
{
  _instance.reset(nullptr);
}

void SoundHandler::playMainMusic()
{
  mainMusic.setLoop(true);
  mainMusic.setVolume(25);
  mainMusic.play();
}

void SoundHandler::playSound(sfxList id, float volume)
{
  auto sound = std::make_unique<sf::Sound>();
  sound->setBuffer(*_instance->getSoundBuffer(id));
  sound->setLoop(false);
  sound->setVolume(volume);
  sound->setPitch(sfxPitch);
  sound->play();
  _instance->_soundsPlaying.push_back(std::move(sound));
}

void SoundHandler::deleteSounds()
{
  _soundsPlaying.erase(std::remove_if(_soundsPlaying.begin(), _soundsPlaying.end(),
            [](auto const &elem)
            {
        return elem->getStatus() != sf::SoundSource::Status::Playing;
            }),
         _soundsPlaying.end());
}

void SoundHandler::addSoundBuffer(sfxList id, std::string const& path)
{
  _instance->_sound[id] = new sf::SoundBuffer();
  _instance->_sound[id]->loadFromFile(path);
}

sf::SoundBuffer* SoundHandler::getSoundBuffer(sfxList id) const
{
  return (_instance->_sound[id]);
}

void SoundHandler::setGlobalPitch(float pitch)
{
  sfxPitch = pitch;
  mainMusic.setPitch(pitch);
}
