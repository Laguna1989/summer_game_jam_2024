#include "bullet.hpp"

#include "game_properties.hpp"
#include <drawable_helpers.hpp>

Bullet::Bullet(std::shared_ptr<jt::Box2DWorldInterface> const& world)
    : m_world { world }
{
}

void Bullet::setIsLeft(bool isLeft) { m_isLeft = isLeft; }

void Bullet::doCreate()
{
    b2BodyDef bodyDef;
    bodyDef.position.x = (m_isLeft ? 0.0f : 160.0f) + 80.0f;
    bodyDef.position.y = -32;
    bodyDef.type = b2BodyType::b2_dynamicBody;
    bodyDef.fixedRotation = false;
    bodyDef.angularDamping = 0.0f;
    m_physicsObject = std::make_shared<jt::Box2DObject>(m_world, &bodyDef);
    m_physicsObject->setVelocity({ 0.0f, GP::ShotVelocity() });

    m_shape = jt::dh::createShapeRect({ 6, 6 }, jt::colors::Green, textureManager());
}

void Bullet::doUpdate(float const elapsed)
{
    m_shape->setPosition(m_physicsObject->getPosition());
    m_shape->update(elapsed);
    if (m_physicsObject->getPosition().y > GP::GetScreenSize().y) {
        kill();
    }
}

void Bullet::doDraw() const { m_shape->draw(renderTarget()); }
