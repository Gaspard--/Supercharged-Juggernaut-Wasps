# include <iostream>
# include "my_glfw.hpp"
# include "Input.hpp"

int main()
{
  try
    {
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      GlfwContext glfwContext{};

      input::Input input(glfwContext.createWindow({1920u, 1080u}, std::string{"Supercharged Juggernaut Wasps"}));
    }
  catch (std::runtime_error const &e)
    {
      std::cerr << "program encoutered runtime error:\n" << e.what() << std::endl;
      return -1;
    }
}
