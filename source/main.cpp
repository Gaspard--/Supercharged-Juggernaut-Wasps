# include <iostream>
# include <thread>
# include <mutex>
# include "Display.hpp"
# include "Input.hpp"
# include "Logic.hpp"

int main()
{
  try {
    GlfwContext glfwContext{};

    input::Input input(glfwContext.createWindow({1920u, 1080u}, std::string{"Supercharged Juggernaut Wasps"}));

    glfwSwapInterval(1);
    Display display(input.getWindow());
    Logic logic;

    std::mutex lock;
    std::thread thread([&logic, &lock]() {
      try {
	while (logic.isRunning())
	  logic.tick(lock);
      } catch (std::runtime_error const &e) {
	std::cerr << "Logic thread encoutered runtime error:" << std::endl
		  << e.what() << std::endl;
      }
    });
    try {
      while (display.isRunning())
	{
	  glfwPollEvents();
          {
            std::lock_guard<std::mutex> scopedLock(lock);

            for (input::Event ev = input.pollEvent(); ev; ev = input.pollEvent()) {
	      logic.handleEvent(display, ev);
	    }
            logic.checkEvents(input);
            display.copyRenderData(logic);
          }
          display.render();
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
