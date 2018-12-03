#pragma once

# include <mutex>
# include <memory>
# include <chrono>
# include "Input.hpp"
# include "State.hpp"

class Display;

class Logic
{

  static constexpr unsigned int eventDelay = 60;
  using Clock = std::conditional<std::chrono::high_resolution_clock::is_steady,
                                 std::chrono::high_resolution_clock,
                                 std::chrono::steady_clock>::type;

  unsigned int time{0};
  unsigned int timeSinceStateStart{0};

  decltype(Clock::now()) lastUpdate;


  DisplayData lastDispData;
  bool running;
  std::unique_ptr<state::State> state;

  void handleEvent(input::Input const &display, input::Event const& event, input::Key const &key);
  void handleEvent(input::Input const &display, input::Event const& event, input::Mouse const &key);
  void handleEvent(input::Input const &display, input::Event const& event, input::Button const &key);
  void handleEvent(input::Input const &display, input::Event const& event, input::None const &key);
public:
  static constexpr std::chrono::microseconds const getTickTime()
  {
    return std::chrono::microseconds(1000000 / 120);
  };

  Logic();

  std::string getTime(void) const;

  void tick(std::mutex &lock);
  void handleEvent(input::Input const &display, input::Event const& event);

  void handleKey(GLFWwindow *window, input::Key key);
  void handleMouse(input::Input const &, GLFWwindow *window, input::Mouse mouse);
  void handleButton(GLFWwindow *window, input::Button button);
  void checkEvents(input::Input& input);
  void getObjectsToRender(DisplayData &displayData);

  bool isRunning() const noexcept
  {
    return running;
  }

  bool &isRunning() noexcept
  {
    return running;
  }
};
