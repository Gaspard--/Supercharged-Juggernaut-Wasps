#ifndef SOUNDHANDLER_HPP_
#define SOUNDHANDLER_HPP_

# include <SFML/Audio.hpp>
# include <map>

class SoundHandler
{
public:
  enum sfxList
  {
    waspTakeHit,
    mobTakeHit
  };

  static void initSoundHandler();
  static SoundHandler& getInstance();
  static void destroySoundHandler();

  void playMainMusic();
  void playSound(sfxList, float = 100.0f);
  void deleteSounds();
  void addSoundBuffer(sfxList, std::string const&);
  sf::SoundBuffer* getSoundBuffer(sfxList) const;

  void setGlobalPitch(float pitch);

private:
  std::map<sfxList, sf::SoundBuffer*> soundList;
  SoundHandler();

  static std::unique_ptr<SoundHandler> _instance;

  sf::Music mainMusic;
  std::map<sfxList, sf::SoundBuffer*> _sound;
  std::vector<std::unique_ptr<sf::Sound>> _soundsPlaying;

};

#endif
