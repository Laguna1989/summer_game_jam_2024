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

    m_animation = std::make_shared<jt::Animation>();
    if (m_isLeft) {
        m_animation->loadFromAseprite("assets/Dragon.aseprite", textureManager());
    } else {
        m_animation->loadFromAseprite("assets/Finger.aseprite", textureManager());
    }
    m_animation->play("loop");

    m_animation->setOffset(jt::OffsetMode::CENTER);
}

void Player::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);
    auto kb = getGame()->input().keyboard();
    jt::Vector2f force { 0.0f, 0.0f };

    auto pos = m_physicsObject->getPosition();
    auto velocity = m_physicsObject->getVelocity();
    auto const minPosX = (m_isLeft ? 0.0f : 160.0f) + 10.0f;
    auto const minPosY = 10.0f;
    auto const maxPosX = (m_isLeft ? 0.0f : 160.0f) + 150.0f;
    auto const maxPosY = GP::GetScreenSize().y - 10.0f;

    if (kb->pressed(jt::KeyCode::Left) || kb->pressed(jt::KeyCode::A)) {
        if (pos.x > minPosX) {
            force.x -= GP::PlayerMoveForce();
        }
    } else if (kb->pressed(jt::KeyCode::Right) || kb->pressed(jt::KeyCode::D)) {
        if (pos.x <= maxPosX) {
            force.x += GP::PlayerMoveForce();
        }
    }

    if (kb->pressed(jt::KeyCode::Up) || kb->pressed(jt::KeyCode::W)) {
        if (pos.y >= minPosY) {
            force.y -= GP::PlayerMoveForce();
        }
    }
    if (kb->pressed(jt::KeyCode::Down) || kb->pressed(jt::KeyCode::S)) {
        if (pos.y <= maxPosY) {
            force.y += GP::PlayerMoveForce();
        }
    }

    m_physicsObject->addForceToCenter(force);

    if (pos.x < minPosX) {
        pos.x = minPosX;
        velocity.x = 0;
    } else if (pos.x > maxPosX) {
        pos.x = maxPosX;
        velocity.x = 0;
    }
    if (pos.y < minPosY) {
        pos.y = minPosY;
        velocity.y = 0;
    } else if (pos.y > maxPosY) {
        pos.y = maxPosY;
        velocity.y = 0;
    }
    m_physicsObject->setVelocity(velocity);
    m_animation->setPosition(m_physicsObject->getPosition());
}

void Player::doDraw() const { m_animation->draw(renderTarget()); }

jt::Vector2f Player::getPosition() const { return m_physicsObject->getPosition(); }
