# include <iostream>
# include <thread>
# include <mutex>
# include "Display.hpp"
# include "Input.hpp"
# include "Logic.hpp"
# include "DisplayData.hpp"
# include "SoundHandler.hpp"
# include "my_glfw.hpp"
int main()
{
  try {
    GlfwContext glfwContext{};

    input::Input input(glfwContext.createWindow({1920u, 1080u}, std::string{"Supercharged Juggernaut Wasps"}));

    glfwSwapInterval(1);
    Display display(input.getWindow());
#ifdef _WIN32
	int x, y;
	glfwGetFramebufferSize(&input.getWindow(), &x, &y);
	display.resize({(uint32_t)x, (uint32_t)y});
	input.setSize({ (uint32_t)x, (uint32_t)y });
#endif
    Logic logic;

    struct SoundHandlerInit
    {
      SoundHandlerInit()
      {
        SoundHandler::initSoundHandler();
      }

      ~SoundHandlerInit()
      {
        SoundHandler::destroySoundHandler();
      }
    } SoundHandlerIniter;

    SoundHandler::getInstance().playMainMusic();

    std::mutex lock;
    std::thread thread([&logic, &lock, &input]() {
      try {
	while (logic.isRunning())
	  {
	    logic.tick(lock);
	  }
      } catch (std::runtime_error const &e) {
	std::cerr << "Logic thread encoutered runtime error:" << std::endl
		  << e.what() << std::endl;
      }
    });
    try {
      while (display.isRunning())
	{
	  DisplayData displayData;

	  glfwPollEvents();
	  logic.checkEvents(input);
          {
            std::lock_guard<std::mutex> scopedLock(lock);

            for (input::Event ev = input.pollEvent(); ev; ev = input.pollEvent()) {
	      logic.handleEvent(input, ev);
	    }
	    logic.getObjectsToRender(displayData);
          }
	  if (auto sizeUpdate = input.consumeSizeUpdate())
	    display.resize(*sizeUpdate);
          display.render(displayData);
	  glfwSwapBuffers(&input.getWindow());
        }
    } catch (std::runtime_error const &e) {
      std::cerr << "Display thread encoutered runtime error:" << std::endl
                << e.what() << std::endl;
    }
    {
      std::lock_guard<std::mutex> scopedLock(lock);
      logic.isRunning() = false;
    }
    thread.join();
  } catch (std::runtime_error const &e) {
    std::cerr << "Program encoutered runtime error:" << std::endl << e.what() << std::endl;
    return -1;
  }
  return 0;
}
