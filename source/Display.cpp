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
				     data[i * 4 + 3] = 1.0f - corner[1];
				     std::cout << destCorner[0] << ", " << destCorner[1] << std::endl;
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
      float animationFrameCount(float(spriteManager[SpriteId::SmolWaspIdle].imageCount));
      float animationOffset(float(uint32_t(smolWasp.animationFrame)));
      std::array<float, 16> data{{-1.0f, -1.0f, 0.0f, 0.0f,
				  1.0f, -1.0f, 1.0f, 0.0f,
				 -1.0f, 1.0f, 0.0f, 1.0f,
				  1.0f, 1.0f, 1.0f, 1.0f}};

      for (uint32_t i(0u); i != 4; ++i)
	{
	  for (uint32_t j(0u); j != 2; ++j)
	    {
	      (data[i * 4 + j] *= 0.03f) += smolWasp.position[j];
	    }
	  (data[i * 4 + 3] += animationOffset) /= animationFrameCount;
	}
      glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    }
    opengl::setUniform(dim, "dim", textureContext.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteManager[SpriteId::SmolWaspIdle].texture);
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
  }
}


void Display::renderBullets(std::vector<BulletInfo> const &bullets)
{
  {
    Bind bind(bulletContext);

    glBindBuffer(GL_ARRAY_BUFFER, bulletBuffer);

    std::vector<float> data;

    data.reserve(bullets.size() * 6 * 6);

    for (auto const &bullet : bullets)
      {
	std::array<float, 12> corner{{-1.0f, -1.0f,
				      1.0f, -1.0f,
				      -1.0f, 1.0f,
				      1.0f, -1.0f,
				      -1.0f, 1.0f,
				      1.0f, 1.0f}};

	for (uint32_t i(0u); i != 6; ++i)
	  {
	    for (uint32_t j(0u); j != 2; ++j)
	      data.emplace_back(corner[i * 2 + j] * bullet.size + bullet.position[j]);
	    for (uint32_t j(0u); j != 4; ++j)
	      data.emplace_back(bullet.color[j]);
	  }
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    opengl::setUniform(dim, "dim", bulletContext.program);
    glDrawArrays(GL_TRIANGLES, 0, uint32_t(bullets.size() * 6));
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

void Display::renderHud(float bigWaspSize, uint32_t score, std::string const &strTime)
{
  uint32_t hps = uint32_t((bigWaspSize - BigWasp::minSize) * 10000);
  if (bigWaspSize < BigWasp::minSize)
    hps = 0;

  renderText("Hps   : " + std::to_string(hps), 1000, {0.025f, 0.05f}, {0.62f / dim[0], 0.855f}, {1.0f, 1.0f, 1.0f});
  renderText("Score : " + std::to_string(score), 1000, {0.025f, 0.05f}, {0.62f / dim[0], 0.755f}, {1.0f, 1.0f, 1.0f});
  renderText("Time  : " + strTime, 1000, {0.025f, 0.05f}, {0.62f / dim[0], 0.655f}, {1.0f, 1.0f, 1.0f});
}

void Display::render(DisplayData const &data)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //do final render here
  glClearColor(0.15f, 0.08f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  if (data.bigWasp)
    renderBigWasp(*data.bigWasp);
  if (!data.bullets.empty())
    renderBullets(data.bullets);
  for (size_t i(0u); i < data.anims.size(); ++i)
    if (!data.anims[i].empty())
      renderAnims(data.anims[i], SpriteId(i));
  if (data.smolWasp)
    renderSmolWasp(*data.smolWasp);
  renderHud((data.bigWasp ? data.bigWasp->size : BigWasp::minSize), data.gameScore, data.stringedTime);
}

void Display::resize(claws::vect<uint32_t, 2u> size)
{
  this->size = size;
  this->dim = {float(size[0]) / float(size[1]), 1.0f};
  glViewport(0, 0, size[0], size[1]);
}
