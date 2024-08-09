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
    bodyDef.linearDamping = 12.0f; // TODO?
    m_physicsObject = std::make_shared<jt::Box2DObject>(m_world, &bodyDef);
    m_shape = jt::dh::createShapeRect({ 15, 15 }, jt::colors::White, textureManager());
}

void Player::doUpdate(float const elapsed)
{
    m_shape->update(elapsed);
    auto kb = getGame()->input().keyboard();
    jt::Vector2f force { 0.0f, 0.0f };
    if (kb->pressed(jt::KeyCode::Left) || kb->pressed(jt::KeyCode::A)) {
        force.x -= GP::PlayerMoveForce() * 25.0f;
    } else if (kb->pressed(jt::KeyCode::Right) || kb->pressed(jt::KeyCode::D)) {
        force.x += GP::PlayerMoveForce() * 25.0f;
    }

    m_physicsObject->addForceToCenter(force);
    m_shape->setPosition(m_physicsObject->getPosition());
}

void Player::doDraw() const { m_shape->draw(renderTarget()); }
