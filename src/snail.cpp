/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "snail.h"
#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"
#include "world.h"
#include "player_entity.h"
#include "projectile.h"
#include "helmet.h"
#include "missile.h"

#include <iostream>

Snail::Snail(SnailType type) {
  health = 100;

  TagPair tags[] = {
    std::make_pair("origin", &origin)
  };

  addTags(&tags[0], &tags[0] + 1);
}

// TODO: fix event system.

// TODO: en klass; Item. Skjuta på den ska anropa en medlem i snigeln
// antar att det blir downcast i alla fall.

void Snail::collided(const Ref<Physics::Geom>::SharedPtr & with) {
  // TODO: this is ugly
  if (Ref<Object>::SharedPtr lockedOwner = with->getOwner().lock()) {
    if (Ref<Missile>::SharedPtr lockedMissile = Cast<Missile>(lockedOwner)) {
      if (lockedMissile->shooter.lock().get() != this) { // hit by a bullet
        lockedMissile->kill();
        increaseHealth(-20);
        const vec2 delta = origin->getTransform().position - lockedMissile->origin->getTransform().position;
        physBody->addImpulse(delta * 30.0f);
      }
      
    }
    else if (Ref<Projectile>::SharedPtr lockedProjectile = Cast<Projectile>(lockedOwner)) {
      if (lockedProjectile->shooter.lock().get() != this) { // hit by a bullet
        lockedProjectile->kill();
        increaseHealth(-5);
        const vec2 delta = origin->getTransform().position - lockedProjectile->origin->getTransform().position;
        physBody->addImpulse(delta * 8.0f);
      }
    }
  }
  
}

void Snail::setEventHandler(const Ref<SnailEventHandler> & newHandler) {
  eventHandler = newHandler;
}

void Snail::increaseHealth(int add) {
  health = std::min(100, health + add);
  
  if (Ref<SnailEventHandler>::SharedPtr lockedHandler = eventHandler.lock()) {      
    lockedHandler->onHealthChange(static_cast<float>(health), add);
  }
  
  if (health <= 0.0f) {
    std::cout << "snail died :-(" << std::endl;
    kill();
    if (Ref<Helmet>::SharedPtr lockedHelmet = helmet.lock())
      lockedHelmet->kill();
  }
  
}
