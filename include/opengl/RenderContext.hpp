#pragma once

#include "opengl/my_opengl.hpp"

namespace opengl
{
  class RenderContext
  {
  public:
    opengl::Vao const vao;
    opengl::Program const program;
  
    void bind();
    void unbind();
  };
}
