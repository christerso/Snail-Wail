/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "missile.h"
#include "physics/body.h"
#include "snail.h"
#include "graphics/particle_emitter.h"
#include "coord_system2.h"

Missile::Missile()
  : timeSinceUpdate(0.0f)
  , fuel(1.4f)
{
}

void Missile::setFuel(float fuel) {
  this->fuel = fuel;
}

void Missile::update(float dt) {
  timeSinceUpdate += dt;
  fuel -= dt;
  
  if (timeSinceUpdate >= 0.01f && fuel > 0.0f) {
    // FIXME: fix this ugly way of handling target seeking!
    
    if (Ref<Snail>::SharedPtr lockedTarget = target.lock()) {
      vec2 vel = body->getVelocity();
      vec2 uvel = vel;
      uvel.normalize();

      vec2 diff = lockedTarget->origin->getTransform().position -
        body->origin->getTransform().position;

      vec2 udiff = diff;
      udiff.normalize();
      
      vec2 impulse = (udiff - uvel * 1.0f) * 200.0f + uvel * 40.0f;
      
      // FIXME: this is clearly magic code. fix it.
      
      body->addImpulse(impulse * 200.0f * dt); //300.0f);
      vel = body->getVelocity();
      vel.normalize();
      
      mat2 spriteOrient(vel, vec2(-vel.y, vel.x));

      body->origin->setTransform(
        CoordSystemData2(body->origin->getTransform().position, spriteOrient)
        );
    }
    
    timeSinceUpdate = 0.0f;
  }
  
  if (fuel > 0.0f) {
    // Send updates to the particle emitter if we're not low on fuel
    if (Ref<Graphics::ParticleEmitter>::SharedPtr lockedEmitter =
        smokeEmitter.lock()) {
      lockedEmitter->update(dt);
    }
  }
}

