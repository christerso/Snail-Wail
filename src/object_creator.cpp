/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object_creator.h"
#include "projectile.h"
#include "snail.h"
#include "cactus.h"
#include "missile.h"
#include "coord_system2.h"

#include "graphics/particle_system.h"
#include "graphics/particle_emitter.h"
#include "graphics/gfx_subsystem.h"
#include "graphics/sprite.h"
#include "player_entity.h"
#include "world.h"
#include "physics/body.h"
#include "physics/geom.h"
#include "ref.h"
#include "helmet.h"
#include "coordsystem_transformer.h"
#include "powerup.h"
#include "rocket_ammo.h"

ObjectCreator::ObjectCreator(World & world)
   : world(world)
{
   smokeParticles = Owning(new Graphics::ParticleSystem);
   smokeParticles->setRenderer(world.graphics.getRenderer("../data/smoke.png"));
}

Ref<Projectile>::SharedPtr ObjectCreator::createProjectile() {
   Ref<Projectile>::SharedPtr newProjectile(new Projectile);
   newProjectile->origin = Owning(new CoordSystemLeaf2);

   // Sprite
   newProjectile->sprite = Owning(world.graphics.createSprite("../data/bullet.png"));
   newProjectile->sprite->setEventHandler(newProjectile);
   newProjectile->sprite->origin = newProjectile->origin;

   // Body
   newProjectile->body = Owning(world.physics.createBody());
   newProjectile->body->setOwner(newProjectile);
   newProjectile->body->origin = newProjectile->origin;

   // Geom
   const rect bulletSize(32, 8);
   newProjectile->geom = Owning(world.physics.createRectGeom(bulletSize));
   newProjectile->geom->setCollisionId(3);
   newProjectile->geom->setCollisionMask(0x0u);
   newProjectile->geom->origin = newProjectile->origin;
   
   return newProjectile;
}

Ref<Snail>::SharedPtr ObjectCreator::createSnail(int team, ObjectCreator & creator) {
   float xPos = 0.0f;
   if (team == 0)
      xPos = 100.0f;
   else
      xPos = 700.0f;
   
   Ref<Snail>::SharedPtr newSnail(new Snail);
   newSnail->origin = Owning(new CoordSystemLeaf2);

   std::string spriteFilename;

   // Sprite
   if (team == 0)
     spriteFilename = "../data/snail_l.png";
   else
     spriteFilename = "../data/snail_r.png";

   newSnail->sprite = Owning(world.graphics.createSprite(spriteFilename));
   newSnail->sprite->origin = newSnail->origin;

   // Phys body
   newSnail->physBody = Owning(world.physics.createBody());
   newSnail->physBody->origin = newSnail->origin;

   // Logic
   newSnail->logic = Owning(new PlayerEntity(xPos, newSnail, creator, world));
   newSnail->logic->setTarget(Observing(newSnail->physBody));
   newSnail->setEventHandler(Observing(newSnail->logic));
   
   world.scheduler.subscribe(0.0f, newSnail->logic);

   // Geom
   const rect snailSize(56, 39);
   newSnail->physGeom = Owning(world.physics.createRectGeom(snailSize));
   newSnail->physGeom->setEventHandler(Observing(newSnail));
   newSnail->physGeom->origin = newSnail->origin;
   newSnail->physGeom->setCollisionMask(0x8u);

   vec2 helmetOffset;
   
   // TODO: this is ugly
   Ref<Helmet> newHelmet;
   
   if (team == 1) {
      newHelmet = Owning(createHelmet(-1));
	  newSnail->physGeom->setOffset(vec2(3.0f, -4.0f));
	  newSnail->logic->weaponDir = vec2(-1.0f, 0.0f);
	  newSnail->logic->weaponPos = vec2(-50.0f, 1.0f); // FIXME: not used anywhere
      helmetOffset = vec2(-15.0f, -14.0f);
   }
   else { // first snail. not obvious.
      newHelmet = Owning(createHelmet(1));
	  newSnail->physGeom->setOffset(vec2(2.0f, -5.0f));
	  newSnail->logic->weaponDir = vec2(1.0f, 0.0f);
	  newSnail->logic->weaponPos = vec2(50.0f, 1.0f);
      helmetOffset = vec2(20.0f, -15.0f);
   }

   newSnail->helmet = Observing(newHelmet);
   newSnail->helmet->snailBody = Observing(newSnail->physBody);

//    newSnail->sprite->setDelegate(Owning(
//       new TransformedCoordSystem2(
//          newSnail->helmet,
//          CoordSystem2::data_type(helmetOffset, mat2::Identity())
//          )
//    ));
   world.insert(newHelmet.lock());
   
   if (team == 0) {
      newSnail->physGeom->setCollisionId(1);
      newSnail->helmet->geom->setCollisionId(1);
   }
   else {
      newSnail->physGeom->setCollisionId(0);
      newSnail->helmet->geom->setCollisionId(0);
   }
   

   newSnail->helmet->geom->setCollisionMask(0x8u);
   
   return newSnail;
}

