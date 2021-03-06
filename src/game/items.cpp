#include <game/items.h>
#include <game/cactus.h>
#include <game/projectile.h>
#include <game/powerup.h>

#include <engine/window_manager.h>
#include <engine/graphics.h>
#include <engine/image_loader.h>
#include <engine/portal.h>
#include <engine/image.h>
#include <engine/texture.h>
#include <game/snails.h>

#include <utils/algorithm.h>

#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>

void Items::init(const class Portal &interfaces) {
  wm = interfaces.requestInterface<WindowManager>();
  gfx = interfaces.requestInterface<Graphics>();
  ImageLoader *imgLoader = interfaces.requestInterface<ImageLoader>();

  {  
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/cactii.png"));
    cactusTexture = gfx->createTexture(img.get());
  }
  {  
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/bullet.png"));
    bulletTexture = gfx->createTexture(img.get());
  }
  {  
    std::auto_ptr<Image> img(imgLoader->loadImage("../data/health_powerup.png"));
    healthPowerup = gfx->createTexture(img.get());
  }

    
  lastUpdate = wm->timeSeconds();
  lastGentime = lastUpdate;
}
                                        
void Items::update() {
  double thisUpdate = wm->timeSeconds();
  double dt = thisUpdate - lastUpdate;
  lastUpdate = thisUpdate;

  // if update returns false, remove
  {
    ItemVector::iterator beg =
      remove_nif(items.begin(), items.end(), &Item::update, dt);
    std::for_each(beg, items.end(), delete_op());    
    items.erase(beg, items.end());
  }
  
  double secSinceGen = thisUpdate - lastGentime;
  
  if (secSinceGen > 0.5) {
    lastGentime = thisUpdate;
    float ran = float(rand()) / RAND_MAX;
    vec2 cactusPos = vec2(400.0f + (ran - 0.5f) * 100.0f, 632.0f);

    if (rand() % 20 > 16) {
      items.push_back(new Powerup(cactusPos, healthPowerup, "health", 20, context));
    }
    else {
      items.push_back(new Cactus(cactusPos, cactusTexture));
    }
  }

  {
    ProjectileVector::iterator beg =
      remove_nif(projectiles.begin(), projectiles.end(),
                 &Projectile::update, dt);
    
    std::for_each(beg, projectiles.end(), delete_op());
    projectiles.erase(beg, projectiles.end());
  }
}

void Items::spawnProjectile(ProjectileType type, const vec2 &pos,
                            const vec2 &dir, class Snail *shooter) {
  projectiles.push_back(new Projectile(pos, dir * 6300.0f,
                                       shooter, bulletTexture, context));
}

void Items::render() {
  std::for_each(items.begin(), items.end(), std::bind2nd(std::mem_fun(&Item::render), gfx));
  std::for_each(projectiles.begin(), projectiles.end(), std::bind2nd(std::mem_fun(&Projectile::render), gfx));
}

Item *Items::intersectingItem(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos) {
  ItemVector::iterator i = items.begin(), e = items.end();
  for (; i != e; ++i) {
    if ((*i)->intersects(start, end, radius, hitpos)) {
      return *i;
    }
  }

  return 0;
}

Item::Item(const vec2 &pos, float radius)
  : pos(pos)
  , radius(radius)
{

}

bool Item::intersects(const vec2 &start, const vec2 &end, float radius, vec2 &hitpos) {
  vec2 closest = closest_point(start, end, pos);
  if ((pos - closest).magnitude() <= radius + this->radius) {
    hitpos = closest;
    return true;
  }
  
  return false;
}


