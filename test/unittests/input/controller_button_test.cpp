#include <gtest/gtest.h>

#include "controller.hpp"

using button = cen::controller_button;

TEST(ControllerButton, EqualityOperator)
{
  EXPECT_EQ(button::invalid, SDL_CONTROLLER_BUTTON_INVALID);
  EXPECT_EQ(button::a, SDL_CONTROLLER_BUTTON_A);
  EXPECT_EQ(button::b, SDL_CONTROLLER_BUTTON_B);
  EXPECT_EQ(button::x, SDL_CONTROLLER_BUTTON_X);
  EXPECT_EQ(button::y, SDL_CONTROLLER_BUTTON_Y);
  EXPECT_EQ(button::back, SDL_CONTROLLER_BUTTON_BACK);
  EXPECT_EQ(button::guide, SDL_CONTROLLER_BUTTON_GUIDE);
  EXPECT_EQ(button::start, SDL_CONTROLLER_BUTTON_START);
  EXPECT_EQ(button::left_stick, SDL_CONTROLLER_BUTTON_LEFTSTICK);
  EXPECT_EQ(button::right_stick, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
  EXPECT_EQ(button::left_shoulder, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
  EXPECT_EQ(button::right_shoulder, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
  EXPECT_EQ(button::dpad_up, SDL_CONTROLLER_BUTTON_DPAD_UP);
  EXPECT_EQ(button::dpad_down, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
  EXPECT_EQ(button::dpad_left, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
  EXPECT_EQ(button::dpad_right, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
  EXPECT_EQ(button::max, SDL_CONTROLLER_BUTTON_MAX);

  EXPECT_EQ(SDL_CONTROLLER_BUTTON_INVALID, button::invalid);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_A, button::a);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_B, button::b);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_X, button::x);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_Y, button::y);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_BACK, button::back);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_GUIDE, button::guide);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_START, button::start);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_LEFTSTICK, button::left_stick);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_RIGHTSTICK, button::right_stick);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, button::left_shoulder);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, button::right_shoulder);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_DPAD_UP, button::dpad_up);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_DPAD_DOWN, button::dpad_down);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_DPAD_LEFT, button::dpad_left);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, button::dpad_right);
  EXPECT_EQ(SDL_CONTROLLER_BUTTON_MAX, button::max);
}

TEST(ControllerButton, InequalityOperator)
{
  EXPECT_NE(button::dpad_right, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
  EXPECT_NE(SDL_CONTROLLER_BUTTON_X, button::a);

  EXPECT_FALSE(SDL_CONTROLLER_BUTTON_INVALID != button::invalid);
}
