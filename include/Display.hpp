#pragma once

# include "opengl/my_opengl.hpp"
# include "opengl/RenderContext.hpp"
# include "my_glfw.hpp"
# include "DisplayData.hpp"
# include "SpriteManager.hpp"
# include "FreeTypeLib.hpp"

class Logic;

class Display
{
  GLFWwindow *window;
  opengl::RenderContext textureContext;
  opengl::RenderContext rectContext;
  opengl::RenderContext textContext;
  opengl::RenderContext bulletContext;
  opengl::Buffer textureBuffer;
  opengl::Buffer rectBuffer;
  opengl::Buffer textBuffer;
  opengl::Buffer bulletBuffer;
  claws::vect<uint32_t, 2u> size{0u, 0u};
  claws::vect<float, 2u> dim{1.0f, 0.0f};
  SpriteManager spriteManager;
  FreeTypeLib fontHandler;

  void renderSingleAnim(AnimInfo const &anim, SpriteId spriteId);
  void renderBigWasp(BigWasp const &bigWasp);
  void renderSmolWasp(SmolWasp const &smolWasp);
  void renderColors(std::vector<ColorInfo> const &colorInfos);
  void renderAnims(std::vector<AnimInfo> const &, SpriteId);
  void renderRotatedAnims(std::vector<RotatedAnimInfo> const &, SpriteId);
  void renderText(std::string const &txt, unsigned int fontSize, claws::vect<float, 2u> step, claws::vect<float, 2u> textPos, claws::vect<float, 3u> color);
  void renderHud(float bigWaspSize, uint32_t score, std::string const &strTime, float time);
  void renderGameOver(uint32_t score, std::string const &strTime);
public:

  Display(GLFWwindow&);
  Display(Display const &) = delete;
  Display(Display &&) = delete;

  void render(DisplayData const &);
  void resize(claws::vect<uint32_t, 2u> size);

  GLFWwindow *getWindow() const
  {
    return window;
  }

  bool isRunning() const
  {
    return (!glfwWindowShouldClose(window));
  }
};
