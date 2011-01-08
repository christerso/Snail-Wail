/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <iostream>
#include <map>
#include <cstdlib>
#include "GL/glfw.h"

#include <engine/glfw/wm.h>
#include <engine/glfw/input.h>
#include <engine/portal.h>

// The GLFW module is a "starter" module, meaning, it implements 'main' and runs
// app_main

extern int app_main(Portal &portal);

int main(int argc, char **argv) {
  Portal interfaces;

  if (!glfwInit()) {
    std::cerr << "glfw: failed to initialize" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "glfw: registering glfw interfaces..." << std::endl;
  interfaces.registerInterface<Glfw::WindowManager>();
  interfaces.registerInterface<Glfw::Input>();
  std::cout << "glfw: initialized" << std::endl;

  app_main(interfaces);
  
  glfwTerminate();
}
