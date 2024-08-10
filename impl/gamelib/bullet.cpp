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
    if (!isLeft) {
        m_physicsObject->setPosition(
            m_physicsObject->getPosition() + jt::Vector2f { GP::GetScreenSize().x / 2, 0.0f });
    }
}

std::weak_ptr<jt::Box2DObject> Bullet::getPhysicsObject() { return m_physicsObject; }

void Bullet::setAnimName(std::string const& string) { m_animName = string; }

void Bullet::doCreate()
{
    b2BodyDef bodyDef;
    bodyDef.position.y = -32;
    bodyDef.type = b2BodyType::b2_dynamicBody;
    bodyDef.fixedRotation = false;
    bodyDef.angularDamping = 0.0f;
    m_physicsObject = std::make_shared<jt::Box2DObject>(m_world, &bodyDef);

    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromAseprite(m_animName, textureManager());
    m_animation->play("loop");
    m_animation->setOffset(jt::OffsetMode::CENTER);
}

void Bullet::doUpdate(float const elapsed)
{
    m_animation->setPosition(m_physicsObject->getPosition());
    m_animation->update(elapsed);
    auto const pos = m_physicsObject->getPosition();

    if (getAge() > 0.5f) {
        if (pos.y > GP::GetScreenSize().y) {
            kill();
        }
        if (m_isLeft) {
            if (pos.x > GP::GetScreenSize().x / 2.0f) {
                kill();
            }
        }
    }
}

void Bullet::doDraw() const { m_animation->draw(renderTarget()); }
