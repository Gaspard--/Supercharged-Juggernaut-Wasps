#pragma once

#include <variant>
#include <queue>
#include <iostream>
#include <optional>
#include <vector>

#include "claws/container/vect.hpp"
#include "my_glfw.hpp"

namespace input
{
  struct Key
  {
    int key;
    int scancode;
    int action;
    int mode;
  };

  struct Mouse
  {
    double x;
    double y;
  };

  struct Button
  {
    int button;
    int action;
    int mods;
  };

  struct None
  {};

  struct Event
  {
    GLFWwindow *window;
    std::variant<None, Key, Mouse, Button> data;

    constexpr operator bool() const noexcept
    {
      return !std::holds_alternative<None>(data);
    };
  };

  class Input
  {
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window;
    std::queue<Event> events;
    claws::vect<uint32_t, 2u> size;
	bool sizeUpdated{false};
    int joystickId;

    void detectJoystick()
    {
      joystickId = GLFW_JOYSTICK_1;
      while (!glfwJoystickPresent(joystickId) && joystickId < GLFW_JOYSTICK_LAST)
	joystickId++;
      if (joystickId == GLFW_JOYSTICK_LAST)
	joystickId = -1;
      std::cout << "Joystick[" << joystickId << "], \"" << (joystickId == -1 ? "No Joystick inserted" : glfwGetJoystickName(joystickId)) << "\"  will be used" << std::endl;
    }

  public:

    auto &getWindow() const
    {
      return *window;
    }

    explicit Input(std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> &&window)
      : window(std::move(window))
      , events()
      , joystickId(-1)
    {
      detectJoystick();
      glfwSetWindowUserPointer(&getWindow(), this);
      // glfwSetInputMode(&getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetKeyCallback(&getWindow(), [](GLFWwindow *window, int key, int scancode, int action, int mode) {
	  Input &input(*static_cast<Input *>(glfwGetWindowUserPointer(window)));
	  Event ev = {window, Key{key, scancode, action, mode}};

	  input.events.push(ev);
	});
      glfwSetCursorPosCallback(&getWindow(), [](GLFWwindow *window, double x, double y) {
	  Input &input(*static_cast<Input *>(glfwGetWindowUserPointer(window)));
	  Event ev = {window, Mouse{x, y}};

	  input.events.push(ev);
	});
      glfwSetMouseButtonCallback(&getWindow(), [](GLFWwindow *window, int button, int action, int mods) {
	  Input &input(*static_cast<Input *>(glfwGetWindowUserPointer(window)));
	  Event ev = {window, Button{button, action, mods}};

	  input.events.push(ev);
	});
      glfwSetFramebufferSizeCallback(&getWindow(), [](GLFWwindow *window, int width, int height) {
	  Input &input(*static_cast<Input *>(glfwGetWindowUserPointer(window)));
#ifdef _WIN32
	  if (!(width + height))
		  return;
#endif

	  input.size = {uint32_t(width), uint32_t(height)};
	  input.sizeUpdated = true;
	});
    }

    Input(Input const &) = delete;
    Input(Input &&) = delete;
    ~Input() = default;

    Event pollEvent()
    {
      if (events.empty())
	return {nullptr, None{}};

      Event event(events.front());

      events.pop();
      return event;
    }

    bool windowShouldClose() const noexcept
    {
      return glfwWindowShouldClose(&getWindow());
    }

    std::vector<claws::vect<float, 2>> getJoystickAxes() const
    {
      int count;
      std::vector<claws::vect<float, 2>> axes;
      if (joystickId != -1) {
	const float *brutAxes = glfwGetJoystickAxes(joystickId, &count);
	for (int i = 0 ; i < count - 1 ; i += 2)
	  axes.push_back({brutAxes[i], brutAxes[i + 1]});
      }
      return axes;
    }

    std::vector<unsigned char> getJoystickButtons() const
    {
      int count;
      std::vector<unsigned char> buttons;
      if (joystickId != -1) {
	const unsigned char *brutButtons = glfwGetJoystickButtons(joystickId, &count);
	for (int i = 0 ; i < count ; ++i)
	  buttons.push_back(brutButtons[i]);
      }
      return buttons;
    }

    bool isKeyPressed(int key) const noexcept
    {
      return glfwGetKey(&getWindow(), key) == GLFW_PRESS;
    }

    bool isMouseButtonPressed(int button) const noexcept
    {
      return glfwGetMouseButton(&getWindow(), button) == GLFW_PRESS;
    }

    claws::vect<uint32_t, 2> getSize() const noexcept
    {
      return size;
    }

	void setSize(const claws::vect<uint32_t, 2>& newSize) noexcept
	{
		size = newSize;
	}

    std::optional<claws::vect<uint32_t, 2>> consumeSizeUpdate()
    {
      if (sizeUpdated)
	{
	  sizeUpdated = false;
	  return getSize();
	}
      return {};
    }
  };
};
