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

  void handleEvent(Display const &display, input::Event const& event, input::Key const &key);
  void handleEvent(Display const &display, input::Event const& event, input::Mouse const &key);
  void handleEvent(Display const &display, input::Event const& event, input::Button const &key);
  void handleEvent(Display const &display, input::Event const& event, input::None const &key);
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
