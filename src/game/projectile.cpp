#include <game/projectile.h>
#include <game/system.h>
#include <game/snails.h>
#include <game/items.h>

#include <engine/graphics.h>
#include <engine/texture.h>

#include <utils/vec.h>
#include <utils/rect.h>

Projectile::Projectile(const vec2 &pos, const vec2 &vel, class Snail *shooter, class Texture *tex, const SystemContext *ctx)
  : tex(tex)
  , pos(pos)
  , vel(vel)
  , shooter(shooter)
  , ctx(ctx)
{
  /*
    strip = ctx->particles()->group(particle_texture)->createTrail(pos);
    strip->addMidpoint(pos)
    strip->setEndpoint(blah);

    group = ctx->particles()->group(particle_texture);
    group->addParticle(pos);
  */

}

bool Projectile::update(double dt) {
  const vec2 prevPos = pos;
  pos += vel * dt;
  
  vec2 hitPos;
  Snail *hit = ctx->snails()->intersectingSnails(prevPos, pos, 1.0f, shooter, hitPos);
  if (hit) {
    hit->takeDamage(hitPos, 10.0f);
    return false;
  }

  Item *hitItem = ctx->items()->intersectingItem(prevPos, pos, 1.0f, hitPos);
  if (hitItem) {
    if (hitItem->takeDamage(hitPos, 10.0f, shooter)) {  // FIXME: takeDamage -> DISINTEGRATE, IGNORE, REFLECT
      return false;
    }

    // If reflect: emitter.addMidpoint()
  }


  // update effect
//  emitter.setPosition(pos);
  
  return pos.x - 64.0f <= 800.0f; // FIXME: screenRect vs pos + radius (circle)
}

void Projectile::render(Graphics *gfx) {
  gfx->setBlend(Graphics::BLEND_ALPHA);
  gfx->enableTextures();
  tex->bind();

  gfx->drawQuad(rect(pos, 32, 32)); // FIXME: get rid of hardcoded sizes

}

