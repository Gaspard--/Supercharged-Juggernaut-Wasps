#pragma once

# include <mutex>
# include <memory>
# include "Input.hpp"
# include "State.hpp"

class Display;

class Logic
{

  bool running;
  std::unique_ptr<state::State> state;

public:

  Logic();

  void tick(std::mutex &lock);
  void handleEvent(Display const &display, input::Event const& event);

  void handleKey(GLFWwindow *window, input::Key key);
  void handleMouse(Display const &, GLFWwindow *window, input::Mouse mouse);
  void handleButton(GLFWwindow *window, input::Button button);
  void checkEvents(input::Input& input);

  bool isRunning() const
  {
    return running;
  }

  bool &isRunning()
  {
    return running;
  }

};
