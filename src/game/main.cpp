#include <iostream>
#include <engine/portal.h>
#include <engine/window_manager.h>
#include <engine/input.h>
#include <engine/graphics.h>

#include <game/background.h>
#include <game/snails.h>
#include <game/control.h>
#include <game/system.h>
#include <game/items.h>
#include <game/particles.h>

#include <cstdlib>

int app_main(Portal &interfaces) {
  WindowManager *wm = interfaces.requestInterface<WindowManager>();
  Input *input = interfaces.requestInterface<Input>();
  Graphics *gfx = interfaces.requestInterface<Graphics>();
  
  wm->createWindow(800, 600);

  const int escape = input->keycode("escape");
  bool running = true;
  double lastTick = wm->timeSeconds();

  SystemContext systems;

  Background bkg;
  Snails snails;
  Control control;
  Items items;
  Particles particles;
  
  systems.set(SystemContext::SYSTEM_SNAILS, &snails);
  systems.set(SystemContext::SYSTEM_BACKGROUND, &bkg);
  systems.set(SystemContext::SYSTEM_CONTROL, &control);
  systems.set(SystemContext::SYSTEM_ITEMS, &items);
  systems.set(SystemContext::SYSTEM_PARTICLES, &particles);
  systems.init(interfaces);
  
  while (running) {      
    double thisTick = wm->timeSeconds();
    float dt = std::max(thisTick - lastTick, 0.00001);

    const rect wndSize = wm->size();
    
    gfx->setViewport(wndSize);
    gfx->setOrtho(wndSize);

    control.update();
    bkg.render();
    particles.render();
    snails.render();
    items.update();
    items.render();
    
    wm->swapBuffers();
    lastTick = thisTick;
    running = !input->keyPressed(escape) &&
      wm->windowState(WindowManager::OPENED);
  }

  return EXIT_SUCCESS;
}