Ref<Object>::SharedPtr ObjectCreator::createObject(const std::string & type, ObjectCreator & creator) {
   Ref<Object>::SharedPtr retval;
   
   if (type == "snail1") {
	  return createSnail(0, creator);
   }
   else if (type == "snail2") {
	  return createSnail(1, creator);
   }
   else if (type == "bullet") {
	  return createProjectile();
   }
   else if (type == "missile") {
      Ref<Missile>::SharedPtr newMissile(new Missile);
      newMissile->origin = Owning(new CoordSystemLeaf2);

      // Missile
      newMissile->sprite = Owning(world.graphics.createSprite("../data/missile.png"));
      newMissile->sprite->setEventHandler(newMissile);
      newMissile->sprite->origin = newMissile->origin;

      // Body
      newMissile->body = Owning(world.physics.createBody());
      newMissile->body->setOwner(newMissile);
      newMissile->body->origin = newMissile->origin;

      // Geom
      const rect bulletSize(32, 8);
      newMissile->geom = Owning(world.physics.createRectGeom(bulletSize));
      newMissile->geom->setCollisionId(3);
      newMissile->geom->setCollisionMask(0x0u);
      newMissile->geom->origin = newMissile->origin;
      
      if (Ref<Graphics::ParticleSystem>::SharedPtr lockedParticles =
          smokeParticles.lock()) {
         newMissile->smokeEmitter = Owning(new Graphics::ParticleEmitter);
         newMissile->smokeEmitter->setParticleSystem(lockedParticles);
    
         // Set the coordinate system as a transformation of the missile's
         // coordinate system so the particles start a bit behind the missile

         // FIXME: ParticleEmitter origin
//         newMissile->smokeEmitter->setCoordSystem(
//             Owning(new TransformedCoordSystem<CoordSystem2>(
//                       Observing(newMissile),
//                       CoordSystem2::data_type(vec2(0.0f, 0.0f),
//                                               mat2::Identity())
//                       )
//                )
//             );
      }
      
      world.scheduler.subscribe(0.1f, Observing(newMissile));
      
      return newMissile;
   }
   else if (type == "cactus") {
	  Ref<Cactus>::SharedPtr newCactus(new Cactus);
      newCactus->origin = Owning(new CoordSystemLeaf2);
      
      // Sprite
	  newCactus->sprite = Owning(world.graphics.createSprite("../data/cactii.png"));
      newCactus->sprite->setGrid(4, 2);
      newCactus->sprite->setCell(0, 0);
	  newCactus->sprite->setEventHandler(newCactus);
      newCactus->sprite->origin = newCactus->origin;

      // Body
	  newCactus->body = Owning(world.physics.createBody());
	  newCactus->body->setOwner(newCactus);
      newCactus->body->origin = newCactus->origin;
      
      // Geom
	  const rect cactusSize(18, 34);
	  newCactus->geom = Owning(world.physics.createRectGeom(cactusSize));
	  newCactus->geom->setCollisionId(4);
	  newCactus->geom->setCollisionMask(0x8u);
	  newCactus->geom->setEventHandler(Observing(newCactus));
      newCactus->geom->origin = newCactus->origin;
      
	  return newCactus;
   }
   else if (type == "healthbox") {
      Ref<PowerUp>::SharedPtr newPowerup(new PowerUp);
      newPowerup->sprite = Owning(world.graphics.createSprite("../data/health_powerup.png"));
      newPowerup->sprite->setEventHandler(newPowerup);
      
      newPowerup->body = Owning(world.physics.createBody());
      //newPowerup->body->setDelegate(newPowerup->sprite);
      newPowerup->body->setOwner(newPowerup); // I guess this is for cascade kill

      newPowerup->geom = Owning(world.physics.createRectGeom(newPowerup->sprite->getSize()));
      //newPowerup->geom->setBody(newPowerup->body);
      newPowerup->geom->setCollisionId(4);
      newPowerup->geom->setEventHandler(Observing(newPowerup));
      
      return newPowerup;
   }
   else if (type == "rockets") {
      Ref<RocketAmmo>::SharedPtr newPowerup(new RocketAmmo);

      // Sprite
      newPowerup->sprite = Owning(world.graphics.createSprite("../data/rocket_ammo.png"));
      newPowerup->sprite->setEventHandler(newPowerup);
      newPowerup->sprite->origin = Owning(new CoordSystemLeaf2);

      // Body
      newPowerup->body = Owning(world.physics.createBody());
      newPowerup->body->setOwner(newPowerup); // I guess this is for cascade kill
      newPowerup->body->origin = newPowerup->sprite->origin;

      // Geom
      newPowerup->geom = Owning(world.physics.createRectGeom(newPowerup->sprite->getSize()));
      newPowerup->geom->setCollisionId(4);
      newPowerup->geom->setEventHandler(Observing(newPowerup));
      newPowerup->geom->origin = newPowerup->sprite->origin;
      
      return newPowerup;
   }
	

   throw std::runtime_error("failed to create object of type '" + type + "', I don't know what it is!");
}

Ref<Helmet>::SharedPtr ObjectCreator::createHelmet(int dir) {
   Ref<Helmet>::SharedPtr newHelmet(new Helmet);
   newHelmet->origin = Owning(new CoordSystemLeaf2);
   
   // Sprite
   newHelmet->sprite = Owning(world.graphics.createSprite("../data/helmets.png"));
   newHelmet->sprite->setGrid(2, 2);
   newHelmet->sprite->origin = newHelmet->origin;
   
   if (dir > 0)
      newHelmet->sprite->setCell(0, 0);
   else
      newHelmet->sprite->setCell(1, 0);
   
   // Body
   newHelmet->body = Owning(world.physics.createBody());
   newHelmet->body->setOwner(newHelmet);
   newHelmet->body->origin = newHelmet->origin;
   
   // Geom
   newHelmet->geom = Owning(world.physics.createRectGeom(rect(24, 14)));
   newHelmet->geom->setPriority(-20);
   newHelmet->geom->setEventHandler(Observing(newHelmet));
   newHelmet->geom->setOffset(vec2(1.0f * static_cast<float>(dir), -6.0f));
   newHelmet->geom->origin = newHelmet->origin;
   
   return newHelmet;
}

