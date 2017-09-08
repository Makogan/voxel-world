#pragma once

#include "../global.hpp"
#include "rendering.hpp"

#define GLEW_DYNAMIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void callBackInit(GLFWwindow* window);

int openGLerror();

double calculateFPS(double prevTime, double currentTime);

GLFWwindow* createWindow();

GLFWwindow* init();
