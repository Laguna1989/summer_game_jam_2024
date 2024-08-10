#include "bullet.hpp"

#include "game_properties.hpp"
#include <drawable_helpers.hpp>

Bullet::Bullet(std::shared_ptr<jt::Box2DWorldInterface> const& world)
    : m_world { world }
{
}

void Bullet::setIsLeft(bool isLeft)
{
    m_isLeft = isLeft;
    if (isLeft) {
        m_physicsObject->setPosition(
            m_physicsObject->getPosition() + jt::Vector2f { GP::GetScreenSize().x / 2, 0.0f });
    }
}

std::weak_ptr<jt::Box2DObject> Bullet::getPhysicsObject() { return m_physicsObject; }

void Bullet::doCreate()
{
    b2BodyDef bodyDef;
    bodyDef.position.y = -32;
    bodyDef.type = b2BodyType::b2_dynamicBody;
    bodyDef.fixedRotation = false;
    bodyDef.angularDamping = 0.0f;
    m_physicsObject = std::make_shared<jt::Box2DObject>(m_world, &bodyDef);

    m_shape = jt::dh::createShapeRect({ 6, 6 }, jt::colors::Green, textureManager());
}

void Bullet::doUpdate(float const elapsed)
{
    m_shape->setPosition(m_physicsObject->getPosition());
    m_shape->update(elapsed);
    auto const pos = m_physicsObject->getPosition();
    if (pos.y > GP::GetScreenSize().y) {
        kill();
    }
    if (m_isLeft) {
        if (pos.x > GP::GetScreenSize().x) { }
    }
}

void Bullet::doDraw() const { m_shape->draw(renderTarget()); }
