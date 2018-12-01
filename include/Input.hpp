#pragma once

#include "my_glfw.hpp"

#include <variant>
#include <queue>

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
    bool sizeUpdated;

  public:
    auto &getWindow() const
    {
      return *window;
    }

    explicit Input(std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> &&window)
      : window(std::move(window))
      , events()
    {
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
