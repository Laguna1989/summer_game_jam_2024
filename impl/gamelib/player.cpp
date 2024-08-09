#include "player.hpp"

#include "conversions.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include <drawable_helpers.hpp>

Player::Player(std::shared_ptr<jt::Box2DWorldInterface> world)
    : m_world { world }
{
}

void Player::setLeft(bool isLeft) { m_isLeft = isLeft; }

void Player::doCreate()
{
    b2BodyDef bodyDef;
    bodyDef.position.x = (m_isLeft ? 0 : 160) + 80;
    bodyDef.position.y = GP::GetScreenSize().y - 32;
    bodyDef.type = b2BodyType::b2_dynamicBody;
    bodyDef.fixedRotation = false;
    bodyDef.angularDamping = 0.0f;
    bodyDef.linearDamping = 12.0f;
    m_physicsObject = std::make_shared<jt::Box2DObject>(m_world, &bodyDef);

    m_shape = jt::dh::createShapeRect({ 15, 15 }, jt::colors::White, textureManager());
    m_shape->setOffset(jt::OffsetMode::CENTER);
}

void Player::doUpdate(float const elapsed)
{
    m_shape->update(elapsed);
    auto kb = getGame()->input().keyboard();
    jt::Vector2f force { 0.0f, 0.0f };

    auto pos = m_physicsObject->getPosition();
    auto velocity = m_physicsObject->getVelocity();
    auto const minPos = (m_isLeft ? 0.0f : 160.0f) + 10.0f;
    auto const maxPos = (m_isLeft ? 0.0f : 160.0f) + 150.0f;

    if (kb->pressed(jt::KeyCode::Left) || kb->pressed(jt::KeyCode::A)) {
        if (pos.x > minPos) {
            force.x -= GP::PlayerMoveForce();
        }
    } else if (kb->pressed(jt::KeyCode::Right) || kb->pressed(jt::KeyCode::D)) {
        if (pos.x <= maxPos) {
            force.x += GP::PlayerMoveForce();
        }
    }

    m_physicsObject->addForceToCenter(force);

    if (pos.x < minPos) {
        pos.x = minPos;
        velocity.x = 0;
    } else if (pos.x > maxPos) {
        pos.x = maxPos;
        velocity.x = 0;
    }
    m_physicsObject->setVelocity(velocity);
    m_shape->setPosition(m_physicsObject->getPosition());
}

void Player::doDraw() const { m_shape->draw(renderTarget()); }
