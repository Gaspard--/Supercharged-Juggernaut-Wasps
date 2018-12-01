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
{
  
  {
    Bind bind(textureContext);

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), reinterpret_cast<void *>(2u * sizeof(float)));
    glVertexAttribPointer(2, 1, GL_FLOAT, false, 5 * sizeof(float), reinterpret_cast<void *>(4u * sizeof(float)));
  }
  {
    Bind bind(rectContext);

    glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
  }
  {
    Bind bind(textContext);

    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), reinterpret_cast<void *>(2u * sizeof(float)));
  }
}

void Display::renderSmolWasp(SmolWasp const &smolWasp)
{
  {
    Bind bind(rectContext);

    glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);

    {
      std::array<float, 8> data({-1.0f, -1.0f,
				 1.0f, -1.0f,
				 -1.0f, 1.0f,
				 1.0f, 1.0f});

      for (uint32_t i(0u); i != 4; ++i)
	for (uint32_t j(0u); j != 2; ++j)
	  (data[i * 2 + j] *= smolWasp.size) += smolWasp.position[j];

      glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    }
    opengl::setUniform(dim, "dim", rectContext.program);
    opengl::setUniform({1.0f, 0.8f, 0.0f, 1.0f}, "rect_color", rectContext.program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

void Display::renderBigWasp(BigWasp const &bigWasp)
{
  {
    Bind bind(rectContext);

    glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);

    {
      std::array<float, 8> data({-1.0f, -1.0f,
				 1.0f, -1.0f,
				 -1.0f, 1.0f,
				 1.0f, 1.0f});

      for (uint32_t i(0u); i != 4; ++i)
	for (uint32_t j(0u); j != 2; ++j)
	  (data[i * 2 + j] *= bigWasp.entities[0].size) += bigWasp.entities[0].position[j];

      glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    }
    opengl::setUniform(dim, "dim", rectContext.program);
    opengl::setUniform({1.0f, 0.8f, 0.0f, 1.0f}, "rect_color", rectContext.program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    {
      std::array<float, 8> data({-1.0f, -1.0f,
				 1.0f, -1.0f,
				 -1.0f, 1.0f,
				 1.0f, 1.0f});

      for (uint32_t i(0u); i != 4; ++i)
	for (uint32_t j(0u); j != 2; ++j)
	  (data[i * 2 + j] *= bigWasp.entities[1].size) += bigWasp.entities[1].position[j];
    
      glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    }
    opengl::setUniform({0.5f, 0.3f, 0.0f, 1.0f}, "rect_color", rectContext.program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

void Display::renderBullets(std::vector<Entity> const &bullets)
{
  {
    Bind bind(rectContext);

    glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);

    std::vector<float> data;

    data.reserve(bullets.size() * 12);

    for (auto const &bullet : bullets)
      {
	std::array<float, 12> corner({-1.0f, -1.0f,
				      1.0f, -1.0f,
				      -1.0f, 1.0f,
				      1.0f, -1.0f,
				      -1.0f, 1.0f,
				      1.0f, 1.0f});

	for (uint32_t i(0u); i != 6; ++i)
	  for (uint32_t j(0u); j != 2; ++j)
	    data.emplace_back(corner[i * 2 + j] * bullet.size + bullet.position[j]);
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    opengl::setUniform(dim, "dim", rectContext.program);
    opengl::setUniform({1.0f, 0.0f, 1.0f, 1.0f}, "rect_color", rectContext.program);
    glDrawArrays(GL_TRIANGLES, 0, bullets.size() * 6);
  }
}


void Display::render(DisplayData const &data)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //do final render here
  glClearColor(0.15f, 0.08f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  if (data.bigWasp)
    renderBigWasp(*data.bigWasp);
  if (data.smolWasp)
    renderSmolWasp(*data.smolWasp);
  if (!data.bullets.empty())
    renderBullets(data.bullets);
}

void Display::resize(claws::vect<uint32_t, 2u> size)
{
  this->size = size;
  this->dim = {float(size[0]) / float(size[1]), 1.0f};
}
