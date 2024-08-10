#include "player.hpp"

#include "conversions.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "random/random.hpp"
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
    bodyDef.position.y = GP::GetScreenSize().y - 96;
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

    std::string const type = m_isLeft ? "dragon" : "digital";
    m_arrowLeft = std::make_shared<jt::Animation>();
    m_arrowLeft->loadFromAseprite("assets/arrow_" + type + "_left.aseprite", textureManager());
    m_arrowLeft->play("loop");
    m_arrowLeft->setAnimationSpeedFactor(jt::Random::getFloat(0.8f, 1.3f));
    m_arrowLeft->setOffset(jt::OffsetMode::CENTER);

    m_arrowRight = std::make_shared<jt::Animation>();
    m_arrowRight->loadFromAseprite("assets/arrow_" + type + "_right.aseprite", textureManager());
    m_arrowRight->play("loop");
    m_arrowRight->setAnimationSpeedFactor(jt::Random::getFloat(0.8f, 1.3f));
    m_arrowRight->setOffset(jt::OffsetMode::CENTER);

    m_arrowUp = std::make_shared<jt::Animation>();
    m_arrowUp->loadFromAseprite("assets/arrow_" + type + "_up.aseprite", textureManager());
    m_arrowUp->play("loop");
    m_arrowUp->setAnimationSpeedFactor(jt::Random::getFloat(0.8f, 1.3f));
    m_arrowUp->setOffset(jt::OffsetMode::CENTER);

    m_arrowDown = std::make_shared<jt::Animation>();
    m_arrowDown->loadFromAseprite("assets/arrow_" + type + "_down.aseprite", textureManager());
    m_arrowDown->play("loop");
    m_arrowDown->setAnimationSpeedFactor(jt::Random::getFloat(0.8f, 1.3f));
    m_arrowDown->setOffset(jt::OffsetMode::CENTER);
}

void Player::updateArrows(float elapsed)
{
    m_arrowLeft->setPosition(m_animation->getPosition() + jt::Vector2f { -24.0f, 0.0f }
        + (m_hasMovedLeft ? jt::Vector2f { -5000, -5000 } : jt::Vector2f { 0.0f, 0.0f }));
    m_arrowRight->setPosition(m_animation->getPosition() + jt::Vector2f { 24.0f, 0.0f }
        + (m_hasMovedRight ? jt::Vector2f { -5000, -5000 } : jt::Vector2f { 0.0f, 0.0f }));
    m_arrowUp->setPosition(m_animation->getPosition() + jt::Vector2f { 0.0f, -24.0f }
        + (m_hasMovedUp ? jt::Vector2f { -5000, -5000 } : jt::Vector2f { 0.0f, 0.0f }));
    m_arrowDown->setPosition(m_animation->getPosition() + jt::Vector2f { 0.0f, 24.0f }
        + (m_hasMovedDown ? jt::Vector2f { -5000, -5000 } : jt::Vector2f { 0.0f, 0.0f }));

    m_arrowLeft->update(elapsed);
    m_arrowRight->update(elapsed);
    m_arrowUp->update(elapsed);
    m_arrowDown->update(elapsed);
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
        m_hasMovedLeft = true;
        if (pos.x > minPosX) {
            force.x -= GP::PlayerMoveForce();
        }
    } else if (kb->pressed(jt::KeyCode::Right) || kb->pressed(jt::KeyCode::D)) {
        m_hasMovedRight = true;
        if (pos.x <= maxPosX) {
            force.x += GP::PlayerMoveForce();
        }
    }

    if (kb->pressed(jt::KeyCode::Up) || kb->pressed(jt::KeyCode::W)) {
        m_hasMovedUp = true;
        if (pos.y >= minPosY) {
            force.y -= GP::PlayerMoveForce();
        }
    }
    if (kb->pressed(jt::KeyCode::Down) || kb->pressed(jt::KeyCode::S)) {
        m_hasMovedDown = true;
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

    updateArrows(elapsed);
}

void Player::doDraw() const
{
    m_animation->draw(renderTarget());

    m_arrowLeft->draw(renderTarget());
    m_arrowRight->draw(renderTarget());
    m_arrowUp->draw(renderTarget());
    m_arrowDown->draw(renderTarget());
}

jt::Vector2f Player::getPosition() const { return m_physicsObject->getPosition(); }
