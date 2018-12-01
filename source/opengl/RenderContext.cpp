#include "opengl/RenderContext.hpp"

namespace opengl
{
  void RenderContext::bind()
  {
    glBindVertexArray(vao);
    glUseProgram(program);
  }

  void RenderContext::unbind()
  {
    glBindVertexArray(0);
    glUseProgram(0);
  }
}
