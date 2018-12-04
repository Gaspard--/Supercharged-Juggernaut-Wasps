#include "Display.hpp"
#include "Logic.hpp"
#include "opengl/RenderContext.hpp"
#include "Bind.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

inline opengl::RenderContext contextFromFiles(std::string name)
{
  std::stringstream vert;
  std::stringstream frag;
  {
    std::ifstream vertInput("shaders/" + name + ".vert");
    std::ifstream fragInput("shaders/" + name + ".frag");

    if (!fragInput || !vertInput)
      {
	std::cout << "shaders/" + name + ".vert" << std::endl;
	std::cout << "shaders/" + name + ".frag" << std::endl;
	throw std::runtime_error("Failed to load shaders");
      }
    vert << vertInput.rdbuf();
    frag << fragInput.rdbuf();
  }
  return {opengl::Vao(), opengl::createProgram<2>({{opengl::createShader(static_cast<unsigned int>(GL_VERTEX_SHADER), vert.str().c_str()),
					            opengl::createShader(static_cast<unsigned int>(GL_FRAGMENT_SHADER), frag.str().c_str())}})};
}

Display::Display(GLFWwindow &window)
  : window(&window)
  , textureContext(contextFromFiles("texture"))
  , rectContext(contextFromFiles("rect"))
  , textContext(contextFromFiles("text"))
  , bulletContext(contextFromFiles("bullet"))
  , fontHandler("./resources/FantasqueSansMono-Regular.ttf")
{

  {
    Bind bind(textureContext);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    uint32_t attrib0 = textureContext.program.getAttribLocation("pos");
    uint32_t attrib1 = textureContext.program.getAttribLocation("coord");
    glEnableVertexAttribArray(attrib0);
    glEnableVertexAttribArray(attrib1);
    glVertexAttribPointer(attrib0, 2, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glVertexAttribPointer(attrib1, 2, GL_FLOAT, false, 4 * sizeof(float), reinterpret_cast<void *>(2u * sizeof(float)));
  }
  {
    Bind bind(bulletContext);

    glBindBuffer(GL_ARRAY_BUFFER, bulletBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    uint32_t attrib0 = bulletContext.program.getAttribLocation("pos");
    uint32_t attrib1 = bulletContext.program.getAttribLocation("color");
    glEnableVertexAttribArray(attrib0);
    glEnableVertexAttribArray(attrib1);
    glVertexAttribPointer(attrib0, 2, GL_FLOAT, false, 6 * sizeof(float), nullptr);
    glVertexAttribPointer(attrib1, 4, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void *>(2u * sizeof(float)));
  }
  {
    Bind bind(rectContext);

    glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    uint32_t attrib0 = rectContext.program.getAttribLocation("pos");
    glEnableVertexAttribArray(attrib0);
    glVertexAttribPointer(attrib0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
  }
  {
    Bind bind(textContext);

    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    uint32_t attrib0 = textContext.program.getAttribLocation("pos");
    uint32_t attrib1 = textContext.program.getAttribLocation("coord");
    glEnableVertexAttribArray(attrib0);
    glEnableVertexAttribArray(attrib1);
    glVertexAttribPointer(attrib0, 2, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glVertexAttribPointer(attrib1, 2, GL_FLOAT, false, 4 * sizeof(float), reinterpret_cast<void *>(2u * sizeof(float)));
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

void Display::renderSingleAnim(AnimInfo const &anim, SpriteId spriteId)
{
  {
    Bind bind(textureContext);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

    std::array<float, 6 * 4> data;

    std::array<float, 12> corner{{0.0f, 0.0f,
				  1.0f, 0.0f,
				  0.0f, 1.0f,
				  1.0f, 0.0f,
				  0.0f, 1.0f,
				  1.0f, 1.0f}};

    for (uint32_t i(0u); i != 6; ++i)
      {
	for (uint32_t j(0u); j != 2; ++j)
	  data[i * 4 + j] = (anim.destMax[j] * corner[i * 2 + j] + anim.destMin[j] * (1.0f - corner[i * 2 + j]));
	data[i * 4 + 2] = (corner[i * 2]);
	data[i * 4 + 3] = ((corner[i * 2 + 1] + float(anim.frame)) / float(spriteManager[spriteId].imageCount));
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    opengl::setUniform(dim, "dim", textureContext.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteManager[spriteId].texture);
    opengl::setUniform(0u, "tex", textureContext.program);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}

void Display::renderBack(float timer)
{
  {
    Bind bind(textureContext);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

    std::array<float, 6 * 4> data;

    std::array<float, 12> corner{{0.0f, 0.0f,
				  1.0f, 0.0f,
				  0.0f, 1.0f,
				  1.0f, 0.0f,
				  0.0f, 1.0f,
				  1.0f, 1.0f}};

    for (uint32_t i(0u); i != 6; ++i)
      {
	for (uint32_t j(0u); j != 2; ++j)
	  data[i * 4 + j] = (corner[i * 2 + j] - (1.0f - corner[i * 2 + j]));
	data[i * 4 + 2] = (corner[i * 2]);
	data[i * 4 + 3] = (corner[i * 2 + 1] + timer);
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    opengl::setUniform(dim, "dim", textureContext.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteManager[SpriteId::Back].texture);
    opengl::setUniform(0u, "tex", textureContext.program);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}

void Display::renderText(std::string const &text, unsigned int fontSize, claws::vect<float, 2u> step, claws::vect<float, 2u> textPos, claws::vect<float, 3u> color)
{
  fontHandler.renderText(text, [this, textPos, color](claws::vect<float, 2u> pen, claws::vect<float, 2u> size, unsigned char *buffer, claws::vect<int, 2u> fontDim)
			       {
				 opengl::Texture texture;
				 Bind bind(textContext);

				 glActiveTexture(GL_TEXTURE0);
				 glBindTexture(GL_TEXTURE_2D, texture);
				 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				 glTexImage2D(GL_TEXTURE_2D,
					      0,
					      GL_RED,
					      fontDim[0],
					      fontDim[1],
					      0,
					      GL_RED,
					      GL_UNSIGNED_BYTE,
					      static_cast<void *>(buffer));
				 float data[16];

				 for (unsigned int i(0); !(i & 4u); ++i)
				   {
				     claws::vect<float, 2u> corner{static_cast<float>(i & 1u), static_cast<float>(i >> 1u)};
				     claws::vect<float, 2u> destCorner(pen + textPos + corner * size);

				     data[i * 4 + 0] = destCorner[0];
				     data[i * 4 + 1] = destCorner[1];
				     data[i * 4 + 2] = corner[0];
				     data[i * 4 + 3] = corner[1];
				   }
				 glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
				 glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
				 opengl::setUniform(dim, "dim", textContext.program);
				 opengl::setUniform(color, "textColor", textContext.program);
				 opengl::setUniform(0u, "tex", textContext.program);
				 glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			       }, fontSize, step);
}

void Display::renderSmolWasp(SmolWasp const &smolWasp)
{
  {
    Bind bind(textureContext);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

    {
      float animationFrameCount(float(spriteManager[SpriteId::SmolWasp].imageCount));
      float animationOffset(float(uint32_t(smolWasp.animationFrame)));
      std::array<float, 16> data{{-1.0f, -1.0f, 0.0f, 0.0f,
				  1.0f, -1.0f, 1.0f, 0.0f,
				 -1.0f, 1.0f, 0.0f, 1.0f,
				  1.0f, 1.0f, 1.0f, 1.0f}};

      for (uint32_t i(0u); i != 4; ++i)
	{
	  for (uint32_t j(0u); j != 2; ++j)
	    {
	      (data[i * 4 + j] *= smolWasp.size * 2.0f) += smolWasp.position[j];
	    }
	  (data[i * 4 + 3] += animationOffset) /= animationFrameCount;
	}
      glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    }
    opengl::setUniform(dim, "dim", textureContext.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteManager[SpriteId::SmolWasp].texture);
    opengl::setUniform(0u, "tex", textureContext.program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

void Display::renderBigWasp(BigWasp const &bigWasp)
{
  {
    renderSingleAnim({bigWasp.entities[1].position - bigWasp.size * 2.5f,
		      bigWasp.entities[1].position + bigWasp.size * 2.5f,
		      0}, SpriteId::WaspLegs);
    renderSingleAnim({bigWasp.entities[0].position - bigWasp.entities[0].size,
		      bigWasp.entities[0].position + bigWasp.entities[0].size,
		      0}, SpriteId::WaspHead);
    renderSingleAnim({bigWasp.entities[1].position - bigWasp.entities[1].size,
		      bigWasp.entities[1].position + bigWasp.entities[1].size,
		      0}, SpriteId::WaspBody);
    renderSingleAnim({bigWasp.entities[2].position - bigWasp.entities[2].size,
		      bigWasp.entities[2].position + bigWasp.entities[2].size,
		      0}, SpriteId::WaspAbdomen);
    claws::vect<float, 2u> offset{0.0f, -bigWasp.size * 1.5f};
    renderSingleAnim({bigWasp.entities[1].position - bigWasp.size * 2.5f + offset,
		      bigWasp.entities[1].position + bigWasp.size * 2.5f + offset,
		      uint32_t(bigWasp.frame)}, SpriteId::WaspWing);
  }
}


void Display::renderColors(std::vector<ColorInfo> const &colorInfos)
{
  {
    Bind bind(bulletContext);

    glBindBuffer(GL_ARRAY_BUFFER, bulletBuffer);

    std::vector<float> data;

    data.reserve(colorInfos.size() * 6 * 6);

    for (auto const &colorInfo : colorInfos)
      {
	std::array<float, 12> corner{{0.0f, 0.0f,
				      1.0f, 0.0f,
				      0.0f, 1.0f,
				      1.0f, 0.0f,
				      0.0f, 1.0f,
				      1.0f, 1.0f}};

	for (uint32_t i(0u); i != 6; ++i)
	  {
	    for (uint32_t j(0u); j != 2; ++j)
	      data.emplace_back(colorInfo.destMax[j] * corner[i * 2 + j] + colorInfo.destMin[j] * (1.0f - corner[i * 2 + j]));
	    for (uint32_t j(0u); j != 4; ++j)
	      data.emplace_back(colorInfo.color[j]);
	  }
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    opengl::setUniform(dim, "dim", bulletContext.program);
    glDrawArrays(GL_TRIANGLES, 0, uint32_t(colorInfos.size() * 6));
  }
}

void Display::renderAnims(std::vector<AnimInfo> const &anims, SpriteId spriteId)
{
  {
    Bind bind(textureContext);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

    std::vector<float> data;

    data.reserve(anims.size() * 6 * 4);

    for (auto const &anim : anims)
      {
	std::array<float, 12> corner{{0.0f, 0.0f,
				      1.0f, 0.0f,
				      0.0f, 1.0f,
				      1.0f, 0.0f,
				      0.0f, 1.0f,
				      1.0f, 1.0f}};

	for (uint32_t i(0u); i != 6; ++i)
	  {
	    for (uint32_t j(0u); j != 2; ++j)
	      data.emplace_back(anim.destMax[j] * corner[i * 2 + j] + anim.destMin[j] * (1.0f - corner[i * 2 + j]));
	    data.emplace_back(corner[i * 2]);
	    data.emplace_back((corner[i * 2 + 1] + float(anim.frame)) / float(spriteManager[spriteId].imageCount));
	  }
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    opengl::setUniform(dim, "dim", textureContext.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteManager[spriteId].texture);
    opengl::setUniform(0u, "tex", textureContext.program);
    glDrawArrays(GL_TRIANGLES, 0, uint32_t(anims.size() * 6));
  }
}

void Display::renderRotatedAnims(std::vector<RotatedAnimInfo> const &rotatedAnims, SpriteId spriteId)
{
  {
    Bind bind(textureContext);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

    std::vector<float> data;

    data.reserve(rotatedAnims.size() * 6 * 4);

    for (auto const &rotatedAnim : rotatedAnims)
      {
	claws::vect<float, 2> dir{rotatedAnim.dir / std::sqrt(rotatedAnim.dir.length2())};
	claws::vect<float, 2> center((rotatedAnim.destMin + rotatedAnim.destMax) * 0.5f);
	std::array<float, 12> corner{{0.0f, 0.0f,
				      1.0f, 0.0f,
				      0.0f, 1.0f,
				      1.0f, 0.0f,
				      0.0f, 1.0f,
				      1.0f, 1.0f}};


	for (uint32_t i(0u); i != 6; ++i)
	  {
	    auto position(rotatedAnim.destMax * claws::vect<float, 2u>{corner[i * 2], corner[i * 2 + 1]} +
			  rotatedAnim.destMin * claws::vect<float, 2u>{1.0f - corner[i * 2], 1.0f - corner[i * 2 + 1]});

	    position -= center;
	    position = (position * dir[1] + claws::vect<float, 2>{position[1], -position[0]} * dir[0]);
	    position += center;
	    for (uint32_t j(0u); j != 2; ++j)
	      data.emplace_back(position[j]);
	    data.emplace_back(corner[i * 2]);
	    data.emplace_back((corner[i * 2 + 1] + float(rotatedAnim.frame)) / float(spriteManager[spriteId].imageCount));
	  }
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    opengl::setUniform(dim, "dim", textureContext.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteManager[spriteId].texture);
    opengl::setUniform(0u, "tex", textureContext.program);
    glDrawArrays(GL_TRIANGLES, 0, uint32_t(rotatedAnims.size() * 6));
  }
}


void Display::renderHud(float bigWaspSize, uint32_t score, std::string const &strTime, float timer)
{

  uint32_t hps = uint32_t((std::log(BigWasp::minSize) - std::log(bigWaspSize)) / std::log(BigWasp::hitPenality)) + 1;
  if (bigWaspSize < BigWasp::minSize)
    hps = 0;

  renderText("  Size  : " + std::to_string(uint32_t(bigWaspSize * 1000.0f)), 400, {0.05f, 0.05f}, {1.0f, 0.855f}, {1.0f, 1.0f, 1.0f});
  renderText("  Hps   : " + std::to_string(hps), 400, {0.05f, 0.05f}, {1.0f, 0.755f}, {1.0f, 1.0f, 1.0f});
  renderText("  Score : " + std::to_string(score), 400, {0.05f, 0.05f}, {1.0f, 0.655f}, {1.0f, 1.0f, 1.0f});
  renderText("  Time  : " + strTime, 400, {0.05f, 0.05f}, {1.0f, 0.555f}, {1.0f, 1.0f, 1.0f});
  auto secondTime((uint32_t(timer) * Logic::getTickTime().count()) / 1000000);
  std::string inGameTime;

  if (secondTime / 60 >= 10)
    inGameTime = std::to_string(secondTime / 60) + " m ";
  else if (secondTime / 60)
    inGameTime = "0" + std::to_string(secondTime / 60) + " m ";
  if ((secondTime) % 60 >= 10)
    inGameTime += std::to_string((secondTime) % 60) + " s";
  else
    inGameTime += "0" + std::to_string((secondTime) % 60) + " s";
  renderText("  In game time : " + inGameTime, 400, {0.05f, 0.05f}, {1.0f, 0.455f}, {1.0f, 1.0f, 1.0f});

}

void Display::renderGameOver(uint32_t score, std::string const &strTime)
{
  renderText("Game Over", 300, {0.07f, 0.07f}, {-0.18f, 0.25f}, {1.0f, 1.0f, 1.0f});
  renderText("Final Time  " + strTime, 200, {0.05f, 0.05f}, {-0.18f, 0.05f}, {1.0f, 1.0f, 1.0f});
  renderText("Final Score " + std::to_string(score), 200, {0.05f, 0.05f}, {-0.18f, -0.05f}, {1.0f, 1.0f, 1.0f});
}

void Display::renderDeadScreen(const std::vector<std::pair<std::string, std::string>>& fellows)
{
  renderSingleAnim({{-1.75f, 0.65f}, {-1.03f, 0.97f}}, SpriteId::DeadFellows);
  float y = 0.55f;
  for (auto i = fellows.begin() ; i != fellows.end() ; ++i) {
    renderText(i->first + " " + i->second, 200, {0.05f, 0.05f}, {-1.65f, y}, {1.0f, 1.0f, 1.0f});
    y -= 0.05f;
    if (y <= -1.0f)
      break;
  }
}

void Display::render(DisplayData const &data)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //do final render here
  glClearColor(0.0f, 0.2f, 0.2f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  renderBack((data.timer + data.screenShake * sin(data.screenShake)) * 0.003f);
  renderColors({{claws::vect<float, 2u>(-1.0f, 1.0f), claws::vect<float, 2u>(1.0f, -1.0f), claws::vect<float, 4u>{data.screenShake * 0.01f, data.screenShake * 0.01f, 0.04f, 0.8f}}});
  if (data.bigWasp)
    renderBigWasp(*data.bigWasp);
  for (size_t i(0u); i < data.anims.size(); ++i)
    if (!data.anims[i].empty())
      renderAnims(data.anims[i], SpriteId(i));
  for (size_t i(0u); i < data.rotatedAnims.size(); ++i)
    if (!data.rotatedAnims[i].empty())
      renderRotatedAnims(data.rotatedAnims[i], SpriteId(i));
  if (!data.colors.empty())
    renderColors(data.colors);
  if (data.smolWasp)
    renderSmolWasp(*data.smolWasp);
  renderColors({{-dim, claws::vect<float, 2u>(-1.0f, 1.0f), claws::vect<float, 4u>{0.0f, 0.0f, 0.0f, 1.0f}},
		{dim, claws::vect<float, 2u>(1.0f, -1.0f), claws::vect<float, 4u>{0.0f, 0.0f, 0.0f, 1.0f}}});
  renderHud((data.bigWasp ? data.bigWasp->size : BigWasp::minSize), data.gameScore, data.stringedTime, data.timer);
  if (data.gameOverHud)
    renderGameOver(data.gameScore, data.stringedTime);
  if (data.deadFellows.size())
    renderDeadScreen(data.deadFellows);
}

void Display::resize(claws::vect<uint32_t, 2u> size)
{
  this->size = size;
  this->dim = {float(size[0]) / float(size[1]), 1.0f};
  glViewport(0, 0, size[0], size[1]);
}
